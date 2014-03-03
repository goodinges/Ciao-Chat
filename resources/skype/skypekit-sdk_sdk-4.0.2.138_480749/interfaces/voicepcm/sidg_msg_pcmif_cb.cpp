#include "SidPlatform.hpp"
#include "SidField.hpp"
#include "sidg_msg_pcmif_cb.hpp"
// constants
namespace Sid {
using namespace SkypePCMCallbackInterfaceMsgs;
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
static const String d3="";
static bool d3field_equals_default(const void* ) { return 0;/*d3 == *reinterpret_cast<const String*>(p);*/}
static void d3set_field_to_default(void* p) { *reinterpret_cast<String*>(p) = d3;}
static const uint d4=0;
static bool d4field_equals_default(const void* ) { return 0;/*d4 == *reinterpret_cast<const uint*>(p);*/}
static void d4set_field_to_default(void* p) { *reinterpret_cast<uint*>(p) = d4;}
// field descriptors
Field Field::M_SkypePCMCallbackInterface_fields[] = {
    //// InputDeviceReady
    /*** MsgInputDeviceReadyRequest */
    /*0*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgInputDeviceReadyRequest::END,0} } },
    /*1*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgInputDeviceReadyRequest*>(base_address)->fs,0} } },
    /*2*/ { { {&d1field_equals_default,&d1set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgInputDeviceReadyRequest*>(base_address)->nChannels,0} } },
    /*3*/ { { {&d2field_equals_default,&d2set_field_to_default,4,0,Sid::Field::KIND_BINARY, (unsigned short) (size_t) &reinterpret_cast<MsgInputDeviceReadyRequest*>(base_address)->data,0} } },
    // request
    //// OutputDeviceReady
    /*** MsgOutputDeviceReadyRequest */
    /*4*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgOutputDeviceReadyRequest::END,0} } },
    /*5*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgOutputDeviceReadyRequest*>(base_address)->fs,0} } },
    /*6*/ { { {&d1field_equals_default,&d1set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgOutputDeviceReadyRequest*>(base_address)->nChannels,0} } },
    // request
    /*** MsgOutputDeviceReadyResponse */
    /*7*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgOutputDeviceReadyResponse::END,0} } },
    // response
    //// NotificationDeviceReady
    /*** MsgNotificationDeviceReadyRequest */
    /*8*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgNotificationDeviceReadyRequest::END,0} } },
    /*9*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgNotificationDeviceReadyRequest*>(base_address)->fs,0} } },
    /*10*/ { { {&d1field_equals_default,&d1set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgNotificationDeviceReadyRequest*>(base_address)->nChannels,0} } },
    // request
    /*** MsgNotificationDeviceReadyResponse */
    /*11*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgNotificationDeviceReadyResponse::END,0} } },
    // response
    //// InputAndOutputDeviceReady
    /*** MsgInputAndOutputDeviceReadyRequest */
    /*12*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgInputAndOutputDeviceReadyRequest::END,0} } },
    /*13*/ { { {&d1field_equals_default,&d1set_field_to_default,2,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgInputAndOutputDeviceReadyRequest*>(base_address)->input_fs,0} } },
    /*14*/ { { {&d1field_equals_default,&d1set_field_to_default,3,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgInputAndOutputDeviceReadyRequest*>(base_address)->output_fs,0} } },
    /*15*/ { { {&d1field_equals_default,&d1set_field_to_default,4,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgInputAndOutputDeviceReadyRequest*>(base_address)->input_channels,0} } },
    /*16*/ { { {&d1field_equals_default,&d1set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgInputAndOutputDeviceReadyRequest*>(base_address)->output_channels,0} } },
    /*17*/ { { {&d2field_equals_default,&d2set_field_to_default,6,0,Sid::Field::KIND_BINARY, (unsigned short) (size_t) &reinterpret_cast<MsgInputAndOutputDeviceReadyRequest*>(base_address)->input_data,0} } },
    // request
    /*** MsgInputAndOutputDeviceReadyResponse */
    /*18*/ { { {&d2field_equals_default,&d2set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_BINARY, MsgInputAndOutputDeviceReadyResponse::END,0} } },
    // response
    //// DeviceStopped
    /*** MsgDeviceStoppedRequest */
    /*19*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgDeviceStoppedRequest::END,0} } },
    // request
    //// DeviceListChanged
    //// CurrentDeviceChanged
    /*** MsgCurrentDeviceChangedRequest */
    /*20*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgCurrentDeviceChangedRequest::END,0} } },
    /*21*/ { { {&d3field_equals_default,&d3set_field_to_default,2,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgCurrentDeviceChangedRequest*>(base_address)->guid,0} } },
    /*22*/ { { {&d3field_equals_default,&d3set_field_to_default,3,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgCurrentDeviceChangedRequest*>(base_address)->name,0} } },
    /*23*/ { { {&d3field_equals_default,&d3set_field_to_default,4,0,Sid::Field::KIND_STRING, (unsigned short) (size_t) &reinterpret_cast<MsgCurrentDeviceChangedRequest*>(base_address)->productID,0} } },
    // request
    //// VolumeChanged
    /*** MsgVolumeChangedRequest */
    /*24*/ { { {&d1field_equals_default,&d1set_field_to_default,1,Sid::Field::FLD_FIRST,Sid::Field::KIND_INT, MsgVolumeChangedRequest::END,0} } },
    /*25*/ { { {&d4field_equals_default,&d4set_field_to_default,2,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgVolumeChangedRequest*>(base_address)->range_min,0} } },
    /*26*/ { { {&d4field_equals_default,&d4set_field_to_default,3,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgVolumeChangedRequest*>(base_address)->range_max,0} } },
    /*27*/ { { {&d4field_equals_default,&d4set_field_to_default,4,0,Sid::Field::KIND_UINT, (unsigned short) (size_t) &reinterpret_cast<MsgVolumeChangedRequest*>(base_address)->volume,0} } },
    /*28*/ { { {&d1field_equals_default,&d1set_field_to_default,5,0,Sid::Field::KIND_INT, (unsigned short) (size_t) &reinterpret_cast<MsgVolumeChangedRequest*>(base_address)->boost,0} } },
    // request
  };
  
  } // namespace Sid
