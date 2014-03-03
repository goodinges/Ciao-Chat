/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 6.

In this example, we are going to write a command-line Skype dialer. It will:
  * take a call target from a command line argument
  * attempt to initiate a call to that target

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;
SEString myCallTarget;


//----------------------------------------------------------------------------
// Interface section

class MyConversation : public Conversation
{
public:
  typedef DRef<MyConversation, Conversation> Ref;
  typedef DRefs<MyConversation, Conversation> Refs;
  MyConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root) {};

  void OnChange(int prop);
};

class MySkype : public Skype
{
public:
  bool callFinished;
  bool isLiveSessionUp;

  MySkype() : Skype() 
  {
    callFinished = false;
    isLiveSessionUp = false;;
  };

  Account*      newAccount(int oid) {return new MyAccount(oid, this);}
  Conversation* newConversation(int oid) {return new MyConversation(oid, this);}

  bool SetupAudioDevices(uint MicNr, uint SpkNr);
};

MySkype* skype;


//----------------------------------------------------------------------------
// Implementation section

void MyConversation::OnChange(int prop)
{
  if (prop == Conversation::P_LOCAL_LIVESTATUS)
  {
    Conversation::LOCAL_LIVESTATUS liveStatus;
    this->GetPropLocalLivestatus(liveStatus);
    SEString liveStatusAsStr = tostring(liveStatus);

    printf("%s\n", (const char*)liveStatusAsStr);

    if (liveStatus == Conversation::IM_LIVE)
    {
      printf("Live session is up.\n");
      skype->isLiveSessionUp = true;
      printf("Press ENTER to drop the call.\n");
    };
        
    if ((liveStatus == Conversation::RECENTLY_LIVE) 
        || (liveStatus == Conversation::NONE && skype->isLiveSessionUp) 
        || (liveStatus == PLAYING_VOICE_MESSAGE))
    {
      // bah, we ended up in Voicemail..
      if (liveStatus == PLAYING_VOICE_MESSAGE) LeaveLiveSession(false);
      printf("Call finished.\n");
      skype->callFinished = true;
      printf("Press ENTER to quit.\n");
    };
  };
};

bool MySkype::SetupAudioDevices(uint micIdx, uint spkIdx)

{
  SEStringList spkHandles, spkNames, spkProductIDs;
  SEStringList micHandles, micNames, micProductIDs;

  GetAvailableOutputDevices (spkHandles, spkNames,  spkProductIDs);
  GetAvailableRecordingDevices (micHandles, micNames, micProductIDs);

  if (micIdx > (micHandles.size() + 1))
  {
    printf("Invalid mic device no. (%d) passed to MySkype::SetupAudioDevices\n", micIdx);
    return false;
  };

  if (spkIdx > (spkHandles.size() + 1))
  {
    printf("Invalid speaker device no. (%d) passed to MySkype::SetupAudioDevices\n", spkIdx);
    return false;
  };

  printf("Setting mic to %s\n", (const char*)micNames[micIdx]);
  printf("Setting speakers to %s\n", (const char*)spkNames[spkIdx]);

  SelectSoundDevices(micHandles[micIdx], spkHandles[spkIdx], spkHandles[spkIdx]);
  SetSpeakerVolume(100);
  return true;
};


//----------------------------------------------------------------------------
// Main

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 6. - Initiating voice call (1 on 1).\n");
  printf("*****************************************************************\n");

  if (argc < 4)
  {
    printf("usage: tutorial_6 <skypename> <password> <calltarget> [mic idx] [playback idx]\n");
    return 0;
  };

  myAccountName   = argv[1];
  myAccountPsw    = argv[2];
  myCallTarget    = argv[3];

  uint inputDeviceIdx   = 0;
  uint outputDeviceIdx  = 0;
  if (argc > 4) { inputDeviceIdx = atoi(argv[4]); };
  if (argc > 5) { outputDeviceIdx = atoi(argv[5]); };

  printf("Creating skype ..\n");
  skype = new MySkype();

  printf("Starting Skype and submitting application token..\n");
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

    if (skype->SetupAudioDevices(inputDeviceIdx, outputDeviceIdx))
    {
      MyConversation::Ref conv;
      bool convOk = skype->GetConversationByIdentity(myCallTarget, conv);
      if (!convOk)
      {
        printf("Unable to create conversation with %s\n", (const char*)myCallTarget);
      }
      else
      {        
        conv->RingOthers();  

        // Block here until user presses ENTER
        getchar();

        if (!skype->callFinished)
        {
          conv->LeaveLiveSession();
        };

      };
    }
    else { printf("Unable to set audio devices.\n"); };

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
