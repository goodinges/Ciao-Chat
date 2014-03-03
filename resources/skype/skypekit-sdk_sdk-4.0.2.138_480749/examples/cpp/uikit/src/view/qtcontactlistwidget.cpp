#include "qtcontactlistwidget.h"
#include "qtcontactlistmodel.h"
#include "qtcontactlistdelegate.h"
#include "skypeimages.h"

#include <QDebug>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QListView>
#include <QVBoxLayout>

namespace Mvp
{

namespace View
{

QtContactListWidget::QtContactListWidget(QWidget *parent)
    :QWidget(parent),
    m_contextMenuEnabled(true),
    m_contactView(new QListView(this)),
    m_contextMenu(new QMenu(m_contactView)),
    m_proxyModel(new QSortFilterProxyModel(this)),
    m_contactModel(new QtContactListModel(this)),
    m_contactDelegate(new QtContactListDelegate(this))
{
    m_proxyModel->setSourceModel(m_contactModel);
    m_contactView->setModel(m_proxyModel);
    m_contactView->setItemDelegate(m_contactDelegate);
    m_contactView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_contactView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(m_contextMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(actionChosen(QAction*)));

    m_chatAction = m_contextMenu->addAction(tr("Start Chat"));
    m_callAction = m_contextMenu->addAction(tr("Start Call"));
    m_fileTransferAction = m_contextMenu->addAction(tr("Send File"));

    m_contactView->setAlternatingRowColors(true);
    m_proxyModel->setSortRole(QtContactListModel::AvailabilityStatus);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_contactView);

    setLayout(layout);

    connect(m_contactView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            SLOT(onSelectionChanged()));
}

QtContactListWidget::~QtContactListWidget()
{
}

void QtContactListWidget::showContextMenu(const QPoint &pos)
{
    if (m_contextMenuEnabled) {
        // First find out which contact was clicked.
        QModelIndex index = m_contactView->indexAt(pos);

        // Then see if they have call and/or chat capabilities.
        bool canIM = index.data(QtContactListModel::CanIM).toBool();
        bool canCall = index.data(QtContactListModel::CanCall).toBool();
        bool canFT = index.data(QtContactListModel::CanFT).toBool();

        // Only enable the chat action if the status can chat.
        m_chatAction->setEnabled(canIM);

        // Disable the call action if the user is offline.
        m_callAction->setEnabled(canCall);

        // Disable the FT action if the user is offline.
        m_fileTransferAction->setEnabled(canFT);

        // Finally enable appropriate actions and exec the menu.
        m_contextMenu->exec(mapToGlobal(pos));
    }
}

void QtContactListWidget::onSelectionChanged()
{
    bool hasSelection = m_contactView->selectionModel()->hasSelection();
    emit selectionChanged(hasSelection);
}

Poco::SharedPtr<Mvp::Model::ContactData> QtContactListWidget::currentContactData()
{
    // Get the current index from the view.
    QModelIndex index = m_contactView->currentIndex();
    QModelIndex realIndex = m_proxyModel->mapToSource(index);

    // Get the data from the model.
    Poco::SharedPtr<Mvp::Model::ContactData> data =
        m_contactModel->dataFromIndex(realIndex);

    // Return the data.
    return data;
}

void QtContactListWidget::enableContextMenu(bool value)
{
    m_contextMenuEnabled = value;
}

void QtContactListWidget::actionChosen(QAction *action)
{
    QModelIndex index = m_contactView->currentIndex();
    QString skypeName = index.data(QtContactListModel::SkypeName).toString();

    if (action == m_chatAction)
        emit chatContact(skypeName);
    else if (action == m_callAction)
        emit callContact(skypeName);
    else if (action == m_fileTransferAction)
        emit fileTransferContact(skypeName);
}

void QtContactListWidget::sortData() const
{
    m_proxyModel->sort(0, Qt::AscendingOrder);
}

void QtContactListWidget::setContactList(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts)
{
    qDebug() << "adding " << contacts.size() << " contacts to the model";

    m_contactModel->setContacts(contacts);
    QMetaObject::invokeMethod(this, "sortData");
}

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >
    QtContactListWidget::contactList() const
{
    return m_contactModel->contacts();
}

void QtContactListWidget::removeContact(const std::string &skypeName)
{
    m_contactModel->removeContact(skypeName);
}

void QtContactListWidget::addContact(
    const Poco::SharedPtr<Mvp::Model::ContactData> contact)
{
    m_contactModel->addContact(contact);
    QMetaObject::invokeMethod(this, "sortData");
}

void QtContactListWidget::updateContact(
        const Poco::SharedPtr<Mvp::Model::ContactData> data)
{
    m_contactModel->updateContact(data);
    QMetaObject::invokeMethod(this, "sortData");
}

};

};
