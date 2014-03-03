#ifndef SID_SidServerConnectionWin_HPP_INCLUDED
#define SID_SidServerConnectionWin_HPP_INCLUDED

#include "SidListener.hpp"
#include "SidConnection.hpp"

namespace Sid {

  class TcpListener : public Listener {
  public:
    TcpListener(int port, int max_parallel_connection, int exit_after_connection, int local_only);
    ~TcpListener();
    void process_command(CommandInitiator*);
    int m_local_only;
    SKYPEKIT_DISABLE_COPY(TcpListener);
  };

  class LocalListener : public Listener {
  public:
    LocalListener(const char* local, int max_parallel_connection, int exit_after_connection);
    ~LocalListener();
    void process_command(CommandInitiator*);
    char* m_local_name;
    SKYPEKIT_DISABLE_COPY(LocalListener);
  };

  class TcpServerConnection : public Connection, public ServerCommandInterface {
  public:
    TcpServerConnection(int socket, const char* log);
    ~TcpServerConnection();
    virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
    virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
    virtual void terminate();
    int get_connid() const;
    SKYPEKIT_DISABLE_COPY(TcpServerConnection);
  };
  
  class LocalServerConnection : public Connection, public ServerCommandInterface {
  public:
    LocalServerConnection(int foo, const char* log);
    ~LocalServerConnection();
    virtual void terminate();
    int get_connid() const;
  protected:
    virtual int  recv(uint num_bytes, char* dest, uint non_blocking_ahead);
    virtual int  send(uint num_bytes, const char* src, bool);
    SKYPEKIT_DISABLE_COPY(LocalServerConnection);
  };
  

} // namespace Sid
 
#endif


