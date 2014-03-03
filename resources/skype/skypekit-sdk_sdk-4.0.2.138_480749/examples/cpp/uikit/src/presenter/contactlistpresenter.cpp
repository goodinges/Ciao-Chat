#include "contactlistpresenter.h"

#include <stdio.h>

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

#include "model/contactdata.h"
#include "model/skypecore.h"

namespace Mvp
{

namespace Presenter
{

ContactListPresenter::ContactListPresenter(
        Mvp::Model::AbstractContactListService *model)
    : BaseContactListPresenter(model)
{
}

void ContactListPresenter::setView(Mvp::View::AbstractContactListView *view)
{
    // Just set the view and initialize it.
    AbstractPresenter<Mvp::View::AbstractContactListView>::setView(view);
    m_view->initialize();

    /* Connect to the view's viewClosed signal so we can react by sending our
     * finished signal.
     */
    m_view->viewClosed +=
        Poco::Delegate<ContactListPresenter, Poco::EventArgs>
        (this, &ContactListPresenter::onViewClose);

    // Connect to the view's other signals.
    m_view->chatContact +=
        Poco::Delegate<ContactListPresenter, const std::string>
        (this, &ContactListPresenter::onChatContact);
    m_view->smsContact +=
        Poco::Delegate<ContactListPresenter, const std::string>
        (this, &ContactListPresenter::onSmsContact);
    m_view->callContact +=
        Poco::Delegate<ContactListPresenter, const std::string>
        (this, &ContactListPresenter::onCallContact);
    m_view->fileTransferContact +=
        Poco::Delegate<ContactListPresenter,
        const std::pair<const std::string, const std::string> >
        (this, &ContactListPresenter::onFileTransferContact);
}

void ContactListPresenter::showView()
{
    // Get the contact list of skype contacts, and show it in the view.
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts =
        m_model->getContactList();
    m_view->setContactList(contacts);
    m_view->setAccountInfo(m_model->accountInformation());
    m_view->showView();
}

void ContactListPresenter::onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact)
{
    // Update the view with the new contact information.
    m_view->updateContact(contact);
}

void ContactListPresenter::onAccountChanged(const void *sender,
                          Poco::SharedPtr<Mvp::Model::ContactData> &accountData)
{
    m_view->setAccountInfo(accountData);
}

void ContactListPresenter::onContactGroupChanged(const void *sender,
        Poco::EventArgs &)
{
    // Get the list of skype contacts again, and give it to the view.
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts =
        m_model->getContactList();

    m_view->setContactList(contacts);
}

void ContactListPresenter::onViewClose(const void *sender,
        Poco::EventArgs &args)
{
    // The view has closed, so emit our finished signal.
    finished(this, args);
}

void ContactListPresenter::onChatContact(const void *sender,
        const std::string &skypeName)
{
    // Request the chat.
    Mvp::Model::SkypeCore::instance()->requestChat(skypeName);
}

void ContactListPresenter::onSmsContact(const void *sender,
    const std::string &skypeName)
{
    // Request the chat in sms mode.
    Mvp::Model::SkypeCore::instance()->requestChat(skypeName, true);
}

void ContactListPresenter::onCallContact(const void *sender,
        const std::string &skypeName)
{
    // Request the call.
    Mvp::Model::SkypeCore::instance()->requestCall(skypeName);
}

void ContactListPresenter::onFileTransferContact(const void *sender,
        const std::pair<const std::string, const std::string> &data)
{
    // Request the file transfer.
    Mvp::Model::SkypeCore::instance()->requestFileTransfer(
            data.first, data.second);
}

};

};

