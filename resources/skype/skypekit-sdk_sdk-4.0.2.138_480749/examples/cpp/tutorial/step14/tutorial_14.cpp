/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 14.

In this example we will write a program that:

 1. If executed without voicemail target argument, will wait for 
    and play incoming voice messages
 2. If executed with valid target argument, will record and send
    a voice message to that target.

Note that you will need at least one account with voice mailbox to 
run this demo.

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;
SEString myVoicemailTarget;

//----------------------------------------------------------------------------
// Interface section

class MyVoicemail : public Voicemail
{
public:
  typedef DRef<MyVoicemail, Voicemail> Ref;
  typedef DRefs<MyVoicemail, Voicemail> Refs;

  bool vmRecordingStarted;
  bool vmFinishedIncoming;
  bool vmFinishedSending;

  MyVoicemail(unsigned int oid, SERootObject* root) : Voicemail(oid, root) 
  {
    vmFinishedIncoming = false;
    vmFinishedSending  = false;
    vmRecordingStarted = false;
  };

  void OnChange(int prop);
};


class MyConversation : public Conversation
{
public:
  typedef DRef<MyConversation, Conversation> Ref;
  typedef DRefs<MyConversation, Conversation> Refs;

  MyVoicemail::Ref activeVM;

  MyConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root) 
  { 
    activeVM = NULL;
  };

  void OnChange(int prop);
  void OnMessage (const MessageRef& message);
};


class MySkype : public Skype
{
public:
  MySkype() : Skype() {}
  Account*    newAccount(int oid)      { return new MyAccount(oid, this);}
  Conversation*  newConversation(int oid)  { return new MyConversation(oid, this);}
  Voicemail*    newVoicemail(int oid)    { return new MyVoicemail(oid, this);}

  MyConversation::Refs inbox;
  void OnConversationListChange (
      const ConversationRef & conversation,
      const Conversation::LIST_TYPE & type,
      const bool & added);
};


//----------------------------------------------------------------------------
// Implementation section

/** In MyVoicemail::OnChange we will keep looking for two cases of P_STATUS property change:
1. status == PLAYED && type == INCOMING - playback of incoming VM has finished.
2. status == UPLOADED || status == FAILED || status == Voicemail::CANCELLED - 
   sending of an outgoing VM has finished with any of the three final states.
   In case of FAILED we will also print out the string version of the P_FAILUREREASON **/

void MyVoicemail::OnChange(int prop)
{
  if (prop == Voicemail::P_STATUS)
  {
    Voicemail::STATUS status;
    this->GetPropStatus(status);

    Voicemail::TYPE type;
    this->GetPropType(type);

    if ((status == Voicemail::PLAYED) && (type == Voicemail::INCOMING))
    {
      vmFinishedIncoming = true;
      printf("VM playback is finished. Press ENTER to quit.\n");
      return;
    };

    if ((status == Voicemail::UPLOADED) || (status == Voicemail::FAILED) || (status == Voicemail::CANCELLED))
    {
      vmFinishedSending = true;

      if (status == Voicemail::FAILED)
      {
        Voicemail::FAILUREREASON failReason;
        this->GetPropFailurereason(failReason);
        printf("%s\n", (const char*)tostring(failReason));
      };
    };

    // Voicemail::STATUS
    printf("%s\n", (const char*)tostring(status));
  }
  else
  {
    // In case of non-P_STATUS property updates, we just print out notifications
    SEStringList dbg;
    SEString propStr = this->GetProp(prop);
    dbg = this->getPropDebug(prop, propStr);
    printf("VM %s -> %s\n", (const char*)dbg[1], (const char*)propStr);  
  };
};

/** MyConversation::OnMessage is where we actually handle detection and playback of incoming 
voicemails. For detection, we will check type of each incoming message for POSTED_VOICE_MESSAGE
For messages of that type, we can retrieve a Voicemail object with Message::GetVoiceMessage
and then start playback with Voicemail::StartPlayback.

We can then detect when the playback is finished from the Voicemail::OnChange callback,
by checking if the type is INCOMING and rthe status is PLAYED. **/

void MyConversation::OnMessage (const MessageRef& message)
{
  Message::TYPE msgType;
  message->GetPropType(msgType);

  if (msgType == Message::POSTED_VOICE_MESSAGE)
  {
    SEString msgAuthor;
    message->GetPropAuthor(msgAuthor);

    if (msgAuthor == myAccountName)
    {
      // This check is necessary for filtering out the VMs we have just
      // sent ou ourselves. Conversation::OnMessage fires for those as well.
      return;
    };

    SEString msgBody;
    message->GetPropBodyXml(msgBody);

    printf("We got incoming voicemail!\n");
    printf("Author: %s\n", (const char*)msgAuthor);
    printf("Body: %s\n\n", (const char*)msgBody);

    if (activeVM)
    {
      printf("We already have an active voicemail object..\n");
    }
    else
    {
      printf("Starting playback.\n");
      message->GetVoiceMessage(this->activeVM);
      this->activeVM->StartPlayback();
    };
  };
};

/** The MyConversation::OnChange is the only place we can get to know when 
Voicemail objects are created by the runtime. For this we need to monitor 
the Conversation::P_ACTIVE_VM_ID property changes. If this property is
updated - we can call GetPropActiveVmId to retrieve a reference to the
current active Voicemail object.

For each outgoing VM, we will have three P_ACTIVE_VM_ID property changes:
1. the incoming "greeting" VM object that gets played back
2. the outgoing VM that will be recorded and sent
3. and finally, the P_ACTIVE_VM_ID property will reset to 0.

NB! The last step - the reset - will happen -before- the VM reaches
it's final state (for example UPLOADDED). To catch the final Voicemail
status update (especially in case it turns out to be FAILED), 
we will need to keep a reference to the active Voicemail object
even after Conversation resets the active vm id to 0.
    
This is also why we use local variable newVM in GetPropActiveVmId
call, instead of immediately reloading the MyConversation::activeVM
variable. **/

void MyConversation::OnChange(int prop)
{  
  if (prop == Conversation::P_ACTIVE_VM_ID)
  {
    MyVoicemail::Ref newVM;
    this->GetPropActiveVmId(newVM);

    if (!newVM)
    {
      printf("Conversation cleared active voicemail ID\n");
      return;
    };

    activeVM = newVM;

    Voicemail::TYPE vmType;
    activeVM->GetPropType(vmType);

    // Printing out the Voicemail type (incoming, greeting, outgoing)
    SEStringList dbg;
    dbg = activeVM->getPropDebug(Voicemail::P_TYPE, vmType);
    printf("Conversation has created a new %s voicemail!\n", (const char*)dbg[2]);

    // If an outgoing VM was created, recording will start automatically.
    if (vmType == Voicemail::OUTGOING)
    {
      activeVM->vmRecordingStarted = true;
    };
    return;
  };
  
  /** Voicemails also trigger other Conversation property changes,
  such as LIVE_START_TIMESTAMP, LOCAL_LIVESTATUS, etc.
  For context, we will printf out all Conversation prop changes
  as well. Handling LOCAL_LIVESTATUS separately here, so that 
  the livestatus value gets printed as string rather than numeric. **/
  if (prop == Conversation::P_LOCAL_LIVESTATUS)
  {
    Conversation::LOCAL_LIVESTATUS liveStatus;
    this->GetPropLocalLivestatus(liveStatus);
    printf("%s\n", (const char*)tostring(liveStatus));

    return;
  };

  SEStringList dbg;
  SEString propStr = this->GetProp(prop);
  dbg = this->getPropDebug(prop, propStr);
  printf("Conv::%s -> %s\n", (const char*)dbg[1], (const char*)propStr);  
};

/** MySkype::OnConversationListChange needs to be monitored for changes 
in INBOX_CONVERSATIONS conversation list, so that we allways have an
up-to-date skype->inbox variable to catch all Conersation::OnMessage
events for incoming voicemails. **/

void MySkype::OnConversationListChange (
    const ConversationRef & conversation,
    const Conversation::LIST_TYPE & type,
    const bool & added)
{
  if (type == Conversation::INBOX_CONVERSATIONS)
  {
    this->GetConversationList(inbox, Conversation::INBOX_CONVERSATIONS);
    fetch(inbox);
  };
};

//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

void SendVoicemail()

{
  MyVoicemail::Ref  vm;
  MyConversation::Ref conv;

  ContactRef contact;
  skype->GetContact(myVoicemailTarget, contact);

  Contact::CAPABILITYSTATUS vmCapability;
  contact->GetCapabilityStatus(Contact::CAPABILITY_CAN_BE_SENT_VM, vmCapability);

  if (vmCapability != Contact::CAPABILITY_EXISTS)
  {
    printf("ERROR: Cannot send voicemail to %s\n", (const char*)myVoicemailTarget);
  }
  else
  {
    printf("Can send voicemail to %s\n", (const char*)myVoicemailTarget);

    skype->GetConversationByIdentity(myVoicemailTarget, conv);

    printf("Press ENTER to start sending the voicemail.\n");
    getchar();

    conv->StartVoiceMessage();

    // Loop until VM object is created. This object is the "greeting" voicemail
    // that will automatically play back the "leave message after beep" audio.
    // Voicemail::P_TYPE is either DEFAULT_GREETING or CUSTOM_GREETING
    while (!conv->activeVM) { Delay(1); };
    printf("Stay tuned while playing 'leave message after the beep' message..\n");

    // Loop until the 2nd VM object is created, with TYPE == OUTGOING
    // The 2nd VM is created automatically once the greeting playback is done.
    while (!conv->activeVM->vmRecordingStarted) { Delay(1); };

    // The 2nd VM will now record local audio until ENTER is pressed.
    printf("We are now recording a voice message.\n");
    printf("Press ENTER to stop recording and send the voicemail.\n");
    getchar();

    // Posting the voicemail to the conversation.
    conv->PostVoiceMessage(vm, "Voicemail body text");

    // If at this point you decide to cancel the voicemail, you should use
    // Conversation::LeaveLiveSession() method, like this:
    // printf("Cancelling VM!\n");
    // conv->LeaveLiveSession();

    /** The string argument will show up on remote side in the BODY_XML property
    of the corresponding Message object, like this:
    Message body: <voicemail alt="Voicemail body text"><message length="7"></message></voicemail>
    Your UI should use this to display the voicemail in a conversation. **/

    while (!conv->activeVM->vmFinishedSending) {Delay(1);};
  };

  printf("Press ENTER to quit.\n");
  getchar();
};


/** The WaitForVoicemail does not actually do much anything. The catching and 
playback of incoming Voicemails is all done in the MyConversation::OnMessage
callback - where incoming messages with type == POSTED_VOICE_MESSAGE will 
indicate that a new voicemail has arrived.

The only trick here is that to get those MyConversation::OnMessage events,
you need to have live references to all relevant conversations in your
wrapper object cache. For this we will need to have a ConversationRefs list
that never goes out of scope (in this case inbox variable in MySkype class).

By retrieving INBOX_CONVERSATIONS conversations into that variable, we will
make sure that we will get all the MyConversation::OnMessage events in our 
active chats. Well, almost.  There is also the case of new or dormant 
conversations being added to the inbox. To handle that, we can override  
MySkype::OnConversationListChange and re-load the inbox again. **/

void WaitForVoicemail()
{
  skype->GetConversationList(skype->inbox, Conversation::INBOX_CONVERSATIONS);
  fetch(skype->inbox);

  printf("Waiting for incoming voicemails..\n");
  printf("Press ENTER to quit.\n");
  getchar();
};

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 14. - Voice messages (Voicemails)\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
      printf("usage: tutorial_14 <skypename> <password> [target]\n");
      return 0;
  };

  myAccountName = argv[1];
  myAccountPsw  = argv[2];

  if (argv[3])
  {
    myVoicemailTarget  = argv[3];
  };

  printf("Creating skype ..\n");
  skype = new MySkype();

  printf("Submitting application token..\n");
  getKeyPair ();
  skype->init(keyBuf, inetAddr, portNum, "streamlog.txt");
  skype->start();

  printf("Retrieving account ..\n");
  MyAccount::Ref account;

  if (skype->GetAccount(myAccountName, account))
  {
    printf("Logging in..\n");
    account->LoginWithPassword(myAccountPsw, false, true);
    account->BlockWhileLoggingIn();
    printf("Loggin complete.\n");
    
    if (argv[3])
    {
      // If we have a target argument - we will send a VM..
      SendVoicemail();
    }
    else
    {
      // ..if we don't, we will wait for incoming VMs
      WaitForVoicemail();
    };

    printf("Logging out..\n");
    account->Logout(false);

    account->BlockWhileLoggingOut();
    printf("Logout complete.\n");
  }
  else
  {
      printf("Account does not exist\n");
  };

  printf("Cleaning up.\n");
  skype->stop();
  delete skype;
  printf("Done.\n");
  return 0;
};
