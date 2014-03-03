/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 16.

In this example, we will write a simple SkypeKit-based file transfer tool.
The program works as auto-accepting file receiver (if launched without target skypename
and filename command-line arguments. With two more arguments: valid skypename and filename,
the program will transmit the file to target and exit.

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

bool isDoneSending      = false;

// For us to keep getting Transfer object property updates
// we need to keep live references to these objects.
TransferRefs * globalTransferList = new TransferRefs();

SEString myAccountName;
SEString myAccountPsw;
SEString targetName = "";   // set from command line argument 3
SEString fileName  = "";    // set from command line argument 4

//----------------------------------------------------------------------------
// Interface section

class MyTransfer : public Transfer
{
public:
  typedef DRef<MyTransfer, Transfer> Ref;
  typedef DRefs<MyTransfer, Transfer> Refs;
  MyTransfer(unsigned int oid, SERootObject* root) : Transfer(oid, root) {};

  void AutoAccept();
  void OnChange(int prop);
};

class MyConversation : public Conversation
{
public:
  typedef DRef<MyConversation, Conversation> Ref;
  typedef DRefs<MyConversation, Conversation> Refs;

  MyConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root) {};
  void OnMessage (const MessageRef &message);
};

class MySkype : public Skype
{
public:
  MySkype() : Skype() {}

  Account*        newAccount(int oid)       { return new MyAccount(oid, this); }
  Conversation*   newConversation(int oid)  { return new MyConversation(oid, this); }
  Transfer*       newTransfer(int oid)      { return new MyTransfer(oid, this); }

  MyConversation::Refs allConvs;

  void OnConversationListChange (
    const ConversationRef & conversation,
    const Conversation::LIST_TYPE & type,
    const bool & added);

  void sendFile (SEString target, SEString fileName);
  void receiveFile ();
};

MySkype* skype;

//----------------------------------------------------------------------------
// Implementation section

void MyTransfer::AutoAccept()
{
  SEString fileName;
  SEString filePath;
  bool success;
  this->GetPropFilename(fileName);

  // NB! Note that we do not specify file path here! As the actual work is done 
  // in the runtime, by default all the incoming files will be placed in runtime's 
  // local directory. Also, if you specify a relative path, the path will be 
  // relative to the location of the runtime.
  printf("Auto-accepting incoming file transfer: %s\n", (const char*)fileName);
  this->Accept(fileName, success);

  if (!success)
  {
    printf("Failed to accept incoming file transfer: %s\n", (const char*)fileName);
  };
};

void MyTransfer::OnChange(int prop)
{
  // Uncomment the following section to see ALL the Transfer class property changes
  /**
  SEString value;
  value = this->GetProp(prop);
  SEStringList dbg = this->getPropDebug(prop, value);
  printf("%s %s -> %s\n", (const char*)dbg[0], (const char*)dbg[1], (const char*)dbg[2]);
  **/
  
  if (prop == Transfer::P_STATUS)
  {
    // Transfer::P_STATUS debug output..
    Transfer::STATUS transferStatus;
    GetPropStatus(transferStatus);
    printf("%s\n", (const char*)tostring(transferStatus));

    // Here we handle the case when an incoming FT was created with PLACEHOLDER status.
    // In that case we could not accept it immediately, and had to wait until the
    // status changed to NEW. Which is this place here.
    if (transferStatus == NEW)
    {
      Transfer::TYPE transferType;
      this->GetPropType(transferType);
      if (transferType == Transfer::INCOMING) this->AutoAccept();
    };

    // Is the transfer done yet?
    if  (  
        (transferStatus == CANCELLED) || 
        (transferStatus == COMPLETED) || 
        (transferStatus == FAILED) || 
        (transferStatus == CANCELLED_BY_REMOTE) 
        )
    {
      printf("Removing Transfer from Conversation transfers list\n");
      if ( globalTransferList->contains(this->ref()) )

      {
        globalTransferList->remove_val(this->ref());
      }

      if (globalTransferList->size() == 0)
      {
        printf("All pending file transfers are complete.\nPress ENTER to quit.");
        isDoneSending = true;
      };
    };
  };
  
  // Transfer progress output.
  if (prop == Transfer::P_BYTESTRANSFERRED)
  {
    SEString transferProgressStr;
    this->GetPropBytestransferred(transferProgressStr);
    uint transferProgress = transferProgressStr.toUInt();
    
    SEString fileSizeStr;
    this->GetPropFilesize(fileSizeStr);
    
    // fileSize is float here, to avoid trouble with 
    // files lessthan 100 bytes in size.
    float fileSize = fileSizeStr.toUInt();
    float progress = transferProgress / (fileSize / 100);

    uint transferRate;
    this->GetPropBytespersecond(transferRate);
    float transferRateInKb = transferRate / 1024;

    printf("Progress: %3.0f%% (%1.0f KB/s)\n", progress, transferRateInKb);
  };
};

void MyConversation::OnMessage (const MessageRef &message)
{
  int messageType = message->GetProp(Message::P_TYPE).toInt();

  if (messageType == Message::POSTED_FILES)
  {  
    MyTransfer::Refs transferList;
    message->GetTransfers(transferList);
    fetch(transferList);

    // In case of incoming transfers, let's auto-accept them..
    Transfer::TYPE transferType;
    Transfer::STATUS transferStatus;

    for (uint i=0; i < transferList.size(); i++)
    {
      // To keep getting MyTransfer class events, we need to store the references
      globalTransferList->append(transferList[i]);

      // For incomings, we need to check for transfer status, just to be sure.
      // In some cases, a transfer can appear with STATUS == PLACEHOLDER
      // As such transfers cannot be accepted, we will need to just store
      // the reference to Transfer Object and then check for further
      // status changes in Transfer::OnChange
      transferList[i]->GetPropType(transferType);
      transferList[i]->GetPropStatus(transferStatus);
      if ( (transferType == Transfer::INCOMING) && (transferStatus == Transfer::NEW) )
      {
        transferList[i]->AutoAccept();
      };
    };

    SEString bodyXml;
    message->GetPropBodyXml(bodyXml);
    printf("File transfer msg BodyXML:\n%s\n", (const char*)bodyXml);
  };
};


// Keeping global conversation list up-to-date to 
// make sure we get all the incoming nessages

void MySkype::OnConversationListChange (
  const ConversationRef & conversation,
  const Conversation::LIST_TYPE & type,
  const bool & added)
{
  if ((added) && (type == Conversation::ALL_CONVERSATIONS))
  {
    this->GetConversationList(this->allConvs);
    fetch(this->allConvs);
  };
};


// Nothing to do here, just waiting for incoming transfers
// Actual work is all in MyConversation::OnMessage
void MySkype::receiveFile ()
{
  getchar();
};


void MySkype::sendFile (SEString target, SEString fileName)
{
  SEStringList targets;
  targets.append(target);

  MyConversation::Ref transferConv;

  GetConversationByParticipants(targets, transferConv, true, true);
  transferConv.fetch();

  SEFilenameList fileList;
  TRANSFER_SENDFILE_ERROR errCode;
  SEFilename errFile;
  
  fileList.append(fileName);

  if ( !transferConv->PostFiles(fileList, "Sending file", errCode, errFile) )
  {
    printf("Error: cannot create file transfer.\n");
    isDoneSending = true;
  };

  while (!isDoneSending) { Delay(1); };
};


//----------------------------------------------------------------------------
// Main

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 16. - File Transfers.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_16 <skypename> <password> [target] [filename]\n");
    return 0;
  };

  myAccountName = argv[1];
  myAccountPsw  = argv[2];

  if (argv[3])
  {
    targetName  = argv[3];
    if (argv[4]) { fileName  = argv[4]; };
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

    skype->GetConversationList(skype->allConvs);
    fetch(skype->allConvs);

    // if we had 3rd and 4th cmd-line arguments - we go into send mode
    if ((targetName != "") && (fileName != ""))
    {
      printf("Sending %s to %s\n", (const char*)fileName, (const char*)targetName);
      skype->sendFile(targetName, fileName);
    }
    else // wait for incoming file transfers
    {
      printf("Now accepting incoming file transfers..\n");
      printf("Press ENTER to quit.\n");
      skype->receiveFile();
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
