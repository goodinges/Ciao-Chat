#ifndef _SKYPECONTACT_H
#define _SKYPECONTACT_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeContact skypecontact.h skypecontact.h
 * @brief Implements basic features of the SkypeKit Contact class.
 *
 * In order to intercept the contact's property change events it's necessary
 * to subclass the Contact class and overwrite some basic virtual methods that
 * will be called when a property is changed. The most important virtual method
 * is:
 *
 * void OnChange(int prop): This method will be called whenever a property is
 * changed. Here it's possible to catch all changes related to the contact,
 * like: Personal Info, Availability status, Authorization status, Capability
 * etc.
 *
 * The Contact class represents an address book entry (aka Buddy) and retains
 * information like Skype Name, Display Name, Avatar, Groups, Personal data and
 * so on.
 *
 * There are 7 possible types of Contact:
 *
 * - UNRECOGNIZED : Contact/account has no pre-identified type. Default type.
 * - SKYPE : Normal skype contact.
 * - PSTN : Normal PSTN contact.
 * - EMERGENCY_PSTN : Emergency number (i.e. 911).
 * - FREE_PSTN :
 * - UNDISCLOSED_PSTN : Undisclosed PSTN number.
 * - EXTERNAL : Currently used by Windows OS for contacts imported from Outlook.
 *
 * The type of Contact also reflects on its Capabilities.
 *
 * @see ContactData
 */
class SkypeContact : public Contact
{
public:
    typedef DRef<SkypeContact, Contact> Ref;

    /**
     * Destructor.
     */
    ~SkypeContact();

    /**
     * Contact changed signal.
     *
     * This signal must be emitted when the contact is changed, notifying that
     * some of its properties were modified.
     */
    Poco::BasicEvent<ContactRef> contactChanged;

protected:
    /**
     * Constructor.
     */
    SkypeContact(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When a Contact's property is changed this method will be called.
     * It's important to know that when this method is called the property
     * itself is already changed inside the Contact's class, which means that
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
