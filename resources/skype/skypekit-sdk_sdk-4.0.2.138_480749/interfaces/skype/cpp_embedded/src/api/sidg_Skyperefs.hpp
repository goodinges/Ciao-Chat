#ifndef __SIDG_SKYPEREFSHPP_INCLUDED__
#define __SIDG_SKYPEREFSHPP_INCLUDED__

#include "SidPlatform.hpp"

#include "skype-object.h"

#ifdef SE_USE_NAMESPACE
namespace Skype {
#endif

        class ContactGroup;
        class Contact;
        class ContactSearch;
        class Participant;
        class Conversation;
        class Message;
        class Video;
        class Voicemail;
        class Sms;
        class Transfer;
        class Account;
        
        /*! \class ContactGroupRef 
         * \brief Reference to an ContactGroup class instance, equivalent to ContactGroup::Ref
         */
        class ContactGroupRef : public SEReference 
        {
        public:
                ContactGroupRef(const unsigned int& oid = 0);
                ContactGroup* operator->() const;
        protected:
                ContactGroupRef(const SEReference& ref) : SEReference(ref) {}
                friend class ContactGroupRefs; 
        }; 
        
        /*! \class ContactGroupRefs
         * \brief list of references to ContactGroup class instances, equivalent to ContactGroup::Refs
         */
        class ContactGroupRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const ContactGroupRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                ContactGroupRefs& append(const ContactGroupRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                ContactGroupRef& operator[](size_t n) const { 
                        return (ContactGroupRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                ContactGroupRefs& operator=(const ContactGroupRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class ContactRef 
         * \brief Reference to an Contact class instance, equivalent to Contact::Ref
         */
        class ContactRef : public SEReference 
        {
        public:
                ContactRef(const unsigned int& oid = 0);
                Contact* operator->() const;
        protected:
                ContactRef(const SEReference& ref) : SEReference(ref) {}
                friend class ContactRefs; 
        }; 
        
        /*! \class ContactRefs
         * \brief list of references to Contact class instances, equivalent to Contact::Refs
         */
        class ContactRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const ContactRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                ContactRefs& append(const ContactRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                ContactRef& operator[](size_t n) const { 
                        return (ContactRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                ContactRefs& operator=(const ContactRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class ContactSearchRef 
         * \brief Reference to an ContactSearch class instance, equivalent to ContactSearch::Ref
         */
        class ContactSearchRef : public SEReference 
        {
        public:
                ContactSearchRef(const unsigned int& oid = 0);
                ContactSearch* operator->() const;
        protected:
                ContactSearchRef(const SEReference& ref) : SEReference(ref) {}
                friend class ContactSearchRefs; 
        }; 
        
        /*! \class ContactSearchRefs
         * \brief list of references to ContactSearch class instances, equivalent to ContactSearch::Refs
         */
        class ContactSearchRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const ContactSearchRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                ContactSearchRefs& append(const ContactSearchRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                ContactSearchRef& operator[](size_t n) const { 
                        return (ContactSearchRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                ContactSearchRefs& operator=(const ContactSearchRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class ParticipantRef 
         * \brief Reference to an Participant class instance, equivalent to Participant::Ref
         */
        class ParticipantRef : public SEReference 
        {
        public:
                ParticipantRef(const unsigned int& oid = 0);
                Participant* operator->() const;
        protected:
                ParticipantRef(const SEReference& ref) : SEReference(ref) {}
                friend class ParticipantRefs; 
        }; 
        
        /*! \class ParticipantRefs
         * \brief list of references to Participant class instances, equivalent to Participant::Refs
         */
        class ParticipantRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const ParticipantRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                ParticipantRefs& append(const ParticipantRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                ParticipantRef& operator[](size_t n) const { 
                        return (ParticipantRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                ParticipantRefs& operator=(const ParticipantRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class ConversationRef 
         * \brief Reference to an Conversation class instance, equivalent to Conversation::Ref
         */
        class ConversationRef : public SEReference 
        {
        public:
                ConversationRef(const unsigned int& oid = 0);
                Conversation* operator->() const;
        protected:
                ConversationRef(const SEReference& ref) : SEReference(ref) {}
                friend class ConversationRefs; 
        }; 
        
        /*! \class ConversationRefs
         * \brief list of references to Conversation class instances, equivalent to Conversation::Refs
         */
        class ConversationRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const ConversationRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                ConversationRefs& append(const ConversationRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                ConversationRef& operator[](size_t n) const {                         return (ConversationRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                ConversationRefs& operator=(const ConversationRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class MessageRef 
         * \brief Reference to an Message class instance, equivalent to Message::Ref
         */
        class MessageRef : public SEReference 
        {
        public:
                MessageRef(const unsigned int& oid = 0);
                Message* operator->() const;
        protected:
                MessageRef(const SEReference& ref) : SEReference(ref) {}
                friend class MessageRefs; 
        }; 
        
        /*! \class MessageRefs
         * \brief list of references to Message class instances, equivalent to Message::Refs
         */
        class MessageRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const MessageRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                MessageRefs& append(const MessageRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                MessageRef& operator[](size_t n) const { 
                        return (MessageRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                MessageRefs& operator=(const MessageRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class VideoRef 
         * \brief Reference to an Video class instance, equivalent to Video::Ref
         */
        class VideoRef : public SEReference 
        {
        public:
                VideoRef(const unsigned int& oid = 0);
                Video* operator->() const;
        protected:
                VideoRef(const SEReference& ref) : SEReference(ref) {}
                friend class VideoRefs; 
        }; 
        
        /*! \class VideoRefs
         * \brief list of references to Video class instances, equivalent to Video::Refs
         */
        class VideoRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const VideoRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                VideoRefs& append(const VideoRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                VideoRef& operator[](size_t n) const { 
                        return (VideoRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                VideoRefs& operator=(const VideoRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class VoicemailRef 
         * \brief Reference to an Voicemail class instance, equivalent to Voicemail::Ref
         */
        class VoicemailRef : public SEReference 
        {
        public:
                VoicemailRef(const unsigned int& oid = 0);
                Voicemail* operator->() const;
        protected:
                VoicemailRef(const SEReference& ref) : SEReference(ref) {}
                friend class VoicemailRefs; 
        }; 
        
        /*! \class VoicemailRefs
         * \brief list of references to Voicemail class instances, equivalent to Voicemail::Refs
         */
        class VoicemailRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const VoicemailRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                VoicemailRefs& append(const VoicemailRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                VoicemailRef& operator[](size_t n) const { 
                        return (VoicemailRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                VoicemailRefs& operator=(const VoicemailRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class SmsRef 
         * \brief Reference to an Sms class instance, equivalent to Sms::Ref
         */
        class SmsRef : public SEReference 
        {
        public:
                SmsRef(const unsigned int& oid = 0);
                Sms* operator->() const;
        protected:
                SmsRef(const SEReference& ref) : SEReference(ref) {}
                friend class SmsRefs; 
        }; 
        
        /*! \class SmsRefs
         * \brief list of references to Sms class instances, equivalent to Sms::Refs
         */
        class SmsRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const SmsRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                SmsRefs& append(const SmsRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                SmsRef& operator[](size_t n) const { 
                        return (SmsRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                SmsRefs& operator=(const SmsRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class TransferRef 
         * \brief Reference to an Transfer class instance, equivalent to Transfer::Ref
         */
        class TransferRef : public SEReference 
        {
        public:
                TransferRef(const unsigned int& oid = 0);
                Transfer* operator->() const;
        protected:
                TransferRef(const SEReference& ref) : SEReference(ref) {}
                friend class TransferRefs; 
        }; 
        
        /*! \class TransferRefs
         * \brief list of references to Transfer class instances, equivalent to Transfer::Refs
         */
        class TransferRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const TransferRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                TransferRefs& append(const TransferRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */                TransferRef& operator[](size_t n) const { 
                        return (TransferRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                TransferRefs& operator=(const TransferRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
        
        /*! \class AccountRef 
         * \brief Reference to an Account class instance, equivalent to Account::Ref
         */
        class AccountRef : public SEReference 
        {
        public:
                AccountRef(const unsigned int& oid = 0);
                Account* operator->() const;
        protected:
                AccountRef(const SEReference& ref) : SEReference(ref) {}
                friend class AccountRefs; 
        }; 
        
        /*! \class AccountRefs
         * \brief list of references to Account class instances, equivalent to Account::Refs
         */
        class AccountRefs : public List_ref 
        {
        public:
                /** Appends a ref at the end of the list. */
                void push_back(const AccountRef & ref) { 
                        List_ref::append(ref); 
                }
                /** Appends a ref at the end of the list. */
                AccountRefs& append(const AccountRef & ref) { 
                        List_ref::append(ref); 
                        return *this; 
                }
                /** Returns a ref specified by its index. */
                AccountRef& operator[](size_t n) const { 
                        return (AccountRef&) List_ref::operator[](n); 
                }
                /** Assigns a shallow copy of sl to this list and returns a reference to it.
                 * This is very fast because the list isn't actually copied.
                 */
                AccountRefs& operator=(const AccountRefs& sl) { 
                        List_ref::operator=(sl); 
                        return *this; 
                }
        };
#ifdef SE_USE_NAMESPACE
} // Skype
#endif

#endif // __SIDG_SKYPEREFSHPP_INCLUDED____
