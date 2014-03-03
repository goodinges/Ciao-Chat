#ifndef _QTCONTACTLISTVIEW_H
#define _QTCONTACTLISTVIEW_H

#include <QDialog>
#include "abstractcontactlistview.h"

class Ui_ContactsDialog;

namespace Mvp
{

namespace View
{

class QtPSTNNumber;

/**
 * @class QtContactListView qtcontactlistview.h qtcontactlistview.h
 * @brief A view used for showing and interacting with a list of contacts.
 *
 * This class is a view which displays a list of contacts and their
 * availability.  It also allows the user to initiate calls and chats with
 * any contact who has that ability.
 *
 * @see AbstractContactListView
 */
class QtContactListView : public QWidget, public AbstractContactListView
{
Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent window if any.
     */
    QtContactListView(QWidget *parent = NULL);

    /**
     * Destructor.
     */
    ~QtContactListView();

    /**
     * Initialize this ContactListView.
     */
    void initialize();

    /**
     * Show this view to the user.
     */
    void showView();

    /**
     * Set the initial contact list.
     *
     * @param contacts The initial list of contacts to show in the list.
     */
    void setContactList(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts);

    /**
     * Set the account's information.
     *
     * @param data a meta contact containing the account's information.
     */
    void setAccountInfo(const Poco::SharedPtr<Mvp::Model::ContactData> data);

    /**
     * Update the given contact's information.
     *
     * @param data The contact's new information to display.
     */
    void updateContact(const Poco::SharedPtr<Mvp::Model::ContactData> data);

    /**
     * Destroy the view object.
     */
    void destroyView();

protected:
    /**
     * Overridden method from QDialog to emit viewClosed signal when
     * this dialog is closed.
     */
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void onCallContact(const QString &skypeName);
    void onChatContact(const QString &skypeName);
    void onHistoryClicked();
    void onFileTransferContact(const QString &skypeName);
    void onDialpadClicked();
    void onPTNNumberFinished(int result);

private:
    Q_DISABLE_COPY(QtContactListView)

    /**
     * A pointer to the ui from qt-contactlist.ui's generated source code.
     */
    Ui_ContactsDialog *ui;

    QtPSTNNumber *m_pstnNumber;
    QPixmap m_defaultAvatar;
};

};

};
#endif
