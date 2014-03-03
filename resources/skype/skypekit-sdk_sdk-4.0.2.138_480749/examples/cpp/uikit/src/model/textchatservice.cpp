#include "textchatservice.h"

#include "skypecore.h"
#include "skypeaccount.h"
#include "skypecontact.h"
#include "skypemessage.h"
#include "skypesms.h"
#include "util.h"

#include <Poco/Delegate.h>

using Poco::Delegate;

namespace Mvp
{

namespace Model
{

TextChatService::TextChatService(const ConversationRef &conversation)
    :m_conversation(conversation)
{
    // Set the conversation to active, so we wont create a new ChatState
    // on further incoming messages.
    m_conversation->setActive(true);

    connectToConversation(m_conversation);
}

TextChatService::~TextChatService()
{
    // Disconnect from our participants.
    unsigned int size = m_participants.size();
    for (unsigned int i = 0; i < size; ++i)
        disconnectFromParticipant(m_participants[i]);

    // Clear our participants cache.
    m_participants.clear();

    disconnectFromConversation();

    /* Set active to false, so a new ChatState will be created if more messages
     * come in on this conversation */
    m_conversation->setActive(false);
}

bool TextChatService::isConsumer()
{
    bool ret = false;

    Conversation::MY_STATUS status;
    m_conversation->GetPropMyStatus(status);

    if (status == Conversation::CONSUMER)
        ret = true;

    return ret;
}

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >
    TextChatService::getParticipants()
{
    // Create the contact's container.
    std::vector<Poco::SharedPtr<ContactData> > contacts;

    // Disconnect from previously cached participants.
    unsigned int size = m_participants.size();
    for (unsigned int i = 0; i < size; ++i)
        disconnectFromParticipant(m_participants[i]);

    m_participants.clear();

    // Gets the list of participants.
    ParticipantRefs callerList;
    m_conversation->GetParticipants(callerList, Conversation::OTHER_CONSUMERS);

    size = callerList.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get the participant.
        SkypeParticipant::Ref participant(callerList[i]);

        // Connect to this participant.
        connectToParticipant(participant);

        // Add it to our cache, so we can disconnect from it later.
        m_participants.push_back(callerList[i]);

        // Get the contact for this participant.
        SkypeContact::Ref contact = Util::contactFromParticipant(callerList[i]);

        // Convert contact in ContactData structure.
        Poco::SharedPtr<ContactData> newData(Util::dataFromContact(contact));

        // Add to the container.
        contacts.push_back(newData);
    }

    return contacts;
}

void TextChatService::onIncomingMessage(const void* sender,
    Poco::SharedPtr< MessageData >& message)
{
    messageReceived(this, message);
}

void TextChatService::onParticipantListChanged(const void *sender,
    Poco::EventArgs &args)
{
    printf("textChatService::onParticipantListChanged called.\n");

    participantListChanged(this, args);
}

void TextChatService::onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference)
{
    printf("TextChatService updating to use newly spawned conference.\n");

    // Disconnect from old conversation.
    disconnectFromConversation();

    // Set the current conversation as inactive.
    m_conversation->setActive(false);

    m_conversation = conference;

    // Set the new conversation as active.
    m_conversation->setActive(true);

    // Connect to new conversation.
    connectToConversation(m_conversation);

    // Signal to other objects that the conversation has changed.
    conversationChanged(this, m_conversation);

    Poco::EventArgs args;
    participantListChanged(this, args);
}

void TextChatService::connectToConversation(const ConversationRef &conversation)
{
    SkypeConversation::Ref skypeConversation(conversation);

    // Connect to the conversation's messageReceived signal.
    skypeConversation->messageReceived += Poco::Delegate
        <AbstractChatService, Poco::SharedPtr<Mvp::Model::MessageData> >
        (this, &AbstractChatService::onIncomingMessage);

    // Connect to the conversation's spawnedConference signal.
    skypeConversation->spawnedConference +=
        Poco::Delegate<AbstractChatService, const SkypeConversation::Ref>
        (this, &AbstractChatService::onSpawnedConference);

    // Connect to the participantListChanged signal.
    skypeConversation->participantListChanged +=
        Poco::Delegate<AbstractChatService, Poco::EventArgs>
        (this, &AbstractChatService::onParticipantListChanged);
}

void TextChatService::disconnectFromConversation()
{
    // Disconnect from the conversation object.
    m_conversation->messageReceived -= Poco::Delegate
        <AbstractChatService, Poco::SharedPtr<Mvp::Model::MessageData> >
        (this, &AbstractChatService::onIncomingMessage);

    // Disconnect to the conversation's spawnedConference signal.
    m_conversation->spawnedConference -=
        Poco::Delegate<AbstractChatService, const SkypeConversation::Ref>
        (this, &AbstractChatService::onSpawnedConference);

    // Disconnect to the participantListChanged signal.
    m_conversation->participantListChanged -=
        Poco::Delegate<AbstractChatService, Poco::EventArgs>
        (this, &AbstractChatService::onParticipantListChanged);
}


void TextChatService::connectToParticipant(SkypeParticipant::Ref &participant)
{
    // Get the ContactRef for the participant.
    SkypeContact::Ref contact = Util::contactFromParticipant(participant);

    contact->contactChanged +=
        Poco::Delegate<TextChatService, ContactRef>
        (this, &TextChatService::onContactChanged);
}

void TextChatService::disconnectFromParticipant(
    SkypeParticipant::Ref &participant)
{
    // Get the ContactRef for the participant.
    SkypeContact::Ref contact = Util::contactFromParticipant(participant);

    if (contact.present()) {
        contact->contactChanged -=
            Poco::Delegate<TextChatService, ContactRef>
            (this, &TextChatService::onContactChanged);
    }
}

std::string TextChatService::chatName()
{
    // Get the Display name of the conversation.
    SEString name;
    m_conversation->GetPropDisplayname(name);

    return std::string(name.data());
}

void TextChatService::onContactChanged(const void *sender,
                                   ContactRef &contact)
{
    Poco::SharedPtr<Mvp::Model::ContactData> data =
        Util::dataFromContact(contact);
    contactChanged(this, data);
}

void TextChatService::postMessage(std::string &messageText)
{
    if (!isConsumer())
        return;

    // Convert the message text to an SEString.
    SEString text(messageText.c_str());
    SkypeMessage::Ref message;

    // Then post the text to the conversation.
    printf("posting message %s\n", text.data());
    if (m_conversation->PostText(text, message)) {
        // The post worked, so connect to the message object's signal.
        if (message)
            message->statusChanged +=
                Poco::Delegate<TextChatService, Message::SENDING_STATUS>
                (this, &TextChatService::onImStatusChanged);
    }
}

void TextChatService::onImStatusChanged(const void *sender,
    Message::SENDING_STATUS &status)
{
    if (status == Message::FAILED_TO_SEND) {
        SkypeMessage *message = const_cast<SkypeMessage *>(
            reinterpret_cast<const SkypeMessage *>(sender));

        SEString text;
        message->GetPropBodyXml(text);
        std::string messageText(text.data());
        imFailed(this, messageText);
    }
}

void TextChatService::postSMS(std::string &messageText)
{
    if (!isConsumer())
        return;

    std::pair<Sms::FAILUREREASON, std::string> pair(Sms::MISC_ERROR, messageText);

    Sid::List_String numbers = participantsMobileNumbers();
    if (numbers.size() > 0) {

        // Create the sms message.
        SkypeSms::Ref sms;
        SkypeCore::instance()->CreateOutgoingSms(sms);

        // Set the body of the sms.
        Sms::SETBODYRESULT result;
        Sid::List_String chunks;
        uint charsUntilNextChunk;
        SEString text(messageText.c_str());
        sms->SetBody(text, result, chunks, charsUntilNextChunk);

        bool success;
        sms->SetTargets(numbers, success);

        if (success) {
            // Then post the text to the conversation.
            printf("posting sms %s\n", text.data());

            if (!m_conversation->PostSMS(sms, text)) {
                // Get the failure reason.
                sms->GetPropFailurereason(pair.first);

                smsError(this, pair);
            }
            else {
                // The post worked, so connect to the SMS object's signal.
                sms->statusChanged +=
                    Poco::Delegate<TextChatService, Sms::STATUS>
                    (this, &TextChatService::onSmsStatusChanged);
            }
        }
        else {
            smsError(this, pair);
        }
    }
    else {
        smsError(this, pair);
    }
}

void TextChatService::onSmsStatusChanged(const void *sender, Sms::STATUS &status)
{
    // Get the sender as an sms object.
    SkypeSms *smsObject = const_cast<SkypeSms *>(
        reinterpret_cast<const SkypeSms *>(sender));
    SEString body;
    smsObject->GetPropBody(body);
    std::string bodyString(body.data());

    // If the status is DELIVERED, emit smsDelivered.
    if (status == Sms::DELIVERED) {
        smsDelivered(this, bodyString);
    }
    else if (status == Sms::SOME_TARGETS_FAILED ||
             status == Sms::FAILED) {

        // If the status is FAILED or SOME_TARGETS_FAILED get the reason.
        Sms::FAILUREREASON reason;
        smsObject->GetPropFailurereason(reason);
        std::pair<Sms::FAILUREREASON, std::string> pair(reason, bodyString);
        smsError(this, pair);
    }
}

Sid::List_String TextChatService::participantsMobileNumbers()
{
    Sid::List_String numbers;

    // Get the conversation's participants.
    ParticipantRefs callerList;
    m_conversation->GetParticipants(callerList, Conversation::OTHER_CONSUMERS);

    unsigned int size = callerList.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get the participant.
        SkypeParticipant::Ref participant(callerList[i]);

        SkypeContact::Ref contact = Util::contactFromParticipant(participant);

        Contact::TYPE type;
        contact->GetType(type);

        SEString number;

        // Get each's number and add it to our list.
        if (type == Contact::SKYPE)
            contact->GetPropPhoneMobile(number);
        else if (type == Contact::PSTN || type == Contact::FREE_PSTN)
            contact->GetPropPstnnumber(number);

        // Add it to the list if it's not empty.
        if (number.length() > 0)
            numbers.push_back(number);
    }

    return numbers;
}

std::vector<Poco::SharedPtr<Mvp::Model::MessageData> >
    TextChatService::messageHistory(unsigned int timestamp)
{
    std::vector<Poco::SharedPtr<Mvp::Model::MessageData> > messages;

    MessageRefs contextMessages;
    MessageRefs unconsumedMessages;
    m_conversation->GetLastMessages(
            contextMessages, unconsumedMessages, timestamp);

    int messageType = 0;
    unsigned int size = contextMessages.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get the message type.
        messageType = contextMessages[i]->GetProp(Message::P_TYPE).toInt();

        if (messageType != Message::POSTED_EMOTE &&
            messageType != Message::POSTED_TEXT &&
            messageType != Message::POSTED_SMS)
            continue;

        Poco::SharedPtr<MessageData> message =
            Util::dataFromMessage(contextMessages[i]);

        messages.push_back(message);
    }

    size = unconsumedMessages.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get the message type.
        messageType = unconsumedMessages[i]->GetProp(Message::P_TYPE).toInt();

        if (messageType != Message::POSTED_EMOTE &&
            messageType != Message::POSTED_TEXT &&
            messageType != Message::POSTED_SMS)
            continue;

        Poco::SharedPtr<MessageData> message =
            Util::dataFromMessage(unconsumedMessages[i]);

        messages.push_back(message);
    }

    return messages;
}

void TextChatService::leaveChat()
{
    // Leave the conversation.
    m_conversation->RetireFrom();
}

void TextChatService::consumeMessages()
{
    // Set all messages as consumed.
    m_conversation->SetConsumedHorizon((uint)time(NULL), true);
}

};

};
