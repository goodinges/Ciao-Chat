#include "loginpresenter.h"

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

#include "view/abstractloginview.h"

using Poco::Delegate;
using Poco::EventArgs;

namespace Mvp
{

namespace Presenter
{

LoginPresenter::LoginPresenter(Mvp::Model::AbstractLoginService *model)
    : BaseLoginPresenter(model)
{
}

void LoginPresenter::setView(Mvp::View::AbstractLoginView *view)
{
    // Give the view to the base class for caching.
    AbstractPresenter<Mvp::View::AbstractLoginView>::setView(view);

    // Connect the view's signals to our slots.
    m_view->logInEvent += Delegate<LoginPresenter, EventArgs>(this,
            &LoginPresenter::onLogIn);
    m_view->logOutEvent += Delegate<LoginPresenter, EventArgs>(this,
            &LoginPresenter::onLogOut);
    m_view->viewClosed +=
        Poco::Delegate<LoginPresenter, Poco::EventArgs>
        (this, &LoginPresenter::onViewClose);

    // Let the view initialize itself.
    m_view->initialize();
}

void LoginPresenter::showView()
{
    m_view->showView();
}

void LoginPresenter::hideView()
{
    m_view->hideView();
}

void LoginPresenter::onViewClose(const void *sender, Poco::EventArgs &args)
{
    finished(this, args);
}

void LoginPresenter::setViewError()
{
    /* Whenever an error occurs we need to get the error code and set the a
     * proper state or error on the view, then the view can report the error
     * to the user.
     */
    Account::LOGOUTREASON reason = m_model->logoutReason();

    switch (reason) {
        case Account::LOGOUT_CALLED:
            break;
        case Account::INVALID_SKYPENAME:
        case Account::UNACCEPTABLE_PASSWORD:
        case Account::SKYPENAME_TAKEN:
        case Account::NO_SUCH_IDENTITY:
        case Account::INCORRECT_PASSWORD:
        case Account::PASSWORD_HAS_CHANGED:
            m_view->setLoginError(
                    Mvp::View::AbstractLoginView::InvalidCredentials);
            break;
        case Account::TOO_MANY_LOGIN_ATTEMPTS:
            m_view->setLoginError(
                    Mvp::View::AbstractLoginView::TooManyAttempts);
            break;
        case Account::DB_IN_USE:
        case Account::DB_DISK_FULL:
        case Account::DB_IO_ERROR:
        case Account::DB_CORRUPT:
        case Account::DB_FAILURE:
            m_view->setLoginError(
                    Mvp::View::AbstractLoginView::DiskError);
            break;
        case Account::INVALID_APP_ID:
        case Account::APP_ID_FAILURE:
            m_view->setLoginError(
                    Mvp::View::AbstractLoginView::InvalidAppId);
            break;
        case Account::UNSUPPORTED_VERSION:
            m_view->setLoginError(
                    Mvp::View::AbstractLoginView::UnsupportedVersion);
            break;
        default:
            m_view->setLoginError(
                    Mvp::View::AbstractLoginView::InternalError);
            break;
    }
}

void LoginPresenter::onLogIn(const void *sender, Poco::EventArgs &)
{
    /* Login was requested by the view, so get the credentials from the
     * view and pass them to the model.
     */
    std::string username = m_view->username();
    std::string password = m_view->password();

    // Validate the username and password from the view.
    if (username.size() == 0 || password.size() == 0) {
        m_view->showStatus(Mvp::View::AbstractLoginView::InputCredentials);
    }
    else {
        if (m_model->login(username, password))
            m_view->showStatus(Mvp::View::AbstractLoginView::LoggingIn);
        else
            m_view->showStatus(Mvp::View::AbstractLoginView::LoggedOut);
    }
}

void LoginPresenter::onLogOut(const void *sender, Poco::EventArgs &)
{
    // Logout was requested by the view, so tell the model to logout.
    m_model->logout();
}

void LoginPresenter::loggedIn(const void *sender, Poco::EventArgs &args)
{
    // The user has logged in, so tell the view to appear that way.
    m_view->showStatus(Mvp::View::AbstractLoginView::LoggedIn);

    // Emit poco signal so the state can do it's work to create a new IdleState.
    loginSucceeded(this, args);
}

void LoginPresenter::loggedOut(const void *sender, Poco::EventArgs &args)
{
    // The user has logged out, so tell the view to appear logged out.
    m_view->showStatus(Mvp::View::AbstractLoginView::LoggedOut);

    // Check if it has been logged out due to an error.
    setViewError();

    // Emit poco signal so the state can do it's work to remove other states.
    logoutSucceeded(this, args);
}

};

};
