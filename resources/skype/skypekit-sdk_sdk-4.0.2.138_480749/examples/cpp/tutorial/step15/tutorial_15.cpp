/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 15.

This excercise consists of almost exact copy of App2App datagram
example (Step 11), except that the "chat" packets are sent and received
with stream reads/writes instead of datagrams.

**/

#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

//----------------------------------------------------------------------------
// Cross-platform keyboard input. We will need this to implement our chat.
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
// Interface section

class MySkype : public Skype
{
public:
  SEString    streamName;     // Stream name.

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

  Account* newAccount(int oid) { return new MyAccount(oid, this);}

  void OnApp2AppStreamListChange (
    const Sid::String &appname,
    const APP2APP_STREAMS &listType,
    const Sid::List_String &streams,
    const Sid::List_uint &receivedSizes);
};

//----------------------------------------------------------------------------
// Implementation section

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
  };  
};

//----------------------------------------------------------------------------
// OnApp2AppStreamListChange. This event will fire when:
//
//  1. Connection is established between two app2app applications. That is, when
//     both parties have an app up with the same name and -both -used App2AppConnect
//     In that case, both parties get this event, with listType ALL_STREAMS
//
//  2. When a packet is sent, the sender will get this event with listType SENDING_STREAMS.
//     For the sender side receivedSizes argument will be empty.

//  3. When a packet is received, receiver will get this event with listType RECEIVED_STREAMS
//     with streams argument containing list of affected stream IDs and receivedSizes
//     containing size of incoming packets in each stream. Note that data needs to be read out
//     of streams for subsequent OnApp2AppStreamListChange to fire.
//
//  4. When the remote party drops app2app connection, the local user will get
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
      // Outgoing stream packet
		  if (listType == Skype::SENDING_STREAMS)
      {
        printf("Sending packet to %s\n", (const char*)streams[i]);
        printf("recvSize = %d\n", receivedSizes.size());
      };

      // Incoming stream packet
		  if (listType == Skype::RECEIVED_STREAMS)
		  {
		    printf("Reading packet from %s - %d bytes\n", (const char*)streams[i], receivedSizes[i]);
		    bool readOk;
		    SEBinary buf;

		    // All data must be read from the stream!
		    // Otherwise, subsequent incoming packets will not
		    // cause the OnApp2AppStreamListChange event to fire.
		    App2AppRead(appname, streams[i], readOk, buf);

		    // Adding string terminator
		    buf.resize(receivedSizes[i]+1);
		    buf[receivedSizes[i]] = 0;
		    printf("Data: %s\n", (const char*)buf);
		  };
      streamName = streams[i];
    };

    // If we weren't connected before, then we are now..
    if (!appConnected)
    {
      appConnected = true;
      printf("You can now type and send packets to remote instance.\n");
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
  printf(" SkypeKit Tutorial, Step 15 - app2app streams.\n");
  printf("*****************************************************************\n");

  if (argc < 4)
  {
    printf("usage: tutorial_15 <skypename> <password> <target>\n");
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

    skype->App2AppCreate(appName, cmdResult);
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
            printf("Sending packet.. %d bytes\n", bufSize);
            SEBinary dataBuffer;
            dataBuffer.set(&kbdBuffer, bufSize);
            skype->App2AppWrite(appName, skype->streamName, dataBuffer, cmdResult);
          }
          else
          {
            printf("No app2app streams connected.\n");
          };
        };
      };
    } while (!skype->quitCommand);


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
