
#ifndef _CHATSTATE_H
#define _CHATSTATE_H

#include "state.h"
#include "skype-embedded_2.h"

namespace Mvp
{

namespace Model
{
class AbstractChatService;
};


namespace View
{
class AbstractView;
class AbstractChatView;
};

namespace Presenter
{
class TextChatPresenter;


/**
 * @class ChatState chatstate.h chatstate.h
 * @brief Implementation of a chat state needed to own all mvp objects for a chat.
 *
 * This class implements a chat state which owns a TextChatService, a
 * TextChatPresenter, and a TextChatView.
 *
 * @see State, IdleState, TextChatService, TextChatPresenter,
 * AbstractChatView.
 */
class ChatState: public State
{
public:
    /**
     * Constructor
     *
     * @param context the context this state belongs to.
     * @param conversation the conversation for this chat.
     * @param smsMode true to begin the chat in sms mode, false to start in IM.
     */
    ChatState(Context &context, const ConversationRef &conversation,
        bool smsMode = false);

    /**
     * Destructor
     */
    ~ChatState();

    /**
     * Perform any initialization needed.
     */
    void init();

    /**
     * Get the unique id of this state.
     *
     * @return a string that uniquely identifies this chat state.
     */
    std::string id() const;

private:
    /**
     * Slot to invoke conversation management with.
     *
     * This method reacts to requests to manage the conversation.  It's only
     * job is to create a convmanager state for this state's conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onManageConversation(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the conversation changed signal.
     *
     * This method reacts to changes in the conversation (when a conference
     * is spawned from a conversation).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conversation the new conversation this call is using.
     */
    void onConversationChanged(const void *sender,
        const ConversationRef &conversation);

    /**
     * Slot to catch the view created by the viewfactory.
     *
     * View creation is asynchronous so this slot is needed to get
     * the pointer to the view after it is requested by
     * viewFactory->requestView(TextChatView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id, and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note the view should be of type AbstractTextChatView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair);

    ConversationRef m_conversation;

    /**
     * Model, Presenter and View needed for the chat of a conversation.
     */
    Mvp::Model::AbstractChatService *m_service;
    Mvp::Presenter::TextChatPresenter *m_presenter;
    Mvp::View::AbstractChatView *m_view;
    bool m_startInSmsMode;
    int m_viewRequestId;
};

};

};
#endif

