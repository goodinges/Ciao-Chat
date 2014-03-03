#include "SidSession.hpp"
#include <stdio.h>

namespace Sid {

  Session::Session(int max_threads, int max_threads_per_connection, int /*max_connections*/)
  : m_thread_pool_begin(0),
    m_num_threads(0),
    m_num_running(0),
    m_max_threads(max_threads),
    m_max_threads_per_connection(max_threads_per_connection),
    m_stopped(false)
  {
  }

  void Session::stop()
  {
    m_thread_pool_lock.lock();
    m_stopped = true;
    for (CommandProcessor* pooled = m_thread_pool_begin, *next = 0; pooled != 0; pooled = next) {
      next = pooled->m_next;
      pooled->m_next = 0;
      pooled->resume(0);
    }
    m_thread_pool_begin = 0;
    m_thread_pool_lock.unlock();
    if (m_num_threads) m_joined.wait();
  }

  Session::~Session()
  {
  }

  CommandProcessor* Session::allocate(CommandInterface* command) 
  { 
    CommandProcessor* allocated = 0;
    int    create               = 0;
    m_thread_pool_lock.lock();
    allocated = m_thread_pool_begin;
    if (allocated == 0) {
      if (m_num_threads < m_max_threads) {
        m_num_threads++;
        m_num_running++;
        create = 1;
      } else {
        notify_no_more_command_processor();
      }
    } else {
      m_num_running++;
      m_thread_pool_begin = allocated->m_next;
      allocated->m_next= 0;
    }
    m_thread_pool_lock.unlock();
    if (allocated) {
      allocated->set_command(command);
    } else if (create) {
       allocated = new CommandProcessor(this, command);
       char* name = new char[sizeof("CmdProcessor%2d")];
       sprintf(name,"CmdProcessor%2d",m_num_threads);
       allocated->start(name); // is it immediate? because would prefer it isn't...
       delete[] name;
    }
//    SID_DBGPRINTF("allocate %d %p%s\n", m_num_threads, allocated,allocated ? "":"=failed");
    return allocated;
  }
  
  void Session::notify_no_more_command_processor() 
  { 
  }

  bool Session::deallocate(CommandProcessor* thread)
  { 
    bool signal;
    m_thread_pool_lock.lock();
    signal = m_num_running == m_max_threads && !m_stopped;
    if (signal&&reuse_command_processor(thread)) { 
      m_thread_pool_lock.unlock();
      return false;
    }
//    SID_DBGPRINTF("deallocate (%d) resumed(%d)\n", m_num_running,thread->m_resumed);
    m_num_running--;
    if (m_stopped) {
//      SID_DBGPRINTF("Session::deallocate %d %d\n", m_num_running, m_num_threads);
      thread->m_resumed = 0;
    } else {
      thread->m_next      = m_thread_pool_begin;
      m_thread_pool_begin = thread;
    }
    m_thread_pool_lock.unlock();
    return true;
  }

  void Session::exit_thread(CommandProcessor* thread) {
    m_thread_pool_lock.lock();
    thread->m_next      = m_thread_pool_begin;
    m_thread_pool_begin = thread;
    if (--m_num_threads == 0) m_joined.notify();
    m_thread_pool_lock.unlock();
  }
  
  bool Session::reuse_command_processor(CommandProcessor*)
  { 
    return false; 
  }

} // namespace Sid

