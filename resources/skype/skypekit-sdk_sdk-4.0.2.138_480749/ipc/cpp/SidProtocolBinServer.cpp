#include "SidProtocolBinServer.hpp"
#include "SidProtocolFactory.hpp"
#include "SidApi.hpp"

namespace Sid { namespace Protocol {
  static ProtocolRegistration<BinServer> skylib_bin_protocol("bin");

  BinServer::BinServer(Api* api, TransportInterface* transport, Field* descriptors)
  : BinCommon(api, transport, descriptors),
    m_assign_response_started(0),
    m_num_get_property(0),
    m_num_property_change(0),
    m_rg_state(RG_COMPLETED)
  {
  }

  BinServer::~BinServer()
  {
    delete m_api;
  }

  Api* BinServer::get_api()
  {
    return BinCommon::get_api();
  }

  Status BinServer::resync_decode(CommandInitiator* thread)
  {
    m_rg_state=RG_COMPLETED;
    return BinCommon::resync_decode(thread);
  }

  Status BinServer::rd_command(CommandInitiator* thread, Command& cmd)
  {
    if (BinCommon::rd_command(thread, cmd) != OK) return ERR_DECODE;
    if (cmd == 'G')
      m_rg_state = RG_BEGIN_PROPS;
    return OK;
  }

  Status BinServer::rd_call (CommandInitiator* thread, uint& rid, uint& modid, uint& action)
  {
    if (   BinCommon::rd_value(thread, modid)  != OK
        || BinCommon::rd_value(thread, action) != OK 
        || BinCommon::rd_value(thread, rid)    != OK 
       ) return ERR_DECODE;
    return OK;
  }

  Status BinServer::rd_parms(CommandInitiator* thread)
  {
    return BinCommon::rd_parms(thread);
  }

  Status BinServer::sk_parms(CommandInitiator* thread)
  {
    return BinCommon::sk_parms(thread);
  }

  Status BinServer::rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin)
  {
    return BinCommon::rd_parms(thread, decoded_message, descriptor_begin);
  }

  Status BinServer::rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    va_start(parms, descriptor_begin);
    result = BinCommon::rd_parms_lst(thread, descriptor_begin, parms);
    va_end(parms);
    return result;
  }

  Status BinServer::rd_parms_vlst(CommandInitiator* thread, uint descriptor_begin, va_list parms)
  {
    return BinCommon::rd_parms_lst(thread, descriptor_begin, parms);
  }

  Status BinServer::wr_response(CommandInitiator* thread, const uint& rid, const void* encoded_message, uint descriptor_begin)
  {
    m_transport->bl_start_encoding(thread);
    m_transport->bl_write_bytes(thread, 2,"Zr");
    wr_value(thread, rid);
    return BinCommon::wr_parms(thread, encoded_message, descriptor_begin, true);
  }

  Status BinServer::wr_response_lst(CommandInitiator* thread, const uint& rid, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    va_start(parms, descriptor_begin);
    m_transport->bl_start_encoding(thread);
    m_transport->bl_write_bytes(thread, 2,"Zr");
    wr_value(thread, rid);
    result = BinCommon::wr_parms_lst(thread, descriptor_begin, true, parms);
    va_end(parms);
    return result;
  }

  Status BinServer::wr_response_vlst(CommandInitiator* thread, const uint& rid, uint descriptor_begin, va_list parms)
  {
    Status result;
    m_transport->bl_start_encoding(thread);
    m_transport->bl_write_bytes(thread, 2,"Zr");
    wr_value(thread, rid);
    result = BinCommon::wr_parms_lst(thread, descriptor_begin, true, parms);
    return result;
  }

  Status BinServer::wr_response(CommandInitiator* thread, const uint& rid)
  {
    m_transport->bl_start_encoding(thread);
    m_transport->bl_write_bytes(thread, 2,"Zr");
    wr_value(thread, rid);
    return BinCommon::wr_parms(thread, true);
  }

  Status BinServer::wr_response_error(CommandInitiator* thread, const uint& rid)
  {
    m_transport->bl_start_encoding(thread);
    m_transport->bl_write_bytes(thread, 2,"Zr");
    wr_value(thread, rid);
    m_transport->bl_write_bytes(thread, 1,"N");
    return BinCommon::wr_parms(thread, true);
  }

/*
  Status BinServer::rd_assign_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop, uchar& kind) 
  {
    return BinCommon::rd_property(thread, modid, obj, prop, kind);
  }
  
  Status BinServer::rd_value(CommandInitiator* thread, Field* field, uint offset_descriptor, void* dst_value, uchar kind)           
  {
    return BinCommon::rd_value(thread, field, offset_descriptor, dst_value, kind);
  }

  Status BinServer::sk_value(CommandInitiator* thread, uchar kind)
  {
    return BinCommon::sk_value(thread, kind);
  }

  Status BinServer::begin_assign_response(CommandInitiator* thread)                             
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"Za") != TransportInterface::OK) goto lost_connection;
//    assert(!m_assign_response_started);
    m_assign_response_started = 1;
    return OK;
  lost_connection:
    m_assign_response_started = 0;
    return (Status) m_transport->end_encoding(thread);
  }
  
  Status BinServer::add_failed_assign(CommandInitiator* thread, uint modid, uint obj, uint prop)
  {
//    assert(m_assign_response_started > 0);
    if (m_assign_response_started > 1) {
      if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;
    } else
      m_assign_response_started = 2;
    if (wr_value(thread, prop) != OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, obj) != OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
    return OK;
  lost_connection:
    m_assign_response_started = 0;
    return (Status) m_transport->end_encoding(thread);
  }

  Status BinServer::end_assign_response(CommandInitiator* thread)                               
  {
    if (m_assign_response_started > 1)
      m_transport->bl_write_bytes(thread, 2,"]z");
    else
      m_transport->bl_write_bytes(thread, 1,"z");
    m_assign_response_started = 0;
    return (Status) m_transport->end_encoding(thread);
  }
*/
  Status BinServer::rd_get_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop) 
  {
    uchar token;
    do {
      switch (m_rg_state) {
      case RG_BEGIN_PROPS:
        {
          uint propid;
          m_rg_props.resize(0);
          m_rg_count = 0;
          do {
            if (BinCommon::rd_value(thread, propid) != OK) return ERR_DECODE; 
            m_rg_props.append(propid);
            if (rd_uchar(thread, token) != OK) return ERR_DECODE; 
          } while (token == ',');
          if (token != ']') return ERR_DECODE;
          if (BinCommon::rd_value(thread, m_rg_modid) != OK) return ERR_DECODE;
          modid = m_rg_modid; 
          if (BinCommon::rd_value(thread, m_rg_oid) != OK) return ERR_DECODE; 
          m_rg_state = RG_OIDS;
          obj        = m_rg_oid; 
          prop       = m_rg_props[m_rg_count++];
          return OK;
        }
      case RG_OIDS: 
        {
          if (m_rg_count<m_rg_props.size()) {
            modid      = m_rg_modid; 
            obj        = m_rg_oid; 
            prop       = m_rg_props[m_rg_count++];
            return OK;
          }
          m_rg_count = 0;
          if (rd_uchar(thread, token) == OK) { 
            if (token == ',') {
              if (BinCommon::rd_value(thread, m_rg_oid) == OK) {
                modid      = m_rg_modid; 
                obj        = m_rg_oid; 
                prop       = m_rg_props[m_rg_count++];
                return OK;
              }
            } else if (token == ']') {
              m_rg_state = RG_BEGIN_PROPS; 
              if (rd_uchar(thread, token) == OK) break;
            }
          }
          m_rg_state = RG_COMPLETED;
          return ERR_DECODE;      
        }
      case RG_COMPLETED:
        return ERR_DECODE;
      }
    } while (token == ',');
    if (token != ']' || rd_uchar(thread, token) != OK || token != 'z') return ERR_DECODE;
    m_rg_state = RG_COMPLETED;
    m_transport->end_decoding(thread);
    return COMPLETED;
  }
  
  Status BinServer::begin_get_response(CommandInitiator* thread) 
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"Zg") != TransportInterface::OK) goto lost_connection;
    return OK;
  lost_connection:
    return (Status) m_transport->end_encoding(thread, false);
  }
  
  Status BinServer::add_failed_get_property(CommandInitiator* thread, uint prop, uint modid, uint obj)
  {
    if (m_num_get_property > 1) {
      if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;
    } else
      m_num_get_property = 2;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, obj) != OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 1,"N") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, prop) != OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"]]") != TransportInterface::OK) goto lost_connection;
    return OK;
  lost_connection:
    m_num_get_property = 0;
    return (Status) m_transport->end_encoding(thread, false);
  }
 
  Status BinServer::add_get_property(CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint descriptor)  
  {
    char kind = m_fields[descriptor].kind();
    if (m_num_get_property > 1) {
      if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;
    } else
      m_num_get_property = 2;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, obj) != OK) goto lost_connection;
    if (kind != 'b') {
      if (m_transport->bl_write_bytes(thread, 1,&kind) != TransportInterface::OK) goto
lost_connection;
    } else {
      kind = * (int*) value ? 'T': 'F';
      if (m_transport->bl_write_bytes(thread, 1,&kind) != TransportInterface::OK) goto
lost_connection;
    }
    if (wr_value(thread, prop) != OK) goto lost_connection;
    wr_value((uchar) kind, thread, value, descriptor);
    if (m_transport->bl_write_bytes(thread, 2,"]]") != TransportInterface::OK) goto lost_connection;
    return OK;
  lost_connection:
    m_num_get_property = 0;
    return (Status) m_transport->end_encoding(thread, false);
  }
 
  Status BinServer::end_get_response  (CommandInitiator* thread)                                                             
  {
    m_num_get_property = 0;
    m_transport->bl_write_bytes(thread, 2,"]z");
    return (Status) m_transport->end_encoding(thread, true);
  }


#   ifdef SKYPE_SID_WITH_NO_PREENCODE
  Status BinServer::wr_event(CommandInitiator* thread, uint modid, uint event, const void* encoded_message, uint descriptor_begin)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZE") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, event) != OK) goto lost_connection;
    return wr_parms(thread, encoded_message, descriptor_begin, false);
  lost_connection:
    return (Status) m_transport->end_encoding(thread, false);
  }

  Status BinServer::wr_event_lst(CommandInitiator* thread, uint modid, uint event, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    va_start(parms, descriptor_begin);
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZE") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, event) != OK) goto lost_connection;
    result = wr_parms_lst(thread, descriptor_begin, false, parms);
    va_end(parms);
    return result;
  lost_connection:
    va_end(parms);
    return (Status) m_transport->end_encoding(thread, false);
  }

  Status BinServer::wr_event_vlst(CommandInitiator* thread, uint modid, uint event, uint descriptor_begin, va_list parms)
  {
    Status result;
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZE") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, event) != OK) goto lost_connection;
    result = wr_parms_lst(thread, descriptor_begin, false, parms);
    return result;
  lost_connection:
    return (Status) m_transport->end_encoding(thread, false);
  }
  
  Status BinServer::wr_event(CommandInitiator* thread, uint modid, uint event)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZE") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, event) != OK) goto lost_connection;
    m_transport->bl_write_bytes(thread, 1,"z");
  lost_connection:
    return (Status) m_transport->end_encoding(thread, false);
  }
#   endif // SKYPE_SID_WITH_NO_PREENCODE
  
  Status BinServer::wr_event(CommandInitiator* thread, const uint& header_size, const char* header)
  { uint foo = 0;
    return BinCommon::wr_preencoded(thread, header_size, header, foo, 0, 1, "z", false);
  }

  Status BinServer::wr_event_lst(CommandInitiator* thread, const uint& header_size, const char* header, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    uint   foo = 0;
    va_start(parms, descriptor_begin);
    result = BinCommon::wr_preencoded_lst(thread, header_size, header, foo, 0, descriptor_begin, false, parms);
    va_end(parms);
    return result;
  }

  Status BinServer::wr_event_vlst(CommandInitiator* thread, const uint& header_size, const char* header, uint descriptor_begin, va_list parms)
  {
    Status result;
    uint   foo = 0;
    result = BinCommon::wr_preencoded_lst(thread, header_size, header, foo, 0, descriptor_begin, false, parms);
    return result;
  }
  
  Status BinServer::wr_event(CommandInitiator* thread, const uint& header_size, const char* header, const void* encoded_message, uint descriptor_begin)
  { uint foo = 0;
    return BinCommon::wr_preencoded(thread, header_size, header, foo, 0, encoded_message, descriptor_begin, false);
  }

  Status BinServer::begin_property_change_event(CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint descriptor)
  {  
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZC") != TransportInterface::OK) goto lost_connection;
//    assert(m_num_property_change == 0);
    m_num_property_change = 1;
    return add_property_change(thread, prop, modid, obj, value, descriptor);
  lost_connection:
    m_num_property_change = 0;
    return (Status) m_transport->end_encoding(thread, false);
  }
  
  Status BinServer::add_property_change(CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint descriptor)
  {
    char kind = m_fields[descriptor].kind();
//    assert(m_num_property_change > 0);
    if (m_num_property_change > 1) {
      if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;
    } else {
      m_num_property_change = 2;
    }
    if (wr_value(thread, modid) != OK) goto lost_connection;
    if (wr_value(thread, obj) != OK) goto lost_connection;
    if (value) {
      if (kind != 'b') {
        if (m_transport->bl_write_bytes(thread, 1,&kind) != TransportInterface::OK) goto
lost_connection;
      } else {
        kind = * (int*) value ? 'T': 'F';
        if (m_transport->bl_write_bytes(thread, 1,&kind) != TransportInterface::OK) goto lost_connection;
      }
      if (wr_value(thread, prop) != OK) goto lost_connection;
      wr_value((uchar)kind, thread, value, descriptor);
    } else {
      if (m_transport->bl_write_bytes(thread, 1,"N") != TransportInterface::OK) goto lost_connection;
      if (wr_value(thread, prop) != OK) goto lost_connection;
    }
    if (m_transport->bl_write_bytes(thread, 2,"]]") != TransportInterface::OK) goto lost_connection;
    return OK;
  lost_connection:
    m_num_property_change = 0;
    return (Status) m_transport->end_encoding(thread, false);
  }
  
  Status BinServer::end_property_change_event(CommandInitiator* thread)                                                           
  {
    m_num_property_change = 0;
    m_transport->bl_write_bytes(thread, 2,"]z");
    return (Status) m_transport->end_encoding(thread, false);
  }

} } // Sid::Protocol

