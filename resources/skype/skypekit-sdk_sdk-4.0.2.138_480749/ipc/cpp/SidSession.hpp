#ifndef Sid_Session_HPP_INCLUDED
#define Sid_Session_HPP_INCLUDED

#include "SidCommandProcessor.hpp"
#include "SidConnection.hpp"

namespace Sid {

	// commandprocessor  <- session -> connection <-  protocol  <-    server
	// 	   |                                 |____create________________^ ^
	// 	   |__________________call________________________________________|

	// commandprocessor  <- session = client -> protocol -> connection 
	// 	   |                           ^  
	// 	   |______call_________________|

  class Session {
  public:
    typedef Sid::CommandInterface CommandInterface;
    typedef Sid::CommandInitiator CommandInitiator;
    typedef Sid::CommandProcessor CommandProcessor;
    typedef Sid::Connection       Connection;
    Session(int max_threads, int max_threads_per_connection, int max_connections);
    virtual ~Session();
    CommandProcessor*  allocate(CommandInterface* command); 
    virtual void       notify_no_more_command_processor();
    bool               deallocate(CommandProcessor* command_processor); 
    virtual bool       reuse_command_processor(CommandProcessor* command_processor);
    virtual void       stop();
    bool is_stopped() const { return m_stopped; }
    void exit_thread(CommandProcessor* command_processor);
  protected:
    CommandProcessor*  m_thread_pool_begin;
    int                m_num_threads;
    int                m_num_running;
  protected:
    int                m_max_threads;
    int                m_max_threads_per_connection;
  protected:
    Mutex              m_thread_pool_lock;
//    int                m_num_connections;
//    int                m_max_connections;
//    int                m_connection_id;
//    Connection**       m_waiting_connections;
//    Connection**       m_running_connections;
    Event              m_joined;
    bool               m_stopped;
    SKYPEKIT_DISABLE_COPY(Session);
  };

} // namespace Sid

#endif

