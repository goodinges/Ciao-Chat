#include "SidProtocolBinClient.hpp"
#include "SidProtocolFactory.hpp"

namespace Sid { namespace Protocol {

  BinClient::BinClient(TransportInterface* transport, Field* descriptors)
  : BinCommon(0, transport, descriptors), 
    m_get_propcount(0)
  {
  }

  BinClient::~BinClient()
  {
  }

  Status BinClient::resync_decode(CommandInitiator* thread)
  {
    return BinCommon::resync_decode(thread);
  }

#ifdef SKYPE_SID_WITH_NO_PREENCODE
  Status BinClient::wr_call(CommandInitiator* thread,const uint& modid,const uint& action, uint& rid, const void* encoded_message, uint descriptor_begin)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    rid = m_rid ++;
    if (m_transport->bl_write_bytes(thread, 2,"ZR") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid ) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, action) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, rid) != Protocol::OK) goto lost_connection;
    return BinCommon::wr_parms(thread, encoded_message, descriptor_begin, true);
  lost_connection:
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }

  Status BinClient::wr_call_lst(CommandInitiator* thread,const uint& modid,const uint& action, uint& rid, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    va_start(parms, descriptor_begin);
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    rid = m_rid ++;
    if (m_transport->bl_write_bytes(thread, 2,"ZR") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid ) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, action) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, rid) != Protocol::OK) goto lost_connection;
    result = BinCommon::wr_parms_lst(thread, descriptor_begin, true, parms);
    va_end(parms);
    return result;
  lost_connection:
    va_end(parms);
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }

  Status BinClient::wr_call_vlst(CommandInitiator* thread,const uint& modid,const uint& action, uint& rid, uint descriptor_begin, va_list parms)
  {
    Status result;
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    rid = m_rid ++;
    if (m_transport->bl_write_bytes(thread, 2,"ZR") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid ) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, action) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, rid) != Protocol::OK) goto lost_connection;
    result = BinCommon::wr_parms_lst(thread, descriptor_begin, true, parms);
    return result;
  lost_connection:
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }

  Status BinClient::wr_call(CommandInitiator* thread,const uint& modid,const uint& action,uint& rid)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    rid = m_rid ++;
    if (m_transport->bl_write_bytes(thread, 2,"ZR") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid ) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, action) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, rid ) != Protocol::OK) goto lost_connection;
    return BinCommon::wr_parms(thread, true);
  lost_connection:
    m_transport->end_encoding(thread, false);
    return Protocol::ERR_ENCODE; 
  }
#endif // SKYPE_SID_WITH_NO_PREENCODE

  Status BinClient::wr_call(CommandInitiator* thread,const uint& header_size, const char* header, uint& rid, const void* encoded_message, uint descriptor_begin)
  {
     Status status = BinCommon::wr_preencoded(thread, header_size, header, rid, 2, encoded_message, descriptor_begin, true);
     return status;
  }

  Status BinClient::wr_call_lst(CommandInitiator* thread, const uint& header_size, const char* header, uint& rid, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    va_start(parms, descriptor_begin);
    result = BinCommon::wr_preencoded_lst(thread, header_size, header, rid, 2, descriptor_begin, true, parms);
    va_end(parms);
    return result;
  }

  Status BinClient::wr_call_vlst(CommandInitiator* thread, const uint& header_size, const char* header, uint& rid, uint descriptor_begin, va_list parms)
  {
    Status result;
    result = BinCommon::wr_preencoded_lst(thread, header_size, header, rid, 2, descriptor_begin, true, parms);
    return result;
  }

  Status BinClient::wr_call(CommandInitiator* thread,const uint& header_size, const char* header, uint& rid)
  {
     Status status = BinCommon::wr_preencoded(thread, header_size, header, rid, 2, 1, "z", true);
     return status;
  }

  Status BinClient::rd_response_or_event(CommandInitiator* thread, Command& cmd) 
  {
    return BinCommon::rd_command(thread,cmd);
  }

  Status BinClient::buffer_events(CommandInitiator* thread, Command& cmd)
  {
    return BinCommon::buffer_events(thread,cmd);
  }

  Status BinClient::rd_event(CommandInitiator* thread, uint& modid, uint& event)
  {
    if (BinCommon::rd_value(thread, modid) != OK) goto lost_connection;
    if (BinCommon::rd_value(thread, event) != OK) goto lost_connection;
    return Protocol::OK;
    lost_connection:
    return Protocol::ERR_DECODE;
  }

  Status BinClient::sk_parms(CommandInitiator* thread)
  {
    return BinCommon::sk_parms(thread);
  }

  Status BinClient::rd_parms(CommandInitiator* thread)
  {
    return BinCommon::rd_parms(thread);
  }

  Status BinClient::rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin)
  {
    return BinCommon::rd_parms(thread, decoded_message, descriptor_begin);
  }

  Status BinClient::rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, ...)
  {
    va_list parms;
    Status result;
    va_start(parms, descriptor_begin);
    result = BinCommon::rd_parms_lst(thread, descriptor_begin, parms);
    va_end(parms);
    return result;
  }

  Status BinClient::rd_parms_vlst(CommandInitiator* thread, uint descriptor_begin, va_list parms)
  {
    return BinCommon::rd_parms_lst(thread, descriptor_begin, parms);
  }

  Status BinClient::rd_response_id(CommandInitiator* thread, uint& rid)
  {
    return BinCommon::rd_value(thread, rid);
  }

  Status BinClient::rd_value(uchar kind, CommandInitiator* thread, void* dst_value, uint offset)
  {
    return BinCommon::rd_value(kind, thread, dst_value, offset);
  }
  
  Status BinClient::sk_value(uchar kind, CommandInitiator* thread)
  {
    return BinCommon::sk_value(kind, thread);
  }

  Status BinClient::rd_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop, uchar& kind) {
    return BinCommon::rd_property(thread, modid, obj, prop, kind);
  } 

//  Status BinClient::begin_set_property(CommandInitiator* , uint , uint , uint , const void* )
//  {
//    return Protocol::OK;
//  }
//  Status BinClient::add_set_property(CommandInitiator* , uint , uint , uint , const void* )
//  {
//    return Protocol::OK;
//  }
//  Status BinClient::end_set_property(CommandInitiator* )
//  {
//    return Protocol::OK;
//  }

  Status BinClient::get_request(CommandInitiator* thread, const uint& size_header, const char* header, const uint& obj)
  {
    uint id = obj;
    return BinCommon::wr_preencoded(thread, size_header, header, id, 1, 2, "]z", true);
  }

  Status BinClient::begin_get_request(CommandInitiator* thread, const uint& size_header, const char* header, const uint& obj)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, size_header, header) != TransportInterface::OK) goto lost_connection;	
    if (wr_value(thread, obj) != OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
    m_get_propcount++;
    return Protocol::OK;
  lost_connection:
    m_get_propcount = 0;    
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }

  Status BinClient::begin_get_request(CommandInitiator* thread, uint modid, uint obj, uint prop)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZG") != TransportInterface::OK) goto lost_connection;	
    return add_get_property(thread, modid, obj, prop);
  lost_connection:
    m_get_propcount = 0;
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }
  
  Status BinClient::add_get_property(CommandInitiator* thread, uint modid, uint obj, uint prop)
  {
    if (m_get_propcount)
        if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;
        
    if (wr_value(thread, prop) != Protocol::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
    if (wr_value(thread, modid) != Protocol::OK) goto lost_connection;
    if (wr_value(thread, obj) != Protocol::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
    m_get_propcount++;
    return Protocol::OK;
  lost_connection:
    m_get_propcount = 0;
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }
  
  Status BinClient::end_get_property(CommandInitiator* thread)
  {
    m_get_propcount = 0;
    if (m_transport->bl_write_bytes(thread, 2,"]z") != TransportInterface::OK) goto lost_connection;
    return (Protocol::Status) m_transport->end_encoding(thread, true);
  lost_connection:
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;	
  }
        
  Status BinClient::begin_get_request(CommandInitiator* thread)
  {
    if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
    if (m_transport->bl_write_bytes(thread, 2,"ZG") != TransportInterface::OK) goto lost_connection;    
    return Protocol::OK;
  lost_connection:
    m_transport->end_encoding(thread, false); 
    return Protocol::ERR_ENCODE;
  }

  Status BinClient::add_get_properties(CommandInitiator* thread, uint modid, List_uint& objs, List_uint& props)
  {
        if (m_get_propcount)
                if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;
        
        uint prop, obj;
        for (uint i = 0; i < (uint)props.size(); i++) {
                prop = props[i];
                if (wr_value(thread, prop) != Protocol::OK) goto lost_connection;
                if (i+1 < (uint)props.size()) { 
                        if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;  
                } 
        }
        if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
        if (wr_value(thread, modid) != Protocol::OK) goto lost_connection;
        for (uint j = 0; j < (uint)objs.size(); j++) {
                obj = objs[j];
                if (wr_value(thread, obj) != Protocol::OK) goto lost_connection;
                if (j+1 < (uint)objs.size()) { 
                        if (m_transport->bl_write_bytes(thread, 1,",") != TransportInterface::OK) goto lost_connection;  
                } 
        }        
        if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) goto lost_connection;
        
        m_get_propcount++;
        return Protocol::OK;
  lost_connection:
        m_get_propcount = 0;
        m_transport->end_encoding(thread, false); 
        return Protocol::ERR_ENCODE;        
  }

  int BinClient::has_buffered_event() const {
    return m_buffer.size() > 0;
  }

} // Protocol
} // Sid


