#include "SidServerSession.hpp"
#include <stdio.h>

namespace Sid {

    ServerSession::TcpServerConnection::TcpServerConnection(Listener* listener, ServerSession* session, int socket, const char* streamlog)
    : Sid::TcpServerConnection(socket, streamlog)
    {
      m_session     = session;
      m_listener    = listener;
      m_num_threads = 0;
      m_protocol    = 0;
    }
    
    ServerSession::TcpServerConnection::~TcpServerConnection()
    {
      terminate();
      delete m_protocol;
    }

    void ServerSession::TcpServerConnection::attach_thread() {
      m_num_threads++;
    }

    void ServerSession::TcpServerConnection::detach_thread() {
      m_num_threads--;
      if (!is_connected() && m_num_threads == 0)
        m_session->del_connection(this, m_listener);
    }

    TransportInterface::Status ServerSession::TcpServerConnection::end_decoding(CommandInitiator* thread)
    {
      if (has_more()) {
        m_session->add_ready(this);
        thread->fork();
      } else {
        m_session->add_waiting(this);
      }
      return OK;
    }

    ServerSession::LocalServerConnection::LocalServerConnection(Listener* listener, ServerSession* session, int socket, const char* streamlog)
    : Sid::LocalServerConnection(socket, streamlog)
    {
      m_session     = session;
      m_listener    = listener;
      m_num_threads = 0;
      m_protocol    = 0;
    }
   
    void ServerSession::LocalServerConnection::attach_thread() {
      m_num_threads++;
    }

    void ServerSession::LocalServerConnection::detach_thread() {
      m_num_threads--;
      if (!is_connected() && m_num_threads == 0) // can be deleted
        m_session->del_connection(this, m_listener);
    }

    TransportInterface::Status ServerSession::LocalServerConnection::end_decoding(CommandInitiator* thread)
    {
      if (has_more()) {
        m_session->add_ready(this);
        thread->fork();
      } else {
        m_session->add_waiting(this);
      }
      return OK;
    }

    ServerSession::LocalServerConnection::~LocalServerConnection()
    {
      terminate();
      delete m_protocol;
    }

    ServerSession::ServerSession(int num_threads, int max_threads_per_connection, bool use_main_thread)
    : Session(use_main_thread ? num_threads - 1: num_threads, max_threads_per_connection, Queue::CAPACITY),
      m_num_connection(0),
      m_selecting(false),
      m_use_main(use_main_thread)
    {
    }

    ServerSession::~ServerSession()
    {
    }

    bool ServerSession::init(Listener* listener)
    {
      if (listener->is_connected()) {
        listener->start_listening(this);
        m_num_connection++;
        m_waiting_queue.push(listener);
        return true;
      }
      return false;
    }

//    bool add_listener(Listener*);
    void ServerSession::del_listener(Listener* )
    {
      Mutex::ScopedLock lock(m_mutex);
      m_num_connection--;
    }

    void ServerSession::accept_from(Listener* listener)
    {
      Mutex::ScopedLock lock(m_mutex);
      m_waiting_queue.push(listener);
    }

    bool ServerSession::add_connection(ServerCommandInterface* connection)
    {
      Mutex::ScopedLock lock(m_mutex);
      if (m_num_connection == Queue::CAPACITY) return false;
      m_waiting_queue.push(connection);
      m_num_connection++;
      return true;
    }

    void ServerSession::add_waiting(ServerCommandInterface* connection)
    {
      Mutex::ScopedLock lock(m_mutex);
      m_waiting_queue.push(connection);
    }

    void ServerSession::add_ready(ServerCommandInterface* connection)
    {
      Mutex::ScopedLock lock(m_mutex);
      m_ready_queue.push(connection);
    }

    void ServerSession::del_connection(ServerCommandInterface* connection, Listener* listener)
    {
      Mutex::ScopedLock lock(m_mutex);
      m_num_connection--;
      if (listener)
        listener->del_connection();
      delete connection;
    }
  
    bool ServerSession::has_connection()
    {
      Thread::Sleep(500);
      Mutex::ScopedLock lock(m_mutex);
      return m_num_connection > 0;
    }

    void ServerSession::start()
    { 
      if (m_use_main) {
        CommandInitiator main_thread(this, this);
        do {
          process_command(&main_thread);
        } while (has_connection());
      } else {
        allocate(this);
      }
    }

    void ServerSession::stop ()
    {
      Mutex::ScopedLock lock(m_mutex);
      m_stopped = true;
      Session::stop();
    }

    bool ServerSession::poll_waiting()
    {
      Mutex::ScopedLock l(m_mutex);
      if (m_selecting) return false;
      m_selecting = true;
      select(); // select is implemented in SidServerConnection<>.cpp
      m_selecting = false;
      return !m_ready_queue.is_empty();
    }

    void ServerSession::process_command(CommandInitiator* thread) {
      ServerCommandInterface* command;
      do {
        while ((command = pop()) != 0) {
          thread->fork(); // if another thread is available, trigger it... 
          command->process_command(thread); 
        }
      } while (poll_waiting());
      // go back to pool if someone else is selecting or noone is waiting
    }

    inline ServerCommandInterface* ServerSession::pop() 
    {
      Mutex::ScopedLock l(m_mutex);
      return m_ready_queue.pop();
    }

    bool ServerSession::reuse_command_processor(CommandProcessor* ) {
      return !m_use_main;
    }

    ServerSession::Queue::Queue() 
    : m_num_queued(0),
      m_rd_ptr(0),
      m_wr_ptr(0)
    { 
    }

    bool ServerSession::Queue::is_empty() const {
      return m_num_queued == 0;
    }

    ServerCommandInterface* ServerSession::Queue::pop() { 
      if (m_num_queued == 0) return 0;
      m_num_queued--;
      ServerCommandInterface* q = m_queued[m_rd_ptr];
      m_rd_ptr = (m_rd_ptr + 1) % CAPACITY;
      return q;
    }

    void ServerSession::Queue::push(ServerCommandInterface* q) { 
      m_num_queued++;
      m_queued[m_wr_ptr] = q;
      m_wr_ptr = (m_wr_ptr + 1) % CAPACITY;
    }

} // Sid


