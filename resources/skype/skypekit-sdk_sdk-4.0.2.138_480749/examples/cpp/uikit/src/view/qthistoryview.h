#ifndef _QTHISTORYVIEW_H
#define _QTHISTORYVIEW_H

#include <QModelIndex>
#include <QWidget>

#include "abstracthistoryview.h"

class Ui_HistoryView;
class QSortFilterProxyModel;

namespace Mvp
{

namespace View
{

class QtHistoryModel;

/**
 * @class QtHistoryView qthistoryview.h qthistoryview.h
 * @brief A view used for showing and interacting with a list of conversations.
 *
 * This class is a view which displays a list of conversations. It also allows
 * the user to initiate calls and chats with any conversation on the list.
 *
 * @see AbstractHistoryView
 */
class QtHistoryView : public QWidget, public AbstractHistoryView
{
Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent window if any.
     */
    QtHistoryView(QWidget *parent = NULL);

    /**
     * Destructor.
     */
    ~QtHistoryView();

    /**
     * Initialize this HistoryView.
     */
    void initialize();

    /**
     * Show this view to the user.
     */
    void showView();

    /**
     * Set the initial contact list.
     *
     * @param history the initial list of history data to show in the list.
     * @param unreadItems true if there are unread items in history, false
     * otherwise.
     */
    void setHistory(
        std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history,
        bool unreadItems);

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

    /**
     * Slot to react to sort order combobox changes.
     *
     * @param sortIndex the sort order to use. 0 - Date, 1 - Name, 2 - Type.
     *
     * @note sortIndex corresponds directly to the values in sortComboBox in
     * qt-history.ui.
     */
    void onSortOrderChanged(int sortIndex);

    /**
     * Slot to update the sort order based on the index in the sortComboBox.
     */
    void reSort();

    /**
     * Slot to react to double clicking an item in the history.
     *
     * When an item is double clicked the conversation is called if it was a
     * call of any kind (Conference, Call, etc.) otherwise the chat is started
     * again if it was a Chat.
     *
     * @param index the index of the item that was double clicked.
     */
    void onItemDoubleClicked(const QModelIndex &index);

    /**
     * Slot to react to mark read button clicks.
     */
    void onMarkReadButtonClicked();

private:
    /**
     * A pointer to the ui from qt-history.ui's generated source code.
     */
    Ui_HistoryView *ui;

    /**
     * The model to hold and display the history data from.
     */
    QtHistoryModel *m_model;

    /**
     * A model to help sort the history.
     */
    QSortFilterProxyModel *m_sortModel;
};


};

};

#endif
