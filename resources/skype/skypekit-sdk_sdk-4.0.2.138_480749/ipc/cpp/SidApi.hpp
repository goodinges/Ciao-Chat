#ifndef Sid_Api_HPP_INCLUDED
#define Sid_Api_HPP_INCLUDED

#include "SidPlatform.hpp"
#include "SidProtocolServerInterface.hpp"
#include "SidSession.hpp"

namespace Sid {

  class  CommandProcessor;
  struct Field;

  typedef Protocol::Status (*ActionPtr)(CommandInitiator*, Protocol::ServerInterface*, Protocol::Rid, void* server);  
  typedef struct { ActionPtr* module_api; uint mod_api_size; Field** properties; uint num_properties; } ModuleAPI;

  class Api {
  public:
    Protocol::Status operator()(CommandInitiator* thread, uint mod_id, uint act_id, Protocol::ServerInterface* p, Protocol::Rid rid, void* server)
    { if (mod_id <= m_num_modules && m_api[mod_id].module_api != 0 && act_id <= m_api[mod_id].mod_api_size && m_api[mod_id].module_api[act_id] != 0)
        return (*m_api[mod_id].module_api[act_id])(thread, p, rid, server);
      return Protocol::ERR_CALL;
    }
    Field* get_property_desc(uint modid, uint propid, uint from_event = 0);
    Field* get_descriptors() { return m_descriptors; }
  protected:
    Api(ModuleAPI* api, uint num_modules,Field* descriptors);
  private:
    Field* __get_property_desc(uint modid, uint propid) const;
    Api* clone(const char* filter) const;
    friend class ApiRegistration;
    ModuleAPI*    m_api;
    uint          m_last_prop;
    uint          m_num_modules;
    ModProp2Field m_modprop2field;
    Field*        m_descriptors;
  };

  class ApiRegistration {
  public:
    static Api* find(const String& api, const char* filter = 0);
    ApiRegistration(const String& name, ModuleAPI* api, uint num_modules, Field* descriptors);
  private:
    Api* find_api(const String& api, const char* filter);
    ApiRegistration*        m_next;
    Api                     m_api;
    String                  m_name;
    static ApiRegistration* M_api_factory;
  };

}

#endif

