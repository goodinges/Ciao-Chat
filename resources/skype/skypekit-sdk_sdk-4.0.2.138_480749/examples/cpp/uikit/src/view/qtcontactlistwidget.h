#ifndef _QTCONTACTLISTWIDGET_H
#define _QTCONTACTLISTWIDGET_H

#include <QModelIndex>
#include <QWidget>

#include <Poco/SharedPtr.h>

#include "model/contactdata.h"

class QAction;
class QListView;
class QListWidgetItem;
class QMenu;
class QSortFilterProxyModel;

namespace Mvp
{

namespace View
{

class QtContactListModel;
class QtContactListDelegate;

/**
 * @class QtContactListWidget qtcontactlistwidget.h qtcontactlistwidget.h
 * @brief A widget that displays a list of contacts.
 *
 * This class is a widget which displays a list of contacts, their
 * availability and avatar. It also allows the user to initiate calls and chats
 * with any contact who has that ability.
 */
class QtContactListWidget : public QWidget
{
Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent window if any.
     */
    QtContactListWidget(QWidget *parent = NULL);

    /**
     * Destructor.
     */
    ~QtContactListWidget();

    /**
     * Set the initial contact list.
     *
     * @param contacts The initial list of contacts to show in the list.
     */
    void setContactList(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts);

    /**
     * Get the current contact list.
     *
     * @return a vector containing the current contact list.
     */
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contactList() const;

    /**
     * Remove a contact from the contact list.
     *
     * @param skypeName the skypename of a contact to remove from the list.
     */
    void removeContact(const std::string &skypeName);

    /**
     * Add a contact to the contact list.
     *
     * @param contact the data to add to the list.
     */
    void addContact(const Poco::SharedPtr<Mvp::Model::ContactData> contact);

    /**
     * Update the given contact's information.
     *
     * @param data The contact's new information to display.
     */
    void updateContact(const Poco::SharedPtr<Mvp::Model::ContactData> data);

    /**
     * Enable or disable the context menu.
     *
     * @param value true to enable the context menu, false to disable it.
     */
    void enableContextMenu(bool value);

    /**
     * Get the currently selected contact data.
     *
     * @return a shared pointer to the currently selected contact data.
     */
    Poco::SharedPtr<Mvp::Model::ContactData> currentContactData();
signals:
    /**
     * Signal to initiate a call with a contact.
     */
    void callContact(const QString &skypeName);

    /**
     * Signal to initiate a chat with a contact.
     */
    void chatContact(const QString &skypeName);

    /**
     * Signal to initiate a file transfer with a contact.
     */
    void fileTransferContact(const QString &skypeName);

    /**
     * Signal a selection has changed.
     *
     * @param hasSelection true if there is a selection, false otherwise.
     */
    void selectionChanged(bool hasSelection);

private slots:

    /**
     * Trigger the mechanism to sort the contact list items by Availability
     * status.
     *
     * This method needs to be called whenever the contact list model
     * get changed.
     */
    void sortData() const;

    /**
     * Slot to show the context menu.
     *
     * @param pos the position to show the context menu at.
     */
    void showContextMenu(const QPoint &pos);

    /**
     * Slot to react to context menu action triggered signal.
     *
     * @param action the action that was triggered.
     */
    void actionChosen(QAction *action);

    void onSelectionChanged();

private:

    bool m_contextMenuEnabled;
    QListView *m_contactView;
    QMenu *m_contextMenu;
    QAction *m_chatAction;
    QAction *m_callAction;
    QAction *m_fileTransferAction;
    QSortFilterProxyModel *m_proxyModel;
    QtContactListModel *m_contactModel;
    QtContactListDelegate *m_contactDelegate;
};

};

};
#endif
