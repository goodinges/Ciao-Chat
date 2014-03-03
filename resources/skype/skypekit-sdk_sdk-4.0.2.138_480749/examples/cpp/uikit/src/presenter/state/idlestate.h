
#ifndef _IDLESTATE_H
#define _IDLESTATE_H

#include "state.h"
#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{
class AbstractContactListService;
};


namespace View
{
class AbstractView;
class AbstractContactListView;
};

namespace Presenter
{
class ContactListPresenter;


/**
 * @class IdleState idlestate.h idlestate.h
 * @brief Implementation of the idle state the application is in after login.
 *
 * This class implements the basic idle state that the application is in
 * once login is successful.  The primary purpose of this state is to show
 * and manage the contact list and launch chat/call states as needed. It also
 * creates and owns a ContactListService, ContactListPresenter and an
 * AbstractContactListView.
 *
 * @see State, LoggedOutState, ContactListService, ContactListPresenter,
 * AbstractContactListView.
 */
class IdleState: public State
{
public:
    /**
     * Constructor
     *
     * @param context The context this state belongs to.
     */
    IdleState(Context &context);

    /**
     * Destructor
     */
    ~IdleState();

    /**
     * Perform any initialization needed.
     */
    void init();

    /**
     * Signal the idlestate is finished.
     *
     * This signal is to let the logeddoutstate know we are done, and it should
     * log out.
     */
    Poco::BasicEvent<Poco::EventArgs> finished;

    /**
     * Overriden slot to react to finished signal from presenter.
     *
     * We override the stateFinished here, so we can emit our own finished
     * signal to let the LoggedOutState know it should log out.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void stateFinished(const void *sender, Poco::EventArgs &);

    /**
     * Get the unique id of this state.
     *
     * @return a string that uniquely identifies this state.
     */
    std::string id() const;

private:
    /**
     * Slot to catch the view created by the viewfactory.
     *
     * View creation is asynchronous so this slot is needed to get
     * the pointer to the view after it is requested by
     * viewFactory->requestView(ContactListView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note the view should be of type AbstractContactListView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair);

    /**
     * Slot to react to new incoming chat signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conversation the new incoming chat's conversation.
     */
    void newIncomingChat(const void *sender,
        const ConversationRef &conversation);

    /**
     * Slot to react to new incoming sms signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conversation the new incoming sms's conversation.
     */
    void newIncomingSms(const void *sender,
                         const ConversationRef &conversation);

    /**
     * Slot to react to new incoming call signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conversation the new incoming call's conversation.
     */
    void newIncomingCall(const void *sender,
        const ConversationRef &conversation);

    /**
     * Slot to react to new incoming file transfer signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param transfer the new incoming filetranfer object.
     */
    void newFileTransfer(const void *sender, const TransferRef &transfer);

    /**
     * Slot to handle viewHistory requests from the view.
     *
     * This method creates a new history state and pushes it onto the context.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void viewHistory(const void *sender, Poco::EventArgs &args);

    /**
     * Model, Presenter and View needed to show the contact list of the logged
     * in account.
     */
    Mvp::Model::AbstractContactListService *m_service;
    Mvp::Presenter::ContactListPresenter *m_presenter;
    Mvp::View::AbstractContactListView *m_view;
    int m_viewRequestId;
};

};

};
#endif
