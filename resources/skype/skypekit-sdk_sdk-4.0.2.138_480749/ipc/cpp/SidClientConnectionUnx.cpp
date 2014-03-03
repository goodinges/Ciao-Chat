#include "SidClientConnection.hpp"

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
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __GNUC__
//http://gcc.gnu.org/bugzilla/show_bug.cgi?id=44924
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace Sid {

  static void terminate(int& socket, ConnectionStateListener* listener)
  {
        static Mutex m;
        Mutex::ScopedLock l(m);
        if (socket == -1) return;
        shutdown(socket, SHUT_RDWR);
        while (close(socket) < 0 && (errno == EINTR || errno == EAGAIN));
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
                timeo.tv_sec  = 1;
                timeo.tv_usec = 0;
//                setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(timeo));
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

  LocalClientConnection::LocalClientConnection(const char* socket_name, const char* log_path, int retries, int retry_delay, ConnectionStateListener* listener)
  : Connection(log_path)
  {
#if !defined(SYMBIAN)
        sockaddr_un* addr = (sockaddr_un*) malloc(sizeof(sockaddr_un)+strlen(socket_name));
        m_socket = ::socket(PF_LOCAL, SOCK_STREAM, 0);
        set_connstate_listener(listener);
        
        if (-1 != m_socket) {
                struct timeval timeo;
                timeo.tv_sec  = 1;
                timeo.tv_usec = 0;
//                setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(timeo));
                int failed = 0;
                addr->sun_family = AF_LOCAL;
                addr->sun_path[0] = '\0';
                strcpy(addr->sun_path + 1, socket_name);
                while ((failed = ::connect(m_socket, (struct sockaddr*) addr,
                       offsetof (struct sockaddr_un, sun_path)+strlen(socket_name)+1)) != 0
                       && retries-->0) {
                       Thread::sleep(retry_delay*1000);
                       if (get_connstate_listener()) get_connstate_listener()->Connecting();
                }
                if (failed) 
                        m_socket = -1;
                else {
                        if (get_connstate_listener()) get_connstate_listener()->Connected();                                                           
                }
        }
        free(addr);
#endif
  }

  int LocalClientConnection::send(uint num_bytes, const char* src, bool )
  {
#if !defined(SYMBIAN)
                if (-1 == m_socket) return m_socket;
                int written = ::write(m_socket, src, num_bytes);
                if (written <= 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) { 
                      terminate();
                      return -1;
                    }
                    written = 0;
                }
                return written;
#else
                return -1;
#endif
  }
  
  int LocalClientConnection::recv(uint num_bytes, char* dest, uint non_blocking_ahead) 
  {
           return socket_recv(m_socket, get_connstate_listener(), num_bytes, dest, non_blocking_ahead);
  }


  void LocalClientConnection::terminate() 
  { 
           Sid::terminate(m_socket, get_connstate_listener());
  }

  LocalClientConnection::~LocalClientConnection()
  { 
         terminate();
  }

  void LocalClientConnection::set_non_blocking() {
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

  void LocalClientConnection::wait_input() {
    Sid::wait_input(m_socket);
  }
}

