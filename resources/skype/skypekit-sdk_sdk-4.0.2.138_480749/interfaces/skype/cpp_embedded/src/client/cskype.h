
#ifndef CSkype_INCLUDED_HPP
#define CSkype_INCLUDED_HPP

#include "skype-embedded_2.h"
#include "skype-object.h"
#include "caccount.h"
#include "cparticipant.h"
#include "cmessage.h"
#include "cconversation.h"
#include "ccontact.h"
#include "ccontactsearch.h"
#include "ctransfer.h"
#include "cvoicemail.h"
#include "csms.h"
#include "ccontactgroup.h"
#include "cvideo.h"

class CSkype : public Skype
{
public:
        CSkype(int num_threads = 1);
        ~CSkype() {}

        // THIS CODE IS TO ENABLE USING SINGLE THREADED EVENT POLLING
        int get_socket_fd() {
                return m_connection->get_id();
        }
        int process_events(int polled) { // polled = true if there is data in socket_fd 
                while (polled || m_protocol->has_buffered_event()) { // or if (polled || has_buffered_events()) : then select loop will be called between each event processing
                        process_command(m_main_command_initiator);
                        polled = 0;
                }
                return is_server_connected();
        }
        // END OF ENABLING USING SINGLE THREADED EVENT POLLING

        /** creators */
        Account* newAccount(int oid) {
                return new CAccount(oid, this);
        }
        Message* newMessage(int oid) {
                return new CMessage(oid, this);
        }
        Conversation* newConversation(int oid) {
                return new CConversation(oid, this);
        }
        Contact* newContact(int oid) {
                return new CContact(oid, this);
        }
        Participant* newParticipant(int oid) {
                return new CParticipant(oid, this);
        }
        ContactSearch* newContactSearch(int oid) {
                return new CContactSearch(oid, this);
        }
        Transfer* newTransfer(int oid) {
                return new CTransfer(oid, this);
        }
        Voicemail * newVoicemail(int oid) {
                return new CVoicemail(oid, this);
        }
        Sms * newSms(int oid) {
                return new CSms(oid, this);
        }
        ContactGroup * newContactGroup(int oid) {
                return new CContactGroup(oid, this);
        } 
        Video * newVideo(int oid) {
                return new CVideo(oid, this);
        }        

        void OnConversationListChange(const Conversation::Ref& conversation, const Conversation::LIST_TYPE& type, const bool& added);
        void OnContactGoneOffline(const Contact::Ref& contact);
        void OnContactOnlineAppearance(const Contact::Ref& contact);
        void OnMessage(const Message::Ref& message, const bool& changesInboxTimestamp, const Message::Ref& supersedesHistoryMessage, const ConversationRef& conversation);

        bool loggedIn();
        bool loggedOut();
        
        CConversation::Ref get_current_conversation();
        void set_current_conversation(const Conversation::Ref& c);
        bool select_conversation(const String skypename);
        
        void OnApp2AppDatagram(const String& appname, const String& stream, const Binary& data);
        void OnApp2AppStreamListChange(const String& appname, const APP2APP_STREAMS& listType, const List_String& streams, const Sid::List_uint &receivedSizes);

        void OnProxyAuthFailure(const PROXYTYPE& type);

        CAccount::Ref get_active_account();
        CMessage::Ref get_last_message() {
                Sid::Mutex::ScopedLock l(m_ev_mutex);
                return last_message;
        };
        void set_last_message(const Message::Ref&);

        void login_default_account();  
               
        //ConnectionStateListener callbacks       
        virtual void Disconnected() { 
                //cleanup
                printf("\n::: Disconnected from skypekit, enter 'r' to reconnect...\n\n");         
                activeAccount = CAccount::Ref(); 
                current_conversation = CConversation::Ref();  
                last_message = CMessage::Ref();                        
        }    
        virtual void Connected() { printf("::: Connected to skypekit\n"); }       
        virtual void Connecting() { printf("::: Connecting to skypekit\n"); }
        virtual void OnError(CONNECTION_ERROR err) { 
                if (err == ConnectionStateListener::SSL_CERTIFICATE_LOAD_ERR) printf("::: error loading ssl certificate\n"); 
        } 
        virtual void OnAvailableVideoDeviceListChange();
        virtual void OnH264Activated();
public:
        CAccount::Ref activeAccount;
        CConversation::Ref current_conversation;
        CMessage::Ref last_message;
        Sid::Mutex m_ev_mutex; // useful when several event threads
};

#endif //CSkype_INCLUDED_HPP

