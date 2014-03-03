/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 5.

In this step of the tutorial, we finally get around to voice calls. We'll write a small SkypeKit client that
  * waits for incoming calls
  * picks them up
  * if there's audio coming in from any of the remote parties, prints audio volume levels

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

//----------------------------------------------------------------------------
// Interface section

class MyParticipant : public Participant
{
public:
  typedef DRef<MyParticipant, Participant> Ref;
  typedef DRefs<MyParticipant, Participant> Refs;
  MyParticipant(unsigned int oid, SERootObject* root) : Participant(oid, root) {};

  void OnChange(int prop);
};

class MyConversation : public Conversation
{
public:
  typedef DRef<MyConversation, Conversation> Ref;
  typedef DRefs<MyConversation, Conversation> Refs;  
  MyConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root) {};

  MyParticipant::Refs callerList;

  void OnChange(int prop);
  void PickUpCall();
};

class MySkype : public Skype
{
public:
  bool isLiveSessionUp;
  MyConversation::Ref liveSession;
  MySkype();

  Account*      newAccount(int oid) { return new MyAccount(oid, this); };
  Participant*  newParticipant(int oid) { return new MyParticipant(oid, this); };
  Conversation* newConversation(int oid) { return new MyConversation(oid, this); };

	void OnConversationListChange(
		const ConversationRef &conversation,
		const Conversation::LIST_TYPE &type,
		const bool &added);
};

MySkype* skype;

//----------------------------------------------------------------------------
// Implementation section

void MyParticipant::OnChange(int prop)
{
  if (prop == Participant::P_SOUND_LEVEL)
  {
    SEString name;
    uint soundLevel;
    GetPropIdentity(name);
    GetPropSoundLevel(soundLevel);
    printf("%s sound level = %d\n", (const char*)name, soundLevel);
  };
};

void MyConversation::PickUpCall()
{
  // CallerList we keep in Conversation class - so that it won't get garbage collected
  // while the conversation object exists in the wrapper. 
  this->GetParticipants(callerList, Conversation::OTHER_CONSUMERS);
  SEString partListAsString = "";
  for (uint i = 0; i < callerList.size(); i++)
  {
    partListAsString = partListAsString 
      + " " 
      + (const char*)callerList[i]->GetProp(Participant::P_IDENTITY);
  };

  // re-loading with filter ALL, so that we get our own 
  // Participant::P_SOUND_LEVEL updates as well
  this->GetParticipants(callerList, Conversation::ALL);
  fetch(callerList);


  if (!skype->isLiveSessionUp)
  {
    printf("Incoming call from: %s \n", (const char*)partListAsString);
    this->JoinLiveSession();
  }
  else
  {
    printf("Another call is coming in from : %s \n", (const char*)partListAsString);
    printf("As we already have a live session up, we will reject it. \n");
    this->LeaveLiveSession(true);
  };
};

void MyConversation::OnChange(int prop)
{
  if (prop == Conversation::P_LOCAL_LIVESTATUS)
  {
    Conversation::LOCAL_LIVESTATUS liveStatus;
    this->GetPropLocalLivestatus(liveStatus);
    if (liveStatus == Conversation::RINGING_FOR_ME)
    {
      printf("RING RING..\n");
      printf("Picking up call from MyConversation::OnChange\n");
      this->PickUpCall();
    };

    if (liveStatus == Conversation::IM_LIVE)
    {
      // Saving the currently live conversation reference..
      skype->liveSession = this->ref();
      printf("Live session is up.\n");
    };

    if ((liveStatus == Conversation::RECENTLY_LIVE) || (liveStatus == Conversation::NONE))
    {
      printf("Call has ended..\n");
      skype->isLiveSessionUp = false;
    };
  };
};

//----------------------------------------------------------------------------
// Subclassing Skype

MySkype::MySkype() : Skype() 
{ 
  isLiveSessionUp   = false;
  liveSession       = 0;
};

void MySkype::OnConversationListChange(
		const ConversationRef &conversation,
		const Conversation::LIST_TYPE &type,
		const bool &added)
{
  if (type == Conversation::LIVE_CONVERSATIONS)
  {
    Conversation::LOCAL_LIVESTATUS liveStatus;
    conversation->GetPropLocalLivestatus(liveStatus);
    SEString liveStatusAsString = tostring(liveStatus);
    printf("OnConversationListChange : %s\n", (const char*)liveStatusAsString);

    if (liveStatus == Conversation::RINGING_FOR_ME)
    {
      printf("RING RING..\n");
      printf("Picking up call from MySkype::OnConversationListChange\n");
      // Saving the currently live conversation reference..
      liveSession = conversation->ref();
      liveSession.fetch();
      liveSession->PickUpCall();
    };
  };
};

//----------------------------------------------------------------------------
// Main

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 5. - Answering incoming audio calls.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_5 <skypename> <password> [mic idx] [playback idx]\n");
    return 0;
  };

  myAccountName   = argv[1];
  myAccountPsw    = argv[2];

  uint inputDeviceIdx   = 0;
  uint outputDeviceIdx  = 0;
  if (argc > 3) { inputDeviceIdx = atoi(argv[3]); };
  if (argc > 4) { outputDeviceIdx = atoi(argv[4]); };

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

    if (account->loggedIn)
    {
      printf("Loggin complete.\n");

      int recentlyLiveTimeout;
      skype->GetInt(SETUPKEY_RECENTLY_LIVE_TIMEOUT, recentlyLiveTimeout);
      printf("Conversation recently live timeout: %d seconds.\n", recentlyLiveTimeout);
    
      // Alternative way to get around having to pick up calls 
      // from two different callbacks:
      //skype->SetInt(SETUPKEY_RECENTLY_LIVE_TIMEOUT, 0);
    
      // ** Setting up audio devices **
      // First, getting a list of audio devices. There are two separate commands:
      // Skype::GetAvailableOutputDevices and Skype->GetAvailableRecordingDevices
      // Both retrurn three SEStringLists.
      // 1. First of them is list of device handles. These can be used as arguments
      // to other audio related commands, such as Skype::SelectSoundDevices
      // 2. Second list will contain descriptive device name.
      // 3. Third list contains product IDs, which can be in most cases safely ignored.

      SEStringList outHandles, outNames, outProductIDs;
      SEStringList inHandles, inNames, inProductIDs;

      printf("** Playback devices:\n");
      skype->GetAvailableOutputDevices (outHandles, outNames, outProductIDs);
      for (uint i = 0; i < outHandles.size(); i++)
      {
        printf("%4d. %s\n", i, (const char*)outNames[i]);
      };

      // Getting a list of audio input devices.
      printf("\n** Recording devices:\n");
      skype->GetAvailableRecordingDevices (inHandles, inNames, inProductIDs);
      for (uint i = 0; i < inHandles.size(); i++)
      {
          printf("%4d. %s\n", i, (const char*)inNames[i]);
      };

      skype->SelectSoundDevices(
        inHandles[inputDeviceIdx], 
        outHandles[outputDeviceIdx], 
        outHandles[outputDeviceIdx]);

      skype->SetSpeakerVolume(100);

      printf("Now accepting incoming calls..\n");
      printf("Press ENTER to quit.\n");
      getchar();

      printf("Logging out..\n");
      account->Logout(false);
      account->BlockWhileLoggingOut();
      printf("Logout complete.\n");
    };
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
