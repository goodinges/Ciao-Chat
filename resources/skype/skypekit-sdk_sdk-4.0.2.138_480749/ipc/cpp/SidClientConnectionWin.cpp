#include "SidClientConnection.hpp"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <winsock2.h>

#pragma warning( disable : 4389 4127 )

namespace Sid {

  TcpClientConnection::TcpClientConnection(const char* server_host, int server_port, const char* log_path, int retries, int retry_delay, ConnectionStateListener* listener)
  : Connection(log_path)
  {
        set_connstate_listener(listener);
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(1, 1);
        int err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
//                SID_DBGPRINTF("SKYPE ERROR - TCPClientConnection(), couldn't startup windows sockets!\n");
                return;
        }
        sockaddr_in addr;
        m_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (-1 != m_socket) {
                memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = inet_addr(server_host);
                addr.sin_port = htons((u_short)server_port);
                int failed;
                while ((failed = ::connect(m_socket, (struct sockaddr*)&addr, sizeof(addr))) < 0 && retries-- > 0) {
                        Thread::sleep(retry_delay*1000);
                        if (get_connstate_listener()) get_connstate_listener()->Connecting();
                }
                if (failed < 0) 
                        m_socket = -1;
                else {
                        //int timeo = 1000; // ms
//                        setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(timeo));
                        if (get_connstate_listener()) get_connstate_listener()->Connected();        
                } 
        }
  }

  TcpClientConnection::~TcpClientConnection() 
  {
                terminate();
  }

  void TcpClientConnection::set_non_blocking() {
        u_long iMode = 1;
        int iResult = ioctlsocket(m_socket, FIONBIO, &iMode);
        if (iResult != NO_ERROR) terminate();
  }

  void TcpClientConnection::wait_input()
  {
     int socket = m_socket;
     if (socket != -1) { // connection might have been terminated when writting...
       fd_set socks;
       FD_ZERO(&socks);
       FD_SET(socket,&socks);
       struct timeval timeout;
       timeout.tv_sec  = 1;
       timeout.tv_usec = 0;
       ::select(socket+1, &socks, (fd_set *) 0,  (fd_set *) 0, &timeout);
    }
  }

  int TcpClientConnection::recv(uint num_bytes, char* dest, uint )
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

  int TcpClientConnection::send(uint num_bytes, const char* src, bool immediate_hint)
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
  void TcpClientConnection::terminate()
  {
        Mutex::ScopedLock l(m);
        if (m_socket == -1) return;
        closesocket(m_socket);
        m_socket = -1;
        if (get_connstate_listener()) get_connstate_listener()->Disconnected();
  }
  
  LocalClientConnection::LocalClientConnection(const char* /*socket_name*/, const char* log_path, int /*retries*/, int /*retry_delay*/, ConnectionStateListener* /*listener*/)
  : Connection(log_path)
  {
    // Not implemented: shall be windows pipe...
  }

  LocalClientConnection::~LocalClientConnection() 
  {
                terminate();
  }

  int LocalClientConnection::recv(uint , char* , uint)
  {
                return -1;
  }

  int LocalClientConnection::send(uint, const char*, bool)
  {
                return -1;
  }

  void LocalClientConnection::terminate()
  {
  }

  void LocalClientConnection::wait_input()
  {
  }

  void LocalClientConnection::set_non_blocking()
  {
  }

}


