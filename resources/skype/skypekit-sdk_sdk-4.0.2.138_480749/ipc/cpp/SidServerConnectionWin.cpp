#include "SidServerConnection.hpp"
#include "SidServerSession.hpp"
#include <errno.h>
#include <stddef.h>
#include <string.h>

#include <winsock2.h>

namespace Sid {

  TcpListener::TcpListener(int port, int max_parallel_connection, int exit_after_connection, int local_only)
  : Listener(max_parallel_connection, exit_after_connection),
    m_local_only(local_only)
  { m_socket = socket (PF_INET, SOCK_STREAM, 0);
    if ( m_socket == -1 ) return;
    int reuse_addr = 1;
    setsockopt (m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse_addr, sizeof (reuse_addr));
    struct sockaddr_in bind_addr;
    memset (&bind_addr, 0, sizeof (bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = local_only ? inet_addr ("127.0.0.1") : INADDR_ANY;
    bind_addr.sin_port = htons (port);
    if (   bind (m_socket, (struct sockaddr *)&bind_addr, sizeof (bind_addr)) == -1 
        || listen (m_socket, 5) == -1
       ) {
      m_socket = -1;
    }
  }

  TcpListener::~TcpListener()
  {
    if (m_socket != -1) {
      closesocket (m_socket);
      m_socket = -1;
    }
  }

  void TcpListener::process_command(CommandInitiator*)
  { typedef int socklen_t;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof (client_addr);
    int clientfd =  accept (m_socket, (struct sockaddr *)&client_addr, &addrlen); // non blocking
    if ( clientfd >= 0 ) { 
      if (m_local_only && client_addr.sin_addr.s_addr != inet_addr ("127.0.0.1")) {
        closesocket (clientfd);
      } else {
        add_connection(clientfd);
      }
    }
  }

  static LocalListener* use_pipes = 0;
  LocalListener::LocalListener(const char* local_name, int max_parallel_connection, int exit_after_connection)
  : Listener(max_parallel_connection, exit_after_connection)
  {  
     use_pipes = this;
  }

  void LocalListener::process_command(CommandInitiator*)
  {
  }

  LocalListener::~LocalListener()
  {
    delete[] m_local_name;
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
                if (-1 != m_socket) terminate();
  }

  int TcpServerConnection::recv(uint num_bytes, char* dest, uint )
  {
                if (-1 == m_socket) return m_socket;
                int res = ::recv(m_socket, dest,num_bytes, 0); //num_bytes-r, 0);
                if (res == -1L && (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEWOULDBLOCK )) res = 0;//|| WSAGetLastError() == WSAETIMEDOUT));
                         if (res < 0) {
                               terminate();
                                return -1;
                         }
                return res;
  }

  int TcpServerConnection::send(uint num_bytes, const char* src, bool immediate_hint)
  {
                if (-1 == m_socket) return m_socket;
                int res = (long int)::send(m_socket, src, num_bytes, 0);
                if (res == -1L && (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEWOULDBLOCK)) res = 0;
                if (res < 0)
                {
                        terminate();
                        return -1;
                }
                if (immediate_hint && num_bytes == static_cast<uint>(res)) {
                         // flush : does it do that on windows?
                         int optval = 1;
                         setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*) &optval, sizeof(optval));
                         // back to naggling
                         optval = 0;
                         setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*) &optval, sizeof(optval));
                }

                return res;
  }

  static Mutex m;
  void TcpServerConnection::terminate()
  {
        Mutex::ScopedLock l(m);
        if (m_socket == -1) return;
        closesocket(m_socket);
        m_socket = -1;
        if (get_connstate_listener()) get_connstate_listener()->Disconnected();
  }

  LocalServerConnection::LocalServerConnection(int,const char* log)
  : Connection(log)
  {
  }

  LocalServerConnection::~LocalServerConnection()
  {
  }
  
  int LocalServerConnection::get_connid() const 
  {
    return m_socket;
  }

  void LocalServerConnection::terminate()
  {
  }
  
  int LocalServerConnection::recv(uint, char*, uint)
  {
    return -1;
  }
  
  int LocalServerConnection::send(uint, const char*, bool)
  {
    return -1;
  }
  
  void ServerSession::select()
  {
    if (use_pipes) {
      return;
    }
    // not that fast but may allow saving threads...
    fd_set socks;
    int    fdmax=-1;
    FD_ZERO(&socks);
    ServerCommandInterface* connection;
    ServerCommandInterface* conns[Queue::CAPACITY];
    int num_c = 0;
    while ((connection = m_waiting_queue.pop()) != 0) {
      int id = connection->get_connid();
      if (id != -1) { // connection might have been terminated when writting...
        conns[num_c++] = connection;
        if (id > fdmax) fdmax = id; 
        FD_SET(id,&socks);
      } else {
        m_ready_queue.push(connection);
      }
    } 
//    if (fdmax < 0) return;
    m_mutex.unlock();
    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;
    int readsocks = ::select(fdmax+1, &socks, (fd_set *) 0,  (fd_set *) 0, &timeout);
    m_mutex.lock();
    if (readsocks > 0) {
      while (num_c-- > 0) {
        connection = conns[num_c];
        int id = connection->get_connid();
        if (id == -1 || FD_ISSET(id,&socks)) {
          m_ready_queue.push(connection);
        } else {
          m_waiting_queue.push(connection);
        }
      }     
    } else { // if (readsocks < 0) { panic!
      while (num_c-- > 0) {
        connection = conns[num_c];
        if (connection->get_connid() == -1) { // connection might have been terminated when writting...
          m_ready_queue.push(connection);
        } else {
          m_waiting_queue.push(connection);
        }
      }
    }
  }

} // namespace Sid


