
#ifndef _ABSTRACTLOGINSERVICE_H
#define _ABSTRACTLOGINSERVICE_H

#include "skypeaccount.h"

#include <string>

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractLoginService abstractloginservice.h abstractloginservice.h
 * @brief Abstract Login Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the login services, such as, and specifically, login and logout from
 * Skype Network and signals to propagate the connection state.
 *
 * @note This class is abstract. If you're looking for a concrete one
 * try looking at LoginService class.
 */
class AbstractLoginService
{
public:
    /**
     * Destructor.
     */
    virtual ~AbstractLoginService() {}

    /**
     * Starts up the login process on the Skype Network.
     *
     * @param username the account's username.
     * @param password the account's password.
     * @return true if login request is in progress, false otherwise.
     */
    virtual bool login(std::string username, std::string password) = 0;

    /**
     * Logout from the Skype Network.
     *
     * @return true if logout is in progress, false otherwise.
     */
    virtual bool logout() = 0;

    /**
     * Query to see if we are currently logged in or not.
     *
     * @return true if we are logged in, false otherwise.
     */
    virtual bool currentlyLoggedIn() = 0;

    /**
     * Return the reason that the app is logged out.
     *
     * Once the application is logged out this method will return if it was
     * due to an user request or if an error occurred.
     *
     * @return the logged out reason.
     */
    virtual Account::LOGOUTREASON logoutReason() = 0;

    /**
     * LoggedIn signal must be emitted once the service has
     * logged into Skype Network.
     */
    Poco::BasicEvent<Poco::EventArgs> loggedIn;

    /**
     * LoggedOut signal must be emitted once the service has been
     * logged out from Skype Network.
     */
    Poco::BasicEvent<Poco::EventArgs> loggedOut;
};

};

};
#endif
