

#include "convmanagerstate.h"

#include "model/skypecore.h"
#include "model/convmanagerservice.h"
#include "presenter/convmanagerpresenter.h"
#include "view/abstractviewfactory.h"

#include <Poco/Delegate.h>

#include <stdio.h>

namespace Mvp
{

namespace Presenter
{

ConvManagerState::ConvManagerState(Context &context,
    const ConversationRef &conversation,
    Mvp::Model::ConvManagerService::Mode convMode)
    : State(context),
    m_conversation(conversation),
    m_service(0),
    m_presenter(0),
    m_view(0),
    m_convMode(convMode)
{
}


ConvManagerState::~ConvManagerState()
{
    // Delete the presenter first, since it disconnects itself from the service.
    delete m_presenter;
    delete m_service;

    if (m_view)
        m_view->destroyView();
}

void ConvManagerState::catchView(const void *sender,
    std::pair<int, Mvp::View::AbstractView *>&pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory so we can detach.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <ConvManagerState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &ConvManagerState::catchView);

        // Cache the view, give it to the presenter, and show it.
        m_view =
            reinterpret_cast<Mvp::View::AbstractConvManagerView *>(pair.second);
        m_presenter->setView(m_view);
        m_presenter->showView();
    }
}

void ConvManagerState::init()
{
    // Allocate contact list service, and presenter.
    if (!m_service)
        m_service =
            new Mvp::Model::ConvManagerService(m_conversation, m_convMode);

    if (!m_presenter)
        m_presenter = new ConvManagerPresenter(m_service);

    if (!m_view) {
        // Get the factory so we can attach our catchView slot.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated += Poco::Delegate
            <ConvManagerState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &ConvManagerState::catchView);

        // Finally, request the view to be created.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::ConvManagerView);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    } else {
        m_presenter->showView();
    }
}

std::string ConvManagerState::id() const
{
    SEString identity;
    m_conversation->GetPropIdentity(identity);
    return std::string("ConvManagerState_") + identity.data();
}

};

};
