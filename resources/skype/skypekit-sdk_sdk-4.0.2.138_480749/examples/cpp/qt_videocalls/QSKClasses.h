#ifndef QSKCLASSES_H
#define QSKCLASSES_H

#include <skype-embedded_2.h>
#include <QtGui/QApplication>
#include <mainwindow.h>
#include <videowidget.h>

/*---------------------------------------------------------------------------------------
Dispatcher class. Instance of this class will handle all the communication between
SkypeKit classes and UI classes. The reason we need this is that SkypeKit class events
do not fire from the main UI thread.

For example: once an Account object obtains logged in status, we will want to disable
the login button and enable the Call button. It would be easy to just call appropriate
mainForm methods - but this will cause your app to crash - as Account::OnChange callback
will get fired from somewhere in the wrapper thread pool.

An easy way to flatten the SkypeKit-GUI communication without writing your own event queue
would be to re-use Qt signals and slots. In theory it is possible to use multiple inheritance
for this - to have you SkypeKit class descendants inherit from QObject as well. On the other
hand, multiple inheritance can get messy. So in this case we will use a single object -
a QObject descendant - as our central event dispatcher.

Instead of calling:
mainForm->enableCallButton(true)

we define a signal: QSKSignalDispatcher::EnableCallBtnSignal
a method that emits the signal: QSKSignalDispatcher::SendEnableLoginBtnSignal
and a corresponding slot: MainWindow::on_EnableCallBtn

After connecting these two, and having dispatcher object available in global scope, we can
use dispatcher->SendEnableCallBtnSignal(bool) from our SkypeKit callbacks without UI crashing.
*/

class QSKSignalDispatcher : public QObject
{
    Q_OBJECT

public:
    void ConnectToUI();

signals:
    void SendToLogSignal                (QString);
    void UpdateContactListSignal        ();
    void EnableLoginBtnSignal           (bool);
    void EnableCallBtnSignal            (bool);
    void ToggleCallBtnSignal            (bool);
    void ToggleCallBtnDropsCallSignal   (bool);
    void OpenVideoWindowSignal          (Participant::Ref);
    void CloseVideoWindowSignal         (Participant::Ref);

public:
    void log(QString S)                                     { emit SendToLogSignal(S);              }
    void SendUpdateContactListSignal ()                     { emit UpdateContactListSignal ();      }
    void SendEnableLoginBtnSignal (bool B)                  { emit EnableLoginBtnSignal (B);        }
    void SendEnableCallBtnSignal (bool B)                   { emit EnableCallBtnSignal (B);         }
    void SendToggleCallBtnDropsCallSignal (bool B)          { emit ToggleCallBtnDropsCallSignal (B);}
    void SendOpenVideoWindowSignal (Participant::Ref part)  { emit OpenVideoWindowSignal (part);    }
    void SendCloseVideoWindowSignal (Participant::Ref part) { emit CloseVideoWindowSignal (part);   }

public slots:
    void on_PlaceCall (QString target);
};


//---------------------------------------------------------------------------------------
// Account class declaration

class QSKAccount : public Account
{
public:
    typedef DRef<QSKAccount, Account> Ref;
    typedef DRefs<QSKAccount, Account> Refs;

    QSKAccount(unsigned int oid, SERootObject* root) : Account(oid, root) {}
    void OnChange(int prop);
};


//---------------------------------------------------------------------------------------
// ContactGroup class declaration

class QSKContactGroup : public ContactGroup
{
public:
    typedef DRef<QSKContactGroup, ContactGroup> Ref;
    typedef DRefs<QSKContactGroup, ContactGroup> Refs;

    QSKContactGroup(unsigned int oid, SERootObject* root) : ContactGroup(oid, root) {}
    void OnChange(const ContactRef& contact);
};


//---------------------------------------------------------------------------------------
// Video class declaration

class QSKParticipant;

class QSKVideo : public Video
{
public:
    typedef DRef<QSKVideo, Video> Ref;
    typedef DRefs<QSKVideo, Video> Refs;
    typedef DRef<QSKParticipant, Participant> QSKParticipantRef;

    bool                incomingStream;
    bool                localPreview;

    QSKParticipantRef   participant;

    QSKVideo(unsigned int oid, SERootObject* root);
    void OnChange(int prop);
};


//---------------------------------------------------------------------------------------
// Participant class declaration

class QSKParticipant : public Participant
{
public:
    typedef DRef<QSKParticipant, Participant> Ref;
    typedef DRefs<QSKParticipant, Participant> Refs;

    bool            isMyself;
    QSKVideo::Ref   video;

    QSKParticipant(unsigned int oid, SERootObject* root);

    void OnChange(int prop);
    void GetVideoIfAvailable();
};

//---------------------------------------------------------------------------------------
// Conversation class declaration

class QSKConversation : public Conversation
{
public:
    typedef DRef<QSKConversation, Conversation> Ref;
    typedef DRefs<QSKConversation, Conversation> Refs;

    QSKParticipant::Refs allParticipants;

    QSKConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root)
    {
        this->GetParticipants(allParticipants);
        fetch(allParticipants);
    }

    void OnParticipantListChange();
};


//---------------------------------------------------------------------------------------
// Skype class declaration

class QSKSkype : public Skype
{
public:
    bool                    isLoggedIn;
    bool                    isLiveSessionUp;
    bool                    isVideoCapable;
    bool                    isPreviewVideoOk;

    SEString                videoDeviceName;
    SEString                videoDeviceId;

    QSKAccount::Ref         account;
    QSKConversation::Ref    liveSession;
    QSKVideo::Ref           previewVideo;

    ContactGroup::Ref       allBuddiesGroup;
    ContactRefs             allBuddies;
    QSKConversation::Refs   inbox;

    QSKSkype() : Skype()
    {
        isLoggedIn          = false;
        isLiveSessionUp     = false;
        isVideoCapable      = false;
        isPreviewVideoOk    = false;
    }

    void OnAvailableVideoDeviceListChange();

    void OnConversationListChange(
        const ConversationRef &conversation,
        const Conversation::LIST_TYPE &type,
        const bool &added);

    void PlaceCall(QString target);

    void Login (SEString accountName, SEString password);

    Account*        newAccount(int oid)         { return new QSKAccount(oid, this);      }
    ContactGroup*   newContactGroup(int oid)    { return new QSKContactGroup(oid, this); }
    Video*          newVideo(int oid)           { return new QSKVideo(oid, this);        }
    Participant*    newParticipant(int oid)     { return new QSKParticipant(oid, this);  }
    Conversation*   newConversation(int oid)    { return new QSKConversation(oid, this); }
};

extern QSKSkype*            skype;
extern QSKSignalDispatcher* dispatcher;

#endif // QSKCLASSES_H
