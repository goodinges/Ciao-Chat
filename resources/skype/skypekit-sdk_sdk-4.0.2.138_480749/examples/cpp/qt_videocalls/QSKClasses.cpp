
#include <mainwindow.h>
#include "QSKClasses.h"
#include "videowidget.h"

MainWindow*             mainForm;
QSKSignalDispatcher*    dispatcher;
QSKSkype*               skype;

//------------------------------------------------------------------------------------------------
// Signaler methods

// The following line is necessary to send Participant references over Qt signaling system
Q_DECLARE_METATYPE(QSKParticipant::Ref)

void QSKSignalDispatcher::ConnectToUI()
{
    // Connecting signals from SkypeKit side (dispatcher) to GUI side (mainForm)
    QObject::connect (
            this,       SIGNAL  (SendToLogSignal(QString)),
            mainForm,   SLOT    (on_LogMessage(QString))
            );

    QObject::connect (
            this,       SIGNAL  (UpdateContactListSignal()),
            mainForm,   SLOT    (on_UpdateContactList())
            );


    QObject::connect (
            this,       SIGNAL  (EnableCallBtnSignal(bool)),
            mainForm,   SLOT    (on_EnableCallBtn(bool))
            );

    QObject::connect (
            this,       SIGNAL  (EnableLoginBtnSignal(bool)),
            mainForm,   SLOT    (on_EnableLoginBtn(bool))
            );

    QObject::connect (
            this,       SIGNAL  (ToggleCallBtnDropsCallSignal(bool)),
            mainForm,   SLOT    (on_callButtonDropsCall(bool))
            );

    // The following line is also necessary to send Participant references over Qt signaling system
    qRegisterMetaType<Participant::Ref>("Participant::Ref");

    QObject::connect (
            this,       SIGNAL  (OpenVideoWindowSignal(Participant::Ref)),
            mainForm,   SLOT    (on_OpenIncomingVideoWindow(Participant::Ref))
            );

    QObject::connect (
            this,       SIGNAL  (CloseVideoWindowSignal(Participant::Ref)),
            mainForm,   SLOT    (on_CloseIncomingVideoWindow(Participant::Ref))
            );

    // Connecting signals from GUI side to SkypeKit side (dispatcher)
    QObject::connect (
            mainForm,   SIGNAL(PlaceCallSignal(QString)),
            this,       SLOT(on_PlaceCall(QString))
            );
};


void QSKSignalDispatcher::on_PlaceCall(QString target)
{
    skype->PlaceCall(target);
};


//------------------------------------------------------------------------------------------------
// Account methods

void QSKAccount::OnChange(int prop)
{
    if (prop == Account::P_STATUS)
    {
        Account::STATUS accountStatus;
        this->GetPropStatus(accountStatus);
        QString loginStatus = QString((const char*)tostring(accountStatus));
        dispatcher->log(loginStatus);

        if (accountStatus == Account::LOGGED_IN)
        {
            // Setting recently live delay to 0 seconds
            // This simplifies detection of incoming calls.
            skype->SetInt(SETUPKEY_RECENTLY_LIVE_TIMEOUT, 0);

            skype->isLoggedIn = true;
            dispatcher->SendEnableLoginBtnSignal(false);
            dispatcher->SendEnableCallBtnSignal(true);
            dispatcher->SendUpdateContactListSignal();

            skype->GetConversationList(skype->inbox, Conversation::INBOX_CONVERSATIONS);
            fetch(skype->inbox);
        }
        else if (accountStatus == Account::LOGGED_OUT)
        {
            skype->isLoggedIn = false;
            dispatcher->log("Logout complete.");
            dispatcher->SendEnableLoginBtnSignal(true);
            dispatcher->SendEnableCallBtnSignal(false);

            Account::LOGOUTREASON whyDidWeLogout;          
            this->GetPropLogoutreason(whyDidWeLogout);
            QString logoutReason = QString((const char*)tostring(whyDidWeLogout));
            dispatcher->log(logoutReason);
        };
    };
};

//------------------------------------------------------------------------------------------------
// ContactGroup methods

void QSKContactGroup::OnChange(const ContactRef&)
{
    dispatcher->SendUpdateContactListSignal();
};


//---------------------------------------------------------------------------------------
// QSKVideo class methods

QSKVideo::QSKVideo(unsigned int oid, SERootObject* root) : Video(oid, root)
{
    localPreview    = false;
    incomingStream  = false;
};

void QSKVideo::OnChange(int prop)

{
    if ( prop == Video::P_STATUS )
    {
        Video::STATUS vidStatus;
        this->GetPropStatus(vidStatus);

        if ( this->localPreview )
        {
            QString dbg = "LOCAL ";
            dbg.append((const char*)tostring(vidStatus));
            dispatcher->log(dbg);
            return;
        };

        QString dbg;
        if ( incomingStream ) dbg = "INCOMING "; else dbg = "OUTGOING ";
        dbg.append((const char*)tostring(vidStatus));
        dispatcher->log(dbg);

        if ( vidStatus == Video::AVAILABLE ) this->Start();

        if ( vidStatus == Video::RUNNING )
        {
            if ( incomingStream )
            {
                dispatcher->log("Incoming video stream is now running.");
                dispatcher->SendOpenVideoWindowSignal(this->participant);
            }
        };

        if ( ( vidStatus == Video::STOPPING ) && ( incomingStream ) )
        {
            dispatcher->SendCloseVideoWindowSignal(this->participant);
        };
    };
};


//---------------------------------------------------------------------------------------
// Participant class methods

QSKParticipant::QSKParticipant(unsigned int oid, SERootObject* root) : Participant(oid, root)
{
    // populating the isMyself variable
    SEString accountName;
    SEString partName;
    skype->account->GetPropSkypename(accountName);
    this->GetPropIdentity(partName);
    isMyself = ( accountName.equals(partName) );

    GetVideoIfAvailable();
};

void QSKParticipant::OnChange(int prop)

{
    if (prop == Participant::P_VIDEO_STATUS ) GetVideoIfAvailable();
};

void QSKParticipant::GetVideoIfAvailable()
{
    Participant::VIDEO_STATUS partVidStatus;
    this->GetPropVideoStatus(partVidStatus);

    if (partVidStatus == Participant::VIDEO_AVAILABLE)
    {
        this->GetVideo(video);
        video.fetch();
        video->incomingStream = !this->isMyself;
        video->participant = this->ref();

        Video::STATUS vidStatus;
        video->GetPropStatus(vidStatus);
        if ( vidStatus == Video::AVAILABLE ) video->Start();
    };
};

//---------------------------------------------------------------------------------------
// Conversation class methods

void QSKConversation::OnParticipantListChange()
{
    // Keeping participant list updated

    this->GetParticipants(allParticipants);
    fetch(allParticipants);
};

//------------------------------------------------------------------------------------------------
// Skype methods

void QSKSkype::OnAvailableVideoDeviceListChange()
{
    SEStringList deviceNames;
    SEStringList deviceIds;
    uint deviceCount;
    this->GetAvailableVideoDevices(deviceNames, deviceIds, deviceCount);
    if (deviceCount > 0)
    {
        dispatcher->log("Video device list retrieved.");

        this->videoDeviceName = deviceNames[0];
        this->videoDeviceId = deviceIds[0];

        QString msg = "Picking ";
        msg.append( (const char*)videoDeviceName );
        dispatcher->log(msg);
        this->isVideoCapable = true;
    }
    else
    {
        this->isVideoCapable = false;
        dispatcher->log("ERROR: no video devices found.");
    };
};


void QSKSkype::OnConversationListChange(
        const ConversationRef &conversation,
        const Conversation::LIST_TYPE &type,
        const bool &added)
{
    // Keeping inbox updated..
    if ( (type == Conversation::INBOX_CONVERSATIONS) && (added) )
    {
        this->GetConversationList(inbox, Conversation::INBOX_CONVERSATIONS);
        fetch(inbox);
    };

    if (type == Conversation::LIVE_CONVERSATIONS)
    {
        // Live call coming up
        if (added)
        {
            SEString whosCalling;
            conversation->GetPropDisplayname(whosCalling);

            if (skype->isLiveSessionUp)
            {
                QString msg = "Another call from. ";
                msg.append( (const char*)whosCalling );
                msg.append(" As we are already in a call, we will ignore it.");
                dispatcher->log(msg);
            }
            else
            {
                skype->isLiveSessionUp = true;
                skype->liveSession = conversation;
                QString msg = "Going live with ";
                msg.append( (const char*)whosCalling );
                dispatcher->log(msg);
                conversation->JoinLiveSession();
                dispatcher->SendToggleCallBtnDropsCallSignal(true);
            };
        }
        // Live call ended
        else
        {
            dispatcher->log("Call finished.");
            skype->isLiveSessionUp = false;
            skype->liveSession = NULL;
            dispatcher->SendToggleCallBtnDropsCallSignal(false);
        };
    };
};


void QSKSkype::PlaceCall(QString target)
{
    if (skype->isLiveSessionUp)
    {
        dispatcher->log("Cant do - we are already in a call.");
    }
    else
    {
        if ( target.isEmpty() )
        {
            dispatcher->log("ERROR: no call target selected.");
        }
        else
        {
            SEString Target = (const char*)target.toAscii();

            QSKConversation::Ref call;
            SEStringList callTargets;

            callTargets.append(Target);
            skype->GetConversationByParticipants(callTargets, call, true, true);

            call->RingOthers();
            dispatcher->log("Calling..");
        };
    };
};

void QSKSkype::Login(SEString accountName, SEString password)
{
    if (skype->GetAccount(accountName, skype->account))
    {
        dispatcher->log("Logging in..");
        skype->account->LoginWithPassword(password, false, false);
    }
    else dispatcher->log("ERROR: Account does not exist!");
};
