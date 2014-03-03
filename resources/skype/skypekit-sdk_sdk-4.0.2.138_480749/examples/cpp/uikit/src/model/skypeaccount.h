#ifndef _SKYPEACCOUNT_H
#define _SKYPEACCOUNT_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeAccount skypeaccount.h skypeaccount.h
 * @brief Implements basic features of the SkypeKit Account class.
 *
 * In order to intercept the account's property change events it's necessary
 * to subclass the Account class and overwrite some basic virtual methods that
 * will be called when a property is changed. The most important virtual method
 * is:
 *
 * void OnChange(int prop): This method will be called whenever a property is
 * changed. Here it's possible catch all kind of changes, like: Status, SkypeOut
 * Balance, Profile, Policy and etc.
 *
 * The Account class provides a set of important features such as Login, Logout,
 * Register new account, Password Change and Availability.
 */
class SkypeAccount : public Account
{
public:
    typedef DRef<SkypeAccount, Account> Ref;

    /**
     * Destructor.
     */
    ~SkypeAccount() {};

    /**
     * Availability status changed signal.
     *
     * This signal must be emitted when the account's availability status is
     * changed, propagating the new status.
     */
    Poco::BasicEvent<Account::STATUS> statusChanged;

    /**
     * Account changed signal.
     *
     * This signal must be emitted when the account's fullname, avatar, or
     * status has changed.
     */
    Poco::BasicEvent<Poco::EventArgs> accountChanged;

protected:
    /**
     * Constructor.
     */
    SkypeAccount(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When an Account's property is changed this method will be called.
     * It's important to know that when this method is called the property
     * itself is already changed inside the Account's class, which means that
     * you have to get the new property value by using the property index passed
     * on this method.
     *
     * @param prop the property index.
     */
    void OnChange(int prop);

private:
    friend class SkypeCore;
};

};

};
#endif
