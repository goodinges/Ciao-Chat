#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    previewVideoWindow = new VideoWidget();
    incomingVideoWindow = NULL;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (previewVideoWindow != NULL) delete previewVideoWindow;
    if (incomingVideoWindow != NULL) delete incomingVideoWindow;
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange: ui->retranslateUi(this); break;
        default: break;
    };
};

void MainWindow::prefillLogin (QString accName, QString accPsw)
{
    ui->loginEdit->setText(accName);
    ui->passwordEdit->setText(accPsw);
    ui->loginButton->setFocus();
};

void MainWindow::log (QString S)
{
    this->ui->eventLog->append(S);
};

void MainWindow::on_LogMessage (QString S)
{
    this->log(S);
};

void MainWindow::on_EnableLoginBtn (bool B)
{
    ui->loginButton->setEnabled(B);
};

void MainWindow::on_EnableCallBtn (bool B)
{
    ui->callButton->setEnabled(B);
};

void MainWindow::on_callButtonDropsCall (bool B)
{
    if (B) { ui->callButton->setText("Drop Call"); }
    else { ui->callButton->setText("Call"); };
};

void MainWindow::on_loginButton_clicked()
{
    ui->loginButton->setEnabled(false);
    SEString accountName;
    accountName = ui->loginEdit->text().toAscii();
    SEString password;
    password = ui->passwordEdit->text().toAscii();
    skype->Login(accountName, password);
};

void MainWindow::on_OpenIncomingVideoWindow(Participant::Ref part)
{
    if (incomingVideoWindow == NULL) incomingVideoWindow = new VideoWidget();
    SEString partId;
    part->GetPropLiveIdentity(partId);

    incomingVideoWindow->setParticipantName((const char*)partId);
    incomingVideoWindow->show();

    QSKParticipant::Ref p    = part;
    p->video->SetRemoteRendererId(incomingVideoWindow->getKey());
    incomingVideoWindow->renderStart();
};

void MainWindow::on_CloseIncomingVideoWindow(Participant::Ref part)
{
    incomingVideoWindow->renderStop();
    incomingVideoWindow->close();

    QSKParticipant::Ref p    = part;
    p->video->SetRemoteRendererId(0);
    delete incomingVideoWindow;
    incomingVideoWindow = NULL;
};

void MainWindow::on_callButton_clicked()
{
    if (!skype->isLiveSessionUp)
    {
        if (this->ui->contactList->count() == 0)
        {
            this->log("You got no contacts.");
        }
        else
        {
            if (this->ui->contactList->currentItem() != NULL)
            {
                QString callTarget = this->ui->contactList->currentItem()->text();
                this->SendPlaceCallSignal(callTarget);
            }
            else
            {
                dispatcher->log("No contact selected.");
            };
        };
    }
    else
    {
        skype->liveSession->LeaveLiveSession(false);
    };
};


void MainWindow::on_previewButton_clicked()
{
    if (previewVideoWindow->isVisible())
    {
        previewVideoWindow->hide();
        previewVideoWindow->renderStop();
        skype->previewVideo->Stop();
        return;
    };

    if (!skype->isVideoCapable)
    {
        dispatcher->log("No video devices were found.");
    }
    else
    {
        if (!skype->isPreviewVideoOk)
        {
            skype->isPreviewVideoOk = skype->GetPreviewVideo(
                Video::MEDIA_VIDEO,
                skype->previewVideo,
                skype->videoDeviceName,
                skype->videoDeviceId);

            skype->previewVideo->localPreview = true;
            previewVideoWindow->setParticipantName("Local preview");
            skype->previewVideo->SetRemoteRendererId( previewVideoWindow->getKey() );
        }
        if (skype->isPreviewVideoOk)
        {
            skype->previewVideo->Start();
            previewVideoWindow->renderStart();
            previewVideoWindow->show();
        };
    };
};

void MainWindow::on_UpdateContactList ()
{
    skype->GetHardwiredContactGroup(ContactGroup::ALL_KNOWN_CONTACTS, skype->allBuddiesGroup);
    skype->allBuddiesGroup->GetContacts(skype->allBuddies);

    this->log("Updating contatct list.");
    this->ui->contactList->clear();    

    skype->GetHardwiredContactGroup(ContactGroup::ALL_KNOWN_CONTACTS, skype->allBuddiesGroup);
    skype->allBuddiesGroup->GetContacts(skype->allBuddies);

    int contactCount = skype->allBuddies.size();
    for (int i = 0; i < contactCount; i++)
    {
        SEString contactName;
        Contact::TYPE contactType;

        skype->allBuddies[i]->GetPropType(contactType);
        if (contactType == Contact::SKYPE)
        {
            skype->allBuddies[i]->GetPropSkypename(contactName);
        }
        else
        {
            skype->allBuddies[i]->GetPropPstnnumber(contactName);
        };

        QString QS = (const char*)contactName;
        QListWidgetItem * contact;
        contact = new(QListWidgetItem);
        contact->setText(QS);
        this->ui->contactList->addItem(contact);
    };
};


void MainWindow::on_contactList_doubleClicked(QModelIndex index)
{
    if (skype->isLiveSessionUp)
    {
        this->log("There is a call up already.");
    }
    else
    {
        if (this->ui->contactList->count() == 0)
        {
            this->log("You got no contacts.");
        }
        else
        {
            QString CallTarget = this->ui->contactList->currentItem()->text();
            this->log("Calling " + CallTarget);
            this->SendPlaceCallSignal(CallTarget);
        }
    };
};
