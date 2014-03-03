#ifndef _QTCALLVIEW_H
#define _QTCALLVIEW_H

#include <QDialog>
#include <QTimer>
#include <QTime>

#include "abstractcallview.h"

class QCloseEvent;

class Ui_CallView;

namespace Mvp
{

namespace View
{

/**
 * @class QtCallView qtcallview.h qtcallview.h
 * @brief A view used for handling Skype Calls.
 *
 * This class is a view that will be used to manage an audio and video call. The
 * user will be able to perform basic actions such as hold and end a call, mute
 * the microphone and see the duration of the call.
 *
 * @see AbstractCallView
 */
class QtCallView : public QWidget, public AbstractCallView
{
Q_OBJECT
public:
    /**
     * Constructor.
     */
    QtCallView();

    /**
     * Destructor.
     */
    ~QtCallView();

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
     * Set the title of the call window.
     *
     * @param title the title to show for this call view.
     */
    void setCallTitle(const std::string &title);

    /**
     * Show the given status code in the user interface.
     *
     * Subclasses should implement this to show the given status code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param status The status code to show.
     */
    void setCallStatus(CallStatus status);

    /**
     * Set the participant list.
     *
     * When the call presenter is initialized, it asks the model  for the list
     * of participants, then passes that list to the view via this
     * setParticipants method.
     *
     * @param contacts A vector of contact data to use initially in the view.
     */
    void setParticipants(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts);

    /**
     * Set local video available status.
     *
     * Once the call is started we need to enable or disable the local video
     * support according to our local capabilities.
     *
     * @param available Set to true if there's video support, otherwise false.
     */
    void setVideoAvailable(bool available);

    /**
    * Update the given contact's contact data.
    *
    * @param contact the contact's new contact data to use.
    */
    void updateContact(Poco::SharedPtr<Mvp::Model::ContactData> &contact);

    /**
     * Destroy the view object.
     */
    void destroyView();

    /**
    * Get the local video widget's handle.
    *
    * @returns the window handle of the local video widget.
    */
    uint getLocalVideoWidget();

    /**
    * Get the remote video widget's handle.
    *
    * @returns the window handle of the remote video widget.
    */
    uint getRemoteVideoWidget();

    /**
    * Show/Hide the remote video widget.
    *
    * @param show true to show, false to hide the remote video widget.
    */
    void showRemoteVideo(bool show);

protected:
    /**
     * Overridden method from QDialog to emit viewClosed signal when
     * this dialog is closed.
     */
    void closeEvent(QCloseEvent *event);

private slots:
    /**
     * Slot called when the hold button is clicked.
     */
    void onHoldClicked();

    /**
     * Slot called when the mute button is clicked.
     */
    void onMuteClicked();

    /**
     * Slot called when the end button is clicked.
     */
    void onEndClicked();

    /**
     * Slot called when the manage button is clicked.
     */
    void onManageClicked();

    /**
     * Slot called when the video button is clicked.
     */
    void onVideoClicked();

    /**
     * Slot called when the dialpad button is clicked.
     */
    void onDialpadClicked();

    /**
     * Slot called whenever a dialpad key is clicked.
     */
    void onDialpadButtonClicked(const QString &name);

    void durationUpdate();

    /**
     * Ask the user to accept or reject the incoming call.
     */
    void confirmAcceptance();

private:
    Q_DISABLE_COPY(QtCallView)

    void enableControls(bool enabled);
    void startDurationCall(bool start);
    void setCallStatusMsg(const QString msg);

    QTimer *m_timerEvent;
    QTime m_durationTime;
    bool m_onHold;

    /**
     * A QDialog object loaded from qt-call.ui's generated code.
     */
    Ui_CallView *ui;
};

};

};

#endif

