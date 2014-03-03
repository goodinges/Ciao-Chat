#include "qtcontactlistview.h"
#include "qtcontactlistwidget.h"
#include "qtpstnnumber.h"
#include "skypeimages.h"
#include "qtimagecache.h"
#include "ui_qt-contactlist.h"

#include <QDebug>
#include <QFileDialog>

namespace Mvp
{

namespace View
{

QtContactListView::QtContactListView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui_ContactsDialog),
      m_pstnNumber(0)
{
    ui->setupUi(this);

    connect(ui->listView, SIGNAL(callContact(QString)),
        this, SLOT(onCallContact(QString)));
    connect(ui->listView, SIGNAL(chatContact(QString)),
        this, SLOT(onChatContact(QString)));
    connect(ui->historyButton, SIGNAL(clicked()),
        this, SLOT(onHistoryClicked()));
    connect(ui->listView, SIGNAL(fileTransferContact(QString)),
        this, SLOT(onFileTransferContact(QString)));
    connect(ui->buttonDialpad, SIGNAL(clicked()),
        this, SLOT(onDialpadClicked()));

    // Default avatar that is set when the user has no avatar image.
    m_defaultAvatar = QPixmap(Mvp::View::SkypeImage::Contact);
}

QtContactListView::~QtContactListView()
{
    delete ui;
    ui = 0;
}

void QtContactListView::destroyView()
{
    deleteLater();
}

void QtContactListView::onCallContact(const QString &skypeName)
{
    callContact(this, skypeName.toStdString());
}

void QtContactListView::onChatContact(const QString &skypeName)
{
    chatContact(this, skypeName.toStdString());
}

void QtContactListView::onHistoryClicked()
{
    qDebug() << "onHistoryClicked called.";
    Poco::EventArgs args;
    viewHistory(this, args);
}

void QtContactListView::onFileTransferContact(const QString &skypeName)
{
    // Ask the user a file to send.
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Select file"));

    if (!fileName.isEmpty() && !fileName.isNull()) {
        // Add the contact name + the file path to a pair and emit the signal.
        std::pair<const std::string, const std::string> data(
                skypeName.toStdString(), fileName.toStdString());
        fileTransferContact(this, data);
    }
}

void QtContactListView::initialize()
{
}

void QtContactListView::showView()
{
    QWidget::show();
}

void QtContactListView::setContactList(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts)
{
    qDebug() << "adding " << contacts.size() << " contacts to the model";

    ui->listView->setContactList(contacts);
}

void QtContactListView::setAccountInfo(
        const Poco::SharedPtr<Mvp::Model::ContactData> data)
{
    Q_ASSERT(data);

    qDebug() << "setting the account information";

    // Set formatted data.
    ui->labelIdentity->setText(tr("<b>%1</b> (%2)").arg(data->displayName.data())
            .arg(data->skypeName.data()));
    ui->labelAvailability->setText(tr("<b><i>%1</i></b>").arg(data->availability.data()));

    QPixmap avatar;

    if (data->avatar.size() > 0) {
        avatar.loadFromData(reinterpret_cast<const uchar *>(
                    data->avatar.rawContent()),
                    data->avatar.size());
        avatar = avatar.scaled(QSize(32, 32));
    }
    else {
        avatar = m_defaultAvatar;
    }

    ui->labelAvatar->setPixmap(avatar);
    QPixmap statusPixmap = QPixmap::fromImage(
            QtImageCache::instance()->image(data->status));

    ui->labelAvailabilityIcon->setPixmap(statusPixmap);
}

void QtContactListView::updateContact(
        const Poco::SharedPtr<Mvp::Model::ContactData> data)
{
    ui->listView->updateContact(data);
}

void QtContactListView::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent caled";
    Poco::EventArgs args;
    viewClosed(this, args);
}

void QtContactListView::onDialpadClicked()
{
    // Lets check if there's already an instance running.
    if (!m_pstnNumber) {
        /* Create a new dialog to get the PSTN number.
         * Note: It's not needed to delete this instance, since the object
         * will destruct/free itself when the widget is closed.
         */
        m_pstnNumber = new QtPSTNNumber(this);

        // Connect a signal to know when it's finished.
        connect(m_pstnNumber, SIGNAL(finished(int)),
                this, SLOT(onPTNNumberFinished(int)));

        // Show the dialog.
        m_pstnNumber->show();
    }
    else {
        m_pstnNumber->activateWindow();
    }
}

void QtContactListView::onPTNNumberFinished(int result)
{
    // Check if it was accepted and has a valid number.
    if (result == QDialog::Accepted &&
        m_pstnNumber->action() != QtPSTNNumber::None) {

        // Start a call/chat with the given number.
        if (m_pstnNumber->action() == QtPSTNNumber::Call)
            callContact(this, m_pstnNumber->number().toStdString());
        else
            smsContact(this, m_pstnNumber->number().toStdString());

    }

    // As the widget will self destruct, lets make it zero too.
    m_pstnNumber = 0;
}

};

};
