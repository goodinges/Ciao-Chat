#ifndef Sid_ProtocolServerInterface_INCLUDED_HPP
#define Sid_ProtocolServerInterface_INCLUDED_HPP

#include "SidProtocolEnums.hpp"
#include <stdarg.h>

namespace Sid {

  class  Api;
  class  CommandInitiator;
  struct Field;

  namespace Protocol {

    struct ServerInterface {
      //// locks access to decode transport and etermine which command is arriving, 
      //   cmd can be a get request, a assign request or a call
      //   so that next API shall respectively be rd_get_property, rd_assing_property or rd_call
      virtual Status rd_command(CommandInitiator* thread, Command& cmd) = 0;

      //// gives the useful parameters to get_api() so that call can be dispatched
      //  rid identifies the call and shall be sent back while responding
      //  once dispatched, the parameters shall be decoded with rd_parms
      // if there is no parameters the empty rd_parms() shall  be read
      virtual Status rd_call (CommandInitiator* thread, uint& rid, uint& modid, uint& action) = 0;

      //// unlocks the decode transport
      virtual Status rd_parms(CommandInitiator* thread) = 0;
      virtual Status sk_parms(CommandInitiator* thread) = 0;
      //// deserialize the parameters in the preallocated message and unlocks the decode transport
      virtual Status rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin) = 0;
      virtual Status rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, ...) = 0;
      virtual Status rd_parms_vlst(CommandInitiator* thread, uint descriptor_begin, va_list parms) = 0;

      virtual Status resync_decode(CommandInitiator* thread) = 0;

      virtual Status wr_response(CommandInitiator* thread, const uint& rid, const void* encoded_message, uint descriptor_begin) = 0;
      virtual Status wr_response_lst(CommandInitiator* thread, const uint& rid, uint descriptor_begin, ...) = 0;
      virtual Status wr_response_vlst(CommandInitiator* thread, const uint& rid, uint descriptor_begin, va_list parms) = 0;
      virtual Status wr_response(CommandInitiator* thread, const uint& rid) = 0;
      virtual Status wr_response_error(CommandInitiator* thread, const uint& rid) = 0;

      ///// read the assignment
      // Status returns OK when there is another assignment one shall call after rd_value() in a preallocated  value, or sk_value()
      // it returns COMPLETED when there are no others, and then modid,  obj and prop are meaningless
//      virtual Status rd_assign_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop, uchar& kind)  = 0;  
//      virtual Status rd_value(CommandInitiator* thread, Field*, uint, void* dst_value, uchar kind)  = 0;
//      virtual Status sk_value(CommandInitiator* thread, uchar kind) = 0;

//      virtual Status begin_assign_response(CommandInitiator* thread)                              = 0;  
//      virtual Status add_failed_assign(CommandInitiator* thread, uint prop, uint modid, uint obj) = 0;
//      virtual Status end_assign_response(CommandInitiator* thread)                                = 0;

      ///// read the assignment
      // Status returns OK when there are other assignments and COMPLETED when there are no others
      // one shall after rd_value() in a preallocated  value
      virtual Status rd_get_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop)  = 0;  

      virtual Status begin_get_response(CommandInitiator* thread)                                                              = 0;  
      virtual Status add_failed_get_property(CommandInitiator* thread, uint prop, uint modid, uint obj)   = 0; 
      virtual Status add_get_property  (CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint desc)   = 0; 
      virtual Status end_get_response  (CommandInitiator* thread)                                                              = 0;

#   ifdef SKYPE_SID_WITH_NO_PREENCODE
      virtual Status wr_event(CommandInitiator* thread, uint modid, uint event, const void* encoded_message, uint descriptor_begin) = 0;  
      virtual Status wr_event_lst(CommandInitiator* thread, uint modid, uint event, uint descriptor_begin, ...) = 0;  
      virtual Status wr_event_vlst(CommandInitiator* thread, uint modid, uint event, uint descriptor_begin, va_list parms) = 0;  
      virtual Status wr_event(CommandInitiator* thread, uint modid, uint event) = 0;  
#   endif // SKYPE_SID_WITH_NO_PREENCODE

      virtual Status wr_event(CommandInitiator* thread, const uint& size_header, const char* header, const void* encoded_message, uint descriptor_begin) = 0;  
      virtual Status wr_event_lst(CommandInitiator* thread, const uint& size_header, const char* header, uint descriptor_begin, ...) = 0;  
      virtual Status wr_event(CommandInitiator* thread, const uint& size_header, const char* header) = 0;  

      virtual Status begin_property_change_event(CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint desc) = 0;  
      virtual Status add_property_change        (CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint desc) = 0;  
      virtual Status end_property_change_event  (CommandInitiator* thread) = 0;

      virtual Api* get_api() = 0;
      
      virtual ~ServerInterface() {}
      ServerInterface() {}
      SKYPEKIT_DISABLE_COPY(ServerInterface);
    };
  }
}

#endif
