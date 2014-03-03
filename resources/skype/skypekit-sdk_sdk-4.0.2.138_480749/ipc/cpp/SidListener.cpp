#include "SidListener.hpp"
#include "SidServerSession.hpp"
#include "stdio.h"

namespace Sid {

    ServerCommandInterface::~ServerCommandInterface()
    {
    }

    Listener::Listener(int max_parallel_connection, int exit_after_connection)
    : m_session(0),
      m_socket(-1),
      m_num_connection(0),
      m_max_parallel_connection(max_parallel_connection),
      m_exit_after_connection(exit_after_connection)
    {
    }

    void Listener::add_connection(int clientfd) {
       if (clientfd >= 0) {
         ServerCommandInterface* server = create_server(clientfd, m_exit_after_connection ? 0 : this);
         if (server && server->get_connid() != -1 && m_session->add_connection(server)) {
            if (++m_num_connection < m_max_parallel_connection && !m_exit_after_connection) {
              m_session->accept_from(this);
            }
         } else {
           fprintf(stderr, "ERR: can not create server %p id(%d) \n", server, (server ? server->get_connid() : 0));
           delete server;
           if (!m_exit_after_connection)
             m_session->accept_from(this); 
         }
       } else {
           fprintf(stderr, "ERR: connection not accepted\n");
           if (!m_exit_after_connection)
             m_session->accept_from(this); 
       }
       if (m_exit_after_connection) m_session->del_listener(this);
    }

    void Listener::del_connection() {
      if (m_num_connection-- == m_max_parallel_connection)
        m_session->accept_from(this);
    }

    int Listener::get_connid() const { return m_socket; }

    bool Listener::start_listening(ServerSession* session)  
    { 
      m_session = session;
      return m_socket != -1; 
    }
} // Sid


