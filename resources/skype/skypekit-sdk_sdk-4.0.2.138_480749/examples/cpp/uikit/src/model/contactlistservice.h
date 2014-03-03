
#ifndef _CONTACTLISTSERVICE_H
#define _CONTACTLISTSERVICE_H

#include "abstractcontactlistservice.h"
#include "skypecontact.h"
#include "skypecontactgroup.h"

namespace Mvp
{

namespace Model
{

/**
 * @class ContactListService contactlistservice.h contactlistservice.h
 * @brief Implements the Skype Contact List Service.
 *
 * This class provides a way to fetch the user's contact list and listen to
 * each contact's property changes.
 *
 * @see AbstractContactListService
 */
class ContactListService: public AbstractContactListService
{
public:
    /**
     * Constructor.
     */
    ContactListService();

    /**
     * Destructor.
     */
    ~ContactListService();

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
    std::vector<Poco::SharedPtr<ContactData> > getContactList();

    /**
     * Returns a meta contact containing the account data.
     *
     * When requested the method will return data for the current
     * user account, such as the skypename, the fullname and the availability.
     *
     * @return a meta contact or NULL if no contact is logged in.
     */
    const Poco::SharedPtr<ContactData> accountInformation();

private:
    /**
     * Slot to track a contact's changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the contact reference that has its properties changed.
     */
    void onContactChange(const void *sender, ContactRef &contact);

    /**
     * Slot to react to a contact group change.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onContactGroupChange(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to account name change.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onAccountChange(const void *sender, Poco::EventArgs &args);

    /*
     * A pointer to the contact group so we don't have to always get it from
     * the skypecore object
     */
    SkypeContactGroup::Ref m_contactGroup;

    /*
     * The contacts we are watching for changes.
     */
    std::vector<SkypeContact::Ref> m_contacts;
};

};

};
#endif
