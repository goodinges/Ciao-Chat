#ifndef Sid_ProtocolBinServer_INCLUDED_HPP
#define Sid_ProtocolBinServer_INCLUDED_HPP

#include "SidProtocolServerInterface.hpp"
#include "SidProtocolBinCommon.hpp"
#include "SidPlatform.hpp"

namespace Sid { namespace Protocol {

  class BinServer : public BinCommon, public ServerInterface {
  public:
    BinServer(Api*, TransportInterface*, Field*);
    virtual ~BinServer();

    virtual Status rd_command(CommandInitiator* thread, Command& cmd);
    virtual Status rd_call (CommandInitiator* thread, uint& rid, uint& modid, uint& action);
    virtual Status rd_parms(CommandInitiator* thread);
    virtual Status sk_parms(CommandInitiator* thread);
    virtual Status rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin);
    virtual Status rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, ...);
    virtual Status rd_parms_vlst(CommandInitiator* thread, uint descriptor_begin, va_list parms);
    virtual Status resync_decode(CommandInitiator* thread);

    virtual Status wr_response(CommandInitiator* thread, const uint& rid, const void* encoded_message, uint descriptor_begin);
    virtual Status wr_response(CommandInitiator* thread, const uint& rid);
    virtual Status wr_response_lst(CommandInitiator* thread, const uint& rid, uint descriptor_begin, ...);
    virtual Status wr_response_vlst(CommandInitiator* thread, const uint& rid, uint descriptor_begin, va_list parms);
    virtual Status wr_response_error(CommandInitiator* thread, const uint& rid);

//    virtual Status rd_assign_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop, uchar& kind) ;  
//    virtual Status rd_value(CommandInitiator* thread, Field*, uint, void* dst_value, uchar kind) ;
//    virtual Status sk_value(CommandInitiator* thread, uchar kind);

//    virtual Status begin_assign_response(CommandInitiator* thread);  
//    virtual Status add_failed_assign(CommandInitiator* thread, uint modid, uint obj, uint prop);
//    virtual Status end_assign_response(CommandInitiator* thread);

    virtual Status rd_get_property(CommandInitiator* thread, uint& modid, uint& obj, uint& prop) ;  
    virtual Status begin_get_response(CommandInitiator* thread)                                                             ;  
    virtual Status add_failed_get_property(CommandInitiator* thread, uint prop, uint modid, uint obj)                       ; 
    virtual Status add_get_property  (CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint descriptor)  ; 
    virtual Status end_get_response  (CommandInitiator* thread)                                                             ;

#   ifdef SKYPE_SID_WITH_NO_PREENCODE
    virtual Status wr_event(CommandInitiator* thread, uint modid, uint event, const void* encoded_message, uint descriptor_begin);  
    virtual Status wr_event_lst(CommandInitiator* thread, uint modid, uint event, uint descriptor_begin, ...);
    virtual Status wr_event_vlst(CommandInitiator* thread, uint modid, uint event, uint descriptor_begin, va_list);  
    virtual Status wr_event(CommandInitiator* thread, uint modid, uint event); 
#   endif // SKYPE_SID_WITH_NO_PREENCODE
 
    virtual Status wr_event(CommandInitiator* thread, const uint& size_header, const char* header, const void* encoded_message, uint descriptor_begin);  
    virtual Status wr_event_lst(CommandInitiator* thread, const uint& size_header, const char* header, uint descriptor_begin, ...);  
    virtual Status wr_event_vlst(CommandInitiator* thread, const uint& size_header, const char* header, uint descriptor_begin, va_list parms);  
    virtual Status wr_event(CommandInitiator* thread, const uint& size_header, const char* header);  
    virtual Status begin_property_change_event(CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint descriptor);  
    virtual Status add_property_change(CommandInitiator* thread, uint prop, uint modid, uint obj, const void* value, uint descriptor);  
    virtual Status end_property_change_event(CommandInitiator* thread);

    virtual Api* get_api();

  protected:
    int m_assign_response_started;
    int m_num_get_property;
    int m_num_property_change;
    enum { RG_COMPLETED, RG_BEGIN_PROPS, RG_OIDS } m_rg_state;
    List_uint m_rg_props;
    uint m_rg_count;
    uint m_rg_oid;
    uint m_rg_modid;
    SKYPEKIT_DISABLE_COPY(BinServer);
  };

} }

#endif
