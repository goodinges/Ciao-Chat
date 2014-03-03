#include "skypesms.h"

namespace Mvp
{

namespace Model
{

SkypeSms::SkypeSms(unsigned int oid, SERootObject* root)
    : Sms(oid, root)
{
    printf("New Sms created.\n.");
}

SkypeSms::~SkypeSms()
{
}

void SkypeSms::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Sms class itself, because once this method is called the property is
     * already changed in the class.
     */

    /* We watch for STATUS changes here, so we can send a signal when
     * this sms status has changed.
     */
    if (prop == Sms::P_STATUS) {
        // Get the sms status.
        Sms::STATUS status;
        GetPropStatus(status);

        statusChanged(this, status);
    }
}

};

};

