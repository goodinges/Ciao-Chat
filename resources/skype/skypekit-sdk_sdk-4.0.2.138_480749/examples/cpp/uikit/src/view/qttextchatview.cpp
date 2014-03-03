#include "qttextchatview.h"
#include "ui_qt-textchat.h"
#include "skypeimages.h"

#include <QDebug>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QDomDocument>
#include <QMessageBox>
#include <QPushButton>

namespace Mvp
{

namespace View
{

QtTextChatView::QtTextChatView()
    : QWidget(NULL),
      m_hasMessageToConsume(false),
      ui(new Ui_TextChatView)
{
    // In the constructor, just initialize the ui object.
    ui->setupUi(this);
    connect(ui->messageEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->buttonManage, SIGNAL(clicked()), SLOT(onManageClicked()));
    connect(ui->messageTypeButton, SIGNAL(clicked()),
        SLOT(onMessageTypeButtonClicked()));
    connect(ui->messageEdit, SIGNAL(textChanged(QString)),
        SLOT(onMessageEditTextChanged(QString)));
    connect(ui->buttonLeave, SIGNAL(clicked()), this, SLOT(onLeaveClicked()));
}

QtTextChatView::~QtTextChatView()
{
    delete ui;
    ui = 0;
}

void QtTextChatView::initialize()
{
    ui->participantList->enableContextMenu(false);
    ui->modeLabel->hide();
    ui->buttonLeave->hide();
}

void QtTextChatView::destroyView()
{
    deleteLater();
}

void QtTextChatView::showView()
{
    // Call QDialog::show with invokeMethod so it runs in the Qt GUI thread.
    if (isVisible() == false)
        QMetaObject::invokeMethod(this, "show");
    else
        activateWindow();
}

void QtTextChatView::sendMessage()
{
    // Get the message to post.
    std::string messageText = ui->messageEdit->text().toUtf8().data();

    // Post it.
   postMessage(this, messageText);

    // Clear the lineedit for the next message.
    ui->messageEdit->clear();
}

void QtTextChatView::setChatTitle(const std::string &title)
{
    QMetaObject::invokeMethod(this, "setWindowTitle",
                              Q_ARG(QString, title.c_str()));
}

void QtTextChatView::addMessage(Poco::SharedPtr<Mvp::Model::MessageData> &message)
{
    QColor nameColor =
        qApp->palette().color(QPalette::Link);

    // Use the authorDisplayname if it's different than the last name we used.
    QString displayName;
    QString span("<span style='color:%1;'>%2</span>");

    QString body = parseBodyXml(message->body);

    // If it's an emote, put the displayname as is.
    if (message->isEmote)
        displayName = message->authorDisplayname.c_str();
    else if (message->authorDisplayname.c_str() == m_lastDisplayname)
        // If it's not an emote and the name is the same use ...
        displayName = span.arg(nameColor.name()).arg("...");
    else
        // Otherwise just wrap the name in the span.
        displayName = span.arg(nameColor.name())
            .arg(message->authorDisplayname.c_str());

    if (!message->isEmote)
        // If it's not an emote, record the current author as the last author.
        m_lastDisplayname = message->authorDisplayname.c_str();
    else
        // If it is an emote, clear the last author.
        m_lastDisplayname.clear();

    QDateTime time = QDateTime::fromTime_t(message->timestamp);

    QString messagetext = QString("[%1] %2 %3")
        .arg(time.time().toString("h:mm:ss AP"))
        .arg(displayName).arg(body);
    QMetaObject::invokeMethod(ui->messageHistory, "append",
                              Q_ARG(QString, messagetext));

    if (isActiveWindow()) {
        // Notify that all messages were consumed.
        Poco::EventArgs args;
        messagesConsumed(this, args);
    }
    else {
        // Mark messages to consume.
        m_hasMessageToConsume = true;
    }
}

void QtTextChatView::reportMessageStatus(StatusCode code, std::string &message)
{
    // Get the color for inactive text.
    QColor statusColor =
        qApp->palette().color(QPalette::Mid);

    // Construct the error message.
    QString messageText =
        QString("<span style='color:%1; font-style:italic;'>"
                "** %2 - text: %3</span><br />")
        .arg(statusColor.name()).arg(statusText(code)).arg(message.c_str());

    // Add the error message to the history view.
    QMetaObject::invokeMethod(ui->messageHistory, "insertHtml",
                              Q_ARG(QString, messageText));
}

QString QtTextChatView::statusText(StatusCode &statusCode)
{
    QString text;

    switch (statusCode) {
        case IMFailed:
            text = "IM failed to send";
            break;
        case SmsDelivered:
            text = "Sms delivered successfully";
            break;
        case MiscError:
            text = "Sms failed: miscellaneous error";
            break;
        case ServerConnectFailed:
            text = "Sms failed: server connection failed";
            break;
        case NoSmsCapability:
            text = "Sms failed: no sms capability";
            break;
        case InsufficientFunds:
            text = "Sms failed: insufficient funds";
            break;
        case InvalidConfirmationCode:
            text = "Sms failed: invalid confirmation code";
            break;
        case UserBlocked:
            text = "Sms failed: user is blocked";
            break;
        case IPBlocked:
            text = "Sms failed: IP blocked";
            break;
        case NodeBlocked:
            text = "Sms failed: node blocked";
            break;
        case NoSenderIDCapability:
            text = "Sms failed: no senderid capability";
            break;
        case GeneralError:
        default:
            text = "Sms failed: general internal error";
    }

    return text;
}

QString QtTextChatView::parseBodyXml(std::string &body)
{
    QString parsedBody(QString::fromUtf8(body.c_str()));

    QDomDocument document;
    if (document.setContent(parsedBody)) {
        /**
         * Here's an example of what sms xml looks like:
         *
         * <sms alt="test sms with some icons :)">
         *     <type>2</type>
         *     <status>4</status>
         *     <failurereason>0</failurereason>
         *     <targets>
         *         <target status="1">+15551234567</target>
         *     </targets>
         *     <price>4294967.295 USD</price>
         *     <body>
         *         <chunk id="0">test sms with some icons :)</chunk>
         *         <untilnext>133</untilnext>
         *     </body>
         *     <sendtimestamp>1289849043</sendtimestamp>
         * </sms>
         *
         * For display purposes we only need to extract the alt property
         * from the sms tag and display that as the message text.
         */

        // The body string is valid xml, so let's break it up.
        QDomNodeList smsNodes = document.elementsByTagName("sms");

        // There are sms nodes.
        if (smsNodes.size() > 0) {

            // We only care about the first sms tag's alt attribute.
            QDomNode node = smsNodes.item(0);
            QDomNamedNodeMap attributes = node.attributes();

            // So get the alt attribute.
            QDomNode altNode = attributes.namedItem("alt");
            if (!altNode.isNull()) {
                QDomAttr altValueNode = altNode.toAttr();

                // And assign it's text to the parsed body.
                if (!altValueNode.isNull())
                    parsedBody = QString("Sms: %1")
                        .arg(altValueNode.value());
            }
        }
    }

    return parsedBody;
}

void QtTextChatView::setParticipants(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts)
{
    ui->participantList->setContactList(contacts);
}

void QtTextChatView::updateContact(
    Poco::SharedPtr<Mvp::Model::ContactData> &contact)
{
    ui->participantList->updateContact(contact);
}

void QtTextChatView::setSmsMode(bool smsMode)
{
    QMetaObject::invokeMethod(this, "slotToggleSendingMode",
                              Q_ARG(bool, smsMode));
}

void QtTextChatView::enableModeToggle(bool canSms)
{
    QMetaObject::invokeMethod(ui->messageTypeButton, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, canSms));

}

void QtTextChatView::showLeaveAction(bool show)
{
    QMetaObject::invokeMethod(ui->buttonLeave, "setVisible",
        Qt::QueuedConnection, Q_ARG(bool, show));
}

void QtTextChatView::onManageClicked()
{
    Poco::EventArgs args;
    manage(this, args);
}

void QtTextChatView::onMessageTypeButtonClicked()
{
    Poco::EventArgs args;
    toggleMode(this, args);
}

void QtTextChatView::slotToggleSendingMode(bool smsMode)
{
    if (smsMode) {
        ui->messageTypeButton->setIcon(QIcon(Mvp::View::SkypeImage::MessageSMS));
        ui->messageTypeButton->setText(tr("SMS Mode"));
        ui->modeLabel->show();
        onMessageEditTextChanged(ui->messageEdit->text());
    }
    else {
        ui->messageTypeButton->setIcon(QIcon(Mvp::View::SkypeImage::Message));
        ui->messageTypeButton->setText(tr("IM Mode"));
        ui->modeLabel->hide();
        ui->modeLabel->setText("");
    }
}

void QtTextChatView::onMessageEditTextChanged(const QString &text)
{
    unsigned int left = text.length() > 160 ? 0 : 160 - text.length();

    ui->modeLabel->setText(QString("%1 characters left").arg(left));
}

void QtTextChatView::enableControls(bool enabled)
{
    enableModeToggle(enabled);

    QMetaObject::invokeMethod(ui->messageEdit, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
    QMetaObject::invokeMethod(ui->buttonManage, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enabled));
}

void QtTextChatView::onLeaveClicked()
{
    // Create a dialog to ask for the user confirmation.
    QMessageBox dialog;

    dialog.setWindowTitle(tr("Really leave chat?"));
    dialog.setInformativeText(tr("Really leave chat?"));
    dialog.setText(tr("If you leave this chat, you will no longer receive " \
                "any new messages from the chat. Are you really sure you " \
                "want to leave the chat?"));

    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    dialog.setDefaultButton(QMessageBox::No);

    int ret = dialog.exec();

    if (ret == QMessageBox::Yes) {
        // Leave the chat.
        Poco::EventArgs args;
        leaveChat(this, args);

        // Close the chat.
        close();
    }
}

bool QtTextChatView::event(QEvent *event)
{
    /* Check if this is the activation event and if there's
     * messages to be consumed.
     */
    if ((event->type() == QEvent::WindowActivate) &&
        m_hasMessageToConsume) {

        // Notify that all messages were consumed.
        Poco::EventArgs args;
        messagesConsumed(this, args);

        // Mark messages as consumed.
        m_hasMessageToConsume = false;
    }

    // Forward the all events to the parents.
	return QWidget::event(event);
}

void QtTextChatView::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent caled";
    Poco::EventArgs args;
    viewClosed(this, args);
}

};

};
