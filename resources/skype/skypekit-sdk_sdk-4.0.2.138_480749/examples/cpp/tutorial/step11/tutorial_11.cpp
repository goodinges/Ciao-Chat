/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 11.

In this exercise, we will use Skype application to application (app2app)
features to make an datagram-based command-line chat between two instances
of the same SkypeKit client.

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

//----------------------------------------------------------------------------
// Cross-platform keyboard input. We will need this to implement our datagram-based chat.
bool __kbhit(void)
{
  #ifndef _WIN32
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec = 0;
    tv.tv_usec = 10000;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    if (select(1, &read_fd, NULL, NULL, &tv) == -1) return false;
    return FD_ISSET(0, &read_fd);
  #else
    return ::_kbhit();
  #endif
}

//----------------------------------------------------------------------------
// Subclassing Skype

class MySkype : public Skype
{
public:
    SEString    streamName;     // Datagrams still require streams IDs so lets keep one here.

    bool        quitCommand;    // Using this to detect remote quit (total stream count goes 0)
                                // or local quit ("q" + enter in keyboard buffer).

    bool        appConnected;   // Gets set first time when OnApp2AppStreamListChange gets fired
                                // with non-zero stream count. Basically, this is for prettiness,
                                // so that we don't display the "connection" message more than once.

    MySkype() : Skype()
    {
        streamName      = "";
        quitCommand     = false;
        appConnected    = false;
    };

    Account* newAccount(int oid)
      { return new MyAccount(oid, this);}
    
  void OnApp2AppStreamListChange (
    const Sid::String &appname, 
    const APP2APP_STREAMS &listType, 
    const Sid::List_String &streams, 
    const Sid::List_uint &receivedSizes);

  void OnApp2AppDatagram  (
    const Sid::String &appname, 
    const Sid::String &stream, 
    const Sid::Binary &data);
};

//----------------------------------------------------------------------------
// OnApp2AppDatagram will fire on incoming datagram packets.

void MySkype::OnApp2AppDatagram  (const Sid::String &appname, const SEString &stream, const SEBinary &data)
{
  SEBinary buf = data;
  buf[data.size()-1] = 0;
  printf("Got datagram in stream %s: %s\n", (const char*)stream, (const char*)buf);
};

//----------------------------------------------------------------------------
// Using this to convert APP2APP_STREAMS stream types into strings.

SEString StreamListType(const Skype::APP2APP_STREAMS type)
{
  switch (type)
  {
    case Skype::ALL_STREAMS: 
      return "all streams"; 
      break;

    case Skype::SENDING_STREAMS: 
      return "sending stream"; 
      break;

    case Skype::RECEIVED_STREAMS: 
      return "receiving stream"; 
      break;

    default:
      return "unknown stream type";
      break;
  };
};

//----------------------------------------------------------------------------
// OnApp2AppStreamListChange. In context of datagrams, this event will fire when:
//
//  1. Connection is established between two app2app applications. That is, when
//     both parties have an app up with the same name and -both -used App2AppConnect
//     In that case, both parties get this event, with listType ALL_STREAMS
//
//  2. When a datagram is sent, the sender will get this event with listType SENDING_STREAMS
//     Receiver of the datagram will get OnApp2AppDatagram event instead.
//
//  3. When the remote party drops app2app connection, the local user will get
//     OnApp2AppStreamListChange with listType ALL_STREAMS and streams.size() zero -
//     which is useful for detecting remote drops.

void MySkype::OnApp2AppStreamListChange (
  const Sid::String &appname, 
  const APP2APP_STREAMS &listType, 
  const Sid::List_String &streams, 
  const Sid::List_uint &receivedSizes)

{
  if (streams.size() != 0)
  {
    // Normally the streamcount in this event should be eithe 1 or 0.
    // More streams are possible when there are more than 2 connected
    // participants running the same application. For purposes of this
    // example, we will assume that there are only 2 participants.
    for (uint i=0; i < streams.size(); i++)
    {
      printf("OnApp2AppStreamListChange: %s %s %s\n",
          (const char*)StreamListType(listType),
          (const char*)appname,
          (const char*)streams[i]);
      streamName = streams[i];
    };
    if (!appConnected)
    {
      appConnected = true;
      printf("You can now type and send datagrams to remote instance.\n");
    };
  }
  else
  {
    if (listType == ALL_STREAMS)
    {
      // Remote side dropped connection.
      printf("No more app2app streams.\n");
      streamName = "";
      quitCommand = true;
    };
  };
};


//----------------------------------------------------------------------------
// Main

MySkype* skype;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 11 - app2app datagrams.\n");
  printf("*****************************************************************\n");

  if (argc < 4)
  {
    printf("usage: tutorial_11 <skypename> <password> <target>\n");
    return 0;
  };

  SEString myAccountName  = argv[1];
  SEString myAccountPsw   = argv[2];
  SEString myBuddyName    = argv[3];

  printf("Creating skype ..\n");
  skype = new MySkype();
  getKeyPair ();
  skype->init(keyBuf, inetAddr, portNum, "streamlog.txt");
  skype->start();
  
  printf("Getting account ..\n");
  MyAccount::Ref account;

  if (skype->GetAccount(myAccountName, account))
  {
    printf("Logging in..\n");
    account->LoginWithPassword(myAccountPsw, false, false);
    account->BlockWhileLoggingIn();

    bool cmdResult;
    SEString appName = "TestApp1";

    // Application creation should pretty much always be successful - unless there
    // already is an application with the same name.
    skype->App2AppCreate(appName, cmdResult);


    // The App2AppConnect connect result does -NOT- mean the connection was actually
    // established. It will return Ok even if the remote party was offline.
    // The actual connection will be only established when both parties have fired
    // App2AppConnect with the same application name, at eachother. At that point,
    // OnApp2AppStreamListChange event will fire and you can start exchanging data.
    skype->App2AppConnect(appName, myBuddyName, cmdResult);

    printf("Waiting for app2app connection to come up.\n");
    printf("Enter q to quit this example\n");
    char kbdBuffer[512];

    do {
      if (__kbhit())
      {
        fgets(kbdBuffer, 79, stdin);
        int bufSize = strlen(kbdBuffer);
        skype->quitCommand = (kbdBuffer[0] == 'q');

        if (!skype->quitCommand)
        {
          if (skype->streamName != "")
          {
            printf("Sending datagram.. %d bytes\n", bufSize);
            SEBinary dataBuffer;
            dataBuffer.set(&kbdBuffer, bufSize);
            skype->App2AppDatagram(appName, skype->streamName, dataBuffer, cmdResult);
          }
          else
          {
            printf("No app2app streams connected.\n");
          };
        };
      };
    } while (!skype->quitCommand);


    // App2AppGetConnectableUsers returns a list of currently online contacts.
    // It does not return a list of contacts who have an app2app application running.
    // There is currently no way of querying whether an application has been launched
    // on the remote side - other than trying to connect to the remote side and waiting
    // for timeout. NB! if you use App2AppGetConnectableUsers immediately after login -
    // then the online presence of yur contact list has not yet updated itself - so this
    // method will most likely return either an empty list or a list with echo123 in it.

    SEStringList appContacts;
    skype->App2AppGetConnectableUsers  (appName, cmdResult, appContacts);
    printf("App2AppGetConnectableUsers returned %d users.\n", appContacts.size());
    for (uint i=0; i < appContacts.size(); i++)
    {
      printf("%d - %s\n", i, (const char*)appContacts[i]);
    };

    printf("Disconnecting and deleting %s\n", (const char*)appName);
    skype->App2AppDisconnect(appName, skype->streamName, cmdResult);
    skype->App2AppDelete(appName, cmdResult);

    printf("Logging out..\n");
    account->Logout(false);
    account->BlockWhileLoggingOut();
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
