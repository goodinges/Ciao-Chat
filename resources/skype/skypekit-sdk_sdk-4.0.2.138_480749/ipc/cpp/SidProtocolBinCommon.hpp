#ifndef Sid_ProtocolBinCommon_INCLUDED_HPP
#define Sid_ProtocolBinCommon_INCLUDED_HPP

#include "SidProtocolEnums.hpp"
#include "SidField.hpp"
#include <stdarg.h>

namespace Sid {

  class  CommandInitiator;
  class  Api;
  struct TransportInterface;

  namespace Protocol {


    class BinCommon { 
    protected:
      class GrowingCircularBuffer {
      public:
          GrowingCircularBuffer();
          ~GrowingCircularBuffer();
          bool empty() const ;
          uint size() const ;
          void  write(uchar c);
          char* write(const uint& n, uint& before_end);
          void  write(const uint& n, const char*);
          char* begin();
          void cancel_write(uint old_size);
          uint  read(uint n, char* dest);
          uchar read();
          void  resize(int sz);
          char* m_buffer;
          uint  m_buffer_mask;
          uint  m_size;
          uint  m_rd_ptr;
          uint  m_wr_ptr;
      };
      enum TypeTag  { 
         RESERVED, 
         TAG_INT         = Field::KIND_INT,     // 'i'
         TAG_UINT        = Field::KIND_UINT,    // 'u'
         TAG_BOOLEAN     = Field::KIND_BOOLEAN, // 'b'
         TAG_TRUE        = 'T',  
         TAG_FALSE       = 'F', 
         TAG_STRING      = Field::KIND_STRING,  // 'S'
         TAG_XML         = Field::KIND_XML,     // 'X'
         TAG_ENUM        = Field::KIND_ENUM,    // 'e'
         TAG_LIST_BEGIN  = Field::KIND_LIST,    // '['
         TAG_LIST_NEXT   = ',',  
         TAG_LIST_END    = ']', 
         TAG_METATAG     = Field::KIND_METATAG, // 'm' 
         TAG_METADATA    = Field::KIND_METADATA,// 'd' 
         TAG_BINARY      = Field::KIND_BINARY,  // 'B'
         TAG_OPAQUE      = Field::KIND_OPAQUE,  // 'o'
         TAG_FILENAME    = Field::KIND_FILENAME,// 'f' 
         TAG_UINT64      = Field::KIND_UINT64,  // 'U'
         TAG_OBJECTID    = Field::KIND_OBJECTID,// 'O'
         TAG_MODULE_BEGIN= Field::KIND_STRUCT,  // '{'
         TAG_TERM        = Field::KIND_TERM,    // 't'
      };

      enum { DEPTH_LIMIT   = 10,
             LIST_MAX_SIZE = 100000 
           };

      BinCommon(Api* api, TransportInterface* transport, Field* descriptors);
      virtual ~BinCommon(); // at least one virtual method to allow member function casting to derived class with visual c++

      Api* get_api();

      Status rd_command(CommandInitiator* thread, Command& cmd);
      inline Status rd_response(CommandInitiator* thread, Command& rsp_or_event) { return rd_command(thread, rsp_or_event); }
      Status buffer_events(CommandInitiator* thread, Command& cmd);

      //// unlocks the decode transport
      Status rd_parms(CommandInitiator* thread) { return sk_parms(thread); }
      Status sk_parms(CommandInitiator* thread);

      //// deserialize the parameters in the preallocated message and unlocks the decode transport
      Status rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin);
      Status rd_parms_lst(CommandInitiator* thread, uint descriptor_begin, va_list parms);

      //// write preencoded request message
      // inc_id == 0: don't insert id after header,
      // inc_id == 1: insert id after header (get request)
      // inc_id == 2: insert id after header and increment id
      Status wr_preencoded(CommandInitiator* thread, const uint& header_size, const char* header, uint& id, const uint& inc_id, const uint& footer_size, const char* footer, bool immediate_send_hint);
      Status wr_preencoded(CommandInitiator* thread, const uint& header_size, const char* header, uint& id, const uint& inc_id, const void* encoded_message, const uint& descriptor_begin, bool immediate_send_hint);
      Status wr_preencoded_lst(CommandInitiator* thread, const uint& header_size, const char* header, uint& id, const uint& inc_id, const uint& descriptor_begin, bool immediate_send_hint, va_list parms);

      //// unlocks the encode transport
      Status wr_parms(CommandInitiator* thread, bool immediate_send_hint);
      //// serialize the parameters in the preallocated message and unlocks the decode transport
      Status wr_parms(CommandInitiator* thread, const void* encoded_message, uint descriptor_begin, bool immediate_send_hint);
      Status wr_parms_lst(CommandInitiator* thread, uint descriptor_begin, bool immediate_send_hint, va_list parms);

      Status rd_property(CommandInitiator* thread, uint& mid, uint& oid, uint& property, uchar& kind);
        
      Status rd_value(CommandInitiator* thread, uint&);
      Status rd_value(CommandInitiator* thread, uint64&);
      Status rd_value(CommandInitiator* thread, int&);
      Status rd_value(CommandInitiator* thread, bool&);
      Status rd_value(CommandInitiator* thread, Binary&);
      Status rd_value(CommandInitiator* thread, String&);
      Status rd_value(CommandInitiator* thread, Filename&);

      Status wr_value(CommandInitiator* thread, const uint&);
      Status wr_value(CommandInitiator* thread, const uint64&);
      Status wr_value(CommandInitiator* thread, const int&);
      Status wr_value(CommandInitiator* thread, const bool&);
      Status wr_value(CommandInitiator* thread, const Binary&);
      Status wr_value(CommandInitiator* thread, const String&);
      Status wr_value(CommandInitiator* thread, const Filename&);

      Status sk_list         (CommandInitiator* thread);
      Status sk_message      (CommandInitiator* thread);
      Status sk_uint         (CommandInitiator* thread);
      Status sk_uint64       (CommandInitiator* thread);
      Status sk_bool         (CommandInitiator* thread);
      Status sk_byte_sequence(CommandInitiator* thread);
      Status sk_error        (CommandInitiator* thread);

      Status resync_decode(CommandInitiator* thread);
      Status rd_uchar(CommandInitiator* thread, uchar&);
      Status rd_bytes(CommandInitiator* curr, uint num_bytes, char* dest);

      typedef Status (BinCommon::*Reader) (CommandInitiator*, void*, uint);
      typedef Status (BinCommon::*Writer) (CommandInitiator*, const void*, uint);
      typedef Status (BinCommon::*Skipper)(CommandInitiator*);

      Status rd_value   (uchar kind, CommandInitiator* thread, void* dst_value, uint descriptor_begin);
      Status wr_value   (uchar kind, CommandInitiator* thread, const void* src_value, uint descriptor_begin);
      Status sk_value   (uchar kind, CommandInitiator* thread);

      Status rd_list    (CommandInitiator* thread, void* dst_value, uint descriptor_begin);
      Status rd_message (CommandInitiator* thread, void*, uint);
      Status rd_uint    (CommandInitiator* thread, void*, uint);
      Status rd_uint64  (CommandInitiator* thread, void*, uint);
      Status rd_int     (CommandInitiator* thread, void*, uint);
      Status rd_true    (CommandInitiator* thread, void*, uint);
      Status rd_false   (CommandInitiator* thread, void*, uint);
      Status rd_binary  (CommandInitiator* thread, void*, uint);
      Status rd_string  (CommandInitiator* thread, void*, uint);
      Status rd_filename(CommandInitiator* thread, void*, uint);
      Status rd_struct  (CommandInitiator* thread, void*, uint);
      Status rd_error   (CommandInitiator* thread, void*, uint);

      Status wr_uint    (CommandInitiator* thread, const void*, uint);
      Status wr_uint64  (CommandInitiator* thread, const void*, uint);
      Status wr_int     (CommandInitiator* thread, const void*, uint);
      Status wr_bool    (CommandInitiator* thread, const void*, uint);
      Status wr_binary  (CommandInitiator* thread, const void*, uint);
      Status wr_string  (CommandInitiator* thread, const void*, uint);
      Status wr_filename(CommandInitiator* thread, const void*, uint);
      Status wr_struct  (CommandInitiator* thread, const void*, uint);
      Status wr_message (CommandInitiator* thread, const void*, uint);

      Status wr_error   (CommandInitiator* thread, const void*, uint);

      Api*                  m_api;
      TransportInterface*   m_transport;
      int                   m_depth;
//      uint                  m_num_property;
      uint                  m_begin_property;    
      uint                  m_mod_property;
      uint                  m_obj_property;
      int                   m_buffering;
      uint                  m_rid;
      GrowingCircularBuffer m_buffer;
      Field*                m_fields;
      Reader*               m_readers;
      Writer*               m_writers;
      Skipper*              m_skippers;
      static Reader*        M_readers;
      static Writer*        M_writers;
      static Skipper*       M_skippers;
      static Reader         F_readers [256];
      static Writer         F_writers [256];
      static Skipper        F_skippers[256];
      SKYPEKIT_DISABLE_COPY(BinCommon);
    };
  }
}

#endif

