#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_multirtpif.hpp"
// constants
namespace Sid {
using namespace SkypeMultiRTPInterfaceMsgs;
static char* base_address 
#if defined(__GNUC__)
__attribute__ ((unused))
#endif
 =0;


static const String d1="";
static bool d1field_equals_default(const void* ) { return 0;/*d1 == *reinterpret_cast<const String*>(p);*/}
static void d1set_field_to_default(void* p) { *reinterpret_cast<String*>(p) = d1;}
static const int d2=0;
static bool d2field_equals_default(const void* ) { return 0;/*d2 == *reinterpret_cast<const int*>(p);*/}
static void d2set_field_to_default(void* p) { *reinterpret_cast<int*>(p) = d2;}
static bool d3field_equals_default(const void*) {
//  List_uint* l = reinterpret_cast<List_uint*>((void* )list);
//  return l->size()==0;
  return 0;
}
static void d3set_field_to_default(void* list) {
  List_uint* l = reinterpret_cast<List_uint*>(list);
  l->resize(0);
}
static void d3append(void* list, uint size, void*& elem) {
  List_uint* l = reinterpret_cast<List_uint*>(list);
  l->append(uint());
  elem = (void*) &(((const List_uint*)l)->operator[](size));
}

static void* d3iterate(void* list, uint count) {
  List_uint* l = reinterpret_cast<List_uint*>(list);
  if (count >= l->size()) return 0;
  return (void*) &(((const List_uint*)l)->operator[](count));
}


static const uint d4=0;
static bool d4field_equals_default(const void* ) { return 0;/*d4 == *reinterpret_cast<const uint*>(p);*/}
static void d4set_field_to_default(void* p) { *reinterpret_cast<uint*>(p) = d4;}
static const MsgCodec d5;
static bool d5field_equals_default(const void* ) { return 0;/*d5 == *reinterpret_cast<const MsgCodec*>(p);*/}
static void d5set_field_to_default(void* p) { *reinterpret_cast<MsgCodec*>(p) = d5;}
static const Binary d6=Sid::Binary();
static bool d6field_equals_default(const void* ) { return 0;/*d6 == *reinterpret_cast<const Binary*>(p);*/}
static void d6set_field_to_default(void* p) { *reinterpret_cast<Binary*>(p) = d6;}
// field descriptors
Field Field::M_SkypeMultiRTPInterface_fields[] = {
  ////// Codec
  /*** MsgCodec */
  /*0*/ { { {&d1field_equals_default,&d1set_field_to_default,10,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCodec::END,0} } },
  /*1*/ { { {&d2field_equals_default,&d2set_field_to_default,11,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->PayloadType,0} } },
  /*2*/ { { {&d2field_equals_default,&d2set_field_to_default,12,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->Freq,0} } },
  /*3*/ { { {&d2field_equals_default,&d2set_field_to_default,13,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->Channels,0} } },
  /*4*/ { { {&d3field_equals_default,&d3set_field_to_default,14,Sid::Field::FLD_LIST,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->SupportedPacketSizes,0} } },
  { { { reinterpret_cast<bool(*)(const void*)>(&d3append),reinterpret_cast<void(*)(void*)>(&d3iterate),0,0,0,0,0}}},
  /*6*/ { { {&d2field_equals_default,&d2set_field_to_default,15,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->DefaultPacketSize,0} } },
  /*7*/ { { {&d2field_equals_default,&d2set_field_to_default,16,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->MinBitrate,0} } },
  /*8*/ { { {&d2field_equals_default,&d2set_field_to_default,17,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->MaxBitrate,0} } },
  /*9*/ { { {&d2field_equals_default,&d2set_field_to_default,18,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCodec*>(base_address)->MaxDesiredPacketLoss,0} } },
  //// GetVersion
  /*** MsgGetVersionResponse */
  /*10*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgGetVersionResponse::END,0} } },
  /*11*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetVersionResponse*>(base_address)->rv,0} } },
  //// GetLastError
  /*** MsgGetLastErrorResponse */
  /*12*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetLastErrorResponse::END,0} } },
  //// Init
  /*** MsgInitResponse */
  /*13*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgInitResponse::END,0} } },
  //// Uninit
  /*** MsgUninitResponse */
  /*14*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgUninitResponse::END,0} } },
  //// CreateChannel
  /*** MsgCreateChannelRequest */
  /*15*/ { { {&d4field_equals_default,&d4set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_UINT, MsgCreateChannelRequest::END,0} } },
  /*16*/ { { {&d4field_equals_default,&d4set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgCreateChannelRequest*>(base_address)->conversation_id,0} } },
  /*** MsgCreateChannelResponse */
  /*17*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgCreateChannelResponse::END,0} } },
  /*18*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCreateChannelResponse*>(base_address)->rv,0} } },
  //// DeleteChannel
  /*** MsgDeleteChannelRequest */
  /*19*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgDeleteChannelRequest::END,0} } },
  /*** MsgDeleteChannelResponse */
  /*20*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgDeleteChannelResponse::END,0} } },
  //// ChangeConversationId
  /*** MsgChangeConversationIdRequest */
  /*21*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgChangeConversationIdRequest::END,0} } },
  /*22*/ { { {&d4field_equals_default,&d4set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgChangeConversationIdRequest*>(base_address)->conversation_id,0} } },
  /*23*/ { { {&d4field_equals_default,&d4set_field_to_default,3,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgChangeConversationIdRequest*>(base_address)->call_member_id,0} } },
  /*** MsgChangeConversationIdResponse */
  /*24*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgChangeConversationIdResponse::END,0} } },
  //// GetCodec
  /*** MsgGetCodecRequest */
  /*25*/ { { {&d4field_equals_default,&d4set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_UINT, MsgGetCodecRequest::END,0} } },
  /*** MsgGetCodecResponse */
  /*26*/ { { {&d5field_equals_default,&d5set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRUCT, MsgGetCodecResponse::END,MsgCodec::BEGIN} } },
  /*27*/ { { {&d4field_equals_default,&d4set_field_to_default,20,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetCodecResponse*>(base_address)->rv,0} } },
  //// GetCodecCount
  /*** MsgGetCodecCountResponse */
  /*28*/ { { {&d4field_equals_default,&d4set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_UINT, MsgGetCodecCountResponse::END,0} } },
  /*29*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetCodecCountResponse*>(base_address)->rv,0} } },
  //// SetSendCodec
  /*** MsgSetSendCodecRequest */
  /*30*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetSendCodecRequest::END,0} } },
  /*31*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgSetSendCodecRequest*>(base_address)->Name,0} } },
  /*32*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetSendCodecRequest*>(base_address)->PacketSize,0} } },
  /*33*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetSendCodecRequest*>(base_address)->BitRate,0} } },
  /*34*/ { { {&d2field_equals_default,&d2set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetSendCodecRequest*>(base_address)->PayloadType,0} } },
  /*** MsgSetSendCodecResponse */
  /*35*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetSendCodecResponse::END,0} } },
  //// InformSendPacketLoss
  /*** MsgInformSendPacketLossRequest */
  /*36*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgInformSendPacketLossRequest::END,0} } },
  /*37*/ { { {&d4field_equals_default,&d4set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgInformSendPacketLossRequest*>(base_address)->packetLossBeforeFEC,0} } },
  /*38*/ { { {&d4field_equals_default,&d4set_field_to_default,3,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgInformSendPacketLossRequest*>(base_address)->packetLossAfterFEC,0} } },
  /*39*/ { { {&d4field_equals_default,&d4set_field_to_default,4,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgInformSendPacketLossRequest*>(base_address)->jitterBeforeFEC,0} } },
  /*40*/ { { {&d4field_equals_default,&d4set_field_to_default,5,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgInformSendPacketLossRequest*>(base_address)->jitterAfterFEC,0} } },
  //// StartPlayback
  /*** MsgStartPlaybackRequest */
  /*41*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartPlaybackRequest::END,0} } },
  /*** MsgStartPlaybackResponse */
  /*42*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartPlaybackResponse::END,0} } },  //// StopPlayback
  /*** MsgStopPlaybackRequest */
  /*43*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStopPlaybackRequest::END,0} } },
  /*** MsgStopPlaybackResponse */
  /*44*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStopPlaybackResponse::END,0} } },
  //// StartRecording
  /*** MsgStartRecordingRequest */
  /*45*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartRecordingRequest::END,0} } },
  /*** MsgStartRecordingResponse */
  /*46*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartRecordingResponse::END,0} } },
  //// StopRecording
  /*** MsgStopRecordingRequest */
  /*47*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStopRecordingRequest::END,0} } },
  /*** MsgStopRecordingResponse */
  /*48*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStopRecordingResponse::END,0} } },
  //// SubmitRTPPacket
  /*** MsgSubmitRTPPacketRequest */
  /*49*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSubmitRTPPacketRequest::END,0} } },
  /*50*/ { { {&d6field_equals_default,&d6set_field_to_default,2,0,Sid::Field::KIND_BINARY, (unsigned short) (size_t) &reinterpret_cast<MsgSubmitRTPPacketRequest*>(base_address)->data,0} } },
  //// SendDTMF
  /*** MsgSendDTMFRequest */
  /*51*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSendDTMFRequest::END,0} } },
  /*52*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSendDTMFRequest*>(base_address)->eventnr,0} } },
  /*53*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSendDTMFRequest*>(base_address)->rtp_type,0} } },
  /*** MsgSendDTMFResponse */
  /*54*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSendDTMFResponse::END,0} } },
  //// SetOutputAmplification
  /*** MsgSetOutputAmplificationRequest */
  /*55*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetOutputAmplificationRequest::END,0} } },
  /*56*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetOutputAmplificationRequest*>(base_address)->scale,0} } },
  /*** MsgSetOutputAmplificationResponse */
  /*57*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetOutputAmplificationResponse::END,0} } },
  //// CustomCommand
  /*** MsgCustomCommandRequest */
  /*58*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCustomCommandRequest::END,0} } },
  /*** MsgCustomCommandResponse */
  /*59*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCustomCommandResponse::END,0} } },
  /*60*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCustomCommandResponse*>(base_address)->rv,0} } },
};

} // namespace Sid
