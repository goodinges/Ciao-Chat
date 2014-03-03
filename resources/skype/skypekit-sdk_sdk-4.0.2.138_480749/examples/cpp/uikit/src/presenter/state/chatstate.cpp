

#include "chatstate.h"

#include "convmanagerstate.h"

#include "model/skypecore.h"
#include "model/textchatservice.h"
#include "presenter/textchatpresenter.h"
#include "view/abstractviewfactory.h"

#include <Poco/Delegate.h>

#include <stdio.h>

namespace Mvp
{

namespace Presenter
{

ChatState::ChatState(Context &context, const ConversationRef &conversation,
    bool smsMode)
    : State(context),
    m_conversation(conversation),
    m_service(0),
    m_presenter(0),
    m_view(0),
    m_startInSmsMode(smsMode)
{
}

ChatState::~ChatState()
{
    // Delete the presenter first, since it disconnects itself from the service.
    if (m_presenter) {
        m_presenter->manageConversation -=
            Poco::Delegate<ChatState, Poco::EventArgs>
            (this, &ChatState::onManageConversation);
        delete m_presenter;
    }

    if (m_service) {
        m_service->conversationChanged -=
            Poco::Delegate<ChatState, const ConversationRef>
            (this, &ChatState::onConversationChanged);
        delete m_service;
    }

    if (m_view)
        m_view->destroyView();
}

void ChatState::catchView(const void *sender,
    std::pair<int, Mvp::View::AbstractView *> &pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory so we can detach.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <ChatState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &ChatState::catchView);

        // Cache the view.
        m_view = reinterpret_cast<Mvp::View::AbstractChatView *>(pair.second);
        // Give it to the presenter.
        m_presenter->setView(m_view);
        // Ask the presenter to show it.
        m_presenter->showView();
    }
}

void ChatState::init()
{
    // Allocate contact list service, and presenter.
    if (!m_service) {
        m_service = new Mvp::Model::TextChatService(m_conversation);
        m_service->conversationChanged +=
            Poco::Delegate<ChatState, const ConversationRef>
            (this, &ChatState::onConversationChanged);
    }

    if (!m_presenter) {
        m_presenter = new TextChatPresenter(m_service, m_startInSmsMode);
        m_presenter->manageConversation +=
            Poco::Delegate<ChatState, Poco::EventArgs>
            (this, &ChatState::onManageConversation);
    }

    if (!m_view) {
        // Get the factory so we can attach our catchView slot.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated += Poco::Delegate
            <ChatState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &ChatState::catchView);

        // Finally, request the view to be created.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::TextChatView);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    } else {
        m_presenter->showView();
    }
}

void ChatState::onManageConversation(const void *sender, Poco::EventArgs &args)
{
    ConvManagerState *newState =
        new ConvManagerState(m_parentContext, m_conversation,
            Mvp::Model::ConvManagerService::Chat);
    m_parentContext.pushState(newState);
}

void ChatState::onConversationChanged(const void *sender,
    const ConversationRef &conversation)
{
    m_conversation = conversation;
}

std::string ChatState::id() const
{
    SEString identity;
    m_conversation->GetPropIdentity(identity);
    return std::string("ChatState_") + identity.data();
}

};

};
