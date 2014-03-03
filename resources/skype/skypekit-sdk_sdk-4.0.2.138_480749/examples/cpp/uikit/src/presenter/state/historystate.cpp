

#include "historystate.h"

#include "model/historyservice.h"
#include "presenter/historypresenter.h"
#include "view/abstractviewfactory.h"

#include <Poco/Delegate.h>

#include <stdio.h>

namespace Mvp
{

namespace Presenter
{

HistoryState::HistoryState(Context &context)
    : State(context),
    m_service(0),
    m_presenter(0),
    m_view(0)
{
}

HistoryState::~HistoryState()
{
    // Delete the presenter first, since it disconnects itself from the service.
    delete m_presenter;
    delete m_service;

    if (m_view)
        m_view->destroyView();
}

void HistoryState::catchView(const void *sender,
    std::pair<int, Mvp::View::AbstractView *> &pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory so we can detach.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <HistoryState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &HistoryState::catchView);

        // Cache the view.
        m_view = reinterpret_cast<Mvp::View::AbstractHistoryView *>(pair.second);
        // Give it to the presenter.
        m_presenter->setView(m_view);
        // Ask the presenter to show it.
        m_presenter->showView();
    }
}

void HistoryState::init()
{
    printf("Initializing history state object.\n");
    // Allocate contact list service, and presenter.
    if (!m_service)
        m_service = new Mvp::Model::HistoryService;

    if (!m_presenter)
        m_presenter = new HistoryPresenter(m_service);

    if (!m_view) {
        // Get the factory so we can attach our catchView slot.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated += Poco::Delegate
            <HistoryState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &HistoryState::catchView);

        // Finally, request the view to be created.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::HistoryView);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    } else {
        m_presenter->showView();
    }
}

std::string HistoryState::id() const
{
    return std::string("HistoryState");
}

};

};

