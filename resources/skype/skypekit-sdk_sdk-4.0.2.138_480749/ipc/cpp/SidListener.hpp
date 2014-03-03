#ifndef Sid_SidListener_HPP_INCLUDED
#define Sid_SidListener_HPP_INCLUDED

#include "SidSession.hpp"

namespace Sid {

  class ServerSession;

  struct ServerCommandInterface : CommandInterface {
    virtual int get_connid() const = 0;
    virtual ~ServerCommandInterface();
  };

  class Listener : public ServerCommandInterface {
  public:
    Listener(int max_parallel_connection, int exit_after_connection);
    void add_connection(int clientfd);
    void del_connection();
    virtual ServerCommandInterface* create_server(int socket, Listener*) = 0; 
    bool is_connected() const { return m_socket != -1; }
    int  get_connid() const;
    bool start_listening(ServerSession* session);
  protected:
    ServerSession* m_session;
    int            m_socket;
    int            m_num_connection;
    int            m_max_parallel_connection;
    bool           m_exit_after_connection;
  };

} // Sid

#endif

