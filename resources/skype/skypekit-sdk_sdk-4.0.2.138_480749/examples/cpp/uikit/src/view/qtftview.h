#ifndef _QTFTVIEW_H
#define _QTFTVIEW_H

#include <QDialog>

#include "abstractftview.h"

class QCloseEvent;

class Ui_FTView;

namespace Mvp
{

namespace View
{

/**
 * @class QtFTView qtftview.h qtftview.h
 * @brief A view used for handling Skype File Transfers.
 *
 * This class is a view that will be used to manage a file transfer session.
 * The user will be able to perform basic actions such as pause and resume a
 * ft, accept, reject and cancel.
 *
 * @see AbstractFTView
 */
class QtFTView : public QWidget, public AbstractFTView
{
Q_OBJECT
public:
    /**
     * Constructor.
     */
    QtFTView();

    /**
     * Destructor.
     */
    ~QtFTView();

    /**
     * Initialize the dialog.
     */
    void initialize();

    /**
     * Ask the view to display.
     *
     * This method just fts QDialog::show().
     */
    void showView();

    /**
     * Set the title of the file transfer window.
     *
     * @param title the title to show for this file transfer view.
     */
    void setFTTitle(const std::string &title);

    /**
     * Show the given status code in the user interface.
     *
     * Subclasses should implement this to show the given status code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param status The status code to show.
     */
    void setFTStatus(FTStatus status);

    /**
     * Give the source filename.
     *
     * @param filename the original filename of the transfer.
     */
    void setFTSourceFileName(const std::string &fileName);

    /**
     * Show the given error code in the user interface.
     *
     * Subclasses should implement this to show the given error code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param error The error code to show.
     */
    void setFTError(FTError error);

    /**
     * Set the file source description.
     *
     * @param source the description of the file being sent.
     */
    void setSourceDesc(const std::string &source);

    /**
     * Set the file destination  description.
     *
     * @param dest the description of the file being sent.
     */
    void setDestinationDesc(const std::string &dest);

    /**
     * Set the file size.
     *
     * When a new session is initiated it's necessary to know what is the size
     * of the file being negotiated before starting the transference. The size
     * format is 'bytes'.
     *
     * @param size The number of bytes contained in the file.
     */
    void setFileSize(const std::string &size);

    /**
     * Set the transfer's direction.
     *
     * As the view needs to handle both transfer directions (Incoming and
     * Outgoing) this method will set a flag saying which kind of transfer the
     * view is going to manage.
     *
     * @param direction the transfer direction going to be managed.
     */
    void setDirection(const FTDirection &direction);

    /**
     * Updates the transfer progress.
     *
     * Whenever the transfer progress is changed this method will be called to
     * update the view.
     *
     * @param bytesTrans the number of bytes already transferred.
     * @param bytesPS the number of bytes being transferred per second (speed).
     */
    void updateTransferData(const std::string &bytesTrans, uint bytesPS);

    /**
     * Destroy the view object.
     */
    void destroyView();

protected:
    /**
     * Overridden method from QDialog to emit viewClosed signal when
     * this dialog is closed.
     */
    void closeEvent(QCloseEvent *event);

private slots:
    /**
     * Slot fted when the hold button is clicked.
     */
    void onPauseResumeClicked();

    /**
     * Slot fted when the mute button is clicked.
     */
    void onCancelClicked();

    /**
     * Ask the user to accept or reject the incoming ft.
     */
    void confirmAcceptance();

private:
    Q_DISABLE_COPY(QtFTView)

    void setFTStatusMsg(const QString msg);
    void enablePauseResume(bool enabled);
    void enableCancel(bool enabled);

    long m_fileSize;
    bool m_paused;
    FTDirection m_direction;
    AbstractFTView::FTStatus m_currentStatus;
    QString m_sourceFileName;

    /**
     * A QDialog object loaded from qt-ft.ui's generated code.
     */
    Ui_FTView *ui;
};

};

};

#endif
