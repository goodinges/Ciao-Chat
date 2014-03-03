

#include "idlestate.h"

#include "chatstate.h"
#include "callstate.h"
#include "historystate.h"
#include "ftstate.h"
#include "model/skypecore.h"
#include "model/contactlistservice.h"
#include "presenter/contactlistpresenter.h"
#include "view/abstractviewfactory.h"

#include <Poco/Delegate.h>

#include <stdio.h>

namespace Mvp
{

namespace Presenter
{

IdleState::IdleState(Context &context)
    : State(context),
    m_service(0),
    m_presenter(0),
    m_view(0)
{
}

IdleState::~IdleState()
{
    // Delete the presenter first, so it can disconnect from the service.
    if (m_presenter) {
        // Disconnect from the SkypeCore newIncomingChat signal.
        Mvp::Model::SkypeCore::instance()->newChatConversation -=
            Poco::Delegate<IdleState, const ConversationRef>
            (this, &IdleState::newIncomingChat);

        Mvp::Model::SkypeCore::instance()->newSmsConversation -=
            Poco::Delegate<IdleState, const ConversationRef>
            (this, &IdleState::newIncomingSms);

        Mvp::Model::SkypeCore::instance()->newCallConversation -=
            Poco::Delegate<IdleState, const ConversationRef>
            (this, &IdleState::newIncomingCall);

        Mvp::Model::SkypeCore::instance()->newFileTransfer -=
            Poco::Delegate<IdleState, const TransferRef>
            (this, &IdleState::newFileTransfer);

        delete m_presenter;
    }

    delete m_service;

    if (m_view) {
        m_view->viewHistory -=
            Poco::Delegate<IdleState, Poco::EventArgs>
            (this, &IdleState::viewHistory);
        m_view->destroyView();
    }
}

void IdleState::stateFinished(const void *sender, Poco::EventArgs &args)
{
    // Emit our signal, so the LoggedOutState can logout.
    finished(this, args);
}

void IdleState::catchView(const void *sender,
    std::pair<int, Mvp::View::AbstractView *> &pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory so we can detach.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <IdleState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &IdleState::catchView);

        // Cache the view.
        m_view =
            reinterpret_cast<Mvp::View::AbstractContactListView *>(pair.second);
        m_view->viewHistory +=
            Poco::Delegate<IdleState, Poco::EventArgs>
            (this, &IdleState::viewHistory);

        // Give it to the presenter.
        m_presenter->setView(m_view);
        // Ask the presenter to show it.
        m_presenter->showView();
    }
}

void IdleState::newIncomingChat(const void *sender,
                                const ConversationRef &conversation)
{
    ConversationRef myconversation(conversation);

    printf("Creating a new ChatState\n");
    ChatState *newstate = new ChatState(m_parentContext, conversation);
    m_parentContext.pushState(newstate);
}

void IdleState::newIncomingSms(const void *sender,
                    const ConversationRef &conversation)
{
    ConversationRef myconversation(conversation);

    printf("Creating a new ChatState\n");
    ChatState *newstate = new ChatState(m_parentContext, conversation, true);
    m_parentContext.pushState(newstate);
}

void IdleState::newIncomingCall(const void *sender,
                                const ConversationRef &conversation)
{
    ConversationRef myconversation(conversation);

    printf("Creating a new CallState\n");
    CallState *newstate = new CallState(m_parentContext, conversation);
    m_parentContext.pushState(newstate);
}

void IdleState::viewHistory(const void *sender, Poco::EventArgs &args)
{
    HistoryState *newstate = new HistoryState(m_parentContext);
    m_parentContext.pushState(newstate);
}

void IdleState::newFileTransfer(const void *sender,
                                const TransferRef &transfer)
{
    printf("Creating a new FtState\n");
    FTState *newstate = new FTState(m_parentContext, transfer);
    m_parentContext.pushState(newstate);
}

void IdleState::init()
{
    // Allocate contact list service, and presenter.
    if (!m_service)
        m_service = new Mvp::Model::ContactListService;

    if (!m_presenter) {
        m_presenter = new ContactListPresenter(m_service);

        // Connect to the newIncomingChat signal from SkypeCore.
        Mvp::Model::SkypeCore::instance()->newChatConversation +=
            Poco::Delegate<IdleState, const ConversationRef>
            (this, &IdleState::newIncomingChat);

        Mvp::Model::SkypeCore::instance()->newSmsConversation +=
            Poco::Delegate<IdleState, const ConversationRef>
            (this, &IdleState::newIncomingSms);

        Mvp::Model::SkypeCore::instance()->newCallConversation +=
            Poco::Delegate<IdleState, const ConversationRef>
            (this, &IdleState::newIncomingCall);

        Mvp::Model::SkypeCore::instance()->newFileTransfer +=
            Poco::Delegate<IdleState, const TransferRef>
            (this, &IdleState::newFileTransfer);
    }

    if (!m_view) {
        // Get the factory so we can attach our catchView slot.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated += Poco::Delegate
            <IdleState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &IdleState::catchView);

        // Finally, request the view to be created.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::ContactListView);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<IdleState, Poco::EventArgs>
            (this, &IdleState::stateFinished);
    } else {
        m_presenter->showView();
    }
}

std::string IdleState::id() const
{
    return "IdleState";
}

};

};
