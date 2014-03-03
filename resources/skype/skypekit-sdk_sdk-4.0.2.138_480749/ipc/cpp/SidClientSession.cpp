#include "SidClientSession.hpp"
#include <string.h>

namespace Sid {

  inline TransportInterface::Status ClientSession::handshake(Connection* connection, const char* request, const char* expected_response)
  {
    TransportInterface::Status status;
    int   rspsz           = strlen(expected_response);
    int   reqsz           = request ? strlen(request) : 0;             
    char* actual_response = new char[rspsz+1];
    char size[9];
    sprintf(size, "%08x", reqsz); // not binary protocol yet: ie encode size of message in hex + space + msg
    
    if (   (status = connection->bl_start_encoding(0)) != TransportInterface::OK
        || (status = connection->bl_write_bytes(0,8, size)) != TransportInterface::OK
        || (reqsz && (status = connection->bl_write_bytes(0,reqsz, request)) != TransportInterface::OK)      
        || (status = connection->end_encoding(0, true)) != TransportInterface::OK
        || (status = connection->bl_read_bytes(0,rspsz, actual_response)) != TransportInterface::OK 
        || ((actual_response[rspsz] = 0) != 0)
        || strcmp(actual_response, expected_response) != 0) {
      status = TransportInterface::ERR_CONNECTION; // or status = *((Status*) actual_response);
//       if (status == ERR_CONNECTION) printf("connection handshake status %s\n", actual_response); 
      connection->terminate();
    }
    delete[] actual_response;
    return status;
  }

  ClientSession::ClientSession(int num_threads, Field* descriptors)
  : Session(num_threads, num_threads, 1),
    m_connection (0),
    m_protocol   (0),
    m_started    (0),
    m_secure_connection (true), //currently on by default
    m_fields(descriptors)
  {
  }

  Protocol::BinClient* ClientSession::create_protocol(Connection* connection) {
    return new Protocol::BinClient(connection, m_fields);
  }

  LocalClientConnection* ClientSession::create_local_connection(const char* certificate_buf, const char* local, const char* streamlog, int retries, int retry_delay)
  {
     LocalClientConnection* connection = 0;
     if (m_secure_connection) {
        connection = new TLSLocalClientConnection(certificate_buf, local, streamlog, retries, retry_delay, this);
     } else {
        connection = new LocalClientConnection(local, streamlog, retries, retry_delay, this);
     } 
     return connection;
  }
        
  TcpClientConnection* ClientSession::create_tcp_connection(const char* certificate_buf, const char* server, int port, const char* streamlog, int retries, int retry_delay)
  {  
     TcpClientConnection* connection = 0;   
     if (m_secure_connection) {
        connection = new TLSTcpClientConnection(certificate_buf, server, port, streamlog, retries, retry_delay, this);
     } else {      
        connection = new TcpClientConnection(server, port, streamlog, retries, retry_delay, this);
     }
     return connection;      
  }

  TransportInterface::Status ClientSession::init_with_setup(const char* setup, const char* certificate_buf, const char* server, int port, const char* streamlog, int retries, int retry_delay)
  {
     TcpClientConnection* connection = create_tcp_connection(certificate_buf, server, port, streamlog, retries, retry_delay);   
     char* req = const_cast<char*>(setup);
     if (!m_secure_connection) {
       req = new char[strlen(setup)+strlen(certificate_buf)+1];
       strcpy(req, certificate_buf);
       strcat(req, setup);
     }
     TransportInterface::Status status = init(connection, req);
     if (req != setup) delete[] req;
     return status;
  }

  TransportInterface::Status ClientSession::init_with_setup(const char* setup, const char* certificate_buf, const char* local, const char* streamlog, int retries, int retry_delay)
  {
     LocalClientConnection* connection = create_local_connection(certificate_buf, local, streamlog, retries, retry_delay);
     char* req = const_cast<char*>(setup);
     if (!m_secure_connection) {
       req = new char[strlen(setup)+strlen(certificate_buf)+1];
       strcpy(req, certificate_buf);
       strcat(req, setup);
     }
     TransportInterface::Status status = init(connection, req);
     if (req != setup) delete[] req;
     return status;
  }

  TransportInterface::Status ClientSession::init(const char* certificate_buf, const char* server, int port, const char* streamlog, int retries, int retry_delay)
  {
     TcpClientConnection* connection = create_tcp_connection(certificate_buf, server, port, streamlog, retries, retry_delay);
     return init(connection, m_secure_connection ? "" : certificate_buf);
  }

  TransportInterface::Status ClientSession::init(const char* certificate_buf, const char* local, const char* streamlog, int retries, int retry_delay)
  {
     LocalClientConnection* connection = create_local_connection(certificate_buf, local, streamlog, retries, retry_delay);
     return init(connection, m_secure_connection ? "" : certificate_buf);
  }

  TransportInterface::Status ClientSession::init(Connection* connection, const char* setup_request)
  {     
     m_connection = connection;
     m_protocol   = create_protocol(connection);
     return handshake(connection, setup_request, "OK");
  }

  void ClientSession::start()
  {
    if (m_connection && m_connection->is_connected() && !m_started) {
       allocate(this); 
       m_started = 1;
    }
  }

  void ClientSession::stop() {
    if (m_connection) m_connection->terminate();
    Session::stop();
  }

  ClientSession::~ClientSession()
  {
    if (m_connection) delete m_connection;
    delete m_protocol;
  }

} // Sid

