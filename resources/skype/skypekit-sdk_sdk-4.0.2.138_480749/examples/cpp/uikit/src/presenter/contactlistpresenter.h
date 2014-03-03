#ifndef _CONTACTLISTPRESENTER_H
#define _CONTACTLISTPRESENTER_H

#include "basecontactlistpresenter.h"

#include <Poco/SharedPtr.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class ContactListPresenter contactlistpresenter.h contactlistpresenter.h
 * @brief Class to present the user's contact list.
 *
 * This class is responsible for connecting the ContactListService to the
 * ContactListView, manipulating the ContactListView (show, update, etc.)
 * and passing data from one to the other and back.
 *
 * @see BaseContactListPresenter, AbstractPresenter, AbstractContactListService,
 * and AbstractContactListView
 */
class ContactListPresenter : public BaseContactListPresenter
{
public:
    /**
     * Constructor.
     *
     * @param model the model we should use to present our contact list.
     */
    ContactListPresenter(Mvp::Model::AbstractContactListService *model);

    /**
     * Set the view to use in this presenter.
     *
     * @param view the ContactListView to use.
     */
    void setView(Mvp::View::AbstractContactListView *view);

    /**
     * Ask the view to show itself.
     *
     * @note: This method is called when the parent state is initialized.
     */
    void showView();

private:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &);

    /**
     * Slot to react to contact name or status changes.
     *
     * When a contact's availability changes, the ContactListService will
     * send a signal with the new contact information.  This slot will tell
     * the view the new contact information.
     *
     * @param sender a pointer to the sender of the signal.
     * @param contact the contact information for the contact that changed.
     */
    void onContactChanged(const void *sender,
                          Poco::SharedPtr<Mvp::Model::ContactData> &contact);

    /**
     * Slot to react to account name or status changes.
     *
     * When the current account's status, fullname, or avatar image changes
     * the ContactListService sends a signal with the new information.
     *
     * @param sender a pointer to the sender of the signal.
     * @param accountData the new contact information to use for the account.
     */
    void onAccountChanged(const void *sender,
                          Poco::SharedPtr<Mvp::Model::ContactData> &accountData);

    /**
     * Slot to react to contact list changes.
     *
     * When a contact list changes, the ContactListService sends a signal.  This
     * slot will then refresh the contact list by getting all the contacts and
     * giving their information to the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onContactGroupChanged(const void *sender, Poco::EventArgs &);

    /**
     * Slot to react to chat contact requests from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @skypeName the skype name of the contact to begin a chat with.
     */
    void onChatContact(const void *sender, const std::string &skypeName);

    /**
     * Slot to react to sms contact requests from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @skypeName the skype name of the contact to begin an sms chat with.
     */
    void onSmsContact(const void *sender, const std::string &skypeName);

    /**
     * Slot to react to call contact requests from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @skypeName the skype name of the contact to call.
     */
    void onCallContact(const void *sender, const std::string &skypeName);

    /**
     * Slot to react to file transfer requests from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param data the contact's skypename and filepath.
     */
    void onFileTransferContact(const void *sender,
            const std::pair<const std::string, const std::string> &data);
};

};

};
#endif
