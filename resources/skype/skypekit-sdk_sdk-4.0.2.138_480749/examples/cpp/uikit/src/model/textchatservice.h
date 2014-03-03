#ifndef _TEXTCHATSERVICE_H_
#define _TEXTCHATSERVICE_H_

#include <string>

#include "abstractchatservice.h"
#include "skypeconversation.h"
#include "skypeparticipant.h"

namespace Mvp
{

namespace Model
{

/**
 * @class TextChatService textchatservice.h textchatservice.h
 * @brief Implements a simple Text Chat Service.
 *
 * This class represents the interaction between a textchat window and the
 * SkypeKit itself.  All incoming messages on the conversation will be emitted
 * from this object, and all outgoing messages both IM and Sms will come from
 * the view into this service to be given to the skypekit.
 *
 * @see AbstractChatService
 */
class TextChatService : public AbstractChatService
{
public:
    /**
     * Constructor
     *
     * Every TextChatService needs a conversation to do its work, so we take
     * the ConversationRef as a parameter at construction time.
     *
     * @param conversation the conversation to use for this text chat.
     */
    TextChatService(const ConversationRef &conversation);

    /**
     * Destructor, leaves the conversation.
     */
    ~TextChatService();

    /**
     * Returns if the current account is a consumer of the conversation.
     *
     * @returns true if the account is a consumer, false if it's not.
     */
    bool isConsumer();

    /**
     * Returns the conversation participants list.
     *
     * @returns a list of contacts that are participating in this conversation.
     */
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > getParticipants();

    /**
     * Slot to handle incoming messages from the conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the message information that came in.
     */
    void onIncomingMessage(const void *sender,
        Poco::SharedPtr<MessageData> &message);

    /**
     * Post a message to the conversation.
     *
     * @param messageText the text to send to the conversation.
     */
    void postMessage(std::string &messageText);

    /**
     * Slot to handle im message changed message.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status the new message status.
     */
    void onImStatusChanged(const void *sender, Message::SENDING_STATUS &status);

    /**
     * Method to handle posting a sms to the conversation.
     *
     * @param messageText the text to post to the sms.
     */
    void postSMS(std::string &messageText);

    /**
     * Method to handle sms status changed message.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status the new status of the sms message.
     */
    void onSmsStatusChanged(const void *sender, Sms::STATUS &status);

    /**
     * Slot to handle when a conference is spawned from the first conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conference the new conversation spawned from the old one.
     */
    void onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference);

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onParticipantListChanged(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle when a contact changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the contactref that has changed.
     */
    void onContactChanged(const void *sender, ContactRef &contact);

    /**
     * Accessor to get the chat name.
     *
     * @returns the name of the conversation.
     */
    std::string chatName();

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
    std::vector<Poco::SharedPtr<Mvp::Model::MessageData> >
        messageHistory(unsigned int timestamp = 0);

    /**
     * Method to leave the conversation.
     *
     * When the conversation is the conference type, it's possible to leave the
     * conversation, which will imply in do not receive any other message
     * posted in the given conversation by other peers.
     */
    void leaveChat();

    /**
     * Method to mark all messages as consumed.
     *
     * The consumed flag is similar to marking a message as read or unread.
     * This method will mark all arrived messages of the given conversation
     * as consumed (read).
     */
    void consumeMessages();

private:
    /**
     * Helper method to get participants sms numbers.
     *
     * @returns a list of strings of participant's mobile numbers.
     */
    Sid::List_String participantsMobileNumbers();

    void connectToConversation(const ConversationRef &conversation);
    void disconnectFromConversation();

    void connectToParticipant(SkypeParticipant::Ref &participant);
    void disconnectFromParticipant(SkypeParticipant::Ref &participant);

    SkypeConversation::Ref m_conversation;
    std::vector<SkypeParticipant::Ref> m_participants;
};

};

};

#endif
