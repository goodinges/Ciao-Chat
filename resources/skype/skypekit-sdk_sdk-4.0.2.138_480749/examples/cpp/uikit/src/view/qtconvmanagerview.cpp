#include "qtconvmanagerview.h"
#include "qtcontactlistwidget.h"
#include "ui_qt-convmanager.h"

#include <QDebug>

namespace Mvp
{

namespace View
{

QtConvManagerView::QtConvManagerView(QWidget *parent)
    : QDialog(parent),
      ui(new Ui_ConversationDialog)
{
    ui->setupUi(this);

    ui->participantsList->enableContextMenu(false);
    ui->nonParticipantsList->enableContextMenu(false);

    connect(ui->addButton, SIGNAL(clicked()), SLOT(onAddClicked()));
    connect(ui->removeButton, SIGNAL(clicked()), SLOT(onRemoveClicked()));
    connect(ui->participantsList, SIGNAL(selectionChanged(bool)),
            SLOT(onParticipantsListSelectionChanged(bool)));
    connect(ui->nonParticipantsList, SIGNAL(selectionChanged(bool)),
            SLOT(onNonParticipantsListSelectionChanged(bool)));
}

QtConvManagerView::~QtConvManagerView()
{
    delete ui;
    ui = 0;
}

void QtConvManagerView::accept()
{
    // Get the new participants list.
    std::vector<std::string> participants;
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > data =
        ui->participantsList->contactList();

    for (int i = 0; i < (int)data.size(); ++i)
        participants.push_back(data[i]->skypeName);

    // Send the signal to tell the presenter what participants have been chosen.
    updateParticipantList(this, participants);

    QDialog::close();
}

void QtConvManagerView::reject()
{
    QDialog::close();
}

void QtConvManagerView::initialize()
{
}

void QtConvManagerView::showView()
{
    QDialog::show();
}

void QtConvManagerView::destroyView()
{
    deleteLater();
}

void QtConvManagerView::onNonParticipantsListSelectionChanged(bool valid)
{
    ui->addButton->setEnabled(valid);
}

void QtConvManagerView::onParticipantsListSelectionChanged(bool valid)
{
    ui->removeButton->setEnabled(valid);
}

void QtConvManagerView::onAddClicked()
{
    // Get the contact data from the nonparticipants list.
    Poco::SharedPtr<Mvp::Model::ContactData> contact =
        ui->nonParticipantsList->currentContactData();

    // Remove it from the nonparticipants list.
    std::string name = contact->skypeName.data();
    ui->nonParticipantsList->removeContact(name);

    // Add it to the participants list.
    ui->participantsList->addContact(contact);
}

void QtConvManagerView::onRemoveClicked()
{
    qDebug() << "remove contact clicked";

    // Get the contact data from the participants list.
    Poco::SharedPtr<Mvp::Model::ContactData> contact =
        ui->participantsList->currentContactData();

    // Remove it from the participants list.
    std::string name = contact->skypeName.data();
    ui->participantsList->removeContact(name);

    // Add it to the nonparticipants list.
    ui->nonParticipantsList->addContact(contact);
}

void QtConvManagerView::setNonParticipants(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > nonParticipants)
{
    ui->nonParticipantsList->setContactList(nonParticipants);
}

void QtConvManagerView::setParticipants(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > participants)
{
    ui->participantsList->setContactList(participants);
}

void QtConvManagerView::setConversationDetails(std::string &title,
        std::string &type)
{
    setWindowTitle(title.c_str());

    ui->participantsLabel->setText(tr("%1 participants").arg(type.c_str()));
    ui->nonParticipantsLabel->setText(
        tr("Possible %1 participants").arg(type.c_str()));
}

void QtConvManagerView::updateContact(
        const Poco::SharedPtr<Mvp::Model::ContactData> data)
{
    /* Update the contact data in both lists, the one that doesn't
     * contain the contact will just ignore it.
     */
    ui->participantsList->updateContact(data);
    ui->nonParticipantsList->updateContact(data);
}

void QtConvManagerView::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent caled";
    Poco::EventArgs args;
    viewClosed(this, args);
}

};

};

