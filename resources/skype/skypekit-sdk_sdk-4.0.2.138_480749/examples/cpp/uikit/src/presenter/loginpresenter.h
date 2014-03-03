#ifndef _LOGINPRESENTER_H
#define _LOGINPRESENTER_H

#include "baseloginpresenter.h"
#include "view/abstractloginview.h"
#include "model/abstractloginservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class LoginPresenter loginpresenter.h loginpresenter.h
 * @brief Class to present login information via LoginView using LoginService.
 *
 * This class is responsible for connecting the LoginService to the LoginView,
 * manipulating the LoginView (show, hide, etc.) and passing data from one
 * to the other and back.
 *
 * @see AbstractPresenter, AbstractLoginService and AbstractLoginView.
 */
class LoginPresenter : public BaseLoginPresenter
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractLoginService to use for logging into Skype network.
     */
    LoginPresenter(Mvp::Model::AbstractLoginService *model);

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    void setView(Mvp::View::AbstractLoginView *view);

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    void showView();

    /**
     * Hide the view.
     *
     * The LoggedOutState will ask the loginpresenter to hide the view once
     * the user has logged in.
     */
    void hideView();

    /**
     * A signal to tell the state login succeeded.
     *
     * This signal is sent when loggedIn() is called, which is connected
     * to the LoginService's LoggedIn signal.
     */
    Poco::BasicEvent<Poco::EventArgs> loginSucceeded;

    /**
     * A signal to tell the state we have logged out.
     *
     * This signal is sent when loggedOut() is called, which is connected
     * to the LoginService's LoggedOut signal.
     */
    Poco::BasicEvent<Poco::EventArgs> logoutSucceeded;

protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &);

    /**
     * Slot to react to login requests from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onLogIn(const void* sender, Poco::EventArgs &);

    /**
     * Slot to react to logout requests from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onLogOut(const void* sender, Poco::EventArgs &);

    /**
     * Slot to react to login event from the LoginService.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void loggedIn(const void* sender, Poco::EventArgs &);

    /**
     * Slot to react to logout event from the LoginService.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void loggedOut(const void* sender, Poco::EventArgs &);

    /**
     * Show the logged out reason on the view.
     *
     * There're two possible way to the application get logged out. One is by
     * the user request and the other due to an error. This method will check
     * if the application has logged out due to an error and set the proper
     * message/status on the login view.
     */
    void setViewError();
};

};

};
#endif
