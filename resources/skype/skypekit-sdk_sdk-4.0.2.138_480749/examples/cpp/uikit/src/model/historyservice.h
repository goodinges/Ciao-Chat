
#ifndef _HISTORYSERVICE_H
#define _HISTORYSERVICE_H

#include "abstracthistoryservice.h"
#include "skypeconversation.h"
#include "skypemessage.h"

#include <Poco/Mutex.h>

namespace Mvp
{

namespace Model
{

/**
 * @class HistoryService historyservice.h historyservice.h
 * @brief Implements the Skype History Service.
 *
 * This class provides a way to fetch the user's event history and listen to
 * history changes.
 *
 * @see AbstractHistoryService
 */
class HistoryService: public AbstractHistoryService
{
public:
    /**
     * Constructor.
     */
    HistoryService();

    /**
     * Destructor.
     */
    ~HistoryService();

    /**
     * Returns a list of history data.
     *
     * When requested the method will return a vector containing a set of
     * history data representing each real Skype Conversation. This history
     * data only provides some data about the real conversation and is
     * identified by the conversation's identity value.
     *
     * @param hasUnread comes back true if there are unread items in the history.
     * @returns a vector containing history data.
     * @see HistoryData
     */
    std::vector<Poco::SharedPtr<HistoryData> > getHistory(bool &hasUnread);

    /**
     * Mark all history item messages as read.
     */
    void markMessagesRead();

private:
    /**
     * Add history data from a conversation to the vector.
     *
     * This is a helper method that getHistory uses to generate the history data
     * from a given conversation.
     *
     * @param conversation the conversation to get the messages from.
     * @param history the vector of history data to add the history data to.
     * @returns true if unread items were found.
     */
    bool addDataFromConversation(const ConversationRef &conversation,
        std::vector<Poco::SharedPtr<HistoryData> > &history);

    /**
     * Slot to react to history item messages coming in.
     *
     * @param sender a pointer to the object that sent the message.
     * @param message the message to get the history data from.
     */
    void onNewHistoryItem(const void *sender, const MessageRef &message);

    /**
     * Slot to react to history item changes coming in.
     *
     * @param sender a pointer to the object that sent the message.
     * @param message the message to get the history data from.
     */
    void onMessageChanged(const void *sender, const MessageRef &message);

    /**
     * Slot to react to conversation spawnedConference signals.
     *
     * @param sender a pointer to the object that sent the message.
     * @param conference the conference spawned from the conversation.
     */
    void onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference);

    /**
     * Helper method to generate HistoryData from a Message.
     *
     * This method is used by addDataFromConversation to create a HistoryData
     * object based on the contents of the given message.  It needs the
     * HistoryType which is a combination of the messagetype and the
     * conversation type.
     *
     * @param message the message to generate the HistoryData from.
     * @param historyType the basic HistoryType
     *
     * @note this method modifies the historyType of the HistoryData returned
     * based on some properties of the message passed in.
     *
     * @returns a pointer to a new HistoryData object filled in.
     */
    Poco::SharedPtr<HistoryData> dataFromMessage(const MessageRef &message,
        HistoryType historyType);

    /**
     * Test the message to see if we should include it in the history.
     *
     * @note the type, convType, and foundMessage are passed by reference so
     *   the calling method can evaluate their changes if they are changed.
     *
     * @param message the message to check.
     * @param type the history type of the message.
     * @param convType the conversation type the message comes from.
     * @param foundMessage a flag so we know if we've found a message from
     *      this conversation already.
     */
    bool checkMessage(MessageRef &message, HistoryType &type,
        Conversation::TYPE convType, bool &foundMessage);

    void connectToConversation(SkypeConversation::Ref &conversation);

    void connectToMessage(SkypeMessage::Ref &message);

    /**
     * Helper method to disconnect  from conversations and message objects.
     */
    void clearCaches();

    std::vector<SkypeConversation::Ref> m_conversations;
    std::vector<SkypeMessage::Ref> m_messages;

    Poco::Mutex m_mutex;
};

};

};

#endif

