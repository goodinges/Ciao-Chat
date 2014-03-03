#include "skype-embedded_2.h"
#include "sidg_Skyperefs.hpp"

#ifdef SE_USE_NAMESPACE
namespace Skype {
#endif
        ContactGroup* ContactGroupRef::operator->() const 
        {
                return static_cast<ContactGroup*>(SEReference::operator->());
        };
        
        ContactGroupRef::ContactGroupRef(const unsigned int& oid) : SEReference(oid, ContactGroup::MODULE_ID) {}
        
        Contact* ContactRef::operator->() const 
        {
                return static_cast<Contact*>(SEReference::operator->());
        };
        
        ContactRef::ContactRef(const unsigned int& oid) : SEReference(oid, Contact::MODULE_ID) {}
        
        ContactSearch* ContactSearchRef::operator->() const 
        {
                return static_cast<ContactSearch*>(SEReference::operator->());
        };
        
        ContactSearchRef::ContactSearchRef(const unsigned int& oid) : SEReference(oid, ContactSearch::MODULE_ID) {}
        
        Participant* ParticipantRef::operator->() const 
        {
                return static_cast<Participant*>(SEReference::operator->());
        };
        
        ParticipantRef::ParticipantRef(const unsigned int& oid) : SEReference(oid, Participant::MODULE_ID) {}
        
        Conversation* ConversationRef::operator->() const 
        {
                return static_cast<Conversation*>(SEReference::operator->());
        };
        
        ConversationRef::ConversationRef(const unsigned int& oid) : SEReference(oid, Conversation::MODULE_ID) {}
        
        Message* MessageRef::operator->() const 
        {
                return static_cast<Message*>(SEReference::operator->());
        };
        
        MessageRef::MessageRef(const unsigned int& oid) : SEReference(oid, Message::MODULE_ID) {}
        
        Video* VideoRef::operator->() const 
        {
                return static_cast<Video*>(SEReference::operator->());
        };
        
        VideoRef::VideoRef(const unsigned int& oid) : SEReference(oid, Video::MODULE_ID) {}
        
        Voicemail* VoicemailRef::operator->() const 
        {
                return static_cast<Voicemail*>(SEReference::operator->());
        };
        
        VoicemailRef::VoicemailRef(const unsigned int& oid) : SEReference(oid, Voicemail::MODULE_ID) {}
        
        Sms* SmsRef::operator->() const 
        {
                return static_cast<Sms*>(SEReference::operator->());
        };
        
        SmsRef::SmsRef(const unsigned int& oid) : SEReference(oid, Sms::MODULE_ID) {}
        
        Transfer* TransferRef::operator->() const 
        {
                return static_cast<Transfer*>(SEReference::operator->());
        };
        
        TransferRef::TransferRef(const unsigned int& oid) : SEReference(oid, Transfer::MODULE_ID) {}
        
        Account* AccountRef::operator->() const 
        {
                return static_cast<Account*>(SEReference::operator->());
        };
        
        AccountRef::AccountRef(const unsigned int& oid) : SEReference(oid, Account::MODULE_ID) {}
        
#ifdef SE_USE_NAMESPACE
} // Skype
#endif
