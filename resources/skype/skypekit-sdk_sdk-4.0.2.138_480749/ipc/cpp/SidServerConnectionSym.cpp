#ifdef _SYMBIAN

#include "SidServerConnection.hpp"
#include "SidServerSession.hpp"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>

#undef __size_t

namespace Sid {

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


	TcpListener::TcpListener(int port, int max_parallel_connection, int exit_after_connection, int local_only)
		: Listener(max_parallel_connection, exit_after_connection),
		m_local_only(local_only)
	{
		m_socket = socket (PF_INET, SOCK_STREAM, 0);
		if ( m_socket == -1 ) return;
		int reuse_addr = 1;
		setsockopt (m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse_addr, sizeof (reuse_addr));
		//int cork = 1;
		//setsockopt(m_socket, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
		int opts = fcntl(m_socket,F_GETFL);
		if (opts < 0) { m_socket = -1; return; }
		opts = (opts | O_NONBLOCK);
		if (fcntl(m_socket,F_SETFL,opts) < 0) { m_socket = -1; return; }
		struct sockaddr_in bind_addr;
		memset (&bind_addr, 0, sizeof (bind_addr));
		bind_addr.sin_family = AF_INET;
		bind_addr.sin_addr.s_addr = local_only ? inet_addr ("127.0.0.1") : INADDR_ANY;
		bind_addr.sin_port = htons (port);
		if ( bind (m_socket, (struct sockaddr *)&bind_addr, sizeof (bind_addr)) == -1
			|| listen (m_socket, 5) == -1
			) {
				m_socket = -1;
		}
	}

	TcpListener::~TcpListener()
	{
		if (m_socket != -1)
		{
			shutdown(m_socket, SHUT_WR);
			close (m_socket);
			m_socket = -1;
		}
	}

	void TcpListener::process_command(CommandInitiator*)
	{
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof (client_addr);
		int clientfd = accept (m_socket, (struct sockaddr *)&client_addr, &addrlen); // non blocking
		if ( clientfd >= 0 )
		{
			if (m_local_only && client_addr.sin_addr.s_addr != inet_addr ("127.0.0.1"))
			{
				close (clientfd);
			}
			else
			{
				add_connection(clientfd); return;
			}
		}
                add_connection(-1);
	}

	static _LIT_SECURITY_POLICY_S0(KClientPolicy, 0xe79b7644); // 0xe79b7644 is the client security id
	static _LIT_SECURITY_POLICY_S0(KSkypeKitPolicy, 0xa0000180);
	static _LIT_SECURITY_POLICY_PASS(KAllowAllPolicy);

	//max number of slots in queue, TODO analysis on if this needs to be increased
	static const TInt KMaxQueueSlotsIn = 64;
	static const TInt KMaxQueueSlotsOut = 128;

	// Thierry Grellier:
	//
	// ugly trick that work as long as local and tcp are exclusive. (assumed requirements in command line)
	// will work on this later... could give the listener pointer down to the LocalConnection class
	static LocalListener* use_local = 0;
	LocalListener::LocalListener(const char* local_name, int max_parallel_connection, int exit_after_connection)
		: Listener(max_parallel_connection, exit_after_connection)
	{
		assertFatal(max_parallel_connection == 1 && exit_after_connection == 1);
		m_socket = 1; // Thierry: let's give him id 1...
		use_local = this;

		InitL();
	}

	LocalListener::~LocalListener()
	{
		if (m_pInQueue != NULL)
		{
			m_pInQueue->Close();
			m_pOutQueue->Close();

			delete m_pInQueue;
			delete m_pOutQueue;
		}

		m_cConnectState.Set(0);
		m_cConnectState.Close();

		m_cQueueState.Set(0);
		m_cQueueState.Close();
	}

	void LocalListener::InitL()
	{
		RProcess loadprocess;
		TUid kSkypeKitSecureId = loadprocess.SecureId();

		//define property to hold client connected state
		TInt err = RProperty::Define(kSkypeKitSecureId, ESkypeKitConnectStateInt, RProperty::EInt, KSkypeKitPolicy, KClientPolicy);
		if (err!=KErrNone)
		{
			//check error
			if (err!=KErrAlreadyExists)
			{
				//__LOGSTR_TOFILE1("Error defining connect property=%d", err);
				User::Leave(err);
			}
			else
			{
				err = RProperty::Delete(kSkypeKitSecureId, ESkypeKitConnectStateInt);
				err = RProperty::Define(kSkypeKitSecureId, ESkypeKitConnectStateInt, RProperty::EInt, KSkypeKitPolicy, KClientPolicy);
				if (err != KErrNone) User::Leave(err);
			}
		}
		//attach to property
		User::LeaveIfError(m_cConnectState.Attach(kSkypeKitSecureId, ESkypeKitConnectStateInt));

		//set value to EQDisconnected
		//err = m_cConnectState.Set(EQDisconnected); // The variable is automatically initialized to 0 (EQDisconnected).
		//User::LeaveIfError(err);

		SetUpQueuePropL();

		SetUpMessageQueuesL();
	}

	void LocalListener::WaitOnMsgQueueConnectL()
	{
		TInt val;
		User::LeaveIfError(m_cConnectState.Get(val));

		if (val != EQConnected)
		{
			TRequestStatus status;

			m_cConnectState.Subscribe(status);
			User::WaitForRequest(status);

			User::LeaveIfError(m_cConnectState.Get(val));
			if (val != EQConnected)
			{
				User::Leave(KErrGeneral);
			}
		}
	}

	void LocalListener::process_command(CommandInitiator*)
	{
		WaitOnMsgQueueConnectL();
		add_connection(2); // Thierry: let's give id connection 2...
	}

	void LocalListener::GiveAwayQueues(RMsgQueue<message_buf> *&pInQueue, RMsgQueue<message_buf> *&pOutQueue)
	{
		pInQueue = m_pInQueue;
		pOutQueue = m_pOutQueue;

		m_pInQueue = NULL;
		m_pOutQueue = NULL;
	}

	void LocalListener::SetUpMessageQueuesL()
	{
		TInt err = KErrNone;

		TInt queueId = rand() + 1; // A value in the range [1;RAND_MAX+1]

		// 10 for the number, 3 for "In" or "Out" and 1 for '\0'
		TBuf<14> in;
		_LIT(inFmt, "%dIn");
		in.Format(inFmt, queueId);

		TBuf<14> out;
		_LIT(outFmt, "%dOut");
		out.Format(outFmt, queueId);

		//open global queue for receiving
		m_pInQueue = new RMsgQueue<message_buf>();
		err = m_pInQueue->CreateGlobal(in, KMaxQueueSlotsIn);
		if (err!=KErrNone)
		{
			if (err!=KErrAlreadyExists) //check if queue already exists - in case skypekit crashed and not cleaned up
			{
				User::Leave(err);
			}
		}

		//open global queue for sending
		m_pOutQueue = new RMsgQueue<message_buf>();
		err = m_pOutQueue->CreateGlobal(out, KMaxQueueSlotsOut);
		if (err!=KErrNone)
		{
			if (err!=KErrAlreadyExists)
			{
				User::Leave(err);
			}
		}

		User::LeaveIfError(m_cQueueState.Set(queueId));
	}

	void LocalListener::SetUpQueuePropL()
	{
		RProcess loadprocess;
		TUid kSkypeKitSecureId = loadprocess.SecureId();

		//define property to hold state of message queue
		TInt err = RProperty::Define(kSkypeKitSecureId, ESkypeKitMessageQStateInt, RProperty::EInt, KClientPolicy, KSkypeKitPolicy);
		if (err!=KErrNone)
		{
			//check error
			if (err!=KErrAlreadyExists)
			{
				//__LOGSTR_TOFILE1("Error defining queue state property=%d", err);
				User::Leave(err);
			}
			else
			{
				err = RProperty::Delete(kSkypeKitSecureId, ESkypeKitMessageQStateInt);
				err = RProperty::Define(kSkypeKitSecureId, ESkypeKitMessageQStateInt, RProperty::EInt, KClientPolicy, KSkypeKitPolicy);
				if (err != KErrNone) User::Leave(err);
			}
		}

		//attach to property
		User::LeaveIfError(m_cQueueState.Attach(kSkypeKitSecureId, ESkypeKitMessageQStateInt));
		User::LeaveIfError(m_cQueueState.Set(0));
	}

	LocalServerConnection::LocalServerConnection(int id, const char *log)
		: Sid::MsgQueueConnection(log)
	{
		RMsgQueue<message_buf> *pInQueue, *pOutQueue;
		use_local->GiveAwayQueues(pInQueue, pOutQueue);
		Initialize(pInQueue, pOutQueue);
		m_socket = id;
	}

	LocalServerConnection::~LocalServerConnection()
	{
		GetInQueue().Close();
		GetOutQueue().Close();

		delete &GetInQueue();
		delete &GetOutQueue();
	}

	int LocalServerConnection::get_connid() const
	{
		return m_socket; // ????
	}

	TcpServerConnection::TcpServerConnection(int socket, const char* log)
		: Connection(log)
	{
		// note: we keep connection socket in blocking mode, works fine with select
		m_socket = socket;
	}

	int TcpServerConnection::get_connid() const
	{
		return m_socket;
	}


  TcpServerConnection::~TcpServerConnection()
  {
    terminate();
  }
  
  int TcpServerConnection::send(uint num_bytes, const char* src, bool immediate_hint)
  {
                if (-1 == m_socket) return m_socket;
//               printf("writting from SocketConnection::recv()\n");
                int written = ::write(m_socket, src, num_bytes);
                if (written <= 0) {
                    if (errno != EAGAIN) { 
                      terminate();
                      return -1;
                    }
                    written = 0;
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

  int TcpServerConnection::recv(uint num_bytes, char* dest, uint non_blocking_ahead)
  {
     return socket_recv(m_socket, get_connstate_listener(), num_bytes, dest, non_blocking_ahead);
  }

  void TcpServerConnection::terminate()
  {
    Sid::terminate(m_socket, get_connstate_listener());
  }

	void ServerSession::select()
	{
		if (use_local)
		{
			ServerCommandInterface *connection = m_waiting_queue.pop();
			if (connection != NULL && connection->get_connid() != -1)
			{
				m_ready_queue.push(connection);
			}
		}
		else
		{
			// Same as in SidServerConnectionUnx.cpp

			// not that fast but may allow saving threads...
			fd_set socks;
			int fdmax=-1;
			FD_ZERO(&socks);
			ServerCommandInterface* connection;
			ServerCommandInterface* conns[Queue::CAPACITY];
			int num_c = 0;
			while ((connection = m_waiting_queue.pop()) != 0)
			{
				int id = connection->get_connid();
				if (id != -1)
				{ // connection might have been terminated when writting...
					conns[num_c++] = connection;
					if (id > fdmax) fdmax = id;
					FD_SET(id,&socks);
				}
				else
				{
					m_ready_queue.push(connection);
				}
			}
			// if (fdmax < 0) return;
			m_mutex.unlock();
			struct timeval timeout;
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			int readsocks = ::select(fdmax+1, &socks, (fd_set *) 0, (fd_set *) 0, &timeout);
			m_mutex.lock();
			if (readsocks > 0)
			{
				while (num_c-- > 0)
				{
					connection = conns[num_c];
					int id = connection->get_connid();
					if (id == -1 || FD_ISSET(id,&socks))
					{
						m_ready_queue.push(connection);
					}
					else
					{
						m_waiting_queue.push(connection);
					}
				}
			}
			else
			{ // if (readsocks < 0) { panic!
				while (num_c-- > 0)
				{
					connection = conns[num_c];
					if (connection->get_connid() == -1)
					{ // connection might have been terminated when writting...
						m_ready_queue.push(connection);
					}
					else
					{
						m_waiting_queue.push(connection);
					}
				}
			}
		}
	}

} // namespace Sid

#endif // _SYMBIAN
