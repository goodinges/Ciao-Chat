
#include "contactlistservice.h"

#include "skypecore.h"
#include "skypecontact.h"
#include "skypeaccount.h"
#include "util.h"

#include <stdio.h>

#include <Poco/Delegate.h>

namespace Mvp
{

namespace Model
{

ContactListService::ContactListService()
{
}

ContactListService::~ContactListService()
{
    if (m_contactGroup)
        m_contactGroup->contactGroupChanged -=
            Poco::Delegate<ContactListService, Poco::EventArgs>
            (this, &ContactListService::onContactGroupChange);

    // To get the account info we need to get the account ref first.
    SkypeAccount::Ref accountRef;

    SEString identity = SkypeCore::instance()->currentIdentity();
    if (SkypeCore::instance()->GetAccount(identity, accountRef))
        // Disconnect the current account to our slots.
        accountRef->accountChanged -=
            Poco::Delegate<ContactListService, Poco::EventArgs>
            (this, &ContactListService::onAccountChange);

    for (int i = 0; i < (int)m_contacts.size(); ++i)
        m_contacts.at(i)->contactChanged -=
            Poco::Delegate<ContactListService, ContactRef>(this,
                &ContactListService::onContactChange);

    m_contacts.clear();
}

std::vector<Poco::SharedPtr<ContactData> > ContactListService::getContactList()
{
    // Gets the Skype Core instance.
    SkypeCore *skype = SkypeCore::instance();

    std::vector<Poco::SharedPtr<ContactData> > contacts;

    /* Fetches the real contact list from the user account.
     *
     * In order to get a list of contacts first we need to decide which kind of
     * contact group we are interested in. The available groups are:
     *
     * - ALL_KNOWN_CONTACTS : The union of all non-custom contactgroups below;
     * - ALL_BUDDIES All : Authorized contacts plus all SKYPEOUT contacts;
     * - SKYPE_BUDDIES All : Authorized Skype contacts;
     * - SKYPEOUT_BUDDIES : All SkypeOut contacts;
     * - ONLINE_BUDDIES : All Authorized contacts currently online;
     * - UNKNOWN_OR_PENDINGAUTH_BUDDIES : Contacts that have not authorized yet;
     * - RECENTLY_CONTACTED_CONTACTS : Contacts I've had recent conversation with;
     * - CONTACTS_WAITING_MY_AUTHORIZATION : Contacts waiting for my auth;
     * - CONTACTS_AUTHORIZED_BY_ME : All contacts that I have authorized;
     * - CONTACTS_BLOCKED_BY_ME : All contacts that I have blocked;
     * - UNGROUPED_BUDDIES : All buddies that do not belong to any custom group;
     * - CUSTOM_GROUP : Custom group; and
     * - EXTERNAL_CONTACTS : Contacts from external address book.
     */

    /* Once we have decided which group to fetch it's necessary to get a
     * contact group object passing which kind of group you want, in our
     * case ALL_BUDDIES.
     */
    if (!m_contactGroup) {
        skype->GetHardwiredContactGroup(ContactGroup::ALL_BUDDIES,
            m_contactGroup);
        m_contactGroup->contactGroupChanged +=
            Poco::Delegate<ContactListService, Poco::EventArgs>
            (this, &ContactListService::onContactGroupChange);
    }

    // Now we have a specialized group with a set of contacts.
    ContactRefs list;
    m_contactGroup->GetContacts(list);

    // Disconnect from any previous contacts we were connected to.
    for (int i = 0; i < (int)m_contacts.size(); ++i)
        m_contacts.at(i)->contactChanged -=
            Poco::Delegate<ContactListService, ContactRef>(this,
                &ContactListService::onContactChange);

    m_contacts.clear();

    // Let's extract some data from the contacts.
    for (int i = 0; i < (int)list.size(); ++i) {
        // Get a reference to the Contact.
        SkypeContact::Ref contact(list[i]->getOID());

        /* Once we have the real reference to the Contact it's time to fetch its
         * data and connect to a signal that will notify us of any property
         * changes inside the Contact object. It will let us track some
         * important properties like Display Name, Availability and
         * Capabilities.
         */
        Poco::SharedPtr<ContactData> newData(Util::dataFromContact(list[i]));
        contact->contactChanged +=
            Poco::Delegate<ContactListService, ContactRef>(this,
                    &ContactListService::onContactChange);

        m_contacts.push_back(contact);

        // Add the contact to the vector we are going to return.
        contacts.push_back(newData);
    }

    return contacts;
}

const Poco::SharedPtr<ContactData> ContactListService::accountInformation()
{
    Poco::SharedPtr<ContactData> data(0);

    // Gets the Skype Core instance.
    SkypeCore *skype = SkypeCore::instance();

    // To get the account info we need to get the account ref first.
    SkypeAccount::Ref accountRef;

    if (skype->GetAccount(skype->currentIdentity(), accountRef)) {
        data = Util::dataFromAccount(accountRef);

        // Connect the current account to our slots.
        accountRef->accountChanged +=
            Poco::Delegate<ContactListService, Poco::EventArgs>
            (this, &ContactListService::onAccountChange);
    }

    return data;
}

void ContactListService::onContactChange(const void *sender, ContactRef &contact)
{
    /* Every time a contact property is changed this slot will be called and we
     * will have the opportunity to make some decisions, update our local data
     * and let other interested objects know about this change.
     */
    printf("Contact list service, contact has changed\n");

    // Update the meta contact data.
    Poco::SharedPtr<ContactData> data = Util::dataFromContact(contact);

    // Fire a signal to update other objects.
    contactChanged(this, data);
}

void ContactListService::onContactGroupChange(const void *sender,
    Poco::EventArgs &args)
{
    contactGroupChanged(this, args);
}

void ContactListService::onAccountChange(const void *sender,
    Poco::EventArgs &args)
{
    // Gets the Skype Core instance.
    SkypeCore *skype = SkypeCore::instance();

    // To get the account info we need to get the account ref first.
    SkypeAccount::Ref accountRef;

    if (skype->GetAccount(skype->currentIdentity(), accountRef)) {
        // Get the current account information.
        Poco::SharedPtr<ContactData> accountData =
            Util::dataFromAccount(accountRef);

        // Tell the presenter about it.
        accountChanged(this, accountData);
    }
}

};

};
