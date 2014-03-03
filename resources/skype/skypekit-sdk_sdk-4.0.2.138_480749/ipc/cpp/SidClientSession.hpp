#ifndef Sid_ClientSession_HPP_INCLUDED
#define Sid_ClientSession_HPP_INCLUDED

#include "SidSession.hpp"
#include "SidClientConnection.hpp"
#include "SidProtocolBinClient.hpp"
#include "SidTLSEncryption.hpp"

namespace Sid {

class ClientSession : public Session, public CommandInterface,  public ConnectionStateListener {
       
public:
        typedef Sid::TcpClientConnection   TcpClientConnection;
        typedef Sid::LocalClientConnection LocalClientConnection;

        class TLSTcpClientConnection : public TcpClientConnection, protected TLSEncryptionSocketServer {
        public:
                TLSTcpClientConnection(const char* certificate_buf, const char* host, int port, const char* log_path = 0, int retries = 0, int retry_delay = 1, ConnectionStateListener* listener = 0)
                        : TcpClientConnection(host, port, log_path, retries, retry_delay, listener) {
                           set_non_blocking();
                           server.init(certificate_buf, m_socket, listener);
                        }                         
                virtual int recv(uint num_bytes, char* dest, uint /*non_blocking_read_ahead*/){
                        int rd;
                        while ((rd = server.read(num_bytes, dest)) == 0) wait_input();
                        return rd;
                }
                virtual int send(uint num_bytes, const char* src, bool){
                        return server.write(num_bytes, src);
                }
                TLSEncryptionSocketServer server;
                SKYPEKIT_DISABLE_COPY(TLSTcpClientConnection);
        };        
             
        class TLSLocalClientConnection : public LocalClientConnection, protected TLSEncryptionSocketServer {
        public:
                TLSLocalClientConnection(const char* certificate_buf, const char* name, const char* log_path = 0, int retries = 0, int retry_delay = 1, ConnectionStateListener* listener = 0)
                        : LocalClientConnection(name, log_path, retries, retry_delay, listener) {
                           set_non_blocking();
                           server.init(certificate_buf, m_socket, listener);
                        }                         
                virtual int recv(uint num_bytes, char* dest, uint /*non_blocking_read_ahead*/){
                        int rd;
                        while ((rd = server.read(num_bytes, dest)) == 0) wait_input();
                        return rd;
                }
                virtual int send(uint num_bytes, const char* src, bool){
                        return server.write(num_bytes, src);
                }
                TLSEncryptionSocketServer server;
                SKYPEKIT_DISABLE_COPY(TLSLocalClientConnection);
        }; 

        ClientSession(int num_threads, Field* descriptors);
        ~ClientSession();
                
        LocalClientConnection* create_local_connection(const char* certificate_buf, const char* local, const char* streamlog, int retries, int retry_delay);         
        TcpClientConnection* create_tcp_connection(const char* certificate_buf, const char* server, int port, const char* streamlog, int retries, int retry_delay);
                
        TransportInterface::Status init_with_setup(const char* setup, const char* certificate_buf, const char* local, const char* streamlog = 0, int retries = 10, int retry_delay = 5);
        TransportInterface::Status init_with_setup(const char* setup, const char* certificate_buf, const char* server, int port, const char* streamlog = 0, int retries = 10, int retry_delay = 5);        
        TransportInterface::Status init(const char* certificate_buf, const char* local, const char* streamlog = 0, int retries = 10, int retry_delay = 5);
        TransportInterface::Status init(const char* certificate_buf, const char* server, int port, const char* streamlog = 0, int retries = 10, int retry_delay = 5);        
        TransportInterface::Status init(Connection*, const char* setup_request = "");        
        
        void start();
        void stop();
        
        virtual Protocol::BinClient* create_protocol(Connection* connection);
        
        //ConnectionStateListener callbacks       
        virtual void Disconnected() { }    
        virtual void Connected() { } 
        virtual void Connecting() { }
        virtual void OnError(CONNECTION_ERROR /*err*/) { }
        
        void set_transport_encryption(bool enabled) {
                m_secure_connection = enabled;        
        } 
        
protected:
        Field* get_descriptors() { return m_fields; }
        TransportInterface::Status handshake(Connection*, const char* request, const char* expected_response);

        Connection*          m_connection;
        Protocol::BinClient* m_protocol;
        int m_started;
        bool m_secure_connection; 
        Field*               m_fields;        
protected:        
        bool is_server_connected() { return m_connection ? m_connection->is_connected() : false; }
        SKYPEKIT_DISABLE_COPY(ClientSession);
};

} // Sid

#endif

