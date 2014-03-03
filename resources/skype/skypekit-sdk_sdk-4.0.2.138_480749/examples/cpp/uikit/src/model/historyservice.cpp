
#include "historyservice.h"

#include "skypecore.h"
#include "skypeconversation.h"
#include "util.h"

#include <stdio.h>

#include <Poco/Delegate.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/DOMSerializer.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeList.h>

namespace Mvp
{

namespace Model
{

HistoryService::HistoryService()
{
}

HistoryService::~HistoryService()
{
    clearCaches();
}

std::vector<Poco::SharedPtr<HistoryData> > HistoryService::getHistory(
    bool &hasUnread)
{
    // Initialize hasUnread to false until proven otherwise
    hasUnread = false;

    // Clear the caches and disconnect.
    clearCaches();

    // Get the Skype Core instance.
    SkypeCore *skype = SkypeCore::instance();

    // Make a vector to store our history in.
    std::vector<Poco::SharedPtr<HistoryData> > history;

    /* Fetch all the conversations from SkypeCore.
     *
     * In order to get a history of conversations first we need to decide which
     * conversations we are interested in. The available groups are:
     *
     * - ALL_CONVERSATIONS : Conversations from the last 30 days;
     * - INBOX_CONVERSATIONS : Conversations from the last 6 months;
     * - BOOKMARKED_CONVERSATIONS : Conversations the user has bookmarked;
     * - LIVE_CONVERSATIONS : Conversations whose local_livestatus is not NONE;
     * - REALLY_ALL_CONVERSATIONS : All Conversations;
     *
     * We will ask for INBOX_CONVERSATIONS to get the last 6 months worth.
     */

    ConversationRefs list;
    skype->GetConversationList(list, Conversation::INBOX_CONVERSATIONS);

    /* Now that we have the conversations we are interested in, we need to
     * extract the messages from them and convert the ones we are interested
     * in into HistoryData objects on our vector.
     */
    unsigned int size = list.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get a reference to the Conversation.
        SkypeConversation::Ref conversation(list[i]->getOID());

        /* Once we have the conversation we need to connect to a signal that
         * will notify us of any new messages in the conversation.
         */
        connectToConversation(conversation);
        m_conversations.push_back(conversation);

        if (addDataFromConversation(conversation, history))
            hasUnread = true;
    }

    return history;
}

void HistoryService::clearCaches()
{
    Poco::Mutex::ScopedLock lock(m_mutex);

    // Disconnect from previous conversations.
    unsigned int size = m_conversations.size();
    for (unsigned int i = 0; i < size; ++i) {
        m_conversations[i]->newHistoryItem -=
            Poco::Delegate<HistoryService, const MessageRef>
            (this, &HistoryService::onNewHistoryItem);

        m_conversations[i]->spawnedConference -=
            Poco::Delegate<HistoryService, const SkypeConversation::Ref>
            (this, &HistoryService::onSpawnedConference);
    }


    // Clear our cache of conversations.
    m_conversations.clear();

    // Disconnect from previous messages.
    size = m_messages.size();
    for (unsigned int i = 0; i < size; ++i)
        m_messages[i]->messageChanged -=
            Poco::Delegate<HistoryService, const MessageRef>
            (this, &HistoryService::onMessageChanged);

    m_messages.clear();
}

void HistoryService::markMessagesRead()
{
    Poco::Mutex::ScopedLock lock(m_mutex);

    // For each conversation
    time_t now = time(NULL);
    for (unsigned int i = 0; i < m_conversations.size(); ++i)
        // Set their consumed horizon to now.
        m_conversations[i]->SetConsumedHorizon((uint)now, true);
}

void HistoryService::connectToConversation(SkypeConversation::Ref &conversation)
{
    conversation->newHistoryItem +=
        Poco::Delegate<HistoryService, const MessageRef>
        (this, &HistoryService::onNewHistoryItem);

    conversation->spawnedConference +=
        Poco::Delegate<HistoryService, const SkypeConversation::Ref>
        (this, &HistoryService::onSpawnedConference);
}

void HistoryService::onSpawnedConference(const void *sender,
    const SkypeConversation::Ref &conference)
{
    SkypeConversation::Ref skypeConference(conference);
    connectToConversation(skypeConference);
    m_conversations.push_back(skypeConference);
}

void HistoryService::connectToMessage(SkypeMessage::Ref &message)
{
    message->messageChanged +=
        Poco::Delegate<HistoryService, const MessageRef>
        (this, &HistoryService::onMessageChanged);
}

bool HistoryService::addDataFromConversation(
    const ConversationRef &conversation,
    std::vector<Poco::SharedPtr<HistoryData> > &history)
{
    bool foundUnread = false;
    bool foundMessage = false;
    Poco::SharedPtr<HistoryData> imData;

    time_t seconds = time(NULL);
    // Go back a week.
    seconds -= (86400 * 1);

    SEString convDisplayName;
    SEString convTopic;
    SEString convIdentity;
    Conversation::TYPE convType;

    // Get the conversation properties.
    conversation->GetPropDisplayname(convDisplayName);
    conversation->GetPropMetaTopic(convTopic);
    conversation->GetPropIdentity(convIdentity);
    conversation->GetPropType(convType);

    MessageRefs contextMessages;
    MessageRefs unconsumedMessages;
    conversation->GetLastMessages(contextMessages, unconsumedMessages, (uint)seconds);

    unsigned int size = contextMessages.size();
    for (unsigned int i = 0; i < size; ++i) {
        MessageRef message = contextMessages[i];

        HistoryType historyType = IncomingCall;

        Message::TYPE type;
        message->GetPropType(type);

        // Figure out if we care about this message.
        if (checkMessage(message, historyType, convType, foundMessage)) {
            // If we care, populate the type, read, text, messageId and timestamp.
            Poco::SharedPtr<HistoryData> newData =
                dataFromMessage(message, historyType);

            // If it's a text message, cache it in imData.
            if (foundMessage && (type == Message::POSTED_TEXT ||
                                 type == Message::POSTED_EMOTE))
                imData = newData;

            SkypeMessage::Ref skypeMessage(message->getOID());
            connectToMessage(skypeMessage);
            m_messages.push_back(skypeMessage);

            newData->conversationIdentity = convIdentity;
            newData->displayName = convDisplayName;

            if (!newData->read)
                foundUnread = true;

            // Push the new HistoryData to the vector.
            history.push_back(newData);
        }
        else if (imData && (type == Message::POSTED_EMOTE ||
                            type == Message::POSTED_TEXT)) {
            // Get the read property from Consumption Status.
            Message::CONSUMPTION_STATUS read;
            message->GetPropConsumptionStatus(read);
            // It's read if it's been consumed, otherwise it's unread.
            imData->read = (read == Message::CONSUMED);
            if (!imData->read)
                foundUnread = true;

            // Get the timestamp straight from the message.
            message->GetPropTimestamp(imData->timestamp);
            SkypeMessage::Ref skypeMessage(message->getOID());
            connectToMessage(skypeMessage);
            m_messages.push_back(skypeMessage);
        }
    }

    size = unconsumedMessages.size();
    for (unsigned int i = 0; i < size; ++i) {
        MessageRef message = unconsumedMessages[i];

        HistoryType historyType = IncomingCall;

        Message::TYPE type;
        message->GetPropType(type);

        // Figure out if we care about this message.
        if (checkMessage(message, historyType, convType, foundMessage)) {

            // If we care, populate the type, read, text, messageId and timestamp.
            Poco::SharedPtr<HistoryData> newData =
                dataFromMessage(message, historyType);

            SkypeMessage::Ref skypeMessage(message->getOID());
            connectToMessage(skypeMessage);
            m_messages.push_back(skypeMessage);

            newData->conversationIdentity = convIdentity;
            if (convTopic.length() > 0)
                newData->displayName = convTopic;
            else
                newData->displayName = convDisplayName;

            if (!newData->read)
                foundUnread = true;

            // Push the new HistoryData to the vector.
            history.push_back(newData);
        }
        else if (imData && (type == Message::POSTED_EMOTE ||
                            type == Message::POSTED_TEXT)) {
            // Get the read property from Consumption Status.
            Message::CONSUMPTION_STATUS read;
            message->GetPropConsumptionStatus(read);
            // It's read if it's been consumed, otherwise it's unread.
            imData->read = (read == Message::CONSUMED);
            if (!imData->read)
                foundUnread = true;

            // Get the timestamp straight from the message.
            message->GetPropTimestamp(imData->timestamp);
            SkypeMessage::Ref skypeMessage(message->getOID());
            connectToMessage(skypeMessage);
            m_messages.push_back(skypeMessage);
        }
    }

    return foundUnread;
}

bool HistoryService::checkMessage(MessageRef &message, HistoryType &historyType,
    Conversation::TYPE convType, bool &foundMessage)
{
    bool addMessage = false;

    Message::TYPE type;
    message->GetPropType(type);

    historyType = IncomingCall;

    switch (type) {
        case Message::STARTED_LIVESESSION:
            addMessage = true;
            if (convType == Conversation::CONFERENCE ||
                convType == Conversation::TERMINATED_CONFERENCE ||
                convType == Conversation::LEGACY_VOICE_CONFERENCE)
                historyType = ConferenceIn;
            foundMessage = true;
            break;
        case Message::POSTED_TEXT:
        case Message::POSTED_EMOTE:
            if (!foundMessage) {
                addMessage = true;
                historyType = Chat;
                foundMessage = true;
            }
            break;
        case Message::POSTED_FILES:
            addMessage = true;
            historyType = IncomingTransfer;
            break;
        case Message::POSTED_VOICE_MESSAGE:
            addMessage = true;
            historyType = VoiceMail;
		default:
            break;
    }

    return addMessage;
}

void HistoryService::onNewHistoryItem(const void *sender,
    const MessageRef &message)
{
    SkypeMessage::Ref skypeMessage(message->getOID());

    connectToMessage(skypeMessage);
    m_messages.push_back(message);

    Poco::EventArgs args;
    historyChanged(this, args);
}

void HistoryService::onMessageChanged(
    const void *sender, const MessageRef &message)
{
    Poco::EventArgs args;
    historyChanged(this, args);
}

Poco::SharedPtr<HistoryData> HistoryService::dataFromMessage(
    const MessageRef &message, HistoryType historyType)
{
    // Get the Skype Core instance.
    SkypeCore *skype = SkypeCore::instance();

    // If we care, populate the type, read, text, messageId and timestamp.
    Poco::SharedPtr<HistoryData> newData =
        Poco::SharedPtr<HistoryData>(new HistoryData);

    // Get the type.
    newData->type = historyType;

    SEString string;
    std::string myIdentity(skype->currentIdentity().data());

    // Get the read property from Consumption Status.
    Message::CONSUMPTION_STATUS read;
    message->GetPropConsumptionStatus(read);
    // It's read if it's been consumed, otherwise it's unread.
    newData->read = (read == Message::CONSUMED);

    // Get the text if any.
    if (historyType == Chat) {
        message->GetPropBodyXml(string);
        newData->text = string.data();
    }
    else if (historyType == IncomingCall || historyType == OutgoingCall
        || historyType == ConferenceIn) {
        message->GetPropBodyXml(string);
        Poco::XML::XMLString xmldata = string.data();

        if (xmldata.length() > 0) {
            Poco::XML::DOMParser parser;
            Poco::XML::Document *document = parser.parseString(xmldata);

            Poco::XML::XMLString part("part");
            Poco::XML::NodeList *elements = document->getElementsByTagName(part);

            Poco::XML::XMLString duration("duration");

            unsigned long length = elements->length();
            for (unsigned long i = 0; i < length; ++i) {
                Poco::XML::Node *node = elements->item(i);
                Poco::XML::NamedNodeMap *attributes = node->attributes();
                Poco::XML::Node *identityNode =
                    attributes->getNamedItem("identity");
                if (identityNode->nodeValue() == myIdentity) {
                    // See if this node has a "duration" node.
                    bool foundDuration = false;

                    Poco::XML::NodeList *children = node->childNodes();
                    unsigned long nodecount = children->length();
                    for (unsigned long n = 0; n < nodecount; ++n) {
                        if (children->item(n)->nodeName() == duration) {
                            foundDuration = true;
                            break;
                        }
                    }
                    children->release();

                    if (!foundDuration)
                        if (newData->type == IncomingCall)
                            newData->type = MissedCall;
                }
                attributes->release();
            }

            // Release each element
            elements->release();
        }
    }

    // To determine the type we might need the author.
    message->GetPropAuthor(string);

    // If the author was me, and it was a call, it was Outgoing.
    if (skype->currentIdentity() == string)
        if (newData->type == IncomingCall)
            newData->type = OutgoingCall;
        else if (newData->type == MissedCall)
            newData->type = FailedCall;
        else if (newData->type == ConferenceIn)
            newData->type = ConferenceOut;
        else if (newData->type == IncomingTransfer)
            newData->type = OutgoingTransfer;

    // Get the messageId.
    SEBinary rawData;
    message->GetPropGuid(rawData);
    newData->messageId.assignRaw(rawData.data(), rawData.size());

    // Get the timestamp straight from the message.
    message->GetPropTimestamp(newData->timestamp);

    return newData;
}

};

};
