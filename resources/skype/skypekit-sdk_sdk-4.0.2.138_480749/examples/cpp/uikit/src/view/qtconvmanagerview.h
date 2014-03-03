#ifndef _QTCONVMANAGERVIEW_H
#define _QTCONVMANAGERVIEW_H

#include <QDialog>
#include <QModelIndex>
#include "abstractconvmanagerview.h"

class Ui_ConversationDialog;

namespace Mvp
{

namespace View
{

/**
 * @class QtConvManagerView qtconvmanagerview.h qtconvmanagerview.h
 * @brief A view used for showing and interacting with a list of contacts.
 *
 * This class is a view which displays the participants and non-participants
 * of a conversation, and allows the user to change them if possible.
 *
 * @see AbstractConvManagerView
 */
class QtConvManagerView : public QDialog, public AbstractConvManagerView
{
Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent a pointer to the object's parent window if any.
     */
    QtConvManagerView(QWidget *parent = NULL);

    /**
     * Destructor.
     */
    ~QtConvManagerView();

    /**
     * Initialize this ConvManagerView.
     */
    void initialize();

    /**
     * Show this view to the user.
     */
    void showView();

    void destroyView();

    /**
     * Set the initial non-participant list.
     *
     * @param nonParticipants the initial list of non-participants.
     */
    void setNonParticipants(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > nonParticipants);

    /**
     * Set the initial participant list.
     *
     * @param participants the initial list of participants.
     */
    void setParticipants(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > participants);

    void setConversationDetails(std::string &title, std::string &type);

    /**
     * Update the given contact's information.
     *
     * @param data The contact's new information to display.
     */
    void updateContact(const Poco::SharedPtr<Mvp::Model::ContactData> data);

public slots:
    /**
     * Slot overridden to react to ok button clicks.
     */
    void accept();

    /**
     * Slot overridden to react to cancel button click.
     */
    void reject();

protected:
    /**
     * Overridden method from QDialog to emit viewClosed signal when
     * this dialog is closed.
     */
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void onNonParticipantsListSelectionChanged(bool valid);

    void onParticipantsListSelectionChanged(bool valid);

    /**
     * Slot to respond to add button clicks.
     */
    void onAddClicked();

    /**
     * Slot to respond to remove button clicks.
     */
    void onRemoveClicked();

private:
    Q_DISABLE_COPY(QtConvManagerView)

    /**
     * A pointer to the ui from qt-convmanager.ui's generated source code.
     */
    Ui_ConversationDialog *ui;
};

};

};
#endif
