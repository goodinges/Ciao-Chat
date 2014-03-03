

#include "loggedoutstate.h"

#include "idlestate.h"

#include <stdio.h>

#include <Poco/Delegate.h>

#include "model/loginservice.h"
#include "presenter/loginpresenter.h"
#include "view/abstractviewfactory.h"

namespace Mvp
{

namespace Presenter
{

LoggedOutState::LoggedOutState(Context &context)
    : State(context),
    m_model(0),
    m_presenter(0),
    m_view(0)
{
}

LoggedOutState::~LoggedOutState()
{
    // Delete our objects.
    delete m_presenter;
    delete m_model;

    if (m_view)
        m_view->destroyView();
}

void LoggedOutState::catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair)
{
    if (pair.first == m_viewRequestId) {
        // Get the factory, so we can disconnect from it.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();
        factory->viewCreated -= Poco::Delegate
            <LoggedOutState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &LoggedOutState::catchView);

        // Cache the view.
        m_view = reinterpret_cast<Mvp::View::AbstractLoginView *>(pair.second);
        // Give it to the presenter.
        m_presenter->setView(m_view);
        // Ask the presenter to show it.
        m_presenter->showView();
    }
}

void LoggedOutState::init()
{
    // Create a model and presenter for login.
    if (!m_model)
        m_model = new Mvp::Model::LoginService;

    if (!m_presenter) {
        m_presenter = new LoginPresenter(m_model);

        // Connect the loginSucceeded signal to our slot.
        m_presenter->loginSucceeded += Poco::Delegate<LoggedOutState,
            Poco::EventArgs>(this, &LoggedOutState::onLogInSuccess);
        m_presenter->logoutSucceeded += Poco::Delegate<LoggedOutState,
            Poco::EventArgs>(this, &LoggedOutState::onLogOutSuccess);

        // Connect the presenter's finished signal to our slot.
        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    }

    if (!m_view) {
        // Get the factory for views.
        Mvp::View::AbstractViewFactory *factory =
            Mvp::View::AbstractViewFactory::instance();

        // Connect the factory's viewCreated signal to our catchView slot.
        factory->viewCreated +=  Poco::Delegate
            <LoggedOutState, std::pair<int, Mvp::View::AbstractView*> >
            (this, &LoggedOutState::catchView);

        // Request our view.
        m_viewRequestId =
            factory->requestView(Mvp::View::AbstractViewFactory::LoginView);

        m_presenter->finished +=
            Poco::Delegate<State, Poco::EventArgs>(this, &State::stateFinished);
    } else {
        m_presenter->showView();
    }
}

void LoggedOutState::onIdleStateFinished(const void *sender, Poco::EventArgs &)
{
    printf("onIdleStateFinished called\n");

    /* Once the Idle state was finished it's needed to remove all other states
     * (Ie: Call, Text, File Transfer, etc) before asking the model to logout
     * from the Skype server. Thus, the other states will have a chance to
     * finish its tasks before the app get logged out from the server.
     */
    if (m_model->currentlyLoggedIn()) {
        cleanupStates();
        m_model->logout();
    }
}

void LoggedOutState::onLogInSuccess(const void *sender, Poco::EventArgs &)
{
    // Hide the login view.
    m_presenter->hideView();

    // Push a new IdleState onto the context.
    IdleState *idleState = new IdleState(m_parentContext);
    idleState->finished +=  Poco::Delegate<LoggedOutState, Poco::EventArgs>(
            this, &LoggedOutState::onIdleStateFinished);

    m_parentContext.pushState(idleState);
}

void LoggedOutState::onLogOutSuccess(const void *sender, Poco::EventArgs &)
{
    printf("onLogOutSuccess called\n");

    /* Remove all other state in case it got loggedout from the bottom up
     * (Ie: Network or Server error).
     */
    cleanupStates();
}

void LoggedOutState::cleanupStates()
{
    // Pop all other states from the context.
    while (m_parentContext.currentState() != this)
        // Note: Deleting the state removes it from the parent context.
        delete m_parentContext.popState();
}

std::string LoggedOutState::id() const
{
    return "LoggedOutState";
}

};

};
