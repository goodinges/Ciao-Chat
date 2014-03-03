#include "SidSession.hpp"
#include "SidTransportInterface.hpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

namespace Sid {

  CommandInitiator::CommandInitiator(Session* session, CommandInterface* command)
  : m_session(session),
    m_command(command),
    m_resumed(1)
  {
//    SID_DBGPRINTF("cmd init created %p\n", this);
  }

  CommandInitiator::~CommandInitiator()
  {
//    SID_DBGPRINTF("cmd init deleted %p\n", this);
  }


  int CommandInitiator::yield()
  {
    if (m_session->is_stopped()) return 0;
    m_sync_event.wait();
    return m_resumed;
  }

  void CommandInitiator::resume(int resumed)
  {
    m_resumed = resumed;
    m_sync_event.notify();
  }

  void CommandInitiator::fork() {
    m_session->allocate(m_command);
  }

  const char* CommandInitiator::get_name() { return "unnamed"; }

  CommandProcessor::CommandProcessor(Session* session, CommandInterface* command)
  : CommandInitiator(session, command),
    m_next(0)
  { 
  }

  void CommandProcessor::Run() {
    int resumed = 1;
//    SID_DBGPRINTF("%p starting %p\n",this,m_session);
    while (resumed > 0) {
      do {
        m_command->process_command(this);
      } while (!m_session->deallocate(this));
      resumed = yield(); // back to pool for another connection
    }
//    SID_DBGPRINTF("%p exiting %p\n",this,m_session);
    m_session->exit_thread(this);
  }

  void CommandProcessor::set_command(CommandInterface* command) {
    m_command = command;
    resume(command == 0 ? 0 : 1);
  }

} // namespace Sid


