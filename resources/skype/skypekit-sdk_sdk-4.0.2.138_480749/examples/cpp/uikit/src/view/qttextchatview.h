#ifndef _QTTEXTCHATVIEW_H
#define _QTTEXTCHATVIEW_H

#include <QDialog>
#include "abstractchatview.h"

class QCloseEvent;

class Ui_TextChatView;

namespace Mvp
{

namespace View
{

/**
 * @class QtTextChatView qttextchatview.h qttextchatview.h
 * @brief A view used for logging in to the Skype Network.
 *
 * This class is a view with a message history view and a LineEdit to enter
 * new messages for the current chat into.  Pressing enter in the LineEdit
 * sends the current text to the chat.
 *
 * @see AbstractTextChatView
 */
class QtTextChatView : public QWidget, public AbstractChatView
{
Q_OBJECT
public:
    /**
     * Constructor.
     */
    QtTextChatView();

    /**
     * Destructor.
     */
    ~QtTextChatView();

    /**
     * Initialize the dialog.
     */
    void initialize();

    /**
     * Ask the view to display.
     *
     * This method just calls QDialog::show().
     */
    void showView();

    /**
     * Set the title of the chat.
     *
     * @param title the title to show for this chat view.
     */
    void setChatTitle(const std::string &title);

    /**
     * Add a message to the view.
     *
     * When a new message comes from skype, this method is called to add
     * the message to the chat window in the log area.
     *
     * @param message the message data to show.
     */
    void addMessage(Poco::SharedPtr<Mvp::Model::MessageData> &message);

    /**
     * Add a message about a status change to the view.
     *
     * @param code the code to display.
     * @param message the message that generated the given status code.
     */
    void reportMessageStatus(StatusCode code, std::string &message);

    /**
     * Set the participant list.
     *
     * When the chat presenter is initialized, it asks the model  for the list
     * of participants, then passes that list to the view via this
     * setParticipants method. Any time the participant list changes the
     * new list is fetched and passed into this method.
     *
     * @param contacts A vector of contact data to use initially in the view.
     */
    void setParticipants(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts);

    /**
     * Update the given contact's contact data.
     *
     * @param contact the contact's new contact data to use.
     */
    void updateContact(Poco::SharedPtr<Mvp::Model::ContactData> &contact);

    /**
     * Set the Sms mode.
     *
     * @param smsMode true to use smsMode, false to use IM mode.
     */
    void setSmsMode(bool smsMode);

    /**
     * Enable or Disable the chat controls.
     *
     * @param enabled true to enable all controls, false to disable.
     */
    void enableControls(bool enabled);

    /**
     * Destroy the view object.
     */
    void destroyView();

    /**
     * Show/hide the leave chat button.
     *
     * @param show true to show the button, false to hide the button.
     */
    void showLeaveAction(bool show);

protected:
    /**
     * Overridden method from QDialog to emit viewClosed signal when
     * this dialog is closed.
     */
     void closeEvent(QCloseEvent *event);

private slots:
    /**
     * Slot to send the current message in the messageedit.
     */
    void sendMessage();

    /**
     * Slot to react to Manage Button being clicked.
     */
    void onManageClicked();

    /**
     * Slot to react to SMS/IM toggle being clicked.
     */
    void onMessageTypeButtonClicked();

    /**
     * Slot to set the mode in the ui.
     */
    void slotToggleSendingMode(bool smsMode);

    /**
     * Slot to react to message edit changes.
     */
    void onMessageEditTextChanged(const QString &text);

    /**
    * Enable the sms/im toggle button if sms mode is currently available.
    *
    * @param canSms true if the current conversation allows sending of sms.
    */
    void enableModeToggle(bool canSms);

    /**
     * Slot to react to the Leave Button being clicked.
     */
    void onLeaveClicked();

protected:
    /**
     * Reimplement the event handler to intercept needed events.
     *
     * @param event the event to be handled.
     */
    bool event(QEvent *event);

private:
    Q_DISABLE_COPY(QtTextChatView)

    /**
     * Prepare the body of the message for viewing, parsing any xml.
     *
     * Message bodyxml can have xml in it, so this helper method is used
     * to simplify the text to what we actually want to display.
     *
     * @param body the body of the message text.
     * @returns the parsed body text ready for display as a QString
     */
    QString parseBodyXml(std::string &body);

    /**
     * Get the status text for the given status code.
     *
     * @returns a string representation of the given status code.
     */
    QString statusText(StatusCode &statusCode);

    /**
     * Last display name shown, so we can use "..." instead of the whole name
     * on repeat messages from the same person.
     */
    QString m_lastDisplayname;

    /**
     * Marks that there're messages to be consumed.
     */
    bool m_hasMessageToConsume;

    /**
     * A QDialog object loaded from qt-textchat.ui's generated code.
     */
    Ui_TextChatView *ui;
};

};

};

#endif

