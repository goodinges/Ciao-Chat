/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 12.

In this exercise, we will assemble an event history, of:
1. Incoming and outgoing calls
2. Voicemails
3. Authorization request
4. Authorizations granted

**/

#include "time.h"
#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString myAccountName;
SEString myAccountPsw;

//----------------------------------------------------------------------------
// Subclassing Skype

class MySkype : public Skype
{
public:
  MySkype() : Skype() {};
  Account* newAccount(int oid) { return new MyAccount(oid, this); };
};

//----------------------------------------------------------------------------
// Main

MySkype* skype;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 12 - Event History.\n");
  printf("*****************************************************************\n");

  if (argc < 3)
  {
    printf("usage: tutorial <skypename> <password>\n");
    return 0;
  };

  myAccountName = argv[1];
  myAccountPsw = argv[2];

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

    Message::Refs history;

    printf("-----------------------------------------------------------------------\n");
    printf("Call history\n");
    printf("-----------------------------------------------------------------------\n");
    skype->GetMessageListByType(Message::STARTED_LIVESESSION, false, history);
    for (uint i=0; i < history.size(); i++)
    {
      // Message::P_AUTHOR tells who initiated the call.
      SEString author=history[i]->GetProp(Message::P_AUTHOR);

      // For duration we unfortunately have to parse the XML
      // the duration that interests me is in
      // <part identity="&me">\n<duration>x</duration>...
      //
      // Real implementation should use a proper xml-parser here
      SEString body=history[i]->GetProp(Message::P_BODY_XML);
      const char *body_str=(const char*)body;
      char partname[256];
      snprintf(partname,256,"<part identity=\"%s\">",(const char*)myAccountName);
      const char *my_part=strstr(body_str,partname);
      int duration=-1;
      if (my_part)
      {
        const char *my_part_end=strstr(my_part,"</part>");
        if (my_part_end)
        {
          const char *duration_str=strstr(my_part,"<duration>");
          if (duration_str && duration_str<my_part_end)
          {
            duration_str+=strlen("<duration>");
            duration=atoi(duration_str);
          }
        }
      }
      // ditto for counting how many parts the body has.
      int num_parts=0;
      char *tmp=(char*)body_str;
      while ((tmp=strstr(tmp,"<part")))
      {
        tmp++;
        num_parts++;
      }
      // get identities
      SEString identities=history[i]->GetProp(Message::P_IDENTITIES);
      // timestamp
      time_t timestamp=history[i]->GetUintProp(Message::P_TIMESTAMP);
      char *time_str=ctime(&timestamp);
      time_str[24]=0; // remove newline
      // last part is to fetch message reason
      SEString reason=history[i]->GetProp(Message::P_REASON);

      printf("[%s] ",time_str);
      if (!strcmp((const char*)author,(const char*)myAccountName))
      {
        // I was the initiator
        printf("outgoing call to %s: ",(const char*)identities);
      } else {
        // somebody else called
        printf("incoming call from %s: ",(const char*)identities);
      }
      if (duration>=0)
      {
        printf("duration %d seconds\n",duration);
      } 
      else 
      {
        if (num_parts>1)
        {
          if (!strcmp((const char*)reason,"manual"))
          {
            printf("refused\n");
          } else {
            printf("failed (%s)\n",(const char*)reason);
          }
        }
        else 
        {
          printf("missed\n");
        }
      };
    };

    printf("\nPress ENTER for voicemail history.\n");
    getchar();
    
    printf("-----------------------------------------------------------------------\n");
    printf("Voicemail history\n");
    printf("-----------------------------------------------------------------------\n");
    skype->GetMessageListByType(Message::POSTED_VOICE_MESSAGE, false, history);
    for (uint i=0; i < history.size(); i++)
    {
      SEString author=history[i]->GetProp(Message::P_AUTHOR);
      // XML parsing again...
      SEString body=history[i]->GetProp(Message::P_BODY_XML);
      
      const char *body_str=(const char*)body;
      
      int length=0;
      const char *len_str=strstr(body_str,"length=\"");
      if (len_str)
      {
        len_str+=strlen("length=\"");
        length=atoi(len_str);
      };

      // get identities
      SEString identities=history[i]->GetProp(Message::P_IDENTITIES);
      // timestamp
      time_t timestamp=history[i]->GetUintProp(Message::P_TIMESTAMP);
      char *time_str=ctime(&timestamp);
      time_str[24]=0; // remove newline
      printf("[%s] ",time_str);
      if (!strcmp((const char*)author,(const char*)myAccountName))
      {
        // I was the initiator
        printf("Sent voicemail to %s: ",(const char*)identities);
      } 
      else 
      {
        // somebody else called
        printf("Got voicemail from %s: ",(const char*)author);
      }
      printf("duration %d sec.\n",length);
    };


    printf("\nPress ENTER for authorization request history.\n");
    getchar();

    printf("-----------------------------------------------------------------------\n");
    printf("Authorization request history\n");
    printf("-----------------------------------------------------------------------\n");
    // Please note that REQUESTED_AUTH is not used to request authorization
    // ALERT is used for that. REQUESTED_AUTH is used only for history
    skype->GetMessageListByType(Message::REQUESTED_AUTH, false, history);
    for (uint i=0; i < history.size(); i++)
    {
      SEString author = history[i]->GetProp(Message::P_AUTHOR);
      // get identities
      SEString identities=history[i]->GetProp(Message::P_IDENTITIES);
      // timestamp
      time_t timestamp=history[i]->GetUintProp(Message::P_TIMESTAMP);
      char *time_str=ctime(&timestamp);
      time_str[24]=0; // remove newline
      printf("[%s] ",time_str);
      printf("Auth. request from %s to %s\n",(const char*)author,(const char*)identities);
    };


    printf("\nPress ENTER for authorizations granted history.\n");
    getchar();

    printf("-----------------------------------------------------------------------\n");
    printf("Authorizations granted history\n");
    printf("-----------------------------------------------------------------------\n");

    skype->GetMessageListByType(Message::GRANTED_AUTH, false, history);
    for (uint i=0; i < history.size(); i++)
    {
      SEString author = history[i]->GetProp(Message::P_AUTHOR);
      SEString identities=history[i]->GetProp(Message::P_IDENTITIES);

      time_t timestamp = history[i]->GetUintProp(Message::P_TIMESTAMP);
      char *time_str = ctime(&timestamp);
      time_str[24] = 0; // remove newline
      printf("[%s] ",time_str);
      printf("%s granted authorization to %s\n",(const char*)author,(const char*)identities);
    };

    printf("\nPress ENTER to quit.\n");
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
