#include "util.h"

#include "skypecore.h"

namespace Mvp
{

namespace Model
{

Poco::SharedPtr<ContactData>
    Util::dataFromContact(const ContactRef &contact)
{
    /* As we are only interested in some data about the Contact, let's fill
     * a small structure with everything we need and then return.
     */
    Poco::SharedPtr<ContactData> newData =
        Poco::SharedPtr<ContactData>(new ContactData);
    newData->canSMS = false;

    SEString value;

    // Get the Skype Name from the Contact.
    Contact::TYPE type;
    contact->GetType(type);

    switch (type) {
        case Contact::SKYPE:
            contact->GetPropSkypename(value);
            break;
        case Contact::PSTN:
            contact->GetPropPstnnumber(value);
            newData->canSMS = true;
            break;
        default:
            value = "Unrecognized";
    }

    newData->skypeName = value;

    // Get the Display Name from the Contact.
    contact->GetPropDisplayname(value);
    newData->displayName = value;

    if (!newData->canSMS) {
        // See if the contact has a mobile number set.
        contact->GetPropPhoneMobile(value);
        if (value.length() > 0)
            newData->canSMS = true;
    }

    /* Get the availability from the Contact and translate it to a readable
     * string.
     */
    Contact::AVAILABILITY availability;
    contact->GetPropAvailability(availability);
    std::pair<ContactAvailability, std::string> ret =
        convertAvailability(availability);
    newData->status = ret.first;
    newData->availability = ret.second;

    //Get the contact Avatar.
    bool hasAvatar = false;
    SEBinary rawData;

    contact->GetAvatar(hasAvatar, rawData);

    // Check if the contact has an avatar and then extract the data from it.
    if (hasAvatar)
        newData->avatar.assignRaw(rawData.data(), rawData.size());

    // Get the contact capabilities.
    contact->HasCapability(Contact::CAPABILITY_TEXT, newData->canIM);
    newData->canCall = (newData->status != Offline);
    newData->canFT = (newData->status != Offline && type == Contact::SKYPE);

    return newData;
}

const Poco::SharedPtr<ContactData>
    Util::dataFromAccount(const AccountRef &account)
{
    Poco::SharedPtr<ContactData> data(0);

    data = Poco::SharedPtr<ContactData>(new ContactData);

    SEString value;

    // Get the Skype Name from the account.
    account->GetPropSkypename(value);
    data->skypeName = value.data();

    // Get the Display Name from the account.
    account->GetPropFullname(value);
    data->displayName = value;

    /* Get the availability from the Account and translate it to a readable
     * string.
     */
    Contact::AVAILABILITY availability;
    account->GetPropAvailability(availability);
    std::pair<ContactAvailability, std::string> ret =
        convertAvailability(availability);
    data->status = ret.first;
    data->availability = ret.second;

    //Get the contact Avatar.
    SEBinary rawData;

    if (account->GetPropAvatarImage(rawData) &&
        rawData.size() > 0)
        data->avatar.assignRaw(rawData.data(), rawData.size());

    return data;
}

Poco::SharedPtr<MessageData> Util::dataFromMessage(const MessageRef &message)
{
    int messageType = message->GetProp(Message::P_TYPE).toInt();

    SEString author;
    SEString authorName;
    SEString bodyXmlString;
    unsigned int timeStamp;
    message->GetPropAuthor(author);
    message->GetPropAuthorDisplayname(authorName);
    message->GetPropBodyXml(bodyXmlString);
    message->GetPropTimestamp(timeStamp);

    Poco::SharedPtr<MessageData> data = new MessageData;
    data->author = author.data();
    data->authorDisplayname = authorName.data();
    data->body = bodyXmlString.data();
    data->timestamp = timeStamp;
    data->isEmote = (messageType == Message::POSTED_EMOTE);

    return data;
}

ContactRef Util::contactFromParticipant(const ParticipantRef &participant)
{
    SEString oid;
    participant->GetPropIdentity(oid);

    SkypeContact::Ref contact;
    SkypeCore::instance()->GetContact(oid, contact);

    return contact;
}

std::pair<ContactAvailability, std::string>
    Util::convertAvailability(Contact::AVAILABILITY availability)
{
    /* Switch case necessary to translate the Contact's availability to a
     * readable string.
     */
    ContactAvailability status;
    std::string statusAsText;

    // Here are all of the possible availability states
    switch (availability) {
        case Contact::UNKNOWN:
            status = Offline;
            statusAsText = "Unknown";
            break;
        case Contact::PENDINGAUTH:
            status = Offline;
            statusAsText = "Pending Authentication";
            break;
        case Contact::BLOCKED:
            status = Offline;
            statusAsText = "Blocked";
            break;
        case Contact::BLOCKED_SKYPEOUT:
            status = Offline;
            statusAsText = "Blocked SkypeOut";
            break;
        case Contact::SKYPEOUT:
            status = SkypeOut;
            statusAsText = "SkypeOut";
            break;
        case Contact::OFFLINE:
            status = Offline;
            statusAsText = "Offline";
            break;
        case Contact::OFFLINE_BUT_VM_ABLE:
            status = Available;
            statusAsText = "Offline but has VoiceMail";
            break;
        case Contact::OFFLINE_BUT_CF_ABLE:
            status = Available;
            statusAsText = "Offline but has Call Forwarding";
            break;
        case Contact::ONLINE:
            status = Available;
            statusAsText = "Online";
            break;
        case Contact::AWAY:
            status = Away;
            statusAsText = "Away";
            break;
        case Contact::NOT_AVAILABLE:
            status = Away;
            statusAsText = "Not Available";
            break;
        case Contact::DO_NOT_DISTURB:
            status = Busy;
            statusAsText = "Do Not Disturb";
            break;
        case Contact::SKYPE_ME:
            status = Available;
            statusAsText = "Skype Me";
            break;
        case Contact::INVISIBLE:
            status = Offline;
            statusAsText = "Invisible";
            break;
        case Contact::CONNECTING:
            status = Offline;
            statusAsText = "Connecting";
            break;
        case Contact::ONLINE_FROM_MOBILE:
            status = Available;
            statusAsText = "Online from Mobile";
            break;
        case Contact::AWAY_FROM_MOBILE:
            status = Away;
            statusAsText = "Away from Mobile";
            break;
        case Contact::NOT_AVAILABLE_FROM_MOBILE:
            status = Away;
            statusAsText = "Not Available from Mobile";
            break;
        case Contact::DO_NOT_DISTURB_FROM_MOBILE:
            status = Busy;
            statusAsText = "Do Not Disturb from Mobile";
            break;
        case Contact::SKYPE_ME_FROM_MOBILE:
            status = Available;
            statusAsText = "Skype Me from Mobile";
            break;
        default:
            status = Offline;
            statusAsText = "Unknown";
    }

    return std::pair<ContactAvailability, std::string>(status, statusAsText);
}

};

};
