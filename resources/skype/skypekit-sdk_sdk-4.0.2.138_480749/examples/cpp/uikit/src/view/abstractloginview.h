
#ifndef _ABSTRACTLOGINVIEW_H
#define _ABSTRACTLOGINVIEW_H

#include <string>

#include "abstractview.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace View
{

/**
 * @class AbstractLoginView abstractloginview.h abstractloginview.h
 * @brief Abstract base class declaring all methods LoginViews should implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any LoginView object.  This includes an enumeration of login
 * statuses, signals for login and logout requests from the view, and methods
 * to get the password and username supplied in the user interface.
 *
 * @see QtLoginView
 */
class AbstractLoginView : public AbstractView
{
public:

    /**
     * Possible logged in/out statuses.
     *
     * This enumeration is used to show a user's logged in or out state
     * and the reason the user is logged out if login was attempted.
     */
    enum LoginStatus
    {
        InputCredentials,
        LoggedOut,
        LoggedIn,
        LoggingIn
    };

    enum LoginError
    {
        InvalidCredentials,
        TooManyAttempts,
        DiskError,
        InvalidAppId,
        UnsupportedVersion,
        InternalError
    };

    /**
     * Accessor for the username.
     *
     * @return The username supplied in the user interface by the user as a
     *   std::string.
     */
    virtual std::string username() = 0;

    /**
     * Accessor for the password.
     *
     * @return The password entered into the user interface by the user as a
     *   std::string.
     */
    virtual std::string password() = 0;

    /**
     * Show the given statuscode in the user interface.
     *
     * Subclasses should implement this to show the given status code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param code The status code to show.
     */
    virtual void showStatus(LoginStatus code) = 0;

    /**
     * Show the given error code in the user interface.
     *
     * Subclasses should implement this to show the given error code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param code the error code to show.
     */
    virtual void setLoginError(LoginError code) = 0;

    /**
     * Ask the view to hide.
     */
    virtual void hideView() = 0;

    /**
     * A signal emitted when the user requests log in (by clicking the Ok
     * button or hitting enter, etc.).
     */
    Poco::BasicEvent<Poco::EventArgs> logInEvent;

    /**
     * A signal emitted when the user requests a log out.
     */
    Poco::BasicEvent<Poco::EventArgs> logOutEvent;
};

};

};
#endif
