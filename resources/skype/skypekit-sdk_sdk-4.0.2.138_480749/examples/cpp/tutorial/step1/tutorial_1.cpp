/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 1.

In this exercise, we'll examine a program that
 1. Takes a username and password as command-line arguments
 2. Initiates login with that username
 3. Waits until the login process is complete
 4. Waits for user to press Enter and initiates logout
 5. Waits until logout is complete
 6. Cleans up and exits
**/

#include "skype-embedded_2.h"
#include "keypair.h"

#ifdef _WIN32
#include "Windows.h"
#else
#include <unistd.h>
#endif

using namespace Sid;

// Parameters for the transport.
SEString  inetAddr  = "127.0.0.1";
uint      portNum   = 8963;

void Delay(int Sec)
{
  #ifdef _WIN32
    Sleep(Sec * 1000);
  #else
    sleep(Sec);
  #endif
};


//----------------------------------------------------------------------------
// Interface section

class MyAccount : public Account
{
public:
  // The purpose of these typedef's is to enable us to delclare reference variables as:
  // MyAccount::Ref someAccount
  // MyAccount::Refs someAccountList
  typedef DRef<MyAccount, Account> Ref;
  typedef DRefs<MyAccount, Account> Refs;

  // Simple globals for the main loop. We set those two bools from within
  // OnChange callback, checking for Account status property changes.
  bool loggedIn;
  bool loggedOut;

  MyAccount(unsigned int oid, SERootObject* root);
  void OnChange(int prop);
};

class MySkype : public Skype
{
public:
  // Every time an account object is created, we will return instance of MyAccount.
  // This basically "registers" our new MyAccount class as the new Account class.
  Account* newAccount(int oid) { return new MyAccount(oid, this);}
};

//----------------------------------------------------------------------------
// Implementation section


MyAccount::MyAccount(unsigned int oid, SERootObject* root) : Account(oid, root) 
{
  loggedIn = false;
  loggedOut = false;
};

// There are no separate events for different account properties -
// this callback will fire for all property changes.

void MyAccount::OnChange(int prop)
{
  // Here we are swicthing loggedIn and loggedOut bools to inform our 
  // "main loop" of the account status changes.

  if (prop == Account::P_STATUS)
  {
    // OnChange callback does not directly give you the new value
    // for the property that changed. You will need to explicitly
    // retrieve the new value.
    Account::STATUS loginStatus;
    this->GetPropStatus(loginStatus);
    if (loginStatus == Account::LOGGED_IN)
    {
      printf("Login complete.\n");
      loggedIn = true;
    }
    else if (loginStatus == Account::LOGGED_OUT)
    {
      // Account can proceed to LOGGED_OUT not only on because of intentional logout.
      // This state is also reached by failure conditions, such as invalid password, etc.
      // For this reason, it makes sense to also check the value of Account::P_LOGOUTREASON
      // property.
      Account::LOGOUTREASON whyLogout;
      this->GetPropLogoutreason(whyLogout);

      // The tostring function conveniently converts an SkypeKit enum value
      // to an SEString in following format: <Class>::<ENUMERATOR>.<ENUM>
      // For example: "Account::LOGOUTREASON.INCORRECT_PASSWORD"
      printf("%s\n", (const char*)tostring(whyLogout));

      loggedOut = true;
      printf("Logout complete.\n");
    };
  };
};


//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 1 - Login with skypename and password.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_1 <skypename> <password>\n");
    return 0;
  };

  SEString accountName  = argv[1];
  SEString accountPsw   = argv[2];

  // At this point the SDK runtime should be already up and running.
  printf("Creating skype ..\n");
  skype = new MySkype();

  // Retrieving the keypair and starting up the Skype instance
  getKeyPair ();
  skype->init(keyBuf, inetAddr, portNum, "streamlog.txt");
  skype->start();

  printf("Getting account ..\n");

  MyAccount::Ref account;

  if (skype->GetAccount(accountName, account))
  {
    printf("Logging in..\n");
    account->LoginWithPassword(accountPsw, false, false);

    // Loop until LoggedIn or login failure
    while ( (!account->loggedIn) && (!account->loggedOut) ) { Delay(1); }; 

    if (account->loggedIn)
    {
      printf("Press ENTER to log out..\n");
      getchar();
    };

    printf("Logging out..\n");
    account->Logout(false);

    while (!account->loggedOut) { Delay(1); }; // Loop until LoggedOut
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
