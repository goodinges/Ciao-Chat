/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 8.

In this example, we'll write a command-line utility for adding, deleting, and 
listing PSTN (telephone number) contacts. The program will take five arguments:

 1. your skypename
 2. your password
 3. contact's phone number (to be added or removed)
 4. contact's displayname
 5. one of the following commands: -a (add a contact);  -d (delete contact); -l  (list all PSTN contacts)

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;


SEString myAccountName;
SEString myAccountPsw;

int countryPrefix = 372;

class MySkype : public Skype
{
public:
  MySkype() : Skype() {}
  Account* newAccount(int oid) {return new MyAccount(oid, this);}
};

//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 8. - Adding PSTN contact.\n");
  printf("*****************************************************************\n");

  if (argc < 6)
  {
    printf("usage: tutorial_8 <skypename> <password> <phoneno> <contact name> <-a|-d|-l>\n");
    printf(" -a  - Add PSTN contact.\n");
    printf(" -d  - Delete PSTN contact.\n");
    printf(" -l  - List PSTNcontacts.\n");
    return 0;
  };

  myAccountName     = argv[1];
  myAccountPsw      = argv[2];
  SEString newPSTN  = argv[3];
  SEString newName  = argv[4];
  SEString command  = argv[5];

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

    // First we will check if the newPSTN argument contains a valid PSTN number.
    SEString normalizedPSTN;
    Skype::NORMALIZERESULT normalizationResult;
    skype->NormalizePSTNWithCountry (newPSTN, normalizationResult, normalizedPSTN, countryPrefix);

    if (normalizationResult != Skype::IDENTITY_OK)
    {
      SEString normalizeResultStr = tostring(normalizationResult);
      printf("Error: %s - %s\n", (const char*)newPSTN, (const char*)normalizeResultStr);
    }
    else
    {
      // Next, lets check if the PSTN contact actually exists. This is relevant for both
      // adding and removing contacts. In current wrapper version, the only way to do this
      // is to loop over a contact group.

      Contact::Ref soContact;
      skype->GetContact(normalizedPSTN, soContact);

      ContactGroup::Ref soContactGroup;
      skype->GetHardwiredContactGroup(ContactGroup::SKYPEOUT_BUDDIES , soContactGroup);

      ContactRefs soContactList;
      soContactGroup->GetContacts(soContactList);

      bool contactAlreadyListed = soContactList.contains(soContact);

      // Listing PSTN contacts
      if (command == "-l")
      {
        if (soContactList.size() != 0)
        {
          printf("Current list of PSTN contacts:\n");
          SEString contactPSTN;
          for (uint i = 0; i < soContactList.size(); i++)
          {
            soContact = soContactList[i];
            contactPSTN = soContact->GetProp(Contact::P_PSTNNUMBER);
            printf("%d. %s\n", i+1, (const char*)contactPSTN);
          };
        }
        else
        {
          printf("There are no PSTN contacts.\n");
        };
      };
      // end of listing PSTN contacts

      // Removing PSTN contact.
      if (command == "-d")
      {
        if (!contactAlreadyListed)
        {
          printf("PSTN contact %s not found.\n", (const char*)normalizedPSTN);
        }
        else
        {
          printf("Removing PSTN contact..\n");
          Contact::Ref removableContact;
          skype->GetContact(normalizedPSTN, removableContact);

          if (soContactGroup->RemoveContact(removableContact)) 
          {
            printf("Contact %s %s removed.\n", (const char*)normalizedPSTN, (const char*)newName);
          }
          else
          {
            printf("Contact %s %s was NOT removed.\n", (const char*)normalizedPSTN, (const char*)newName);
          };
        };
      };
      // end of removing PSTN contact.

      // Adding PSTN contact.
      if (command == "-a")
      {
        if (contactAlreadyListed)
        {
          printf("Error: %s already present in contact list.\n", (const char*)normalizedPSTN);
        }
        else
        {
          printf("Adding PSTN contact..\n");
          Contact::Ref newContact;
          skype->GetContact(normalizedPSTN, newContact);
          newContact->GiveDisplayName(newName);
          if (soContactGroup->AddContact(newContact))
          {
            printf("Contact %s %s added.\n", (const char*)normalizedPSTN, (const char*)newName);
          }
          else
          {
            printf("Contact %s %s was NOT added. Possibly the PSTN number format is too odd?\n", 
              (const char*)normalizedPSTN, 
              (const char*)newName);
          };
        };
      };
      // end of Adding PSTN contact.
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
