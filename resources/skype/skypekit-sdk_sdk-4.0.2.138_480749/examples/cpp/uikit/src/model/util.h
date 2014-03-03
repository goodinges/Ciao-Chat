#ifndef _UTIL_H
#define _UTIL_H

#include <Poco/SharedPtr.h>

#include "contactdata.h"
#include "messagedata.h"
#include "skypecontact.h"
#include "skypeaccount.h"

namespace Mvp
{

namespace Model
{

class Util
{
public:
    /**
     * Returns a meta contact from a Skype Contact.
     *
     * The method returns a meta contact containing information
     * from a real Skype Contact.
     *
     * @param contact a real Skype Contact reference.
     *
     * @returns a ContactData representing the real contact.
     *
     * @see ContactData
     * @see SkypeContact
     */
    static Poco::SharedPtr<ContactData>
        dataFromContact(const ContactRef &contact);

    /**
     * Returns a contact from a Participant.
     *
     * The method returns a meta contact containing information
     * from a real Skype Contact.
     *
     * @param participant the participant to get the contact for.
     *
     * @returns a ContactRef representing the real contact.
     *
     * @see SkypeContact
     */
    static ContactRef
        contactFromParticipant(const ParticipantRef &participant);

    /**
     * Returns a meta contact from a Skype Account.
     *
     * The method returns a meta contact containing information from a real
     * Skype Account.
     *
     * @param account the account to get the contact data for.
     *
     * @returns a ContactData representing the real account.
     *
     * @see ContactData
     * @see SkypeAccount
     */
    static const Poco::SharedPtr<ContactData>
        dataFromAccount(const AccountRef &account);

    /**
     * Returns a meta message from a Skype Message.
     *
     * The method returns a meta message containing information from a real
     * Skype Message.
     *
     * @param message the message to get the message data for.
     *
     * @returns a MessageData representing the real message.
     *
     * @see MessageData
     */
    static Poco::SharedPtr<MessageData>
        dataFromMessage(const MessageRef &message);

    /**
     * Returns a translated Status and string description from Availability.
     *
     * The method translates the Contact::AVAILABILITY enum into a readable
     * string and a simplified enumeration.
     *
     * @param availability the contact status to get the availability for.
     *
     * @returns a pair containing a simple enum status and a string representing
     * the status.
     */
    static std::pair<ContactAvailability, std::string>
        convertAvailability(Contact::AVAILABILITY availability);
};

};

};
#endif
