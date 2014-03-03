#ifndef Sid_ProtocolClientInterface_INCLUDED_HPP
#define Sid_ProtocolClientInterface_INCLUDED_HPP

#include "SidProtocolEnums.hpp"
#include <stdarg.h>

namespace Sid {

	class  CommandInitiator;
	struct Field;

namespace Protocol {

	struct ClientInterface {
#   ifdef SKYPE_SID_WITH_NO_PREENCODE
		virtual Status wr_call(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid, const void* encoded_message, uint descriptor_begin) = 0;
		virtual Status wr_call_lst(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid, uint descriptor_begin, ...) = 0;
		virtual Status wr_call_vlst(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid, uint descriptor_begin, va_list parms) = 0;
		virtual Status wr_call(CommandInitiator* thread,const uint& modid, const uint& action, uint& rid) = 0;
#   endif // SKYPE_SID_WITH_NO_PREENCODE

		virtual Status wr_call(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid, const void* encoded_message, uint descriptor_begin) = 0;
		virtual Status wr_call_lst(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid, uint descriptor_begin, ...) = 0;
		virtual Status wr_call_vlst(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid, uint descriptor_begin, va_list parms) = 0;
		virtual Status wr_call(CommandInitiator* thread,const uint& size_header, const char* header, uint& rid) = 0;
                                                        
		virtual Status rd_response_or_event(CommandInitiator* thread, Command& cmd) = 0;
		virtual Status buffer_events(CommandInitiator* thread, Command& cmd) = 0;
		//// gives the useful parameters to get_api() so that call can be dispatched
		//  rid identifies the call and shall be sent back while responding
		//  once dispatched, the parameters shall be decoded with rd_parms
		// if there is no parameters the empty rd_parms() shall  be read
		virtual Status rd_event(CommandInitiator* thread, uint& modid, uint& event) = 0;

		//// unlocks the decode transport
		virtual Status rd_parms(CommandInitiator* thread) = 0;
		virtual Status sk_parms(CommandInitiator* thread) = 0;
		//// deserialize the parameters in the preallocated message and unlocks the decode transport
		virtual Status rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin) = 0;
		virtual Status rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, ...) = 0;
		virtual Status rd_parms_vlst(CommandInitiator* thread, uint descriptor_begin, va_list parms) = 0;
		virtual Status rd_response_id(CommandInitiator* thread, uint& rid) = 0;

		// returns COMPLETED if no more properties are listed, then modid, obj and prop and kind are irrevelent
		// kind = 'N' if the value is unknown or can be used to call rd_value or sk_value
		virtual Status rd_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop, uchar& kind) = 0;
                              
		// allows reading a property value in case of a property change or a get response
		virtual Status rd_value(uchar kind, CommandInitiator* thread, void* dst_value, uint)	= 0;
		// allows skipping a property value in case of property change returning an unknown property
		virtual Status sk_value(uchar kind, CommandInitiator* thread) = 0;

//		virtual Status begin_set_property(CommandInitiator* thread, uint modid, uint obj, uint prop, const void* value) = 0;
//		virtual Status add_set_property(CommandInitiator* thread, uint modid, uint obj, uint prop, const void* value)  = 0;
//		virtual Status end_set_property(CommandInitiator* thread) = 0;

		virtual Status get_request(CommandInitiator* thread, const uint& size_header, const char* header, const uint& obj) = 0;
		virtual Status begin_get_request(CommandInitiator* thread, const uint& size_header, const char* header, const uint& obj) = 0;  
		virtual Status begin_get_request(CommandInitiator* thread, uint modid, uint obj, uint prop) = 0;
		virtual Status begin_get_request(CommandInitiator* thread) = 0;		
		virtual Status add_get_property(CommandInitiator* thread, uint modid, uint obj, uint prop)  = 0;
		virtual Status add_get_properties(CommandInitiator* thread, uint modid, List_uint& objs, List_uint& props) = 0;		
		virtual Status end_get_property(CommandInitiator* thread) = 0;
                
		virtual Status resync_decode(CommandInitiator* thread) = 0;
                
                virtual int has_buffered_event() const = 0;

                virtual ~ClientInterface() {}
                ClientInterface() {}
                SKYPEKIT_DISABLE_COPY(ClientInterface);
	};

  }
}

#endif
