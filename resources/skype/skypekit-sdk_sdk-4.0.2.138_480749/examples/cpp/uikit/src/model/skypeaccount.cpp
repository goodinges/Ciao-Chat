#include "skypeaccount.h"

#include "skypecore.h"

using namespace Sid;

namespace Mvp
{

namespace Model
{

SkypeAccount::SkypeAccount(unsigned int oid, SERootObject *root)
    : Account(oid, root)
{
}

void SkypeAccount::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail, act incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Account class itself, because once this method is called the property is
     * already changed in the class.
     */

    /* Once we know how to filter it, let's create a nice case for each type of
     * property.
     */
    Poco::EventArgs args;
    switch(prop) {
        /* Availability Status */
        case Account::P_STATUS:
            /* So the first thing to do is to get the new value of the
             * availability status. For this we only need to grab it from the
             * property list by using the GetPropStatus() method.
             */
            Account::STATUS loginStatus;
            GetPropStatus(loginStatus);

            /* Now we take the right action for each new status.*/
            if (loginStatus == Account::LOGGED_IN)
                printf("Login complete.\n");
            else if (loginStatus == Account::LOGGED_OUT)
                printf("Logout complete.\n");

            /* And then propagate the new status via a dedicated signal, to let
             * the other interested objects know about this change.
             */
            statusChanged(this, loginStatus);
            break;

        case Account::P_FULLNAME:
        case Account::P_AVATAR_IMAGE:
            accountChanged(this, args);
            break;

        default:
            /* If there's not proper case for this property lets just log that
             * it has changed.
             */
            printf("Account's property changed: %d\n", prop);
            break;
    }
}

};

};
