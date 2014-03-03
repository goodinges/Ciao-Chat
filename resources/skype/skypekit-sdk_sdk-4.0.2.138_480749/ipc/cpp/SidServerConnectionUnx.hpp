#ifndef SID_SidServerConnectionUnx_HPP_INCLUDED
#define SID_SidServerConnectionUnx_HPP_INCLUDED

#include "SidListener.hpp"
#include "SidConnection.hpp"

namespace Sid {

  class TcpListener : public Listener {
  public:
    TcpListener(int port, int max_parallel_connection, int exit_after_connection, int local_only);
    ~TcpListener();
    void process_command(CommandInitiator*);
    int m_local_only;
  };

  class LocalListener : public Listener {
  public:
    LocalListener(const char* local, int max_parallel_connection, int exit_after_connection);
    ~LocalListener();
    void process_command(CommandInitiator*);
    char* m_local_name;
  };  

  class TcpServerConnection : public Connection, public ServerCommandInterface {
  public:
    TcpServerConnection(int socket, const char* log);
    ~TcpServerConnection();
    virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
    virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
    virtual void terminate();
    int get_connid() const;
  };
  
  class LocalServerConnection : public Connection, public ServerCommandInterface {
  public:
    LocalServerConnection(int socket, const char* log);
    ~LocalServerConnection();
    virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
    virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
    virtual void terminate();
    int get_connid() const;
  };

} // namespace Sid
 
#endif

