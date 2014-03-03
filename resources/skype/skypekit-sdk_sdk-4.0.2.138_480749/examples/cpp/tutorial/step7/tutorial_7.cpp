/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 7.

In the previous tutorial, we wrote a SkypeKit dialer that took a skypename from a command-line argument and initiated 
a voice call. In this step, we will have two call targets, and we'll form a conference call. Our program will:
 1. Create a new Conversation object.
 2. Create an SEStringList and add the skypenames of our call targets to it
 3. Assign the SEStringList to our conference with Conversation::AddConsumers
 4. Get back a list of Participant references
 5. Loop through the list of participants and call Participant::Ring on each one (except ourselves)

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

SEString myCallTarget1;
SEString myCallTarget2;

//----------------------------------------------------------------------------
// Interface section

class MyParticipant : public Participant
{
public:
  typedef DRef<MyParticipant, Participant> Ref;
  typedef DRefs<MyParticipant, Participant> Refs;

  bool isRinging;
  MyParticipant(unsigned int oid, SERootObject* root) : Participant(oid, root) { isRinging = false; };

  void OnChange(int prop);
};

class MyConversation : public Conversation
{
public:
  typedef DRef<MyConversation, Conversation> Ref;
  typedef DRefs<MyConversation, Conversation> Refs;

  MyParticipant::Refs participantList;
  bool callFinished;
  MyConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root) 
  { 
    callFinished = false; 
  };

  void OnChange(int prop);
  void OnParticipantListChange();
};

class MySkype : public Skype
{
public:
  MySkype() : Skype() {};
  Account*      newAccount(int oid) { return new MyAccount(oid, this); }
  Conversation* newConversation(int oid) { return new MyConversation(oid, this); } 
  Participant*  newParticipant(int oid) { return new MyParticipant(oid, this); }
  bool SetupAudioDevices(uint MicNr, uint SpkNr);
};


//----------------------------------------------------------------------------
// Implementation section

void MyParticipant::OnChange(int prop)
{
  SEString identity;
  GetPropIdentity(identity);

  if (prop == Participant::P_VOICE_STATUS)
  {
    Participant::VOICE_STATUS voiceStatus;
    GetPropVoiceStatus(voiceStatus);
    if (voiceStatus == Participant::RINGING) { isRinging = true; };

    switch (voiceStatus)
    {
      case Participant::RINGING : 
          printf("Calling %s..\n", (const char*)identity);
          break;
      
      case Participant::SPEAKING : 
          printf("%s has joined live state\n", (const char*)identity);
          break;

      case Participant::VOICE_STOPPED : 
          printf("%s has dropped from live state\n", (const char*)identity);
          break;
      
      default:
        // Ignoring other states for now..
        break;
    };
  };
};

void MyConversation::OnParticipantListChange()
{
  this->GetParticipants(participantList, Conversation::ALL);
  fetch(participantList);

  SEString partListAsStr = "New state of participant list:";
  for (uint i = 0; i < participantList.size(); i++)
  {
    SEString identity;
    participantList[i]->GetPropIdentity(identity);
    partListAsStr += " ";
    partListAsStr += identity;
  };
  printf("%s\n", (const char*)partListAsStr);
};

void MyConversation::OnChange(int prop)
{
  if (prop == Conversation::P_LOCAL_LIVESTATUS)
  {
    Conversation::LOCAL_LIVESTATUS liveStatus;
    GetPropLocalLivestatus(liveStatus);
    if ((liveStatus == Conversation::RECENTLY_LIVE) || (liveStatus == Conversation::NONE))
    {
      printf("Call finished.\n");
      this->callFinished = true;
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

MySkype* skype;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 7. - Initiating voice conference (1 on 2).\n");
  printf("*****************************************************************\n");

  if (argc != 5)
  {
      printf("usage: tutorial_7 <skypename> <password> <1st calltarget> <2nd calltarget>\n");
      return 0;
  };

  myAccountName = argv[1];
  myAccountPsw  = argv[2];
  myCallTarget1 = argv[3];
  myCallTarget2 = argv[4];

  printf("Creating skype ..\n");
  skype = new MySkype();

  // Starting the skype thread and submitting the Application Token.
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

    MyConversation::Ref conv;
    SEStringList callTargets;

    skype->CreateConference(conv);
    callTargets.append(myCallTarget1);
    callTargets.append(myCallTarget2);
    conv->AddConsumers(callTargets);

    conv->RingOthers();

    while (!conv->callFinished) {Delay(1);};

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
