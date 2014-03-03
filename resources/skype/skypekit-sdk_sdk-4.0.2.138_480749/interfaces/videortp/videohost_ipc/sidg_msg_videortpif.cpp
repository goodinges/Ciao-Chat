#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_videortpif.hpp"
// constants
namespace Sid {
using namespace SkypeVideoRTPInterfaceMsgs;
static char* base_address __attribute__ ((unused)) = 0;

static const bool d1=false;
static bool d1field_equals_default(const void* ) { return 0;/*d1 == *reinterpret_cast<const bool*>(p);*/}
static void d1set_field_to_default(void* p) { *reinterpret_cast<bool*>(p) = d1;}
static const int d2=0;
static bool d2field_equals_default(const void* ) { return 0;/*d2 == *reinterpret_cast<const int*>(p);*/}
static void d2set_field_to_default(void* p) { *reinterpret_cast<int*>(p) = d2;}
static const MsgVideoCodec d3;
static bool d3field_equals_default(const void* ) { return 0;/*d3 == *reinterpret_cast<const MsgVideoCodec*>(p);*/}
static void d3set_field_to_default(void* p) { *reinterpret_cast<MsgVideoCodec*>(p) = d3;}
static const MsgFixedRes d4;
static bool d4field_equals_default(const void* ) { return 0;/*d4 == *reinterpret_cast<const MsgFixedRes*>(p);*/}
static void d4set_field_to_default(void* p) { *reinterpret_cast<MsgFixedRes*>(p) = d4;}
static const MsgResCap d5;
static bool d5field_equals_default(const void* ) { return 0;/*d5 == *reinterpret_cast<const MsgResCap*>(p);*/}
static void d5set_field_to_default(void* p) { *reinterpret_cast<MsgResCap*>(p) = d5;}
static const String d6="";
static bool d6field_equals_default(const void* ) { return 0;/*d6 == *reinterpret_cast<const String*>(p);*/}
static void d6set_field_to_default(void* p) { *reinterpret_cast<String*>(p) = d6;}
static const Binary d7=Sid::Binary();
static bool d7field_equals_default(const void* ) { return 0;/*d7 == *reinterpret_cast<const Binary*>(p);*/}
static void d7set_field_to_default(void* p) { *reinterpret_cast<Binary*>(p) = d7;}
// field descriptors
Field Field::M_SkypeVideoRTPInterface_fields[] = {
  //// GetInterfaceVersion
  /*** MsgGetInterfaceVersionResponse */
  /*0*/ { { {&d1field_equals_default,&d1set_field_to_default,2,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetInterfaceVersionResponse::END,0} } },
  /*1*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetInterfaceVersionResponse*>(base_address)->version,0} } },
  //// GetLastError
  /*** MsgGetLastErrorResponse */
  /*2*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetLastErrorResponse::END,0} } },
  //// Init
  /*** MsgInitResponse */
  /*3*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgInitResponse::END,0} } },
  //// Uninit
  /*** MsgUninitResponse */
  /*4*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgUninitResponse::END,0} } },
  ////// VideoCodec
  /*** MsgVideoCodec */
  /*5*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgVideoCodec::END,0} } },
  /*6*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgVideoCodec*>(base_address)->recording_capable,0} } },
  /*7*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_ENUM, (unsigned short) (size_t) &reinterpret_cast<MsgVideoCodec*>(base_address)->payload_format,0} } },
  //// GetCodecCount
  /*** MsgGetCodecCountResponse */
  /*8*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetCodecCountResponse::END,0} } },
  /*9*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_BOOLEAN, (unsigned short) (size_t) &reinterpret_cast<MsgGetCodecCountResponse*>(base_address)->rv,0} } },
  //// GetCodec
  /*** MsgGetCodecRequest */
  /*10*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetCodecRequest::END,0} } },
  /*** MsgGetCodecResponse */
  /*11*/ { { {&d1field_equals_default,&d1set_field_to_default,3,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetCodecResponse::END,0} } },
  /*12*/ { { {&d3field_equals_default,&d3set_field_to_default,4,0,Sid::Field::KIND_STRUCT, (unsigned short) (size_t) &reinterpret_cast<MsgGetCodecResponse*>(base_address)->codec,MsgVideoCodec::BEGIN} } },
  ////// FixedRes
  /*** MsgFixedRes */
  /*13*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgFixedRes::END,0} } },
  /*14*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgFixedRes*>(base_address)->height,0} } },
  /*15*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgFixedRes*>(base_address)->preference,0} } },
  ////// ResCap
  /*** MsgResCap */
  /*16*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgResCap::END,0} } },
  /*17*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgResCap*>(base_address)->max_height,0} } },
  /*18*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgResCap*>(base_address)->min_width,0} } },
  /*19*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgResCap*>(base_address)->min_height,0} } },
  /*20*/ { { {&d2field_equals_default,&d2set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgResCap*>(base_address)->preference,0} } },
  //// GetSenderResolutionCount
  /*** MsgGetSenderResolutionCountResponse */
  /*21*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetSenderResolutionCountResponse::END,0} } },
  /*22*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetSenderResolutionCountResponse*>(base_address)->resolution_count,0} } },
  //// GetSenderResolution
  /*** MsgGetSenderResolutionRequest */
  /*23*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetSenderResolutionRequest::END,0} } },
  /*** MsgGetSenderResolutionResponse */
  /*24*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetSenderResolutionResponse::END,0} } },
  /*25*/ { { {&d4field_equals_default,&d4set_field_to_default,2,0,Sid::Field::KIND_STRUCT, (unsigned short) (size_t) &reinterpret_cast<MsgGetSenderResolutionResponse*>(base_address)->resolution,MsgFixedRes::BEGIN} } },
  //// GetReceiverResolutionCount
  /*** MsgGetReceiverResolutionCountResponse */
  /*26*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetReceiverResolutionCountResponse::END,0} } },
  /*27*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetReceiverResolutionCountResponse*>(base_address)->resolution_count,0} } },
  //// GetReceiverResolution
  /*** MsgGetReceiverResolutionRequest */
  /*28*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetReceiverResolutionRequest::END,0} } },
  /*** MsgGetReceiverResolutionResponse */
  /*29*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetReceiverResolutionResponse::END,0} } },
  /*30*/ { { {&d5field_equals_default,&d5set_field_to_default,2,0,Sid::Field::KIND_STRUCT, (unsigned short) (size_t) &reinterpret_cast<MsgGetReceiverResolutionResponse*>(base_address)->resolution,MsgResCap::BEGIN} } },
  //// GetIntProperty
  /*** MsgGetIntPropertyRequest */
  /*31*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetIntPropertyRequest::END,0} } },
  /*32*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetIntPropertyRequest*>(base_address)->index,0} } },
  /*** MsgGetIntPropertyResponse */
  /*33*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetIntPropertyResponse::END,0} } },
  /*34*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_BOOLEAN, (unsigned short) (size_t) &reinterpret_cast<MsgGetIntPropertyResponse*>(base_address)->rv,0} } },
  //// SetIntProperty
  /*** MsgSetIntPropertyRequest */
  /*35*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetIntPropertyRequest::END,0} } },
  /*36*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetIntPropertyRequest*>(base_address)->index,0} } },
  /*37*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetIntPropertyRequest*>(base_address)->value,0} } },
  /*** MsgSetIntPropertyResponse */
  /*38*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgSetIntPropertyResponse::END,0} } },
  //// GetStringProperty
  /*** MsgGetStringPropertyRequest */
  /*39*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetStringPropertyRequest::END,0} } },
  /*40*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetStringPropertyRequest*>(base_address)->index,0} } },
  /*** MsgGetStringPropertyResponse */
  /*41*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgGetStringPropertyResponse::END,0} } },  /*42*/ { { {&d6field_equals_default,&d6set_field_to_default,2,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetStringPropertyResponse*>(base_address)->value,0} } },
  //// SetStringProperty
  /*** MsgSetStringPropertyRequest */
  /*43*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetStringPropertyRequest::END,0} } },
  /*44*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetStringPropertyRequest*>(base_address)->index,0} } },
  /*45*/ { { {&d6field_equals_default,&d6set_field_to_default,3,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgSetStringPropertyRequest*>(base_address)->value,0} } },
  /*** MsgSetStringPropertyResponse */
  /*46*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgSetStringPropertyResponse::END,0} } },
  ////// H264SenderParameters
  /*** MsgH264SenderParameters */
  /*47*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgH264SenderParameters::END,0} } },
  /*48*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_ENUM, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderParameters*>(base_address)->profile_iop,0} } },
  /*49*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderParameters*>(base_address)->level_idc,0} } },
  ////// H264ReceiverParameters
  /*** MsgH264ReceiverParameters */
  /*50*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgH264ReceiverParameters::END,0} } },
  /*51*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_ENUM, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->profile_iop,0} } },
  /*52*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->level_idc,0} } },
  /*53*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->max_mbps,0} } },
  /*54*/ { { {&d2field_equals_default,&d2set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->max_fs,0} } },
  /*55*/ { { {&d2field_equals_default,&d2set_field_to_default,6,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->max_cpb,0} } },
  /*56*/ { { {&d2field_equals_default,&d2set_field_to_default,7,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->max_dpb,0} } },
  /*57*/ { { {&d2field_equals_default,&d2set_field_to_default,8,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->max_br,0} } },
  /*58*/ { { {&d2field_equals_default,&d2set_field_to_default,9,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->max_smbps,0} } },
  /*59*/ { { {&d2field_equals_default,&d2set_field_to_default,10,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264ReceiverParameters*>(base_address)->redundant_pic_cap,0} } },
  ////// H264SenderConstraints
  /*** MsgH264SenderConstraints */
  /*60*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgH264SenderConstraints::END,0} } },
  /*61*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_ENUM, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->profile_iop,0} } },
  /*62*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->level_idc,0} } },
  /*63*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->max_mbps,0} } },
  /*64*/ { { {&d2field_equals_default,&d2set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->max_fs,0} } },
  /*65*/ { { {&d2field_equals_default,&d2set_field_to_default,6,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->max_cpb,0} } },
  /*66*/ { { {&d2field_equals_default,&d2set_field_to_default,7,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->max_dpb,0} } },
  /*67*/ { { {&d2field_equals_default,&d2set_field_to_default,8,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->max_br,0} } },
  /*68*/ { { {&d2field_equals_default,&d2set_field_to_default,9,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->max_smbps,0} } },
  /*69*/ { { {&d2field_equals_default,&d2set_field_to_default,10,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgH264SenderConstraints*>(base_address)->redundant_pic_cap,0} } },
  //// GetBinaryProperty
  /*** MsgGetBinaryPropertyRequest */
  /*70*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetBinaryPropertyRequest::END,0} } },
  /*71*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetBinaryPropertyRequest*>(base_address)->index,0} } },
  /*** MsgGetBinaryPropertyResponse */
  /*72*/ { { {&d7field_equals_default,&d7set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgGetBinaryPropertyResponse::END,0} } },
  /*73*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_BOOLEAN, (unsigned short) (size_t) &reinterpret_cast<MsgGetBinaryPropertyResponse*>(base_address)->rv,0} } },
  //// SetBinaryProperty
  /*** MsgSetBinaryPropertyRequest */
  /*74*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetBinaryPropertyRequest::END,0} } },
  /*75*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetBinaryPropertyRequest*>(base_address)->index,0} } },
  /*76*/ { { {&d7field_equals_default,&d7set_field_to_default,3,0,Sid::Field::KIND_BINARY, (unsigned short) (size_t) &reinterpret_cast<MsgSetBinaryPropertyRequest*>(base_address)->value,0} } },
  /*** MsgSetBinaryPropertyResponse */
  /*77*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgSetBinaryPropertyResponse::END,0} } },
  //// RequestKeyFrame
  /*** MsgRequestKeyFrameResponse */
  /*78*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgRequestKeyFrameResponse::END,0} } },
  //// RequestRecoveryFrame
  /*** MsgRequestRecoveryFrameRequest */
  /*79*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgRequestRecoveryFrameRequest::END,0} } },
  /*** MsgRequestRecoveryFrameResponse */
  /*80*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgRequestRecoveryFrameResponse::END,0} } },
  //// SetBitrate
  /*** MsgSetBitrateRequest */
  /*81*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetBitrateRequest::END,0} } },
  /*** MsgSetBitrateResponse */
  /*82*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgSetBitrateResponse::END,0} } },
  //// SetFramerate
  /*** MsgSetFramerateRequest */
  /*83*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetFramerateRequest::END,0} } },  /*** MsgSetFramerateResponse */
  /*84*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgSetFramerateResponse::END,0} } },
  //// SetMaxPacketSize
  /*** MsgSetMaxPacketSizeRequest */
  /*85*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetMaxPacketSizeRequest::END,0} } },
  /*** MsgSetMaxPacketSizeResponse */
  /*86*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgSetMaxPacketSizeResponse::END,0} } },
  //// StartPlayback
  /*** MsgStartPlaybackRequest */
  /*87*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartPlaybackRequest::END,0} } },
  /*88*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartPlaybackRequest*>(base_address)->width,0} } },
  /*89*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartPlaybackRequest*>(base_address)->height,0} } },
  /*** MsgStartPlaybackResponse */
  /*90*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgStartPlaybackResponse::END,0} } },
  //// RtpPacketReceived
  /*** MsgRtpPacketReceivedRequest */
  /*91*/ { { {&d7field_equals_default,&d7set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgRtpPacketReceivedRequest::END,0} } },
  //// FrameReceived
  /*** MsgFrameReceivedRequest */
  /*92*/ { { {&d7field_equals_default,&d7set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgFrameReceivedRequest::END,0} } },
  /*93*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgFrameReceivedRequest*>(base_address)->timestamp_ms,0} } },
  //// StopPlayback
  /*** MsgStopPlaybackResponse */
  /*94*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgStopPlaybackResponse::END,0} } },
  //// StartRecording
  /*** MsgStartRecordingRequest */
  /*95*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartRecordingRequest::END,0} } },
  /*96*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartRecordingRequest*>(base_address)->initial_max_packet_size,0} } },
  /*97*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartRecordingRequest*>(base_address)->initial_bitrate_bytes_sec,0} } },
  /*98*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartRecordingRequest*>(base_address)->width,0} } },
  /*99*/ { { {&d2field_equals_default,&d2set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartRecordingRequest*>(base_address)->height,0} } },
  /*100*/ { { {&d2field_equals_default,&d2set_field_to_default,6,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgStartRecordingRequest*>(base_address)->initial_framerate_fps,0} } },
  /*** MsgStartRecordingResponse */
  /*101*/ { { {&d1field_equals_default,&d1set_field_to_default,3,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgStartRecordingResponse::END,0} } },
  //// StopRecording
  /*** MsgStopRecordingResponse */
  /*102*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgStopRecordingResponse::END,0} } },
  //// CustomCommand
  /*** MsgCustomCommandRequest */
  /*103*/ { { {&d6field_equals_default,&d6set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCustomCommandRequest::END,0} } },
  /*** MsgCustomCommandResponse */
  /*104*/ { { {&d6field_equals_default,&d6set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCustomCommandResponse::END,0} } },
  /*105*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_BOOLEAN, (unsigned short) (size_t) &reinterpret_cast<MsgCustomCommandResponse*>(base_address)->rv,0} } },
  //// VideoCallStart
  /*** MsgVideoCallStartResponse */
  /*106*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgVideoCallStartResponse::END,0} } },
  //// VideoCallEnd
  /*** MsgVideoCallEndResponse */
  /*107*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BOOLEAN, MsgVideoCallEndResponse::END,0} } },
};

} // namespace Sid
