#include "qtftview.h"
#include "ui_qt-ft.h"

#include <QDebug>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFileDialog>


namespace Mvp
{

namespace View
{

QtFTView::QtFTView()
    :QWidget(NULL),
    m_fileSize(0),
    m_paused(false),
    m_direction(Incoming),
    m_currentStatus(AbstractFTView::Done),
    m_sourceFileName(""),
    ui(new Ui_FTView)
{
    // In the constructor, just initialize the ui object.
    ui->setupUi(this);

    // Connect control buttons to their slots.
    connect(ui->buttonPauseResume, SIGNAL(clicked()),
            SLOT(onPauseResumeClicked()));
    connect(ui->buttonCancel, SIGNAL(clicked()), SLOT(onCancelClicked()));

    // Disable Pausing and wait for the first status.
    enablePauseResume(false);

    // Enable Canceling as default.
    enableCancel(true);
}

QtFTView::~QtFTView()
{
    delete ui;
    ui = 0;
}

void QtFTView::initialize()
{
    ui->labelStatus->setText(tr("Starting..."));
}

void QtFTView::destroyView()
{
    deleteLater();
}

void QtFTView::showView()
{
    // Call QDialog::show with invokeMethod so it runs in the Qt GUI thread.
    if (isVisible() == false)
        QMetaObject::invokeMethod(this, "show");
    else
        activateWindow();
}

void QtFTView::setFTStatus(FTStatus status)
{
    // Check if the new status is the same as the current status.
    if (m_currentStatus == status)
        return;

    // Assigned the new status as the current status.
    m_currentStatus = status;

    // Handle status changes.
    switch (status) {
        case AbstractFTView::Connecting:
            setFTStatusMsg(tr("Connecting..."));

            // Accept or Reject the incoming ft.
            if (m_direction == Incoming)
                QMetaObject::invokeMethod(this, "confirmAcceptance");
            break;
        case AbstractFTView::WaitingForAccept:
            setFTStatusMsg(tr("Waiting for authorization"));
            break;
        case AbstractFTView::Transferring:
            m_paused = false;
            ui->buttonPauseResume->setText(tr("Pause"));
            setFTStatusMsg(tr("Transferring file"));
            enablePauseResume(true);
            enableCancel(true);
            break;
        case AbstractFTView::Paused:
            m_paused = true;
            ui->buttonPauseResume->setText(tr("Resume"));
            setFTStatusMsg(tr("Paused"));
            break;
        case AbstractFTView::Failed:
            setFTStatusMsg(tr("Failed"));
            enablePauseResume(false);
            enableCancel(false);
            break;
        case AbstractFTView::Done:
            setFTStatusMsg(tr("Completed"));
            enablePauseResume(false);
            enableCancel(false);
            break;
    }
}

void QtFTView::setFTSourceFileName(const std::string &fileName)
{
    m_sourceFileName = QString::fromUtf8(fileName.c_str());
}

void QtFTView::setFTError(FTError error)
{
    // Handle errors.
    switch (error) {
        case AbstractFTView::Cancelled:
            setFTStatusMsg(tr("File cancelled"));
            break;
        case AbstractFTView::FTNotSupported:
            setFTStatusMsg(tr("Contact can't send or receive files"));
            break;
        case AbstractFTView::Timeout:
            setFTStatusMsg(tr("Timeout"));
            break;
        case AbstractFTView::FailedOnRead:
            setFTStatusMsg(tr("Failed on read"));
            break;
        case AbstractFTView::FailedOnWrite:
            setFTStatusMsg(tr("Failed on write"));
            break;
    }
}

void QtFTView::setFTStatusMsg(const QString msg)
{
    QMetaObject::invokeMethod(ui->labelStatus, "setText",
                              Q_ARG(QString, msg));
}

void QtFTView::setFTTitle(const std::string &title)
{
    QString text = QString("File Transfer Session - %1")
        .arg(title.c_str());

    QMetaObject::invokeMethod(this, "setWindowTitle",
                              Q_ARG(QString, text));
}

void QtFTView::setSourceDesc(const std::string &source)
{
    QMetaObject::invokeMethod(ui->labelSourceDesc, "setText",
                              Q_ARG(QString, source.c_str()));
}

void QtFTView::setDestinationDesc(const std::string &dest)
{
    QMetaObject::invokeMethod(ui->labelDestinationDesc, "setText",
                              Q_ARG(QString, dest.c_str()));
}

void QtFTView::setFileSize(const std::string &size)
{
    m_fileSize = QString(size.c_str()).toLong();
}

void QtFTView::updateTransferData(const std::string &bytesTrans,
        uint bytesPS)
{
    // Format the transfer progress into a good description.
    const double byteRate = 1024;
    QString transStr(bytesTrans.c_str());

    double trans = (transStr.toDouble() / byteRate) / byteRate;
    double bps = bytesPS / byteRate;
    double size = (m_fileSize / byteRate) / byteRate;

    QString verbose = QString(tr("%L1 Mb of %L2 Mb (%L3 Kbytes/sec)"))
        .arg(trans, 0, ' ', 2)
        .arg(size, 0, ' ', 2)
        .arg(bps, 0, ' ', 2);

    // Update the text description.
    QMetaObject::invokeMethod(ui->labelProgressDesc, "setText",
                              Q_ARG(QString, verbose));

    // Update the progress bar.
    int value = (trans * 100) / size;
    QMetaObject::invokeMethod(ui->progressBarTransfer, "setValue",
                              Q_ARG(int, value));
}

void QtFTView::setDirection(const FTDirection &direction)
{
    m_direction = direction;
}

void QtFTView::enablePauseResume(bool enabled)
{
    QMetaObject::invokeMethod(ui->buttonPauseResume, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
}

void QtFTView::enableCancel(bool enabled)
{
    QMetaObject::invokeMethod(ui->buttonCancel, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
}

void QtFTView::confirmAcceptance()
{
    QMessageBox dialog;

    QString text;
    text = QString("File: %1").arg(ui->labelSourceDesc->text());
    dialog.setText(text);
    dialog.setWindowTitle("Incoming file transfer");

    QPushButton *acceptButton = dialog.addButton(tr("Accept"),
            QMessageBox::YesRole);
    QPushButton *rejectButton = dialog.addButton(tr("Decline"),
            QMessageBox::NoRole);

    dialog.setDefaultButton(acceptButton);

    dialog.exec();

    Poco::EventArgs args;

     if (dialog.clickedButton() == acceptButton) {
        // Ask the user a place to save the file.
         std::string fileName;
        fileName = QFileDialog::getSaveFileName(this,
                tr("Save as"), m_sourceFileName).toStdString();

        if (fileName.empty())
            rejected(this, args);
        else
            accepted(this, fileName);
     }
     else if (dialog.clickedButton() == rejectButton) {
        rejected(this, args);
     }
}

void QtFTView::onPauseResumeClicked()
{
    Poco::EventArgs args;

    if (m_paused)
        resume(this, args);
    else
        pause(this, args);
}

void QtFTView::onCancelClicked()
{
    Poco::EventArgs args;
    cancel(this, args);
}

void QtFTView::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent caled";
    Poco::EventArgs args;
    viewClosed(this, args);
}

};

};
