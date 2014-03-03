
#ifndef _ABSTRACTCHATSERVICE_H
#define _ABSTRACTCHATSERVICE_H

#include <string>

#include "messagedata.h"
#include "skypeconversation.h"
#include "contactdata.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractChatService abstractchatservice.h abstractchatservice.h
 * @brief Abstract Text Chat Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the text chat services, such as, and specifically getting and sending
 * messages of both IM and Sms types.
 *
 * @note This class is abstract. If you're looking for a concrete one
 * look at TextChatService.
 *
 */
class AbstractChatService
{
public:
    /**
     * Constructor
     */
    AbstractChatService() {}

    /**
     * Virtual destructor
     */
    virtual ~AbstractChatService() {}

    /**
     * Returns if the current account is a consumer of the conversation.
     *
     * @returns true if the account is a consumer, false if it's not.
     */
    virtual bool isConsumer() = 0;

    /**
     * Returns the conversation participants list.
     *
     * @returns a list of contacts.
     */
    virtual std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >
        getParticipants() = 0;

    /**
     * Slot to handle incoming messages from the conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the message information that came in.
     */
    virtual void onIncomingMessage(const void *sender,
        Poco::SharedPtr<MessageData> &message) = 0;

    /**
     * Slot to handle when a conference is spawned from the first conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conference the new conversation spawned from the old one.
     */
    virtual void onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference) = 0;

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onParticipantListChanged(const void *sender,
        Poco::EventArgs &args) = 0;

    /**
     * Signal that a new message was received.
     */
    Poco::BasicEvent<Poco::SharedPtr<MessageData> > messageReceived;

    /**
     * Method to handle posting a message to the conversation.
     *
     * @param messageText the text to post to the chat.
     */
    virtual void postMessage(std::string &messageText) = 0;

    /**
     * Method to handle posting a sms to the conversation.
     *
     * @param messageText the text to post to the sms.
     */
    virtual void postSMS(std::string &messageText) = 0;

    /**
     * Accessor to get the conversation name.
     *
     * In one to one conversations the chat name will be the name of the other
     * person on the chat.  In other conversations the chat name can be set
     * by the user. This should always be a displayable string.
     *
     * @returns the name of the conversation this service is watching.
     */
    virtual std::string chatName() = 0;

    /**
     * Returns the message history.
     *
     * The method will return a list of old messages that were handled by the
     * given conversation. The default timestamp is zero, which means that
     * the messages from the last 24 hours will be fetched and returned.
     *
     * @param timestamp the timestamp to fetch the messages.
     *
     * @returns the list of old messages from the conversation.
     */
    virtual std::vector<Poco::SharedPtr<Mvp::Model::MessageData> >
        messageHistory(unsigned int timestamp = 0) = 0;

    /**
     * Method to leave the conversation.
     *
     * When the conversation is the conference type, it's possible to leave the
     * conversation, which will imply in do not receive any other message
     * posted in the given conversation by other peers.
     */
    virtual void leaveChat() = 0;

    /**
     * Method to mark all messages as consumed.
     *
     * The consumed flag is similar to marking a message as read or unread.
     * This method will mark all arrived messages of the given conversation
     * as consumed (read).
     */
    virtual void consumeMessages() = 0;

    /**
     * Signal that the participant list has changed.
     */
    Poco::BasicEvent<Poco::EventArgs> participantListChanged;

    /**
     * Signal for contact changes notification.
     *
     * This signal will be emitted whenever a Skype Contact has its properties
     * changed. Thus it acts as a central point for change notifications.
     */
    Poco::BasicEvent<Poco::SharedPtr<ContactData> > contactChanged;

    /**
     * Signal that the current conversation object has changed.
     *
     * As the Conversation object can change during the Chat Service life tine,
     * it's necessary to have an signal responsible to propagate the new object
     * to other classes.
     */
    Poco::BasicEvent<const ConversationRef> conversationChanged;

    /**
     * Signal that the sms has been delivered successfully.
     */
    Poco::BasicEvent<std::string> smsDelivered;

    /**
     * Send an error message about failures in sms.
     */
    Poco::BasicEvent<std::pair<Sms::FAILUREREASON, std::string> > smsError;

    /**
     * Signal that the im message failed to be sent.
     *
     * The value of the string is the text of the message that failed.
     */
    Poco::BasicEvent<std::string> imFailed;
};

};

};
#endif
