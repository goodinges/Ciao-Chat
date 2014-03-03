#include "SidProtocolFactory.hpp"
#include "SidApi.hpp"

namespace Sid {

  Protocol::ServerInterface* ProtocolFactory::create(const String& protocol, const String& api, TransportInterface* transport, const String& filter)
  { if (M_protocol_factory) return M_protocol_factory->create_protocol(protocol, api, transport, filter);
    return 0;
  }

  ProtocolFactory::ProtocolFactory(const String& name, Protocol::ServerInterface* (*constructor)(Api*,TransportInterface*,Field*))
  : m_next(M_protocol_factory),
    m_constructor(constructor),
    m_name(name)
  { M_protocol_factory = this;
    // check for duplicate? 
  }

  Protocol::ServerInterface* ProtocolFactory::create_protocol(const String& protocol_name, const String& api_name, TransportInterface* transport, const String& filter) 
  { if (protocol_name == m_name) {
      Api* api = ApiRegistration::find(api_name, filter);
      return api ? (*m_constructor)(api, transport,api->get_descriptors()) : 0;
    }
    if (m_next) return m_next->create_protocol(protocol_name, api_name, transport, filter);
    return 0;
  }

  ProtocolFactory* ProtocolFactory::M_protocol_factory;

//  Protocol::ServerInterface* Protocol::ServerInterface::factory(const String& protocol_name, const String& api_name, TransportInterface* transport)
//  { 
//    return ProtocolFactory::create(protocol_name, api_name, transport);
//  } 

}
