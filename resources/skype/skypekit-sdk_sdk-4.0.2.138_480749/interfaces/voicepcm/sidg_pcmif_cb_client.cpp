#include "sidg_pcmif_cb_client.hpp"

namespace Sid {
using namespace SkypePCMCallbackInterfaceMsgs;
  void SkypePCMCallbackInterfaceClient::InputDeviceReady(int noOfSamples, int fs, int nChannels, const Binary& data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\001", rid, MsgInputDeviceReadyRequest::BEGIN,&noOfSamples,&fs,&nChannels,&data,NULL) != Protocol::OK) return;
  }
  void SkypePCMCallbackInterfaceClient::OutputDeviceReady(int noOfSamples, int fs, int nChannels, Binary& data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\002", rid, MsgOutputDeviceReadyRequest::BEGIN,&noOfSamples,&fs,&nChannels,NULL) != Protocol::OK) return;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return;
    if(m_protocol.rd_parms_lst(NULL, MsgOutputDeviceReadyResponse::BEGIN,&data,NULL)) return;
  }
  void SkypePCMCallbackInterfaceClient::NotificationDeviceReady(int noOfSamples, int fs, int nChannels, Binary& data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\003", rid, MsgNotificationDeviceReadyRequest::BEGIN,&noOfSamples,&fs,&nChannels,NULL) != Protocol::OK) return;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return;
    if(m_protocol.rd_parms_lst(NULL, MsgNotificationDeviceReadyResponse::BEGIN,&data,NULL)) return;
  }
  void SkypePCMCallbackInterfaceClient::InputAndOutputDeviceReady(int length_in_ms, int input_fs, int output_fs, int input_channels, int output_channels, const Binary& input_data, Binary& output_data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\004", rid, MsgInputAndOutputDeviceReadyRequest::BEGIN,&length_in_ms,&input_fs,&output_fs,&input_channels,&output_channels,&input_data,NULL) != Protocol::OK) return;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return;
    if(m_protocol.rd_parms_lst(NULL, MsgInputAndOutputDeviceReadyResponse::BEGIN,&output_data,NULL)) return;
  }
  void SkypePCMCallbackInterfaceClient::DeviceStopped(int deviceType){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\012", rid, MsgDeviceStoppedRequest::BEGIN,&deviceType,NULL) != Protocol::OK) return;
  }
  void SkypePCMCallbackInterfaceClient::DeviceListChanged(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call(NULL,4,"ZR\000\024", rid) != Protocol::OK) return;
  }
  void SkypePCMCallbackInterfaceClient::CurrentDeviceChanged(int deviceType, const String& guid, const String& name, const String& productID){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\025", rid, MsgCurrentDeviceChangedRequest::BEGIN,&deviceType,&guid,&name,&productID,NULL) != Protocol::OK) return;
  }
  void SkypePCMCallbackInterfaceClient::VolumeChanged(int deviceType, uint range_min, uint range_max, uint volume, int boost){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\037", rid, MsgVolumeChangedRequest::BEGIN,&deviceType,&range_min,&range_max,&volume,&boost,NULL) != Protocol::OK) return;
  }

} // namespace Sid
