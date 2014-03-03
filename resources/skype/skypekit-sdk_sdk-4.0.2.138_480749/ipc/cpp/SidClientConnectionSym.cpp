#ifdef _SYMBIAN

#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#include "SidClientConnectionSym.hpp"

#define LOG(message) do {} while (0)

template<class T> inline T min(const T &a, const T &b) { return (a<b)?a:b; }

namespace Sid
{

  static void terminate(int& socket, ConnectionStateListener* listener)
  {
        static Mutex m;
        Mutex::ScopedLock l(m);
        if (socket == -1) return;
        shutdown(socket, SHUT_RDWR);
        close(socket);
        socket = -1;
        if (listener) listener->Disconnected();
  }

  static int socket_recv(int& socket, ConnectionStateListener* listener, uint num_bytes, char* dest, uint non_blocking_ahead)
  {
//                printf("reading from SocketConnection::recv()\n");
                if (-1 == socket) return socket;
                int ret, rd = 0;
                if (non_blocking_ahead) {
                  ret = ::recv(socket, dest, non_blocking_ahead, MSG_DONTWAIT);
                  if (ret <= 0) {
                    if (errno != EAGAIN) { //printf("terminate from SocketConnection::recieve()\n");
                      terminate(socket, listener);
                      return -1;
                    }
                  } else if ((uint)ret >= num_bytes) {
                    return ret;
                  } else {
                    num_bytes -= ret;
                    rd = ret;
                  }
                }
                if (num_bytes > 0) {
                    ret = ::recv(socket, dest+rd, num_bytes, 0);
                    if (ret <= 0 ) {// printf("terminate from SocketConnection::send()\n");                   
                        if (errno != EAGAIN) {// || errno != EWOULDBLOCK) {
                          terminate(socket, listener);
                          return -1;
                        }
                    } else {
                       rd += ret;
                       num_bytes -= ret;
                    }
               }
//                printf("read from SocketConnection::recv()\n");
               return rd;
  }


	TcpClientConnection::TcpClientConnection(const char* server_host, int server_port, const char* log_path, int retries, int retry_delay, ConnectionStateListener* listener)
        : Connection(log_path)
	{
		sockaddr_in addr;
		m_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		set_connstate_listener(listener);

		if (-1 != m_socket) {
			struct timeval timeo;
			timeo.tv_sec = 1;
			timeo.tv_usec = 0;
			// setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(timeo));
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr(server_host);
			addr.sin_port = htons(server_port);

			int failed;
			while ((failed = ::connect(m_socket, (struct sockaddr*)&addr, sizeof(addr))) < 0 && retries-- > 0) {
				Thread::sleep(retry_delay*1000);
				if (get_connstate_listener()) get_connstate_listener()->Connecting();
			}
			if (failed < 0)
				m_socket = -1;
			else
				if (get_connstate_listener()) get_connstate_listener()->Connected();
			//int cork = 1;
			//setsockopt(m_socket, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
		}
	}

  int TcpClientConnection::send(uint num_bytes, const char* src, bool immediate_hint)
  {
                  if (-1 == m_socket) return m_socket;
  //               printf("writting from SocketConnection::recv()\n");
                  int written = ::write(m_socket, src, num_bytes);
                  if (written < 0)
                  {       terminate();
                          return -1;
                  }
                  if (immediate_hint && num_bytes == static_cast<uint>(written)) {
                           // flush
                           int optval = 1;
                           setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
                           // back to naggling
                           optval = 0;
                           setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
                           // back to cork
                           //setsockopt(m_socket, IPPROTO_TCP, TCP_CORK, &optval, sizeof(optval));
                  }
                  return written;
  }
  
  int TcpClientConnection::recv(uint num_bytes, char* dest, uint non_blocking_ahead) {
           return socket_recv(m_socket, get_connstate_listener(), num_bytes, dest, non_blocking_ahead);
  }

  void TcpClientConnection::terminate() 
  { 
           Sid::terminate(m_socket, get_connstate_listener());
  }

  TcpClientConnection::~TcpClientConnection()
  { 
         terminate();
  }

  void TcpClientConnection::set_non_blocking() {
    int opts = fcntl(m_socket,F_GETFL);
    if (opts < 0) { terminate(); return; } 
    opts = (opts | O_NONBLOCK);
    if (fcntl(m_socket,F_SETFL,opts) < 0)  { terminate(); } 
  }

  static void wait_input(int socket)
  {
     fd_set socks;
     FD_ZERO(&socks);
     if (socket != -1) { // connection might have been terminated when writting...
       FD_SET(socket,&socks);
       struct timeval timeout;
       timeout.tv_sec  = 1;
       timeout.tv_usec = 0;
       ::select(socket+1, &socks, (fd_set *) 0,  (fd_set *) 0, &timeout);
    }
  }

  void TcpClientConnection::wait_input() {
    Sid::wait_input(m_socket);
  }


	LocalClientConnection::LocalClientConnection(TSecureId id, const char *log_path)
		:MsgQueueConnection(log_path)
		,m_iSkypeKit(id)
	{
		if (Connect())
			Initialize(&m_rRcvQ, &m_rSndQ);
	}

	LocalClientConnection::LocalClientConnection(const char *local, const char *streamlog, int retries, int retry_delay, ConnectionStateListener *pConnStateListener)
		:MsgQueueConnection(streamlog)
	{
		assert(false && "This constructor exists only because of socket based ClientSession init function!");
	}

	LocalClientConnection::~LocalClientConnection()
	{
		Disconnect();
	}

	void LocalClientConnection::terminate()
	{
		MsgQueueConnection::terminate();
		Disconnect();
	}

	bool LocalClientConnection::Connect()
	{
		if (is_connected())
			return true;

		//get handle to property
		TInt err= m_rConnectState.Attach(m_iSkypeKit, ESkypeKitConnectStateInt);
		if (err!=KErrNone)
		{
			//check error
			LOG("SKYPE ERROR - LocalClientConnection, failed to attach to connect state property ");
			return false;
		}

		//check & wait for message queue creation
		TInt queueId = CheckMessageQueues();
		if (queueId == 0)
		{
			LOG("SKYPE ERROR - LocalClientConnection skypekit message q's not created");
			return false;
		}

		TBuf<14> in;
		_LIT(inFmt, "%dIn");
		in.Format(inFmt, queueId);

		TBuf<14> out;
		_LIT(outFmt, "%dOut");
		out.Format(outFmt, queueId);

		//open skypekit message queues for sending
		err = m_rSndQ.OpenGlobal(in);
		if (err != KErrNone)
		{
			LOG("SKYPE ERROR - LocalClientConnection, failed to open sending create Q");
			return false;
		}

		//open skypekit message queues for receiving
		err = m_rRcvQ.OpenGlobal(out);
		if (err != KErrNone)
		{
			LOG("SKYPE ERROR - LocalClientConnection, failed to open reciving Q");
			return false;
		}

		//signal skypekit process that client connected
		err = m_rConnectState.Set(EQConnected);
		if (err!=KErrNone)
		{
			//check error
			LOG("SKYPE ERROR - LocalClientConnection, error setting property val");
			return false;
		}

		m_socket = 0; // Anything but -1 means 'connected'

		return true;
	}

	void LocalClientConnection::Disconnect()
	{
		if (m_socket == -1)
			return;

		m_rRcvQ.Close();
		m_rSndQ.Close();

		TInt err = m_rConnectState.Set(EQDisconnected);
		if (err != KErrNone)
		{
			LOG("SKYPE ERROR - LocalClientConnection, error setting property val");
		}

		m_rConnectState.Close();

		m_socket = -1;
	}

	TInt LocalClientConnection::CheckMessageQueues() const
	{
		RProperty queueState;
		//get handle to property
		TInt err = queueState.Attach(m_iSkypeKit, ESkypeKitMessageQStateInt);
		if (err != KErrNone)
		{
			//check error
			LOG("SKYPE ERROR - LocalClientConnection, failed to attach to queue state property ");
			return 0;
		}

		//check value
		TInt val = 0;
		err = queueState.Get(val);
		//test
		LOG("SKYPE INFO - LocalClientConnection, Queue prop val = " + SEString::from(val));

		if (err != KErrNone)
		{
			if (err != KErrNotFound) //can get KErrNotFound if skypekit has not yet defined prop
			{
				LOG("SKYPE ERROR - LocalClientConnection, error= " + SEString::from(err) + " getting property val");
				queueState.Close();
				return 0;
			}
			else val=0; //added in case random/garbage value retrieved when Get() returns KErrNotfound
		}

		//if message queues not yet created then wait...
		if (val == 0)
		{
			do //Property will be set to 0 then to the queue id
			{
				TRequestStatus status;
				queueState.Subscribe(status);

				RTimer timer;
				TRequestStatus timerStatus;
				timer.CreateLocal();
				timer.After(timerStatus, KQueueTimeout);

				//wait for skypekit to change property or timeout
				User::WaitForRequest(status,timerStatus);

				if (timerStatus==KRequestPending)
				{
					timer.Cancel();
					User::WaitForRequest(timerStatus);
					err = KErrNone;
				}
				else
				{
					queueState.Cancel();
					User::WaitForRequest(status);
					err = KErrNotFound;
				}

				//get value
				queueState.Get(val);

				timer.Close();
			}
			while ((val==0) && (err==KErrNone));
		}

		//close property
		queueState.Close();

		return val;
	}

} // namespace Sid

#endif // _SYMBIAN
