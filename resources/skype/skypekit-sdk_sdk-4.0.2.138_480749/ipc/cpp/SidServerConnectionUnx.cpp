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
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(_SYMBIAN)
#undef __size_t
#endif

namespace Sid {

  static void terminate(int& socket, ConnectionStateListener* listener)
  {
        static Mutex m;
        Mutex::ScopedLock l(m);
        if (socket == -1) return;
        shutdown(socket, SHUT_RDWR);
        while (close(socket) < 0 && (errno == EINTR || errno == EAGAIN)) {}
        socket = -1;
        if (listener) listener->Disconnected();
  }

  static int socket_recv(int& socket, ConnectionStateListener* listener, uint num_bytes, char* dest, uint non_blocking_ahead)
  {
                //printf("reading %d from SocketConnection::recv()\n", num_bytes);
                if (-1 == socket) return socket;
                int ret, rd = 0;
                if (non_blocking_ahead) {
                  ret = ::recv(socket, dest, non_blocking_ahead, MSG_DONTWAIT);
                  if (ret <= 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) { //printf("terminate from SocketConnection::recieve()\n");
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
                        if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
                          terminate(socket, listener);
                          return -1;
                        }
                  } else {
                     rd += ret;
                  }
                }
                //printf("read %d from SocketConnection::recv()\n", rd);
                return rd;
  }

  TcpListener::TcpListener(int port, int max_parallel_connection, int exit_after_connection, int local_only)
  : Listener(max_parallel_connection, exit_after_connection),
    m_local_only(local_only)
  { m_socket = socket (PF_INET, SOCK_STREAM, 0);
    if ( m_socket == -1 ) return;
    int reuse_addr = 1;
    setsockopt (m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse_addr, sizeof (reuse_addr));
    //int cork = 1;
    //setsockopt(m_socket, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
    int opts = fcntl(m_socket,F_GETFL);
    if (opts < 0) { fprintf(stderr, "ERR: F_GETFL failed\n"); m_socket = -1; return; } 
    opts = (opts | O_NONBLOCK);
    if (fcntl(m_socket,F_SETFL,opts) < 0)  { fprintf(stderr, "ERR: set O_NONBLOCK failed\n");  m_socket = -1; return; } 
    struct sockaddr_in bind_addr;
    memset (&bind_addr, 0, sizeof (bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = local_only ? inet_addr ("127.0.0.1") : INADDR_ANY;
    bind_addr.sin_port = htons (port);
    if (   bind (m_socket, (struct sockaddr *)&bind_addr, sizeof (bind_addr)) == -1 
        || listen (m_socket, 5) == -1
       ) {
      terminate(m_socket, 0);
      fprintf(stderr,  "ERR: can not bind socket %x %d\n",bind_addr.sin_addr.s_addr,port);
    }
  }

  TcpListener::~TcpListener()
  {
    if (m_socket != -1) {
//      shutdown(m_socket, SHUT_WR);
      while (close(m_socket) < 0 && (errno == EINTR || errno == EAGAIN)) {}
      m_socket = -1;
    }
  }
  
  void TcpListener::process_command(CommandInitiator*)
  { 
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof (client_addr);
        int clientfd =  accept (m_socket, (struct sockaddr *)&client_addr, &addrlen); // non blocking
        while (clientfd <= 0 && (errno == EAGAIN || errno == EINTR)) {
                clientfd =  accept (m_socket, (struct sockaddr *)&client_addr, &addrlen);
        }
        if ( clientfd > 0 ) { 
                if (m_local_only && client_addr.sin_addr.s_addr != inet_addr ("127.0.0.1")) {
                        while (close(clientfd) < 0 && (errno == EINTR || errno == EAGAIN)) {}
                        add_connection(-1);
                } else {
                        add_connection(clientfd);
                }
        } else add_connection(-1);
  }

  LocalListener::LocalListener(const char* local_name, int max_parallel_connection, int exit_after_connection)
  : Listener(max_parallel_connection, exit_after_connection)
  {
    sockaddr_un* bind_addr = (sockaddr_un*) malloc(sizeof(sockaddr_un)+strlen(local_name));
    bind_addr->sun_family = AF_LOCAL;
    bind_addr->sun_path[0] = '\0';
    strcpy (bind_addr->sun_path + 1, local_name);
    m_socket = socket (PF_LOCAL, SOCK_STREAM, 0);
    m_local_name = 0;
    if ( m_socket != -1 ) {
      int opts = fcntl(m_socket,F_GETFL);
      if (opts < 0) { m_socket = -1; return; } 
      opts = (opts | O_NONBLOCK);
      if (fcntl(m_socket,F_SETFL,opts) < 0)  { m_socket = -1; return; } 
      // offsetof() macro evaluates to the offset of a given member within a struct or union type
      // +1 is because abstract sockets have the leading nul in front
      if (  bind (m_socket, (struct sockaddr*) bind_addr,
                offsetof (struct sockaddr_un, sun_path) + strlen(local_name) + 1) == -1 
          || listen (m_socket, 5) == -1 ) {
          terminate(m_socket, 0);
      }
      m_local_name = new char[strlen(local_name)+1];
      strcpy (m_local_name, local_name);
    }
    free(bind_addr);
  }

  void LocalListener::process_command(CommandInitiator*)
  {
    struct sockaddr_un client_addr;
    socklen_t addrlen = sizeof (client_addr);
    int clientfd = accept (m_socket, (struct sockaddr *)&client_addr, &addrlen); // non blocking
    while (clientfd <= 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        clientfd =  accept (m_socket, (struct sockaddr *)&client_addr, &addrlen);
    }
    add_connection(clientfd);
  }

  LocalListener::~LocalListener()
  {
    if (m_socket != -1) {
      while (close(m_socket) < 0 && (errno == EINTR || errno == EAGAIN)) {}
      unlink(m_local_name); // here?s
      m_socket = -1;
    }
    delete[] m_local_name;
  }

  TcpServerConnection::TcpServerConnection(int socket, const char* log)
  : Connection(log)
  {
    // note: we keep connection socket in blocking mode, works fine with select
    m_socket = socket;   
  }

  TcpServerConnection::~TcpServerConnection()
  {
    terminate();
  }

  int TcpServerConnection::get_connid() const 
  {
    return m_socket;
  }
  
  int TcpServerConnection::send(uint num_bytes, const char* src, bool immediate_hint)
  {
                //printf("sending %d from SocketConnection::send()\n", num_bytes);
                int socket = m_socket;
                if (-1 == socket) return socket;
                int written = ::write(socket, src, num_bytes);
                if (written <= 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) { 
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
                //printf("sent %d from SocketConnection::send()\n", written);
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


  LocalServerConnection::LocalServerConnection(int socket, const char* log)
  : Connection(log)
  {
    // note: we keep connection socket in blocking mode, works fine with select
    m_socket = socket;   
  }

  LocalServerConnection::~LocalServerConnection()
  {
     terminate();
  }

  int LocalServerConnection::get_connid() const 
  {
    return m_socket;
  }

  int LocalServerConnection::send(uint num_bytes, const char* src, bool )
  {
                int socket = m_socket;
                if (-1 == socket) return socket;
                int written = ::write(socket, src, num_bytes);
                if (written <= 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) { 
                      terminate();
                      return -1;
                    }
                    written = 0;
                }
                return written;
  }

  int LocalServerConnection::recv(uint num_bytes, char* dest, uint non_blocking_ahead)
  {
     return socket_recv(m_socket, get_connstate_listener(), num_bytes, dest, non_blocking_ahead);
  }

  void LocalServerConnection::terminate()
  {
    Sid::terminate(m_socket, get_connstate_listener());
  }

  void ServerSession::select()
  {
    // not that fast but may allow saving threads...
    fd_set socks;
    int    fdmax=-1;
    FD_ZERO(&socks);
    ServerCommandInterface* connection;
    ServerCommandInterface* conns[Queue::CAPACITY];
    int num_c = 0;
    while ((connection = m_waiting_queue.pop()) != 0) {
      int id = connection->get_connid();
//printf("selecting connection %d\n",id);
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
//printf("connection has data or is closed %d, ready\n",id);
          m_ready_queue.push(connection);
        } else {
//printf("connection has no data %d, waiting\n",id);
          m_waiting_queue.push(connection);
        }
      }     
    } else { // if (readsocks < 0) { panic!
//printf("readsock < 0!\n");
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

