/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 2.

In this example we will:
 1. Take a skypename and password as command-line arguments
 2. Retrieve a list of Conversation objects
 3. Retrieve the display name (P_DISPLAYNAME) property of each Conversation
 4. Retrieve multiple properties of the Conversation with the GetProps method
**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

//----------------------------------------------------------------------------
// Interface section

class MySkype : public Skype
{
public:
  // Note that to reduce clutter in this and all the further tutorials, 
  // MyAccount classcomes from tutorial_common.h
  Account* newAccount(int oid) {return new MyAccount(oid, this);}
};

//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 2. - Conversation list and properties\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_2 <skypename> <password>\n");
    return 0;
  };

  SEString myAccountName  = argv[1];
  SEString myAccountPsw   = argv[2];

  printf("creating skype ..\n");
  skype = new MySkype();

  printf("starting Skype and submitting application token..\n");
  getKeyPair ();
  skype->init(keyBuf, inetAddr, portNum, "streamlog.txt");
  skype->start();

  printf("getting account ..\n");
  MyAccount::Ref account;

  if (skype->GetAccount(myAccountName, account))
  {
    printf("Logging in..\n");
    account->LoginWithPassword(myAccountPsw, false, true);

    account->BlockWhileLoggingIn();

    if (account->loggedIn)
    {
      Conversation::Ref conv;

      printf("Example 1: one Conversation (using GetConversationByParticipants)\n");
      SEStringList partnerNames;    
      partnerNames.append("partnernamehere"); // <- you may want to change this argument

      // Note that setting the 1st boolean argument true will cause this method
      // to automatically create Conversation object, even if there is actually
      // no such contact as given in partnerNames.
      if (skype->GetConversationByParticipants(partnerNames, conv, true, false))
      {
        SEString displayName;
        conv->GetPropDisplayname(displayName);
        printf("Conversation disaplayname: %s\n", (const char*)displayName);
      }
      else
      {
        printf("No conversation on this account with this participant.\n");
      };

      printf("Press ENTER to continue..\n");
      getchar();

      printf("Example 2: List of Conversations (using GetConversationList)\n");
      Conversation::Refs inbox;
      skype->GetConversationList(inbox, Conversation::INBOX_CONVERSATIONS);
      printf("Conversations in Inbox : %d\n", inbox.size());

      for (uint i = 0; i < inbox.size(); i++)
      {
        conv = inbox[i];
        SEString displayName;
        conv->GetPropDisplayname(displayName);
        printf("%2d. %s\n", i+1, (const char*)displayName);
      };

      printf("Press ENTER to continue..\n");
      getchar();

      printf("Example 3: List of Conversations and multiple properties at once (using GetProps)\n");
      skype->GetConversationList(inbox, Conversation::INBOX_CONVERSATIONS);
      printf("Conversations in Inbox : %d\n", inbox.size());

      SEIntList propIds;
      SEIntDict propValues;
      propIds.append(Conversation::P_TYPE);
      propIds.append(Conversation::P_DISPLAYNAME);

      SEString convType;
      SEString displayName;
      for (uint i = 0; i < inbox.size(); i++)
      {
        // GetProps will return a list with both P_TYPE and P_DISPLAYNAME for the object.
        // Weakness here is that the while values are returned as SEStrings, enum values
        // are not converted to their human-readable equivalents. convType for example
        // will be either "1" or "2", rather than "DIALOG" or "CONFERENCE".
        propValues  = inbox[i]->GetProps(propIds);
        convType    = propValues.find(Conversation::P_TYPE);
        displayName = propValues.find(Conversation::P_DISPLAYNAME);
        printf("Type: %s  Name: %s \n", (const char*)convType, (const char*)displayName);
      };

      printf("Press ENTER to log out..\n");
      getchar();

      printf("Logging out..\n");
      account->Logout(false);
      account->BlockWhileLoggingOut();
    }
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
