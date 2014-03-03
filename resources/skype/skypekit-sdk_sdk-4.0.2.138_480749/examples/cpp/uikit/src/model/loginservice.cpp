#include "loginservice.h"

#include "skypecore.h"
#include "skypeaccount.h"

#include <Poco/Delegate.h>

using Poco::Delegate;

namespace Mvp
{

namespace Model
{

bool LoginService::login(std::string username, std::string password)
{
    bool retval = false;

    // Get the username and password.
    SEString accountName = username.c_str();
    SEString accountPwd = password.c_str();

    SkypeCore *skype = SkypeCore::instance();

    printf("Getting account...\n");

    /* Searches for an already defined account.
     * Note: GetAccount() method will create a new account if the account name
     * is not found, or return false if an error occurs.
     */
    if (skype->GetAccount(accountName, m_account)) {
        printf("Logging in...\n");

        /* Try to login into the Skype Network.
         * The first of the two boolean arguments specifies whether the supplied
         * password will be saved in the local database (so that for subsequent
         * logins we can use Login instead of LoginWithPassword). Passing false
         * as the second argument prevents all updates to the local database.
         */
        retval = m_account->LoginWithPassword(accountPwd, false, true);

        // Store the current identity being used for loging in.
        skype->setCurrentIdentity(accountName);

        /* You might know that calling the LoginWithPassword() method doesn't
         * mean you're already logged in. The login process is completely
         * asynchronous - which means that the call returns after making the
         * request. To know whether the account is already logged in, you need
         * to watch out for Accounts status changes, by subclassing the Account
         * class.
         *
         * Thus in order to follow the account's status changes the
         * statusChanged signal was created and must be connected. It will
         * report all status changes when they happen.
         */
        m_account->statusChanged +=
            Delegate<LoginService, Account::STATUS>(this,
                    &LoginService::statusChanged);
    }
    else {
        printf("Account does not exist!\n");
    };

    return retval;
}

bool LoginService::logout()
{
    printf("Logging out...\n");

    /* Logs out the current account. Note that calling this method doesn't mean
     * the application will exit. Passing false to the argument means that
     * there's no need to clear the password from the local data base, since
     * we're not saving it there for this application.
     */
    return m_account->Logout(false);
}

bool LoginService::currentlyLoggedIn()
{
    bool retval = false;

    if (m_account) {
        Account::STATUS status;
        if (m_account->GetPropStatus(status))
            if (status == Account::LOGGED_IN)
                retval = true;
    }

    return retval;
}

Account::LOGOUTREASON LoginService::logoutReason()
{
    Account::LOGOUTREASON reason;
    m_account->GetPropLogoutreason(reason);

    return reason;
}

void LoginService::statusChanged(const void *sender, Account::STATUS &status)
{
    // Check what the new Account's connection status is.
    if (status == Account::LOGGED_IN) {
        printf("Status changed, logged in.\n");

        // Emit a signal saying that the application is logged in.
        Poco::EventArgs args;
        loggedIn(this, args);
    }
    else if (status == Account::LOGGED_OUT) {
        printf("Status changed, logged out.\n");

        // Emit a signal saying that the application is logged out.
        Poco::EventArgs args;
        loggedOut(this, args);
    }
}

LoginService::~LoginService()
{
    printf("Done.\n");

    delete SkypeCore::instance();
}

};

};
