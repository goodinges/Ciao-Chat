#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_pcmif.hpp"
// constants
namespace Sid {
using namespace SkypePCMInterfaceMsgs;
static char* base_address 
#if defined(__GNUC__)
__attribute__ ((unused))
#endif
 =0;

static const int d1=0;
static bool d1field_equals_default(const void* ) { return 0;/*d1 == *reinterpret_cast<const int*>(p);*/}
static void d1set_field_to_default(void* p) { *reinterpret_cast<int*>(p) = d1;}
static const String d2="";
static bool d2field_equals_default(const void* ) { return 0;/*d2 == *reinterpret_cast<const String*>(p);*/}
static void d2set_field_to_default(void* p) { *reinterpret_cast<String*>(p) = d2;}
static const uint d3=0;
static bool d3field_equals_default(const void* ) { return 0;/*d3 == *reinterpret_cast<const uint*>(p);*/}
static void d3set_field_to_default(void* p) { *reinterpret_cast<uint*>(p) = d3;}
static bool d4field_equals_default(const void*) {
//  List_String* l = reinterpret_cast<List_String*>((void* )list);
//  return l->size()==0;
  return 0;
}
static void d4set_field_to_default(void* list) {
  List_String* l = reinterpret_cast<List_String*>(list);
  l->resize(0);
}
static void d4append(void* list, uint size, void*& elem) {
  List_String* l = reinterpret_cast<List_String*>(list);
  l->append(uint());
  elem = (void*) &(((const List_String*)l)->operator[](size));
}

static void* d4iterate(void* list, uint count) {
  List_String* l = reinterpret_cast<List_String*>(list);
  if (count >= l->size()) return 0;
  return (void*) &(((const List_String*)l)->operator[](count));
}


static bool d5field_equals_default(const void*) {
//  List_uint* l = reinterpret_cast<List_uint*>((void* )list);
//  return l->size()==0;
  return 0;
}
static void d5set_field_to_default(void* list) {
  List_uint* l = reinterpret_cast<List_uint*>(list);
  l->resize(0);
}
static void d5append(void* list, uint size, void*& elem) {
  List_uint* l = reinterpret_cast<List_uint*>(list);
  l->append(uint());
  elem = (void*) &(((const List_uint*)l)->operator[](size));
}

static void* d5iterate(void* list, uint count) {
  List_uint* l = reinterpret_cast<List_uint*>(list);
  if (count >= l->size()) return 0;
  return (void*) &(((const List_uint*)l)->operator[](count));
}


// field descriptors
Field Field::M_SkypePCMInterface_fields[] = {
  //// Init
  /*** MsgInitResponse */
  /*0*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgInitResponse::END,0} } },
  //// Start
  /*** MsgStartRequest */
  /*1*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartRequest::END,0} } },
  /*** MsgStartResponse */
  /*2*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStartResponse::END,0} } },
  //// Stop
  /*** MsgStopRequest */
  /*3*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStopRequest::END,0} } },
  /*** MsgStopResponse */
  /*4*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgStopResponse::END,0} } },
  //// GetDefaultDevice
  /*** MsgGetDefaultDeviceRequest */
  /*5*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetDefaultDeviceRequest::END,0} } },
  /*** MsgGetDefaultDeviceResponse */
  /*6*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetDefaultDeviceResponse::END,0} } },
  /*7*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetDefaultDeviceResponse*>(base_address)->guid,0} } },
  /*8*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetDefaultDeviceResponse*>(base_address)->name,0} } },
  /*9*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetDefaultDeviceResponse*>(base_address)->productID,0} } },
  //// UseDefaultDevice
  /*** MsgUseDefaultDeviceRequest */
  /*10*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgUseDefaultDeviceRequest::END,0} } },
  /*** MsgUseDefaultDeviceResponse */
  /*11*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgUseDefaultDeviceResponse::END,0} } },
  //// GetCurrentDevice
  /*** MsgGetCurrentDeviceRequest */
  /*12*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetCurrentDeviceRequest::END,0} } },
  /*** MsgGetCurrentDeviceResponse */
  /*13*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetCurrentDeviceResponse::END,0} } },
  /*14*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetCurrentDeviceResponse*>(base_address)->guid,0} } },
  /*15*/ { { {&d2field_equals_default,&d2set_field_to_default,3,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetCurrentDeviceResponse*>(base_address)->name,0} } },
  /*16*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetCurrentDeviceResponse*>(base_address)->productID,0} } },
  //// UseDevice
  /*** MsgUseDeviceRequest */
  /*17*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgUseDeviceRequest::END,0} } },
  /*18*/ { { {&d2field_equals_default,&d2set_field_to_default,2,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgUseDeviceRequest*>(base_address)->guid,0} } },
  /*** MsgUseDeviceResponse */
  /*19*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgUseDeviceResponse::END,0} } },
  //// GetDeviceCount
  /*** MsgGetDeviceCountRequest */
  /*20*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetDeviceCountRequest::END,0} } },
  /*** MsgGetDeviceCountResponse */
  /*21*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetDeviceCountResponse::END,0} } },
  /*22*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetDeviceCountResponse*>(base_address)->count,0} } },
  //// GetDevices
  /*** MsgGetDevicesRequest */
  /*23*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetDevicesRequest::END,0} } },
  /*** MsgGetDevicesResponse */
  /*24*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetDevicesResponse::END,0} } },
  /*25*/ { { {&d4field_equals_default,&d4set_field_to_default,2,Sid::Field::FLD_LIST,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetDevicesResponse*>(base_address)->guid,0} } },
  { { { reinterpret_cast<bool(*)(const void*)>(&d4append),reinterpret_cast<void(*)(void*)>(&d4iterate),0,0,0,0,0}}},
  /*27*/ { { {&d4field_equals_default,&d4set_field_to_default,3,Sid::Field::FLD_LIST,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetDevicesResponse*>(base_address)->name,0} } },
  { { { reinterpret_cast<bool(*)(const void*)>(&d4append),reinterpret_cast<void(*)(void*)>(&d4iterate),0,0,0,0,0}}},
  /*29*/ { { {&d4field_equals_default,&d4set_field_to_default,4,Sid::Field::FLD_LIST,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgGetDevicesResponse*>(base_address)->productID,0} } },
  { { { reinterpret_cast<bool(*)(const void*)>(&d4append),reinterpret_cast<void(*)(void*)>(&d4iterate),0,0,0,0,0}}},
  //// GetVolumeParameters
  /*** MsgGetVolumeParametersRequest */
  /*31*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetVolumeParametersRequest::END,0} } },
  /*** MsgGetVolumeParametersResponse */
  /*32*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetVolumeParametersResponse::END,0} } },
  /*33*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetVolumeParametersResponse*>(base_address)->range_min,0} } },
  /*34*/ { { {&d3field_equals_default,&d3set_field_to_default,3,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetVolumeParametersResponse*>(base_address)->range_max,0} } },
  /*35*/ { { {&d3field_equals_default,&d3set_field_to_default,4,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetVolumeParametersResponse*>(base_address)->volume,0} } },
  /*36*/ { { {&d1field_equals_default,&d1set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetVolumeParametersResponse*>(base_address)->boost,0} } },
  //// SetVolume
  /*** MsgSetVolumeRequest */
  /*37*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetVolumeRequest::END,0} } },
  /*38*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgSetVolumeRequest*>(base_address)->volume,0} } },
  /*** MsgSetVolumeResponse */
  /*39*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetVolumeResponse::END,0} } },
  //// SetInputBoost
  /*** MsgSetInputBoostRequest */
  /*40*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetInputBoostRequest::END,0} } },
  /*** MsgSetInputBoostResponse */
  /*41*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetInputBoostResponse::END,0} } },
  //// GetMute
  /*** MsgGetMuteRequest */
  /*42*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetMuteRequest::END,0} } },
  /*** MsgGetMuteResponse */
  /*43*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetMuteResponse::END,0} } },  /*44*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgGetMuteResponse*>(base_address)->muted,0} } },
  //// SetMute
  /*** MsgSetMuteRequest */
  /*45*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetMuteRequest::END,0} } },
  /*46*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetMuteRequest*>(base_address)->mute,0} } },
  /*** MsgSetMuteResponse */
  /*47*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetMuteResponse::END,0} } },
  //// GetSampleRateCount
  /*** MsgGetSampleRateCountRequest */
  /*48*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetSampleRateCountRequest::END,0} } },
  /*** MsgGetSampleRateCountResponse */
  /*49*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetSampleRateCountResponse::END,0} } },
  /*50*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetSampleRateCountResponse*>(base_address)->count,0} } },
  //// GetSupportedSampleRates
  /*** MsgGetSupportedSampleRatesRequest */
  /*51*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetSupportedSampleRatesRequest::END,0} } },
  /*** MsgGetSupportedSampleRatesResponse */
  /*52*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetSupportedSampleRatesResponse::END,0} } },
  /*53*/ { { {&d5field_equals_default,&d5set_field_to_default,2,Sid::Field::FLD_LIST,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetSupportedSampleRatesResponse*>(base_address)->sampleRates,0} } },
  { { { reinterpret_cast<bool(*)(const void*)>(&d5append),reinterpret_cast<void(*)(void*)>(&d5iterate),0,0,0,0,0}}},
  //// GetCurrentSampleRate
  /*** MsgGetCurrentSampleRateRequest */
  /*55*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetCurrentSampleRateRequest::END,0} } },
  /*** MsgGetCurrentSampleRateResponse */
  /*56*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgGetCurrentSampleRateResponse::END,0} } },
  /*57*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgGetCurrentSampleRateResponse*>(base_address)->sampleRate,0} } },
  //// SetSampleRate
  /*** MsgSetSampleRateRequest */
  /*58*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetSampleRateRequest::END,0} } },
  /*59*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgSetSampleRateRequest*>(base_address)->sampleRate,0} } },
  /*** MsgSetSampleRateResponse */
  /*60*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetSampleRateResponse::END,0} } },
  //// SetNumberOfChannels
  /*** MsgSetNumberOfChannelsRequest */
  /*61*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetNumberOfChannelsRequest::END,0} } },
  /*62*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgSetNumberOfChannelsRequest*>(base_address)->numberOfChannels,0} } },
  /*** MsgSetNumberOfChannelsResponse */
  /*63*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgSetNumberOfChannelsResponse::END,0} } },
  //// CustomCommand
  /*** MsgCustomCommandRequest */
  /*64*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCustomCommandRequest::END,0} } },
  /*** MsgCustomCommandResponse */
  /*65*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_STRING, MsgCustomCommandResponse::END,0} } },
  /*66*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgCustomCommandResponse*>(base_address)->rv,0} } },
};

} // namespace Sid
