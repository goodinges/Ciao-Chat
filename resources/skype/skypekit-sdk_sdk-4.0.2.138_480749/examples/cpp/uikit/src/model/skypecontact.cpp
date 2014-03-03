#include "skypecontact.h"

namespace Mvp
{

namespace Model
{

SkypeContact::SkypeContact(unsigned int oid, SERootObject* root)
    : Contact(oid, root)
{
    printf("New Contact created.\n.");
}

SkypeContact::~SkypeContact()
{
}

void SkypeContact::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Contact class itself, because once this method is called the property is
     * already changed in the class.
     */

    /* For our Contact class we will ony watch for the following
     * properties:
     *
     * - Contact::P_AVAILABILITY : Changes when the online status is set.
     * - Contact::P_DISPLAYNAME  : Changes when a new Display Name is set.
     * - Contact::P_SKYPENAME    : Changes when the skypename is updated.
     */
    if (prop == Contact::P_AVAILABILITY ||
        prop == Contact::P_DISPLAYNAME ||
        prop == Contact::P_SKYPENAME) {

        // Let's log that it has changed!
        SEString skypeName;
        GetPropSkypename(skypeName);

        printf("%s: Contact's property changed.\n", skypeName.data());

        /* And then notify the other interested objects that some of these
         * properties have changed.
         */
        ContactRef ref = this->ref();
        contactChanged(this, ref);
    }
}

};

};
