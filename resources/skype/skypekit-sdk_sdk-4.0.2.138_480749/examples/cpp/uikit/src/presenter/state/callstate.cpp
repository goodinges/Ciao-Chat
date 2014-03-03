#include "callstate.h"

#include "convmanagerstate.h"

#include "model/skypecore.h"
#include "model/callservice.h"
#include "presenter/callpresenter.h"
#include "view/abstractviewfactory.h"
#include "view/abstractcallview.h"

#include <Poco/Delegate.h>

#include <stdio.h>

namespace Mvp
{

namespace Presenter
{

CallState::CallState(Context &context, const ConversationRef &conversation)
    : State(context),
    m_conversation(conversation),
    m_service(0),
    m_presenter(0),
    m_view(0)
{
    SEString identity;
    m_conversation->GetPropIdentity(identity);
    printf("new callstate created with id: %s\n", identity.data());
}

CallState::~CallState()
{
    // Delete the presenter first, since it disconnects itself from the service.
    if (m_presenter) {
        m_presenter->manageConversation -=
            Poco::Delegate<CallState, Poco::EventArgs>
            (this, &CallState::onManageConversation);
        delete m_presenter;
    }

    if (m_service) {
        m_service->conversationChanged -=
            Poco::Delegate<CallState, const ConversationRef>
            (this, &CallState::onConversationChanged);
        delete m_service;
    }

    if (m_view)
        m_view->destroyView();
}

void CallState::onManageConversation(const void *sender, Poco::EventArgs &args)
{
    // Create a new Conversation Manager State for our conversation.
    ConvManagerState *newState =
        new ConvManagerState(m_parentContext, m_conversation,
            Mvp::Model::ConvManagerService::Call);

    // Push the new state to the context.
    m_parentContext.pushState(newState);
}

void CallState::onConversationChanged(const void *sender,
    const ConversationRef &conversation)
{
    m_conversation = conversation;
}

void CallState::catchView(const void *sender,
    std::pair<int, Mvp::View::AbstractView *> &pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory so we can detach.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <CallState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &CallState::catchView);

        // Cache the view.
        m_view = reinterpret_cast<Mvp::View::AbstractCallView *>(pair.second);
        // Give it to the presenter.
        m_presenter->setView(m_view);
        // Ask the presenter to show it.
        m_presenter->showView();
    }
}

void CallState::init()
{
    // Allocate call service, and presenter.
    if (!m_service) {
        m_service = new Mvp::Model::CallService(m_conversation);
        m_service->conversationChanged +=
            Poco::Delegate<CallState, const ConversationRef>
            (this, &CallState::onConversationChanged);
    }

    if (!m_presenter) {
        m_presenter = new CallPresenter(m_service);
        m_presenter->manageConversation +=
            Poco::Delegate<CallState, Poco::EventArgs>
            (this, &CallState::onManageConversation);
    }

    if (!m_view) {
        // Get the factory so we can attach our catchView slot.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated += Poco::Delegate
            <CallState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &CallState::catchView);

        // Finally, request the view to be created.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::CallView);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    } else {
        m_presenter->showView();
    }
}

std::string CallState::id() const
{
    SEString identity;
    m_conversation->GetPropIdentity(identity);
    return std::string("CallState_") + identity.data();
}

};

};
