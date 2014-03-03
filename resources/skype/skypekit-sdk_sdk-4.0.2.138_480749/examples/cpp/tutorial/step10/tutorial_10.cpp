/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 10.

In this step we will create a html snipped that can be used to join public chats
by clicking on a web page.

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

//----------------------------------------------------------------------------
// Interface section

class MySkype : public Skype
{
public:
  MySkype() : Skype() {};
  Account* newAccount(int oid) {return new MyAccount(oid, this);}
};

//----------------------------------------------------------------------------
// Main

MySkype* skype;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 10. - Creating Public Chat BLOB.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_10 <skypename> <password>\n");
    return 0;
  };

  myAccountName     = argv[1];
  myAccountPsw      = argv[2];

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
    
    Conversation::Ref conv;
    if (skype->CreateConference(conv))
    {      
      // NB! Setting chat options must be done before asking for a join BLOB.
      conv->SetOption(Conversation::P_OPT_JOINING_ENABLED,  true);
      conv->SetOption(Conversation::P_OPT_ENTRY_LEVEL_RANK, Participant::SPEAKER);
      conv->SetOption(Conversation::P_OPT_DISCLOSE_HISTORY, true);
      
      SEString blob;
      if (conv->GetJoinBlob(blob))
      {
        printf("You can copy&paste the following html link and use it in a web page to join Public Chat:\n\n");
        printf("<a href=\"skype:?chat&blob=%s\">Click here.</a>\n\n", (const char*)blob);
        printf("Note that the creator of this chat - %s - needs to be online for joining to function.\n", 
          (const char*)myAccountName);
      }
      else 
      {
        printf("Unable to retrieve join BLOB from conversation.\n");
      };
    }
    else
    {
      printf("Unable to create conversation.\n");
    };
    
    printf("Press ENTER to quit.\n");
    getchar();

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
