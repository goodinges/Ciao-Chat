
#ifndef _ABSTRACTCONTACTLISTVIEW_H
#define _ABSTRACTCONTACTLISTVIEW_H

#include "abstractview.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include <string>

#include "model/contactdata.h"

namespace Mvp
{

namespace View
{

/**
 * @class AbstractContactListView abstractcontactlistview.h
 * abstractcontactlistview.h
 * @brief Abstract base class declaring all methods ContactListViews should
 * implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any ContactListView object.  This includes methods to
 * initially set the contact list, and to update a given contact.  It also
 * includes signals for calling and starting a chat with a contact.
 *
 * @see QtContactListView
 */
class AbstractContactListView : public AbstractView
{
public:
    /**
     * Destructor.
     */
    virtual ~AbstractContactListView(){};

    /**
     * Set the contact list for initial display.
     *
     * When the contact list presenter is initialized, it asks the model
     * for the list of contacts, then passes that list to the view via this
     * setContactList method.
     *
     * @note Subclasses need to implement this, and are free to cache any
     * data passed in for future use.
     * @note If any previous data was cached it should be cleared at this point.
     *
     * @param contacts A vector of contact data to use initially in the view.
     */
    virtual void setContactList(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts) = 0;

    /**
     * Set the Account's information.
     *
     * When the contact list presenter is initialized, it asks the model for the
     * logged in Account information, then passes this data to the view via this
     * setAccountInfo method.
     *
     * @note Subclasses need to implement this, and are free to cache any
     * data passed in for future use.
     * @note If any previous data was cached it should be cleared at this point.
     *
     * @param data a meta contact containing the account's information.
     */
    virtual void setAccountInfo(
            const Poco::SharedPtr<Mvp::Model::ContactData> data) = 0;

    /**
     * Update a given contact's details (displayname, availability, etc.).
     *
     * Whenever a contact's features change, this method will be called to
     * update the contact's display information.  Any previous information
     * for this contact can be safely forgotten.
     *
     * @param data The new data to use for the given contact.
     *
     * @note The contact's skypeName should never change, so this can be used
     * to safely know which contact to update.
     */
    virtual void updateContact(
            const Poco::SharedPtr<Mvp::Model::ContactData> data) = 0;

    /**
     * A signal to call a given contact.  The contact's skypeName is passed in
     * the signal.
     */
    Poco::BasicEvent<const std::string> callContact;

    /**
     * A signal to start a chat with a given contact.  The contact's skypeName
     * is passed in the signal.
     */
    Poco::BasicEvent<const std::string> chatContact;

    /**
     * A signal to start an sms with a given contact.  The contact's skypeName
     * is passed in the signal.
     */
    Poco::BasicEvent<const std::string> smsContact;

    /**
     * A signal to show the history dialog.
     */
    Poco::BasicEvent<Poco::EventArgs> viewHistory;

    /*
     * A signal to start a file transfer session with a given contact. the
     * contact's skypeName and the file path is passed in the signal.
     */
    Poco::BasicEvent<const std::pair<const std::string,
        const std::string> > fileTransferContact;
};

};

};
#endif
