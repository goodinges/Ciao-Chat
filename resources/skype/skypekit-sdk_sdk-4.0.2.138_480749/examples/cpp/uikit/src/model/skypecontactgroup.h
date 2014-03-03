#ifndef _SKYPECONTACTGROUP_H
#define _SKYPECONTACTGROUP_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeContactGroup skypecontactgroup.h skypecontactgroup.h
 * @brief Implements basic features of the SkypeKit ContactGroup class.
 *
 * In order to intercept the contact group's property change events it's
 * necessary to subclass the ContactGroup class and overwrite some basic virtual
 * methods that will be called when a property is changed. The most important
 * virtual method is:
 *
 * void OnChange(const ContactRef &contact): This method will be called whenever
 * a contact is added or removed from this contactgroup.
 *
 * The ContactGroup class represents a list of Contacts.
 *
 * @see SkypeContact
 */
class SkypeContactGroup : public ContactGroup
{
public:
    typedef DRef<SkypeContactGroup, ContactGroup> Ref;

    /**
     * Destructor.
     */
    ~SkypeContactGroup();

    /**
     * ContactGroup changed signal.
     *
     * This signal must be emitted when a the group changes, notifying that
     * some of its contacts were added or removed.
     */
    Poco::BasicEvent<Poco::EventArgs> contactGroupChanged;

protected:
    /**
     * Constructor.
     */
    SkypeContactGroup(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When a Contact is added or removed from this group, this method
     * is called..
     *
     * @param contact the contact that was added or removed from this group.
     */
    void OnChange(const ContactRef &contact);

private:
    friend class SkypeCore;
};

};

};
#endif
