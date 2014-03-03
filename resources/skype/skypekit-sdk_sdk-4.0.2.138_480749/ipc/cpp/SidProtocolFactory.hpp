#ifndef Factories_HPP_INCLUDED
#define Factories_HPP_INCLUDED 

#include "SidPlatform.hpp"
#include "SidProtocolServerInterface.hpp"

namespace Sid {

  struct TransportInterface;
  class  Api;

  class ProtocolFactory {
  public:
    static Protocol::ServerInterface* create(const String& protocol, const String& api, TransportInterface* transport, const String& filter);
    ProtocolFactory(const String& name, Protocol::ServerInterface* (*constructor)(Api*,TransportInterface*,Field*));
  private:
    Protocol::ServerInterface* create_protocol(const String& protocol_name, const String& api, TransportInterface* transport, const String& filter);
    ProtocolFactory* m_next;
    Protocol::ServerInterface* (*m_constructor)(Api*,TransportInterface*,Field*);
    String m_name;
    static ProtocolFactory* M_protocol_factory;
  };

  template<class ProtocolImpl> class ProtocolRegistration : protected ProtocolFactory {
  public:
    ProtocolRegistration(const String& name) 
    : ProtocolFactory(name, &constructor)
    { }
  private:
    static Protocol::ServerInterface* constructor(Api* api, TransportInterface* transport, Field* descriptors)
    { return new ProtocolImpl(api, transport, descriptors); }
  };
}

#endif
