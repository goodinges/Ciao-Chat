#include "SidPlatform.hpp"
#include "SidField.hpp"
#include "sidg_msg_multirtpif_cb.hpp"
// constants
namespace Sid {
using namespace SkypeMultiRTPCallbackInterfaceMsgs;
static char* base_address 
#if defined(__GNUC__)
__attribute__ ((unused))
#endif
 =0;

static const int d1=0;
static bool d1field_equals_default(const void* ) { return 0;/*d1 == *reinterpret_cast<const int*>(p);*/}
static void d1set_field_to_default(void* p) { *reinterpret_cast<int*>(p) = d1;}
static const Binary d2=Sid::Binary();
static bool d2field_equals_default(const void* ) { return 0;/*d2 == *reinterpret_cast<const Binary*>(p);*/}
static void d2set_field_to_default(void* p) { *reinterpret_cast<Binary*>(p) = d2;}
// field descriptors
Field Field::M_SkypeMultiRTPCallbackInterface_fields[] = {
    //// SendRTPPacket
    /*** MsgSendRTPPacketRequest */
    /*0*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSendRTPPacketRequest::END,0} } },
    /*1*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_BINARY, (unsigned short) (size_t) &reinterpret_cast<MsgSendRTPPacketRequest*>(base_address)->data,0} } },
    // request
  };
  
  } // namespace Sid
