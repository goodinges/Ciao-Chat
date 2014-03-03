#ifndef _CALLSTATE_H
#define _CALLSTATE_H

#include "state.h"
#include "skype-embedded_2.h"

namespace Mvp
{

namespace Model
{
class AbstractCallService;
};


namespace View
{
class AbstractView;
class AbstractCallView;
};

namespace Presenter
{
class CallPresenter;

/**
 * @class CallState callstate.h callstate.h
 * @brief Implementation of a call state needed to own all mvp objects for a call.
 *
 * This class implements a call state which owns a CallService, a
 * CallPresenter, and a CallView.
 *
 * @see State, IdleState, CallService, CallPresenter,
 * AbstractCallView.
 */
class CallState: public State
{
public:
    /**
     * Constructor
     *
     * @param context the context this state belongs to.
     * @param conversation the conversation for this call.
     */
    CallState(Context &context, const ConversationRef &conversation);

    /**
     * Destructor
     */
    ~CallState();

    /**
     * Perform any initialization needed.
     */
    void init();

    /**
     * Get the unique id of this state.
     *
     * @return a string that uniquely identifies this call state.
     */
    std::string id() const;

private:

    /**
     * Slot to react to manage conversation requests.
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
     * viewFactory->requestView(CallView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id, and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note the view should be of type AbstractCallView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair);

    ConversationRef m_conversation;

    /**
     * Model, Presenter and View needed for the call aspect of a conversation.
     */
    Mvp::Model::AbstractCallService *m_service;
    Mvp::Presenter::CallPresenter *m_presenter;
    Mvp::View::AbstractCallView *m_view;
    int m_viewRequestId;
};

};

};
#endif

