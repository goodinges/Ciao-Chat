#include "qtloginview.h"
#include "ui_qt-login.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>

namespace Mvp
{

namespace View
{

QtLoginView::QtLoginView()
    : QDialog(NULL),
      ui(new Ui_LoginView)
{
    // In the constructor, just initialize the ui object.
    this->ui->setupUi(this);

    // Change the standard Ok button to a more appropriate one.
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Sign In"));
}

QtLoginView::~QtLoginView()
{
    delete ui;
    ui = 0;
}

void QtLoginView::accept()
{
    // Emit the loginEvent signal.
    Poco::EventArgs args;
    logInEvent(this, args);
}

void QtLoginView::closeEvent(QCloseEvent *event)
{
    Poco::EventArgs args;
    viewClosed(this, args);
}

void QtLoginView::destroyView()
{
    deleteLater();
}

void QtLoginView::showStatus(AbstractLoginView::LoginStatus code)
{
    // Show the LoginStatus as text.
    switch (code) {
    case AbstractLoginView::LoggedIn:
        ui->statusLabel->setText(tr("Signed In"));
        QMetaObject::invokeMethod(ui->passwordEdit, "clear");
        break;
    case AbstractLoginView::LoggedOut:
        ui->statusLabel->setText(tr("Signed Out"));
        break;
    case AbstractLoginView::LoggingIn:
        ui->statusLabel->setText(tr("Signing in..."));
        break;
    case AbstractLoginView::InputCredentials:
        QMessageBox::warning(this, tr("Input Credentials"),
            tr("Skype Name and Password are required"));
        break;
    }

    // Lets disable and enable the fields and button to avoid multiple requests.
    bool enable = code != AbstractLoginView::LoggingIn;
    QMetaObject::invokeMethod(ui->skypenameEdit, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enable));
    QMetaObject::invokeMethod(ui->passwordEdit, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enable));
    QMetaObject::invokeMethod(ui->buttonBox, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, enable));
}

void QtLoginView::setLoginError(LoginError code)
{
    // Show the LoginError as text.
    switch (code) {
        case AbstractLoginView::InvalidCredentials:
            ui->statusLabel->setText(tr("Invalid username or password"));
            break;
        case AbstractLoginView::TooManyAttempts:
            ui->statusLabel->setText(tr("Too many login attemps"));
            break;
        case AbstractLoginView::DiskError:
            ui->statusLabel->setText(tr("Database Disk error"));
            break;
        case AbstractLoginView::InvalidAppId:
            ui->statusLabel->setText(tr("Invalid Application ID"));
            break;
        case AbstractLoginView::UnsupportedVersion:
            ui->statusLabel->setText(tr("Unsupported Skype version"));
            break;
        case AbstractLoginView::InternalError:
            ui->statusLabel->setText(tr("Internal error"));
            break;
    }
}

void QtLoginView::initialize()
{
    // Initialize the status label to say Logged Out
    ui->statusLabel->setText(tr("Signed Out"));
}

void QtLoginView::showView()
{
    // Call QDialog::show with invokeMethod so it runs in the Qt GUI thread.
    QMetaObject::invokeMethod(this, "show");
}

void QtLoginView::hideView()
{
    // Call QDialog::hide with invokeMethod so it runs in the Qt GUI thread.
    QMetaObject::invokeMethod(this, "hide");
}

std::string QtLoginView::username()
{
    // Get the username text as an std::string from the ui's QLineEdit.
    return ui->skypenameEdit->text().toStdString();
}

std::string QtLoginView::password()
{
    // Get the password text as an std::string from the ui's QLineEdit.
    return ui->passwordEdit->text().toStdString();
}

};

};
