#ifndef _Field_HPP_INCLUDED
#define _Field_HPP_INCLUDED

#include "SidPlatform.hpp"

namespace Sid {

  struct Field {
    enum Kind  { 
       RESERVED, 
       KIND_INT      = 'i', 
       KIND_UINT     = 'u',
       KIND_BOOLEAN  = 'b', // only T/F values are sent
       KIND_TRUE     = 'T',
       KIND_FALSE    = 'F',   
       KIND_STRING   = 'S',
       KIND_XML      = 'X', 
       KIND_ENUM     = 'e', 
       KIND_LIST     = '[',
       KIND_STRUCT   = '{',
       KIND_METATAG  = 'm', 
       KIND_METADATA = 'd', 
       KIND_BINARY   = 'B', 
       KIND_OPAQUE   = 'o', 
       KIND_FILENAME = 'f', 
       KIND_UINT64   = 'U', 
       KIND_OBJECTID = 'O', 
       KIND_TERM     = 't',
       KIND_NULL     = 'N', // not used in fields but in property-change... to mention absence of optional value
    };
    enum Flags { FLD_NO_FLAG    = 0, 
                 FLD_LIST       = 1,  // field is a list
                 FLD_FIRST      = 4,  // field is first of a message => offset is 0, and contains size of message...};
               };
    Kind kind() const                                { return (Kind) m_regular.m_kind;            }
    int is_list() const                              { return m_regular.m_flags & FLD_LIST;       }
    uint get_objecttype() const                      { /* assert(kind() == KIND_OBJECTID); */ return m_regular.m_field_info;     } 
    uint get_struct_offset() const                   { /* assert(kind() == KIND_STRUCT); */ return m_regular.m_field_info;     } 
    void* offset(const void* base) const             { if (m_regular.m_flags&FLD_FIRST) return (void*) base;
                                                       return (void*)(((char*) base) + m_regular.m_offset);        }
    uint  get_end() const                            { /*assert(m_flags & FLD_FIRST);*/ return m_regular.m_offset; }
    uint  get_tag() const                            { return m_regular.m_tag;                                }
    void  set_field_to_default(void* p) const        { (*m_regular.set_field_to_default)(p);                  }
    void  set_to_default(void* p) const              { (*m_regular.set_field_to_default)(offset(p));          }
    bool  equals_default(const void* p) const        { return (*m_regular.field_equals_default)(offset(p));   }
    bool  field_equals_default(const void* p) const  { return (*m_regular.field_equals_default)(p);           }
    void  reserve(void* list, uint size) const       { (*(this+1)->m_list_helpers.reserve)(list, size);       }
    void  append (void* list, uint size, void*&elem) const  { (*(this+1)->m_list_helpers.append)(list, size, elem);  }
    void* iterate(void* list, uint size) const       { return (*(this+1)->m_list_helpers.iterate)(list, size);}
    const Field* next() const                        { return this+1+(m_regular.m_flags & FLD_LIST); }

    union {
      struct {
        bool (*field_equals_default)(const void*);
        void (*set_field_to_default)(void*);
        unsigned short  m_tag;
        uchar           m_flags;      
        uchar           m_kind;
        unsigned short  m_offset; 
        unsigned short  m_field_info; 
      } m_regular;
      struct {
        void (*append) (void* list, uint size, void*&elem); // in case of list, a second Field row contains these helpers fct
        void*(*iterate)(void* list, uint size);
        void (*reserve)(void* list, uint size);
      } m_list_helpers;
    };
    static Field M_SkypePCMInterface_fields[]; 
    static Field M_SkypePCMCallbackInterface_fields[]; 
    static Field M_SkypeMultiRTPInterface_fields[]; 
    static Field M_SkypeMultiRTPCallbackInterface_fields[];
    static Field M_SkypeVideoRTPInterface_fields[]; 
    static Field M_SkypeVideoRTPInterfaceCb_fields[];
  };
}

#endif

