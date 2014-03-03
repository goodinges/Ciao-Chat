#ifndef Sid_CommandProcessor_HPP_INCLUDED
#define Sid_CommandProcessor_HPP_INCLUDED

#include "SidCommonConfig.hpp"
#include "SidPlatform.hpp"

namespace Sid {

  class Session;
  class CommandInitiator;

  struct CommandInterface {
    CommandInterface() {}
    virtual void process_command(CommandInitiator*) = 0;
    virtual ~CommandInterface() {}
    SKYPEKIT_DISABLE_COPY(CommandInterface);
  };

  class CommandInitiator {
  public:
    CommandInitiator(Session* session, CommandInterface* command);
    virtual ~CommandInitiator();
    virtual const char* get_name();
    int  yield();
    void resume(int normal_resume=1);
    void fork();
    int  join() { return yield(); }
    CommandInterface* get_command() const { return m_command; }
  protected:
    Session*          m_session;
    CommandInterface* m_command;
    int               m_resumed;
    Event             m_sync_event;
    SKYPEKIT_DISABLE_COPY(CommandInitiator);
  };
  
  class CommandProcessor : public CommandInitiator, public Sid::Thread {
  public:
    CommandProcessor(Session* session, CommandInterface* command);
    void set_command(CommandInterface* command);
    const char* get_name() { return Thread::get_name(); }
  protected:
    void Run();
    friend class Session;
    CommandProcessor* m_next;
    SKYPEKIT_DISABLE_COPY(CommandProcessor);
  }; 

  // implemented in SidSession.cpp

} // namespace Sid

#endif

