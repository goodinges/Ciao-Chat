#ifndef Sid_ProtocolBinClient_INCLUDED_HPP
#define Sid_ProtocolBinClient_INCLUDED_HPP

#include "SidProtocolClientInterface.hpp"
#include "SidProtocolBinCommon.hpp"
#include "SidPlatform.hpp"

namespace Sid { namespace Protocol {

  class BinClient : public BinCommon, public ClientInterface {
  public:
      BinClient(TransportInterface*, Field*);
      virtual ~BinClient();
	
      Status rd_value(CommandInitiator* thread, uint& value)
      {
        return BinCommon::rd_value(thread,value);
      }

      virtual Status resync_decode(CommandInitiator* thread);

#   ifdef SKYPE_SID_WITH_NO_PREENCODE
      virtual Status wr_call(CommandInitiator* thread,const uint& modid,const uint& action, uint& rid, const void* encoded_message, uint descriptor_begin);
      virtual Status wr_call_lst(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid, uint descriptor_begin, ...);
      virtual Status wr_call_vlst(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid, uint descriptor_begin, va_list parms);
      virtual Status wr_call(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid);
#   endif

      virtual Status wr_call(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid, const void* encoded_message, uint descriptor_begin);
      virtual Status wr_call_lst(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid, uint descriptor_begin, ...);
      virtual Status wr_call_vlst(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid, uint descriptor_begin, va_list parms);
      virtual Status wr_call(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid);

      virtual Status rd_response_or_event(CommandInitiator* thread, Command& cmd);
      virtual Status buffer_events(CommandInitiator* thread, Command& cmd);

      //// gives the useful parameters to get_api() so that call can be dispatched
      //  rid identifies the call and shall be sent back while responding
      //  once dispatched, the parameters shall be decoded with rd_parms
      // if there is no parameters the empty rd_parms() shall  be read
      virtual Status rd_event(CommandInitiator* thread, uint& modid, uint& event);  

      //// unlocks the decode transport
      virtual Status rd_parms(CommandInitiator* thread);
      virtual Status sk_parms(CommandInitiator* thread);
      //// deserialize the parameters in the preallocated message and unlocks the decode transport
      virtual Status rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin);
      virtual Status rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, ...);
      virtual Status rd_parms_vlst(CommandInitiator* thread, uint descriptor_begin, va_list parms);

      virtual Status rd_response_id(CommandInitiator* thread, uint& rid);  

      // returns COMPLETED if no more properties are listed, then modid, obj and prop and kind are irrevelent
      // kind = 'N' if the value is unknown or can be used to call rd_value or sk_value
      virtual Status rd_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop, uchar& kind);
      // allows reading a property value in case of a property change or a get response
      virtual Status rd_value(uchar kind, CommandInitiator* thread, void* dst_value, uint offset);
      // allows skipping a property value in case of property change returning an unknown property
      virtual Status sk_value(uchar kind, CommandInitiator* thread);

//      virtual Status begin_set_property(CommandInitiator* thread, uint modid, uint obj, uint prop, const void* value);  
//      virtual Status add_set_property(CommandInitiator* thread, uint modid, uint obj, uint prop, const void* value);  
//      virtual Status end_set_property(CommandInitiator* thread);

      virtual Status get_request(CommandInitiator* thread, const uint& size_header, const char* header, const uint& obj);  
      virtual Status begin_get_request(CommandInitiator* thread, const uint& size_header, const char* header, const uint& obj);  
      virtual Status begin_get_request(CommandInitiator* thread, uint modid, uint obj, uint prop);  
      virtual Status add_get_property(CommandInitiator* thread, uint modid, uint obj, uint prop);
      virtual Status end_get_property(CommandInitiator* thread);
      
      virtual Status begin_get_request(CommandInitiator* thread);
      virtual Status add_get_properties(CommandInitiator* thread, uint modid, List_uint& objs, List_uint& props);
      
      virtual int has_buffered_event() const;
  protected:
      uint m_get_propcount;
      SKYPEKIT_DISABLE_COPY(BinClient);
  };

} }

#endif

