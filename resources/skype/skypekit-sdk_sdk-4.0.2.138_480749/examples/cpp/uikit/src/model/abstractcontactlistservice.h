
#ifndef _ABSTRACTCONTACTLISTSERVICE_H
#define _ABSTRACTCONTACTLISTSERVICE_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include <vector>

#include "contactdata.h"

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractContactListService
   @headerfile abstractcontactlistservice.h abstractcontactlistservice.h
 * @brief Abstract Contact List Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the Contact List services, such as fetching the Contact List from
 * the user account and signals to propagate the contact changes.
 *
 * @note This class is abstract. If you're looking for a concrete one
 * look at ContactListService.
 *
 */
class AbstractContactListService {
public:
    /**
     * Constructor.
     */
    AbstractContactListService(){};

    /**
     * Destructor.
     */
    virtual ~AbstractContactListService(){};

    /**
     * Returns a list of meta contacts.
     *
     * When requested the method will return a vector containing a set of meta
     * contacts representing each real Skype Contact in the list. This meta
     * contact only provides some data about the real contact and is
     * identified by the contact's skypename value.
     *
     * There's no available way to filter the contacts and the method will
     * return all buddies added to the account's contact list.
     *
     * @returns vector<ContactData *> A vector containing meta contacts.
     * @see ContactData
     */
    virtual std::vector<Poco::SharedPtr<ContactData> > getContactList() = 0;

    /**
     * Returns a meta contact containing the account data.
     *
     * When requested the method will return data for the current
     * user account, such as the skypename, the fullname and the availability.
     *
     * @return a meta contact or NULL if no contact is logged in.
     */
    virtual const Poco::SharedPtr<ContactData> accountInformation() = 0;

    /**
     * Signal for contact changes notification.
     *
     * This signal will be emitted whenever a Skype Contact has its properties
     * changed. Thus it acts as a central point for change notifications.
     */
    Poco::BasicEvent<Poco::SharedPtr<ContactData> > contactChanged;

    /**
     * Signal for account change notification.
     *
     * This signal is emitted whenever the current account has properties
     * changed.
     */
    Poco::BasicEvent<Poco::SharedPtr<ContactData> > accountChanged;

    /**
     * Signal for contact list changes notification.
     *
     * This signal is emitted whenever the whole contact list changes, for
     * example when a new contact is added to the list, or a contact is removed.
     */
    Poco::BasicEvent<Poco::EventArgs> contactGroupChanged;
};

};

};
#endif

