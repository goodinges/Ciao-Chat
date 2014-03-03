
/****************************************************************************

This include file contains mainly the MyAccount class, so that it doesn't
need to be re-implemented in every single tutorial example.

**/

#include "skype-embedded_2.h"

SEString inetAddr   = "127.0.0.1";
uint portNum        = 8963;

#ifdef _WIN32
  #include "Windows.h"
  #include "conio.h"
#else
  #include <unistd.h>
#endif

void Delay(int Ms)
{
	#ifdef _WIN32
		Sleep(Ms);
	#else
		sleep(Ms);
	#endif
};

class MyAccount : public Account
{
public:
  typedef DRef<MyAccount, Account> Ref;
  typedef DRefs<MyAccount, Account> Refs;

  bool loggedIn;
  bool loggedOut;

  MyAccount(unsigned int oid, SERootObject* root);
  void OnChange(int prop);

  void BlockWhileLoggingIn ();
  void BlockWhileLoggingOut ();
};

MyAccount::MyAccount(unsigned int oid, SERootObject* root) : Account(oid, root) 
{
  loggedIn  = false;
  loggedOut = false;
};

void MyAccount::OnChange(int prop)
{
  if (prop == Account::P_STATUS)
  {
    Account::STATUS loginStatus;
    this->GetPropStatus(loginStatus);
    if (loginStatus == Account::LOGGED_IN)  { loggedIn  = true; };
    if (loginStatus == Account::LOGGED_OUT) 
    { 
      loggedOut = true; 
      Account::LOGOUTREASON whyLogout;
      this->GetPropLogoutreason(whyLogout);
      if (whyLogout != Account::LOGOUT_CALLED)
      {
        printf("%s\n", (const char*)tostring(whyLogout));
      };
    };
  };
};

void MyAccount::BlockWhileLoggingIn ()
{
  while ( (!this->loggedIn) && (!this->loggedOut) ) 
  { 
    Delay(1); 
  };

  if (!this->loggedIn)
  {
    printf("Login failed.\n");
  };
};

void MyAccount::BlockWhileLoggingOut ()
{
  while (!this->loggedOut) { Delay(1); };
};
