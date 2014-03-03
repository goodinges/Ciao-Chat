/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 3.

In this example, we will write a simple SkypeKit-based auto-reply bot. The bot will:
 1. Take a skypename and password as commmand-line arguments
 2. Log in
 3. Catch all incoming text chat messages
 4. Send a canned reply message
**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

//----------------------------------------------------------------------------
// Interface section

class MyConversation : public Conversation
{
public:
  typedef DRef<MyConversation, Conversation> Ref;
  typedef DRefs<MyConversation, Conversation> Refs;
  MyConversation(unsigned int oid, SERootObject* root) : Conversation(oid, root) {};

  void OnMessage(const MessageRef & message);
};


class MySkype : public Skype
{
public:
  Account*      newAccount(int oid)       {return new MyAccount(oid, this);}
  Conversation* newConversation(int oid)  {return new MyConversation(oid, this);}

  MyConversation::Refs inbox;
  void OnConversationListChange(const ConversationRef &conversation, const Conversation::LIST_TYPE &type, const bool &added);
};

//----------------------------------------------------------------------------
// Implementation section

void MyConversation::OnMessage(const MessageRef & message)
{
  Message::TYPE messageType;
  message->GetPropType(messageType);

  if (messageType == Message::POSTED_TEXT)
  {
    SEIntList propIds;
    SEIntDict propValues;
    propIds.append(Message::P_AUTHOR);
    propIds.append(Message::P_BODY_XML);
    propValues = message->GetProps(propIds);

    if (propValues[0] != myAccountName)
    {
      printf("%s : %s\n",
      (const char*)propValues[0],   // P_AUTHOR
      (const char*)propValues[1]);  // P_BODY_XML

      Message::Ref reply;
      this->PostText("This is an automated reply.", reply, false);
    };
  };
};

void MySkype::OnConversationListChange(
  const ConversationRef &conversation, 
  const Conversation::LIST_TYPE &type, 
  const bool &added)
{
  // if conversation was added to inbox and was not in there already (just in case..) -> append to our own inbox list
  if ( (type == Conversation::INBOX_CONVERSATIONS) && (added) && (!inbox.contains(conversation)) )
  {
    inbox.append(conversation);
  };
};

//----------------------------------------------------------------------------
// Main

MySkype* skype;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 3. - Getting and sending chat messages.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_3 <skypename> <password>\n");
    return 0;
  };

  myAccountName   = argv[1];
  myAccountPsw    = argv[2];

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

      skype->GetConversationList(skype->inbox, Conversation::INBOX_CONVERSATIONS);
      fetch(skype->inbox);

      printf("Now accepting incoming chat messages.\n");
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
