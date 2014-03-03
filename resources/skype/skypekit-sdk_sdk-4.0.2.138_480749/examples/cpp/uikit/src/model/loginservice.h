#include <string>

#include "abstractloginservice.h"
#include "skypeaccount.h"

namespace Mvp
{

namespace Model
{

/**
 * @class LoginService loginservice.h loginservice.h
 * @brief Implements a simple Skype Login Service.
 *
 * This class will provide a way to login, logout and listen for
 * connection changes on the Skype Network.
 *
 * @see AbstractLoginService
 */
class LoginService : public AbstractLoginService
{
public:
    /**
     * Starts up the login process on the Skype Network.
     *
     * @param username the account's username.
     * @param password the account's password.
     * @return true if login request is in progress, false otherwise.
     */
    bool login(std::string username, std::string password);

    /**
     * Logout from the Skype Network.
     *
     * @return true if logout is in progress, false otherwise.
     */
    bool logout();

    /**
     * Query to see if we are currently logged in or not.
     *
     * @return true if we are logged in, false otherwise.
     */
    bool currentlyLoggedIn();

    /**
     * Return the reason that the app has logged out.
     *
     * Once the application is logged out this method will return if it was
     * due to an user request or if an error occurred.
     *
     * @return the logged out reason.
     *
     * @see Account::LOGOUTREASON
     */
    Account::LOGOUTREASON logoutReason();

    /**
     * Destructor. It will close the connection with the Skype Engine.
     */
    ~LoginService();

    /**
     * Propagate Account's status changes.
     *
     * As a network connection can changes its status at any time, it's
     * really important to constantly listen to the status report, since
     * you'll probably need to take some action when a change happens.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status the new account's connection status.
     */
    void statusChanged(const void *sender, Account::STATUS &status);

private:
    SkypeAccount::Ref m_account;
};

};

};
