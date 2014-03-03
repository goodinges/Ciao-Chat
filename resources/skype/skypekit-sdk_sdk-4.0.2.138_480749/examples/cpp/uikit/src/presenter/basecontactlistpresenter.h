#ifndef _BASECONTACTLISTPRESENTER_H
#define _BASECONTACTLISTPRESENTER_H

#include "abstractpresenter.h"
#include "view/abstractcontactlistview.h"
#include "model/abstractcontactlistservice.h"

#include <Poco/Delegate.h>
#include <Poco/SharedPtr.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class BaseContactListPresenter basecontactlistpresenter.h basecontactlistpresenter.h
 * @brief Base class for contact list presenters.
 *
 * This class is responsible for connecting the ContactListService to the
 * ContactListView, manipulating the ContactListView (show, update, etc.)
 * and passing data from one to the other and back.
 *
 * @see ContactListPresenter, AbstractPresenter, AbstractContactListService, and
 * AbstractContactListView
 */
class BaseContactListPresenter : public
    AbstractPresenter<Mvp::View::AbstractContactListView>
{
public:
    /**
     * Constructor.
     *
     * @param model the model we should use to present our contact list.
     */
    BaseContactListPresenter(Mvp::Model::AbstractContactListService *model)
    : m_model(model)
    {
        // Connect the model's signals to our slots.
        m_model->contactChanged += Poco::Delegate
            <BaseContactListPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseContactListPresenter::onContactChanged);
        m_model->contactGroupChanged +=
            Poco::Delegate<BaseContactListPresenter, Poco::EventArgs>
            (this, &BaseContactListPresenter::onContactGroupChanged);
        m_model->accountChanged += Poco::Delegate
            <BaseContactListPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseContactListPresenter::onAccountChanged);
    }

    virtual ~BaseContactListPresenter()
    {
        // Connect the model's signals to our slots.
        m_model->contactChanged -= Poco::Delegate
            <BaseContactListPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseContactListPresenter::onContactChanged);
        m_model->contactGroupChanged -=
            Poco::Delegate<BaseContactListPresenter, Poco::EventArgs>
            (this, &BaseContactListPresenter::onContactGroupChanged);
        m_model->accountChanged -= Poco::Delegate
            <BaseContactListPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseContactListPresenter::onAccountChanged);
    }

    /**
     * Set the view to use in this presenter.
     *
     * @param view the ContactListView to use.
     */
    virtual void setView(Mvp::View::AbstractContactListView *view) = 0;

    /**
     * Ask the view to show itself.
     *
     * @note: This method is called when the parent state is initialized.
     */
    virtual void showView() = 0;

protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onViewClose(const void *sender, Poco::EventArgs &) = 0;

    /**
     * Slot to react to contact name or status changes.
     *
     * When a contact's availability changes, the ContactListService will
     * send a signal with the new contact information.  This slot will tell
     * the view the new contact information.
     *
     * @param sender a pointer to the sender of the signal.
     * @param contact the new contact information for the changed contact.
     */
    virtual void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact) = 0;

    /**
     * Slot to react to account name or status changes.
     *
     * When the current account's status, fullname, or avatar image changes
     * the ContactListService sends a signal with the new information.
     *
     * @param sender a pointer to the sender of the signal.
     * @param accountData the new contact information to use for the account.
     */
    virtual void onAccountChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &accountData) = 0;

    /**
     * Slot to react to contact list changes.
     *
     * When a contact list changes, the ContactListService sends a signal.  This
     * slot will then refresh the contact list by getting all the contacts and
     * giving their information to the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onContactGroupChanged(const void *sender, Poco::EventArgs &) = 0;

    /**
     * The model to use for getting the contact list, etc.
     */
    Model::AbstractContactListService *m_model;
};

};

};
#endif
