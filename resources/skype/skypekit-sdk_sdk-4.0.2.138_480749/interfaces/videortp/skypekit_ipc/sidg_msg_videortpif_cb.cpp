#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_videortpif_cb.hpp"
// constants
namespace Sid {
using namespace SkypeVideoRTPInterfaceCbMsgs;
static char* base_address __attribute__ ((unused)) = 0;

static const Binary d1=Sid::Binary();
static bool d1field_equals_default(const void* ) { return 0;/*d1 == *reinterpret_cast<const Binary*>(p);*/}
static void d1set_field_to_default(void* p) { *reinterpret_cast<Binary*>(p) = d1;}
static const int d2=0;
static bool d2field_equals_default(const void* ) { return 0;/*d2 == *reinterpret_cast<const int*>(p);*/}
static void d2set_field_to_default(void* p) { *reinterpret_cast<int*>(p) = d2;}
// field descriptors
Field Field::M_SkypeVideoRTPInterfaceCb_fields[] = {
  //// SendRTPPacket
  /*** MsgSendRTPPacketRequest */
  /*0*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgSendRTPPacketRequest::END,0} } },
  //// SendFrame
  /*** MsgSendFrameRequest */
  /*1*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgSendFrameRequest::END,0} } },
  /*2*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSendFrameRequest*>(base_address)->timestamp_ms,0} } },
};

} // namespace Sid
