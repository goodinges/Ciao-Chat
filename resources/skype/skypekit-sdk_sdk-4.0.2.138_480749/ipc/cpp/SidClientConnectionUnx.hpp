#ifndef _Sid_client_transport_HPP_INCLUDED_
#define _Sid_client_transport_HPP_INCLUDED_

#include "SidConnection.hpp"

namespace Sid {
        class ConnectionStateListener;

        class TcpClientConnection : public Connection  {
        public:
                TcpClientConnection(const char* host, int port, 
                        const char* log_path = 0, int retries = 0, int retry_delay = 1, ConnectionStateListener* listener = 0);
                ~TcpClientConnection();
                virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
                virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
                virtual void terminate();
                void set_non_blocking();
                void wait_input();
                SKYPEKIT_DISABLE_COPY(TcpClientConnection);
        };

        class LocalClientConnection : public Connection {
        public:
                LocalClientConnection(const char* name, const char* log_path = 0, 
                        int retries = 0, int retry_delay = 1, ConnectionStateListener* listener = 0);
                ~LocalClientConnection();
                virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
                virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
                virtual void terminate();
                void set_non_blocking();
                void wait_input();
                SKYPEKIT_DISABLE_COPY(LocalClientConnection);
        };                
}

#endif

