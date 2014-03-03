/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 4.

In this example we will write a program to

 1. Populate the contact list
 2. Catch contact online status updates

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

//----------------------------------------------------------------------------
// Interface section

class MyContact : public Contact
{
public:
  typedef DRef<MyContact, Contact> Ref;
  typedef DRefs<MyContact, Contact> Refs;
  MyContact(unsigned int oid, SERootObject* root) : Contact(oid, root) {};

  void OnChange(int prop);
};

//----------------------------------------------------------------------------
// Overrideing contact group class here, to handle cases when this tutorial is used to
// log in with an already existing account first time on a local system. In this case
// the contact list starts out empty - as the local database for account was just created.
// The contact list will then build itself up during CBL synchronization. To obtain
// references to these newly synchronized contacts, it is necessary to catch 
// ContactGroup's OnChange event and add/remove references to our live reference list.

class MyContactGroup : public ContactGroup
{
public:
  typedef DRef<MyContactGroup, ContactGroup> Ref;
  typedef DRefs<MyContactGroup, ContactGroup> Refs;
  MyContactGroup(unsigned int oid, SERootObject* root) : ContactGroup(oid, root) {};

  MyContact::Refs contactList;
  virtual void OnChange( const ContactRef& contact );
};

class MySkype : public Skype
{
public:
  MySkype() : Skype() {};

  Account*      newAccount(int oid) {return new MyAccount(oid, this);}
  ContactGroup* newContactGroup(int oid) {return new MyContactGroup(oid, this);}
  Contact*      newContact(int oid) {return new MyContact(oid, this);}
};

//----------------------------------------------------------------------------
// Implementation section

void MyContact::OnChange(int prop)
{
  if (prop == Contact::P_AVAILABILITY)
  {
    SEString name;
    GetPropDisplayname(name);
    
    Contact::AVAILABILITY availability;
    GetPropAvailability(availability);

    SEString statusAsText;
    statusAsText = tostring(availability);

    printf("%s -> %s\n", (const char*)name, (const char*)statusAsText);
  };
};

void MyContactGroup::OnChange(const ContactRef& contact)
{
  ContactGroup::TYPE groupType;
  this->GetPropType(groupType);

  if (groupType == ContactGroup::SKYPE_BUDDIES)
  {
    SEString contactName;
    contact->GetPropDisplayname(contactName);

    if (!contactList.contains(contact))
    {
      contactList.append(contact);
      contact.fetch();
      printf("%s was added.\n", (const char*)contactName);
    }
    else
    {
      contactList.remove_val(contact);
      printf("%s was removed.\n", (const char*)contactName);
    };
  };
};

//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 4. - Contacts and online status events.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial_4 <skypename> <password>\n");
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

      MyContactGroup::Ref skypeNamesContactGroup;
      skype->GetHardwiredContactGroup(ContactGroup::SKYPE_BUDDIES, skypeNamesContactGroup);

      skypeNamesContactGroup->GetContacts(skypeNamesContactGroup->contactList);
      fetch(skypeNamesContactGroup->contactList);

      for (uint i = 0; i < skypeNamesContactGroup->contactList.size(); i++)
      {
        SEString contactName;
        skypeNamesContactGroup->contactList[i]->GetPropDisplayname(contactName);
        printf("%3d. %s\n", i+1, (const char*)contactName);
      };
		
      printf("Waiting for contact status change events..\n");
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
