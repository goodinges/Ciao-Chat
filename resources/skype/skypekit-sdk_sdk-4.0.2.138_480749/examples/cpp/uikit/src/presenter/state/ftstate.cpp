#include "ftstate.h"

#include "model/skypecore.h"
#include "model/ftservice.h"
#include "presenter/ftpresenter.h"
#include "view/abstractviewfactory.h"
#include "view/abstractftview.h"

#include <Poco/Delegate.h>

#include <stdio.h>
#include <sstream>

namespace Mvp
{

namespace Presenter
{

FTState::FTState(Context &context, const TransferRef &transfer)
    : State(context),
    m_transfer(transfer),
    m_service(0),
    m_presenter(0),
    m_view(0)
{
}

FTState::~FTState()
{
    // Delete the presenter first, since it disconnects itself from the service.
    delete m_presenter;
    delete m_service;

    if (m_view)
        m_view->destroyView();
}

void FTState::catchView(const void *sender,
    std::pair<int, Mvp::View::AbstractView *> &pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory so we can detach.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <FTState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &FTState::catchView);

        // Cache the view.
        m_view = reinterpret_cast<Mvp::View::AbstractFTView *>(pair.second);
        // Give it to the presenter.
        m_presenter->setView(m_view);
        // Ask the presenter to show it.
        m_presenter->showView();
    }
}

void FTState::init()
{
    // Allocate ft service, and presenter.
    if (!m_service)
        m_service = new Mvp::Model::FTService(m_transfer);

    if (!m_presenter)
        m_presenter = new FTPresenter(m_service);

    if (!m_view) {
        // Get the factory so we can attach our catchView slot.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated += Poco::Delegate
            <FTState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &FTState::catchView);

        // Finally, request the view to be created.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::FTView);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    } else {
        m_presenter->showView();
    }
}

std::string FTState::id() const
{
    Sid::String file, handler;

    m_transfer->GetPropFilename(file);
    m_transfer->GetPropPartnerHandle(handler);

    /* As we don't need to handle all FT in the same state, here we are
     * generating a random number to be used as the state ID. It will guarantee
     * that each FT request will be managed in its own session/state.
     */
    std::ostringstream random;
    random << rand();

    return std::string("FTState_") +  handler.data() +
        file.data() + random.str();
}

};

};
