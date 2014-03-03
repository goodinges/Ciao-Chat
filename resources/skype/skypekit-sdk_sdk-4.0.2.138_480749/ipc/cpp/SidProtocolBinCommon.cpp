#include "SidProtocolBinCommon.hpp"
#include "SidTransportInterface.hpp"
#include "SidProtocolFactory.hpp"
#include "SidField.hpp"
#include <stdio.h>
#include <assert.h>

#include <ctype.h>
#include <stdlib.h>

namespace Sid { 

TransportInterface::Status TransportInterface::end_decoding_with_error(CommandInitiator* curr)
{
  return end_decoding(curr);
}

namespace Protocol {

BinCommon::Reader  BinCommon::F_readers[256];
BinCommon::Writer  BinCommon::F_writers[256];
BinCommon::Skipper BinCommon::F_skippers[256];

BinCommon::Reader*  BinCommon::M_readers  = 0;
BinCommon::Writer*  BinCommon::M_writers  = 0;
BinCommon::Skipper* BinCommon::M_skippers = 0;

BinCommon::~BinCommon() 
{
  if (m_readers != M_readers)   delete[] m_readers;
  if (m_skippers != M_skippers) delete[] m_skippers;
  if (m_writers != M_writers)   delete[] m_writers;
}

BinCommon::BinCommon(Api* api, TransportInterface* transport, Field* field_descriptors) 
: m_api(api), 
  m_transport(transport),
  m_depth(0),
//  m_num_property(0),
  m_begin_property(0),
  m_mod_property(0),
  m_obj_property(0),
  m_buffering(0),
  m_rid(0),
  m_fields(field_descriptors)
{
  if (M_readers == 0) {
    M_readers  = &F_readers[0];
    M_skippers = &F_skippers[0];
    M_writers  = &F_writers[0];
    for (int i = 0; i < 256; i++) {
      M_readers [i] = &BinCommon::rd_error;
      M_writers [i] = &BinCommon::wr_error;
      M_skippers[i] = &BinCommon::sk_error;
    }
    M_readers ['i'] = &BinCommon::rd_int;
    M_readers ['O'] = &BinCommon::rd_uint;
    M_readers ['u'] = &BinCommon::rd_uint;
    M_readers ['e'] = &BinCommon::rd_uint;
    M_readers ['U'] = &BinCommon::rd_uint64;
    M_readers ['f'] = &BinCommon::rd_filename;
    M_readers ['S'] = &BinCommon::rd_string;
    M_readers ['X'] = &BinCommon::rd_string;
    M_readers ['B'] = &BinCommon::rd_binary;
    M_readers ['T'] = &BinCommon::rd_true;
    M_readers ['F'] = &BinCommon::rd_false;
    M_readers ['['] = &BinCommon::rd_list;
    M_readers ['{'] = &BinCommon::rd_struct;
    M_skippers['i'] = &BinCommon::sk_uint;
    M_skippers['O'] = &BinCommon::sk_uint;
    M_skippers['u'] = &BinCommon::sk_uint;
    M_skippers['e'] = &BinCommon::sk_uint;
    M_skippers['U'] = &BinCommon::sk_uint64;
    M_skippers['f'] = &BinCommon::sk_byte_sequence;
    M_skippers['S'] = &BinCommon::sk_byte_sequence;
    M_skippers['X'] = &BinCommon::sk_byte_sequence;
    M_skippers['B'] = &BinCommon::sk_byte_sequence;
    M_skippers['T'] = &BinCommon::sk_bool;
    M_skippers['F'] = &BinCommon::sk_bool;
    M_skippers['N'] = &BinCommon::sk_bool;
    M_skippers['['] = &BinCommon::sk_list;
    M_skippers['{'] = &BinCommon::sk_message;
    M_writers ['i'] = &BinCommon::wr_int;
    M_writers ['O'] = &BinCommon::wr_uint;
    M_writers ['u'] = &BinCommon::wr_uint;
    M_writers ['e'] = &BinCommon::wr_uint;
    M_writers ['U'] = &BinCommon::wr_uint64;
    M_writers ['f'] = &BinCommon::wr_filename;
    M_writers ['S'] = &BinCommon::wr_string;
    M_writers ['X'] = &BinCommon::wr_string;
    M_writers ['B'] = &BinCommon::wr_binary;
    M_writers ['T'] = &BinCommon::wr_bool;
    M_writers ['F'] = &BinCommon::wr_bool;
//    M_writers ['['] = &BinCommon::wr_list;
    M_writers ['{'] = &BinCommon::wr_struct;
  }
  m_readers  = M_readers;
  m_writers  = M_writers;
  m_skippers = M_skippers;
}

Api* BinCommon::get_api() { return m_api; }


//
// Decoding routines
//
//static uint peak = 512, read_buf = 0, writen_buf  =0;
  BinCommon::GrowingCircularBuffer::GrowingCircularBuffer() {
    m_buffer_mask = 512;
    m_buffer = new char[m_buffer_mask];
    m_rd_ptr = m_wr_ptr = m_size = 0;
    m_buffer_mask--;
  }
  BinCommon::GrowingCircularBuffer::~GrowingCircularBuffer() { delete[] m_buffer; /*printf("EV BUFFER = %u, peak = %u, wr = %u, rd = %u\n", m_buffer_mask+1,peak,read_buf,writen_buf);*/}
  bool BinCommon::GrowingCircularBuffer::empty() const { return m_size == 0; }
  uint BinCommon::GrowingCircularBuffer::size() const { return m_size; }
  void  BinCommon::GrowingCircularBuffer::write(uchar c) { 
    if (m_size == (m_buffer_mask+1)) resize(m_size+1); 
    m_size++;
//writen_buf++;
    m_buffer[m_wr_ptr] = c;
    m_wr_ptr = (m_wr_ptr + 1) & m_buffer_mask; 
  }
  char* BinCommon::GrowingCircularBuffer::write(const uint& n, uint& before_end) {
    if ((m_size+n) > m_buffer_mask) resize(m_size+n); 
    char* start = &m_buffer[m_wr_ptr];
    before_end = m_buffer_mask + 1 - m_wr_ptr;
    m_wr_ptr = (m_wr_ptr + n) & m_buffer_mask; 
    m_size += n; 
//writen_buf+=n;
    return start;
  }
  void BinCommon::GrowingCircularBuffer::write(const uint& n, const char* p) {
    for (uint i = 0; i < n; i++) write(*p++);
  }
  char* BinCommon::GrowingCircularBuffer::begin() { return &m_buffer[0]; } 
  void BinCommon::GrowingCircularBuffer::cancel_write(uint /*old_size*/) {
  }
  uint BinCommon::GrowingCircularBuffer::read(uint n, char* dest) {
    if (n > m_size) n = m_size;
    if (dest)
      for (uint i = 0; i < n; i++) dest[i] = m_buffer[(m_rd_ptr+i)&m_buffer_mask];
    m_rd_ptr = (m_rd_ptr+n)&m_buffer_mask;
    m_size -= n;
//read_buf+=n;
    return n;
  }
  uchar BinCommon::GrowingCircularBuffer::read() {
    uchar rd = m_buffer[m_rd_ptr];
    m_rd_ptr = (m_rd_ptr + 1) & m_buffer_mask;
    if (--m_size == 0 && m_buffer_mask > 511) resize(512); 
    return rd;
  }
  void BinCommon::GrowingCircularBuffer::resize(int sz) {
     int new_sz = 512; while (new_sz < sz) new_sz *= 2;
     char* buf = new char[new_sz];
     for (uint i=0; i < m_size; i++) { buf[i] = m_buffer[(m_rd_ptr+i)&m_buffer_mask]; }
     m_rd_ptr = 0;
     m_wr_ptr = m_size;
     delete[] m_buffer;
//printf("RESIZE old=%d sz=%d new = %d\n",m_buffer_mask+1,sz,new_sz);
     m_buffer_mask = new_sz-1;
//peak = new_sz > peak ? new_sz : peak;
     m_buffer = buf;
  }

Status BinCommon::rd_uchar(CommandInitiator* thread, uchar& byte)
{
//printf("rd_uchar %d %d\n", m_buffering, m_buffer.size()); 
  if (m_buffering != 0 || m_buffer.empty()) { // we have not buffered and may be buffering or processing the request after buffering
    Status status = (Status) m_transport->bl_read_bytes(thread, 1, &(char&)byte);
    if (status == OK && m_buffering > 0) {
//      printf("BUFFERING uchar %c\n", isprint(byte) ? byte : ' ');
      m_buffer.write(byte);
    } else {
//      printf("m_transport->bl_read_bytes\n");
    }
    return status;
  }
//   printf("m_buffer.read()\n");
  byte = m_buffer.read();
//printf("deBUFFERING uchar %c\n", isprint(byte) ? byte : ' ');
  return Protocol::OK;
}

Status BinCommon::rd_bytes(CommandInitiator* thread, uint n, char* dest)
{
  if (m_buffering > 0) { // we are buffering events
    // assert(dest == 0);
//printf("BUFFERING %d\n", n);
    uint before_end;
    dest = m_buffer.write(n, before_end);
    if (n > before_end) {
      if ((Status) m_transport->bl_read_bytes(thread, before_end, dest) != OK) return ERR_DECODE;
      dest = m_buffer.begin();
      n   -= before_end;
    }
    //  return (Status) m_transport->bl_read_bytes(thread, n, dest);
  } else if (m_buffering == 0 && m_buffer.size() > 0) { // we are debuffering events
    uint r = m_buffer.read(n, dest);
//printf("deBUFFERING %d/%d\n", r, n);
    if (r == n) return Protocol::OK;
    n -= r;
    if (dest) dest += r;
  }
  // we are processing the response right after the buffering or 
  return (Status) m_transport->bl_read_bytes(thread, n, dest);
}

Status BinCommon::resync_decode(CommandInitiator* thread) 
{
//  m_num_property = 0;
  m_begin_property = 0;
  m_depth = 0;
  m_buffering = 0;
  return (Status) m_transport->end_decoding_with_error(thread);
}

Status BinCommon::rd_command(CommandInitiator* thread, Command& cmd)
{
  uchar z;
  if ((Status) m_transport->bl_start_decoding(thread) != OK) return ERR_DECODE;
  if (rd_uchar(thread, z) != OK || z != 'Z') return ERR_DECODE;
  if (rd_uchar(thread, z) != OK) return ERR_DECODE;
  cmd = (Command) z;
  return OK;
}

Status BinCommon::rd_property(CommandInitiator* thread, uint& mid, uint& oid, uint& property, uchar& kind)
{
        uchar token;
//         printf("m_begin_property %d\n", m_begin_property);
        
        if (!m_begin_property) {
                if (rd_value(thread, m_mod_property) != Protocol::OK) goto lost_connection;  
                if (rd_value(thread, m_obj_property) != Protocol::OK) goto lost_connection;  
                if (rd_uchar(thread, kind) != Protocol::OK) goto lost_connection;
                if (rd_value(thread, property) != Protocol::OK) goto lost_connection; 
        } else {
                if (rd_uchar(thread, token) != Protocol::OK) goto lost_connection; 
                if (token == ']') {
                        if (rd_uchar(thread, token) != Protocol::OK) goto lost_connection;
                        if (token == ',') { //continue
                                if (rd_value(thread, m_obj_property) != Protocol::OK) goto lost_connection;    
                                if (rd_uchar(thread, kind) != Protocol::OK) goto lost_connection;
                                if (rd_value(thread, property) != Protocol::OK) goto lost_connection;                     
                        } else if (token == ']') {
                                m_begin_property = 0;
                                if (rd_uchar(thread, token) != Protocol::OK) goto lost_connection;
                                if (token == ']') {
                                        rd_uchar(thread, token); //z
                                
                                        if (m_buffering < 0) m_buffering = 0;
                                        m_transport->end_decoding(thread);
                                
                                        return Protocol::COMPLETED;
                                } else if (token == ',') {
                                        return rd_property(thread, mid, oid, property, kind);
                                }        
                        }
                        goto lost_connection;                       
                } else {
                        kind = token;
                        if (rd_value(thread, property) != Protocol::OK) goto lost_connection;
                }
        }
        
        m_begin_property++;
        mid = m_mod_property;
        oid = m_obj_property;
        
        return Protocol::OK;
        
lost_connection:
        m_begin_property = 0;
        return Protocol::ERR_DECODE;      
}

Status BinCommon::rd_parms(CommandInitiator* thread, void* decoded_message, uint descriptor_begin)
{
  Status status;
  if ((status = rd_message(thread, decoded_message, descriptor_begin)) != OK) return status;
  if (m_buffering < 0) m_buffering = 0;
  m_transport->end_decoding(thread);
  return status;
}

Status BinCommon::rd_message(CommandInitiator* thread, void* message_received, uint offset)
{
  uchar token = 0;
  uint  tag = 0;
  uint  prev_tag = 0;
  bool  peek = true;
  uint  unkown_tags_limit = 30; 
  if (m_depth++ > DEPTH_LIMIT) return ERR_DECODE;
  const Field* b = m_fields;
  const Field* f = &b[offset];
  const Field* e = &b[f->get_end()];     // message descriptor ended
  while (f != e) {
    if (peek) { 
      if (rd_uchar(thread, token) != OK) return ERR_DECODE;
      if (token != 'z') {
        if (token == 'N') { 
          if (rd_uchar(thread, token) != OK && token != 'z') return ERR_DECODE;
          return ERR_CALL;
        }
        if (rd_value(thread, tag) != OK || tag < prev_tag) return ERR_DECODE;
        prev_tag = tag;
      }
    }
    if (token == 'z') {
      // we shall reset all remaining fields
      f->set_to_default(message_received);
      peek = false; // no more to read...
    } else if (tag == f->get_tag()) {  
      // check that type in message matches the field type flag
      // (except for boolean, as no type flag is given but value is immediately given)
      if (!(token == f->kind() 
            || (f->kind() == 'b' && (token == 'F' || token == 'T')) 
            || (f->is_list() && token == '[')
            )
          )
        return ERR_DECODE;
      if (rd_value(token, thread, f->offset(message_received), f-m_fields) != OK) return ERR_DECODE;
      peek = true;
    } else {
      // tag is unknown: skip the value
      peek = tag < f->get_tag();
      if (peek) {
        if (unkown_tags_limit-- == 0 || sk_value(token, thread) != OK) return ERR_DECODE;
        continue; // keep current field
      }
      f->set_to_default(message_received);
    }
    // goto next field...
    f = f->next(); 
  }
  while (token != 'z') {
    if (rd_uchar(thread, token) != OK) return ERR_DECODE;
    if (token != 'z') {
      if (token == 'N') { 
        if (rd_uchar(thread, token) != OK && token != 'z') return ERR_DECODE;
        return ERR_CALL;
      }
      if (rd_value(thread, tag) != OK || tag < prev_tag) return ERR_DECODE;
      prev_tag = tag;
      if (unkown_tags_limit-- == 0 || sk_value(token, thread) != OK) return ERR_DECODE;
    }
  }
  m_depth--;
  return OK;
}

Status BinCommon::rd_struct(CommandInitiator* thread, void* message_received, uint offset)
{
  uint struct_offset = m_fields[offset].get_struct_offset();
  return rd_message(thread, message_received, struct_offset);
}

Status BinCommon::rd_value(uchar kind, CommandInitiator* thread, void* dst, uint offset)
{
  return (this->*m_readers[kind])(thread, dst, offset);
}

Status BinCommon::rd_list(CommandInitiator* thread, void* dst_value, uint offset) 
{
  const Field* b = m_fields;
  const Field* f = b+offset;
  uint size = 0;
  // currently the protocol doesn't provide list size
  f->set_field_to_default(dst_value);  
  //f->reserve(dst_value, size);
  char kind = (char)f->kind();
  void* elem;
  uchar token;
  while (rd_uchar(thread, token) == OK) {
    if (token == ']') return OK;
    if (token == kind || (kind == 'b' && (token == 'T' || token == 'F'))) {
      f->append(dst_value, size++, elem);
      if (rd_value(token, thread, elem, offset) != OK) return ERR_DECODE;
    } else break;
  }
  return ERR_DECODE;
}

Status BinCommon::rd_filename(CommandInitiator* thread, void* value, uint) 
{
  return rd_value(thread, *static_cast<Filename*>(value));
}

Status BinCommon::rd_string(CommandInitiator* thread, void* value, uint)  
{ 
  return rd_value(thread, *static_cast<String*>(value));
}

Status BinCommon::rd_binary(CommandInitiator* thread, void* value, uint)
{
  return rd_value(thread, *static_cast<Binary*>(value));
}

Status BinCommon::rd_value(CommandInitiator* thread, Binary& value)
{
  uint size;
  Status status = rd_value(thread, size);
  if (status == OK) {
    if (size > 0) { // protect from unrealistic size?
      value.reserve(size);
      status = rd_bytes(thread, size, (char*) value.data());
    }
  }
  return status;
}

Status BinCommon::rd_int(CommandInitiator* thread, void* val, uint)
{
  return rd_value(thread, *static_cast<int*>(val));
}

Status BinCommon::rd_value(CommandInitiator* thread, int& val)
{
  uint uval;
  Status status = rd_value(thread, uval);
  if (status == OK) 
    val = (uval >> 1) ^ -static_cast<int>(uval & 1);
  return status;
}

Status BinCommon::rd_uint(CommandInitiator* thread, void* val, uint)
{
  return rd_value(thread, *static_cast<uint*>(val));
}

Status BinCommon::rd_value(CommandInitiator* thread, uint &val)
{
  val=0;
  uchar c;
  uint shift=0;
  do {
    if (rd_uchar(thread, c) != OK) return ERR_DECODE;
    val|=((uint)c&0x7f)<<shift;
    shift+=7;
    if (shift > 32 && c&0xf0) return ERR_DECODE;
  } while(c&0x80);
  return OK;
}

Status BinCommon::rd_parms_lst(CommandInitiator* thread, uint offset, va_list parms)
{
  const Field* b = m_fields;
  uchar token = 0;
  uint  tag = 0;
  uint  prev_tag = 0;
  bool  peek = true;
  uint  unkown_tags_limit = 30; 
  if (m_depth++ > DEPTH_LIMIT) return ERR_DECODE;
  const Field* f = &b[offset];
  const Field* e = &b[f->get_end()];     // message descriptor ended
  while (f != e) {
    if (peek) { 
      if (rd_uchar(thread, token) != OK) return ERR_DECODE;
      if (token != 'z') {
        if (token == 'N') { 
          if (rd_uchar(thread, token) != OK && token != 'z') return ERR_DECODE;
          return ERR_CALL;
        }
        if (rd_value(thread, tag) != OK || tag < prev_tag) return ERR_DECODE;
        prev_tag = tag;
      }
    }
    if (token == 'z') {
      // we shall reset all remaining fields
      f->set_field_to_default(va_arg(parms, void*));
      peek = false; // no more to read...
    } else if (tag == f->get_tag()) {  
      // check that type in message matches the field type flag
      // (except for boolean, as no type flag is given but value is immediately given)
      if (!(token == f->kind() 
            || (f->kind() == 'b' && (token == 'F' || token == 'T')) 
            || (f->is_list() && token == '[')
            )
          )
        return ERR_DECODE;
      if (rd_value(token, thread, va_arg(parms, void*), f-b) != OK) return ERR_DECODE;
      peek = true;
    } else {
      // tag is unknown: skip the value
      peek = tag < f->get_tag();
      if (peek) {
        if (unkown_tags_limit-- == 0 || sk_value(token, thread) != OK) return ERR_DECODE;
        continue; // keep current field
      }
      f->set_field_to_default(va_arg(parms, void*));
    }
    // goto next field...
    f = f->next(); 
  }
  while (token != 'z') {
    if (rd_uchar(thread, token) != OK) return ERR_DECODE;
    if (token != 'z') {
      if (token == 'N') { 
        if (rd_uchar(thread, token) != OK && token != 'z') return ERR_DECODE;
        return ERR_CALL;
      }
      if (rd_value(thread, tag) != OK || tag < prev_tag) return ERR_DECODE;
      prev_tag = tag;
      if (unkown_tags_limit-- == 0 || sk_value(token, thread) != OK) return ERR_DECODE;
    }
  }
  m_depth--;
  if (m_buffering < 0) m_buffering = 0;
  m_transport->end_decoding(thread);
  return OK;
}

Status BinCommon::rd_true(CommandInitiator*, void* value, uint)
{
  *static_cast<bool*>(value) = true;
  return OK;
}

Status BinCommon::rd_false(CommandInitiator*, void* value, uint)
{
  *static_cast<bool*>(value) = false;
  return OK;
}

Status BinCommon::rd_uint64(CommandInitiator* thread, void* val, uint)
{
  return rd_value(thread, *static_cast<uint64*>(val));
}

Status BinCommon::rd_value(CommandInitiator* thread, uint64& val)
{
  val=0;
  uchar c;
  uint shift=0;
  do {
    if (rd_uchar(thread, c) != OK) return ERR_DECODE;
    val|=((uint64)c&0x7f)<<shift;
    shift+=7;
    // needed ?
    if (shift > 64 && c&0xfe) return ERR_DECODE;
  } while(c&0x80);
  return OK;
}

Status BinCommon::rd_error(CommandInitiator*, void*, uint)
{
  return ERR_DECODE;
}


//
// Decoding routines: skippers
//

Status BinCommon::sk_value(uchar kind, CommandInitiator* thread)
{
  return (this->*m_skippers[kind])(thread);
}

Status BinCommon::sk_error(CommandInitiator*)
{
  return ERR_DECODE;
}

Status BinCommon::sk_parms(CommandInitiator* thread)
{
  Status status = sk_message(thread);
  if (status != OK) return status;
  if (m_buffering < 0) m_buffering = 0;
  m_transport->end_decoding(thread);
  return status;
}

Status BinCommon::sk_message(CommandInitiator* thread)
{
  uchar token;
  uint tag;
  int unkown_tags_limit = 30;
  if (m_depth++ > DEPTH_LIMIT) return ERR_DECODE;
  while (rd_uchar(thread, token) == OK) {
    if (token == 'z') { m_depth--; return OK; }
    if (token == 'N') { 
      if (rd_uchar(thread, token) != OK && token != 'z') return ERR_DECODE;
      return ERR_CALL;
    }
    if (unkown_tags_limit-- == 0 || rd_value(thread, tag) != OK || sk_value(token, thread) != OK) return ERR_DECODE;
  }
  return ERR_DECODE;
}

Status BinCommon::sk_list(CommandInitiator* thread)
{
  uchar token;
  while (rd_uchar(thread, token) == OK) {
    if (token == ']') return OK;
    if (sk_value(token, thread) != OK) break; 
  }
  return ERR_DECODE;
}

Status BinCommon::sk_bool(CommandInitiator*)
{
  return OK; // value is in the kind
}

Status BinCommon::sk_byte_sequence(CommandInitiator* thread)
{
  uint size;
  Status status = rd_value(thread, size);
  if (status == OK && size > 0)
    status = rd_bytes(thread, size, 0);
  return status;   
}

Status BinCommon::sk_uint(CommandInitiator* thread)
{
  uint foo;
  return rd_value(thread, foo);
}

Status BinCommon::sk_uint64(CommandInitiator* thread)
{
  uint64 foo;
  return rd_value(thread, foo);
}

//
// Decoding: Buffering
//

Status BinCommon::buffer_events(CommandInitiator* thread, Command& cmd) {
  Status status = ERR_DECODE;
  uchar token;
  m_buffering = -1; // force read in transport
//printf("Buffering mode\n");
  if (rd_response(thread, cmd) == OK) {
    switch (cmd) {
    case 'C': {
//printf("Buffering C\n");
      int size = m_buffer.size();
      m_buffering = 1+m_buffer.size();
      m_buffer.write(2, "ZC");
      do {
        if (sk_uint(thread) != OK) goto decode_error; // mod id
        do {
          if (sk_uint(thread) != OK) goto decode_error;  // obj id
          for(;;) {
            if (rd_uchar(thread,token) != OK) goto decode_error;    // kind
            if (token == ']') break;
            if (sk_uint(thread) != OK || sk_value(token, thread) != OK) goto decode_error;  // propid
          }
          if (rd_uchar(thread,token) != OK) goto decode_error;
        } while (token == ',');
        if (token != ']' || rd_uchar(thread,token) != OK) goto decode_error;
      } while (token == ',');
      if (token != ']' || rd_uchar(thread,token) != OK || token != 'z') goto decode_error;
//printf("%p Buffered C\n", thread);
      m_buffering = 0;
      return (Status) m_transport->end_decoding(thread);
    decode_error: 
      status = ERR_DECODE;
      m_buffer.cancel_write(size);
      break;
    }
    case 'E': {
//printf("Buffering E\n");
      int size = m_buffer.size();
      m_buffering = 1+m_buffer.size();
      m_buffer.write(2, "ZE");
      if ((status = sk_uint(thread))    != OK ||
          (status = sk_uint(thread))    != OK ||
          (status = sk_message(thread)) != OK) goto E_decode_error;
//printf("Buffered E\n");
      m_buffering = 0;
      return (Status) m_transport->end_decoding(thread);
    E_decode_error: 
      status = ERR_DECODE;
      m_buffer.cancel_write(size);
      break;
    }
    case 'r':
    case 'g':
//printf("Stop Buffering r or g\n");
      return OK; // stop buffering, stay in -1 mode to finish processing the message
    default:
      status = ERR_DECODE;
      break;
    }
  }
  if (status == ERR_DECODE) {
    m_buffering = 0;
  }
  return status;
}

//
// Encoding routines
//

Status BinCommon::wr_value(uchar kind, CommandInitiator* thread, const void* src, uint descriptor)
{
  return (this->*m_writers[kind])(thread, src, descriptor);
}

Status BinCommon::wr_string(CommandInitiator* thread, const void* src, uint)
{
  return wr_value(thread, *reinterpret_cast<const String*>(src));
}

Status BinCommon::wr_filename(CommandInitiator* thread, const void* src, uint)
{
  return wr_value(thread, *reinterpret_cast<const Filename*>(src));
}

Status BinCommon::wr_binary(CommandInitiator* thread, const void* src, uint)
{
  return wr_value(thread, *reinterpret_cast<const Binary*>(src));
}

Status BinCommon::wr_value(CommandInitiator* thread, const Binary& val)
{
  uint size = val.size();
  if (wr_value(thread, size) != OK) return ERR_ENCODE;
  if (size > 0)
    if (m_transport->bl_write_bytes(thread, size,val.data()) != TransportInterface::OK) return ERR_ENCODE;
  return OK;
}

Status BinCommon::wr_struct(CommandInitiator* thread, const void* msg_struct, uint offset)
{
  uint struct_offset = m_fields[offset].get_struct_offset();
  return wr_message(thread, msg_struct, struct_offset);
}

Status BinCommon::wr_message(CommandInitiator* thread, const void* message_to_send, uint descriptor_begin)
{
  const Field* base = m_fields;
  const Field* f = &base[descriptor_begin]; // message descriptor begin
  const Field* e = &base[f->get_end()];     // message descriptor ended
  void*  dest;
  void*  listbase = 0;
  int    islist = 0;
  // iterate through all message tags in IDL order
  // all fields shall be present in order (encoder shall put them in order)
  for (;f < e;f = f->next()) {
    if (!f->equals_default(message_to_send)) {
      dest = f->offset(message_to_send);
      islist = f->is_list();
      uint elems = 0;
      if (islist) {
        if (m_transport->bl_write_bytes(thread, 1,"[") != TransportInterface::OK) goto lost_connection;
        if (wr_value(thread, f->get_tag()) != OK) goto lost_connection;
        listbase = dest;
        dest = f->iterate(listbase, elems++);
      }
      char kind = (char)f->kind(); 
      while (dest != 0) {
        if (kind == TAG_BOOLEAN) {
          if (m_transport->bl_write_bytes(thread, 1,*reinterpret_cast<bool*>(dest) ? "T" : "F") != TransportInterface::OK) goto lost_connection;
          if (!islist && wr_value(thread, f->get_tag()) != OK) goto lost_connection;
        } else {
          if (m_transport->bl_write_bytes(thread, 1,&kind) != TransportInterface::OK) goto lost_connection;
          if (!islist && wr_value(thread, f->get_tag()) != OK) goto lost_connection;
          wr_value(kind, thread, dest, f-base);
        }
        dest = 0;
        if (islist)
          dest = f->iterate(listbase, elems++);
      }
      if (islist) {
        if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) return Protocol::ERR_ENCODE;
      }
    }
  }
  if (m_transport->bl_write_bytes(thread, 1,"z") != TransportInterface::OK) goto lost_connection;
  return OK;
lost_connection:
  return (Status) m_transport->end_encoding(thread, false);
}

Status BinCommon::wr_parms(CommandInitiator* thread, const void* message_to_send, uint descriptor_begin, bool immediate)
{
  Status status = wr_message(thread, message_to_send, descriptor_begin);
  if (status == OK)
    status = (Status) m_transport->end_encoding(thread, immediate);
  return status;
}

Status BinCommon::wr_int(CommandInitiator* thread, const void* src, uint)
{
  return wr_value(thread, *reinterpret_cast<const int*>(src));
}

Status BinCommon::wr_value(CommandInitiator* thread, const int& value)
{
//printf("encoding signed %d %x\n", value, value);
  // Note:  the right-shift must be arithmetic
  return wr_value(thread, (uint)((value << 1) ^ (value >> 31)));
}

Status BinCommon::wr_uint(CommandInitiator* thread, const void* src, uint)
{
  return wr_value(thread, *reinterpret_cast<const uint*>(src));
}

Status BinCommon::wr_value(CommandInitiator* thread, const uint& val)
{
//printf("encoding %d %x\n", val, val);
  uint value = val;
  int  sz = 0;
  char buf[(32+7)/7];
  for(;value>0x7f;value>>=7)
    buf[sz++] = (char)((value&0x7f)|0x80);
  buf[sz++]=(char)value;
  return (Status) m_transport->bl_write_bytes(thread, sz, &buf[0]);
}

Status BinCommon::wr_parms_lst(CommandInitiator* thread, uint descriptor_begin, bool immediate, va_list parms)
{
  const Field* base = m_fields;
  const Field* f = &base[descriptor_begin]; // message descriptor begin
  const Field* e = &base[f->get_end()];     // message descriptor ended
  void*  dest;
  void*  listbase = 0;
  int    islist = 0;
  // iterate through all message tags in IDL order
  // all fields shall be present in order (encoder shall put them in order)
  for (;f < e;f=f->next()) {
    dest = va_arg(parms, void*);
    if (!f->field_equals_default(dest)) {
      islist = f->is_list();
      uint elems = 0;
      if (islist) {
        if (m_transport->bl_write_bytes(thread, 1,"[") != TransportInterface::OK) goto lost_connection;
        if (wr_value(thread, f->get_tag()) != OK) goto lost_connection;
        listbase = dest;
        dest = f->iterate(listbase, elems++);
      }
      char kind = (char)f->kind(); 
      while (dest != 0) {
        if (kind == TAG_BOOLEAN) {
          if (m_transport->bl_write_bytes(thread, 1,*reinterpret_cast<bool*>(dest) ? "T" : "F") != TransportInterface::OK) goto lost_connection;
          if (!islist && wr_value(thread, f->get_tag()) != OK) goto lost_connection;
        } else {
          if (m_transport->bl_write_bytes(thread, 1,&kind) != TransportInterface::OK) goto lost_connection;
          if (!islist && wr_value(thread, f->get_tag()) != OK) goto lost_connection;
          wr_value(kind, thread, dest, f-base);
        }
        dest = 0;
        if (islist)
          dest = f->iterate(listbase, elems++);
      }
      if (islist) {
        if (m_transport->bl_write_bytes(thread, 1,"]") != TransportInterface::OK) return Protocol::ERR_ENCODE;
      }
    }
  }
  if (m_transport->bl_write_bytes(thread, 1,"z") != TransportInterface::OK) goto lost_connection;
lost_connection:
  return (Status) m_transport->end_encoding(thread, immediate);
}

Status BinCommon::wr_bool(CommandInitiator*, const void*, uint)
{
  return OK;
}

Status BinCommon::wr_uint64(CommandInitiator* thread, const void* src, uint)
{
  return wr_value(thread, *reinterpret_cast<const uint64*>(src));
}

Status BinCommon::wr_value(CommandInitiator* thread, const uint64& val){
  uint64 value = val;
  int  sz = 0;
  char buf[(64+7)/7];
  for(;value>0x7f;value>>=7)
    buf[sz++] = (char)((value&0x7f)|0x80);
  buf[sz++]=(char)value;
  return (Status) m_transport->bl_write_bytes(thread, sz, &buf[0]);
}

Status BinCommon::wr_error(CommandInitiator*, const void*, uint)
{
  return ERR_ENCODE;
}

Status BinCommon::wr_parms(CommandInitiator* thread, bool immediate)
{
  m_transport->bl_write_bytes(thread, 1,"z");
  return (Status) m_transport->end_encoding(thread, immediate);
}

Status BinCommon::wr_preencoded(CommandInitiator* thread, const uint& header_size, const char* header, uint& id, const uint& add_id, const void* encoded_message, const uint& descriptor_begin, bool immediate)
{
  if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
  if (m_transport->bl_write_bytes(thread, header_size, header) != TransportInterface::OK) goto lost_connection;
  if (add_id == 2) id = m_rid;
  m_rid += add_id >> 1; 
  if (add_id && wr_value(thread, id) != OK) goto lost_connection;
  return wr_parms(thread, encoded_message, descriptor_begin, immediate);
lost_connection:
  return (Status) m_transport->end_encoding(thread, immediate);
}

Status BinCommon::wr_preencoded_lst(CommandInitiator* thread, const uint& header_size, const char* header, uint& id, const uint& add_id, const uint& descriptor_begin, bool immediate, va_list parms)
{
  if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
  if (m_transport->bl_write_bytes(thread, header_size, header) != TransportInterface::OK) goto lost_connection;
  if (add_id == 2) id = m_rid;
  m_rid += add_id >> 1; 
  if (add_id && wr_value(thread, id) != OK) goto lost_connection;
  return wr_parms_lst(thread, descriptor_begin, immediate, parms);
lost_connection:
  return (Status) m_transport->end_encoding(thread, immediate);
}

Status BinCommon::wr_preencoded(CommandInitiator* thread, const uint& header_size, const char* header, uint& id, const uint& add_id, const uint& footer_size, const char* footer, bool immediate) 
{
  if (m_transport->bl_start_encoding(thread) != TransportInterface::OK) goto lost_connection;
  if (m_transport->bl_write_bytes(thread, header_size,header) != TransportInterface::OK) goto lost_connection;
  if (add_id == 2) id = m_rid;
  m_rid += add_id >> 1; 
  if (add_id && wr_value(thread, id) != OK) goto lost_connection;
  m_transport->bl_write_bytes(thread, footer_size, footer);
lost_connection:
  return (Status) m_transport->end_encoding(thread, immediate);
}

} } // namepace Sid::Protocol

// encoders for strings and filenames
#include "SidProtocolBinCommonPlatform.cpp"


