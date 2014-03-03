/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 9.

This program will take two command-linearguments:
 1. skypename
 2. password

 It will wait for incoming contact authorization request from remote party
 and display a notification message on screen.

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

class MyContactGroup : public ContactGroup
{
public:
  typedef DRef<MyContactGroup, ContactGroup> Ref;
  typedef DRefs<MyContactGroup, ContactGroup> Refs;
  MyContactGroup(unsigned int oid, SERootObject* root) : ContactGroup(oid, root) {};
  void OnChange(const ContactRef& contact);
};

void MyContactGroup::OnChange(const ContactRef& contact)
{
  // This method will fire for all contact groups. As we are only interersted in
  // one particular group - CONTACTS_WAITING_MY_AUTHORIZATION we will need to check
  // the P_TYPE property.
  ContactGroup::TYPE Type;
  GetPropType(Type);
  if (Type == ContactGroup::CONTACTS_WAITING_MY_AUTHORIZATION)
  {
    SEString AuthRequestText;
    contact->GetPropReceivedAuthrequest(AuthRequestText);

    if (!AuthRequestText.isEmpty())
    {
      SEString ContactSkypeName;
      contact->GetPropSkypename(ContactSkypeName);
      printf("Auth. request from: %s - %s\n", (const char*)ContactSkypeName, (const char*)AuthRequestText);
      // Possible values: NONE, AUTHORIZED_BY_ME, BLOCKED_BY_ME
      if (contact->SetBuddyStatus(Contact::AUTHORIZED_BY_ME))
      {
        printf("%s is now authorized\n", (const char*)ContactSkypeName);
      }
      else
      {
        printf("Authorization failed.\n");
      };
    };
  };
};

class MySkype : public Skype
{
public:
  MySkype() : Skype() {};
  Account*      newAccount(int oid) { return new MyAccount(oid, this); }
  ContactGroup* newContactGroup(int oid) { return new MyContactGroup(oid, this); }
};


//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" Getting Started With SkypeKit. Tutorial, Step 9.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_9 <skypename> <password>\n");
    return 0;
  };

  myAccountName   = argv[1];
  myAccountPsw    = argv[2];

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

    // Here we will get ourselves a MyContactGroup object, so that we will get
    // OnChange events when an incoming auth. request occurs.
    MyContactGroup::Ref waitingList;
    skype->GetHardwiredContactGroup(ContactGroup::CONTACTS_WAITING_MY_AUTHORIZATION , waitingList);
    waitingList.fetch();

    // The rest of the interesting stuff will take place in MyContactGroup::OnChange
    printf("Waiting for incoming auth. requests..\n");
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

