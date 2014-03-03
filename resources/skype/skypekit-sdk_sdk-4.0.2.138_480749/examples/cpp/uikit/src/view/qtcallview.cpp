#include "qtcallview.h"
#include "ui_qt-call.h"

#include <QDebug>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDateTime>

namespace Mvp
{

namespace View
{

QtCallView::QtCallView()
    :QWidget(NULL),
    m_timerEvent(new QTimer(this)),
    m_onHold(false),
    ui(new Ui_CallView)
{
    // In the constructor, just initialize the ui object.
    ui->setupUi(this);
    ui->localVideoWidget->hide();
    ui->remoteVideoWidget->hide();
    ui->dialpadWidget->hide();

    // Connect control buttons to their slots.
    connect(ui->buttonHold, SIGNAL(clicked()), SLOT(onHoldClicked()));
    connect(ui->buttonMute, SIGNAL(clicked()), SLOT(onMuteClicked()));
    connect(ui->buttonEnd, SIGNAL(clicked()), SLOT(onEndClicked()));
    connect(ui->buttonManage, SIGNAL(clicked()), SLOT(onManageClicked()));
    connect(ui->buttonVideo, SIGNAL(clicked()), SLOT(onVideoClicked()));
    connect(ui->buttonDialpad, SIGNAL(clicked()), SLOT(onDialpadClicked()));

    connect(ui->dialpadWidget, SIGNAL(buttonClicked(const QString &)),
            this, SLOT(onDialpadButtonClicked(const QString &)));

    // Set the call duration update interval to 1 second.
    m_timerEvent->setInterval(1000);
    connect(m_timerEvent, SIGNAL(timeout()), this, SLOT(durationUpdate()));
}

QtCallView::~QtCallView()
{
    delete ui;
    ui = 0;
}

void QtCallView::initialize()
{
    ui->labelStatus->setText(tr("Starting..."));

    enableControls(false);
    ui->listView->enableContextMenu(false);
}

void QtCallView::destroyView()
{
    deleteLater();
}

void QtCallView::showView()
{
    // Call QDialog::show with invokeMethod so it runs in the Qt GUI thread.
    if (isVisible() == false)
        QMetaObject::invokeMethod(this, "show");
    else
        activateWindow();
}

void QtCallView::setCallStatus(CallStatus status)
{
    switch (status) {
        case AbstractCallView::Starting:
            setCallStatusMsg(tr("Starting..."));
            break;
        case AbstractCallView::Ringing:
            setCallStatusMsg(tr("Ringing..."));

            // Accept or Reject the incoming call.
            QMetaObject::invokeMethod(this, "confirmAcceptance");
            break;
        case AbstractCallView::Ongoing:
            setCallStatusMsg(tr("Talking..."));
            enableControls(true);

            if (!m_onHold) {
                m_durationTime.restart();
                m_onHold = false;
            }

            startDurationCall(true);
            break;
        case AbstractCallView::OnHold:
            m_onHold = true;
            startDurationCall(false);
            setCallStatusMsg(tr("Call on hold"));
            break;
        case AbstractCallView::Done:
            setCallStatusMsg(tr("Call ended"));
            enableControls(false);
            startDurationCall(false);
            QMetaObject::invokeMethod(this, "close");
            break;
    }
}

void QtCallView::setCallStatusMsg(const QString msg)
{
    QMetaObject::invokeMethod(ui->labelStatus, "setText",
                              Q_ARG(QString, msg));
}

void QtCallView::setCallTitle(const std::string &title)
{
    QMetaObject::invokeMethod(this, "setWindowTitle",
                              Q_ARG(QString, title.c_str()));
}

void QtCallView::setParticipants(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts)
{
    ui->listView->setContactList(contacts);
}

void QtCallView::setVideoAvailable(bool available)
{
    QMetaObject::invokeMethod(ui->buttonVideo, "setEnabled",
                              Qt::QueuedConnection, Q_ARG(bool, available));
}

void QtCallView::updateContact(Poco::SharedPtr<Mvp::Model::ContactData> &contact)
{
    ui->listView->updateContact(contact);
}

void QtCallView::confirmAcceptance()
{
    QMessageBox dialog;

    QString text;
    text = QString("%1 is calling you").arg(windowTitle());
    dialog.setText(text);
    dialog.setWindowTitle("Incoming call");

    QPushButton *acceptButton = dialog.addButton(tr("Answer"),
            QMessageBox::YesRole);
    QPushButton *rejectButton = dialog.addButton(tr("Decline"),
            QMessageBox::NoRole);

    dialog.setDefaultButton(acceptButton);

    dialog.exec();

    Poco::EventArgs args;

     if (dialog.clickedButton() == acceptButton)
        accepted(this, args);
     else if (dialog.clickedButton() == rejectButton)
        rejected(this, args);
}

void QtCallView::enableControls(bool enabled)
{
    QMetaObject::invokeMethod(ui->buttonHold, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
    QMetaObject::invokeMethod(ui->buttonMute, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
    QMetaObject::invokeMethod(ui->buttonManage, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
    QMetaObject::invokeMethod(ui->buttonDialpad, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
}

void QtCallView::startDurationCall(bool start)
{
    if (start)
        QMetaObject::invokeMethod(m_timerEvent, "start");
    else
        QMetaObject::invokeMethod(m_timerEvent, "stop");

}

void QtCallView::onHoldClicked()
{
    bool checked = ui->buttonHold->isChecked();
    hold(this, checked);
}

void QtCallView::onMuteClicked()
{
    bool checked = ui->buttonMute->isChecked();
    mute(this, checked);
}

void QtCallView::onEndClicked()
{
    Poco::EventArgs args;
    end(this, args);
}

void QtCallView::onManageClicked()
{
    Poco::EventArgs args;
    manage(this, args);
}

void QtCallView::onVideoClicked()
{
    qDebug() << "onVideoClicked() called.";

    bool checked = ui->buttonVideo->isChecked();

    if (checked)
        ui->localVideoWidget->show();
    else
        ui->localVideoWidget->hide();

    // Disable the manage button once video starts. Enable once it stops.
    ui->buttonManage->setDisabled(checked);
    startLocalVideo(this, checked);
}

void QtCallView::onDialpadClicked()
{
    ui->dialpadWidget->setVisible(ui->buttonDialpad->isChecked());
}

void QtCallView::onDialpadButtonClicked(const QString &name)
{
    qDebug() << "Dialpad button:" << name;

    std::string data = name.toStdString();
    dtmfTone(this, data);
}

uint QtCallView::getLocalVideoWidget()
{
#ifdef _WIN32
    void* handle = ui->localVideoWidget->winId();
#else
    WId handle = ui->localVideoWidget->winId();
#endif
    qDebug() << "Got local video widget " << handle;

    return (uint)handle;
}

uint QtCallView::getRemoteVideoWidget()
{
#ifdef _WIN32
    void* handle = ui->remoteVideoWidget->winId();
#else
    WId handle = ui->remoteVideoWidget->winId();
#endif
    qDebug() << "Got remote video widget " << handle;

    return (uint)handle;
}

void QtCallView::showRemoteVideo(bool show)
{
    if (show) {
        QMetaObject::invokeMethod(ui->remoteVideoWidget, "show");
        // Disable the manage button once video starts.
        QMetaObject::invokeMethod(ui->buttonManage, "setDisabled",
            Q_ARG(bool, true));
    }
    else
        QMetaObject::invokeMethod(ui->remoteVideoWidget, "hide");
}

void QtCallView::durationUpdate()
{
    QDateTime dtime;
    dtime.setTimeSpec(Qt::UTC);
    dtime = dtime.addMSecs(m_durationTime.elapsed());
    ui->labelStatus->setText(tr("Call duration: ") +
            dtime.time().toString());
}

void QtCallView::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent caled";
    Poco::EventArgs args;
    viewClosed(this, args);
}

};

};
