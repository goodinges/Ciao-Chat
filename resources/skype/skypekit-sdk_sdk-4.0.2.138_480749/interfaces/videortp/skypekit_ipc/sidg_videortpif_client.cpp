#include "sidg_videortpif_client.hpp"

namespace Sid {
using namespace SkypeVideoRTPInterfaceMsgs;
  bool SkypeVideoRTPInterfaceClient::GetInterfaceVersion(int& version){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\001", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetInterfaceVersionResponse::BEGIN,&rv,&version,NULL)) return 0;
    return rv;
  }
  int SkypeVideoRTPInterfaceClient::GetLastError(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    int error;
    if (m_protocol.wr_call(NULL,4,"ZR\000\002", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetLastErrorResponse::BEGIN,&error,NULL)) return 0;
    return error;
  }
  bool SkypeVideoRTPInterfaceClient::Init(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\003", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgInitResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::Uninit(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\004", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgUninitResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetCodecCount(int& codec_count){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\005", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetCodecCountResponse::BEGIN,&codec_count,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetCodec(int index, SkypeVideoRTPInterface::VideoCodec& codec){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\006", rid, MsgGetCodecRequest::BEGIN,&index,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetCodecResponse::BEGIN,&rv,&codec,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetSenderResolutionCount(int& resolution_count){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\036", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetSenderResolutionCountResponse::BEGIN,&rv,&resolution_count,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetSenderResolution(int index, SkypeVideoRTPInterface::FixedRes& resolution){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\037", rid, MsgGetSenderResolutionRequest::BEGIN,&index,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetSenderResolutionResponse::BEGIN,&rv,&resolution,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetReceiverResolutionCount(int& resolution_count){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000 ", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetReceiverResolutionCountResponse::BEGIN,&rv,&resolution_count,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetReceiverResolution(int index, SkypeVideoRTPInterface::ResCap& resolution){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000!", rid, MsgGetReceiverResolutionRequest::BEGIN,&index,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetReceiverResolutionResponse::BEGIN,&rv,&resolution,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetIntProperty(int prop_id, int index, int& value){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\007", rid, MsgGetIntPropertyRequest::BEGIN,&prop_id,&index,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetIntPropertyResponse::BEGIN,&value,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::SetIntProperty(int prop_id, int index, int value){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\010", rid, MsgSetIntPropertyRequest::BEGIN,&prop_id,&index,&value,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgSetIntPropertyResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetStringProperty(int prop_id, int index, String& value){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\011", rid, MsgGetStringPropertyRequest::BEGIN,&prop_id,&index,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetStringPropertyResponse::BEGIN,&rv,&value,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::SetStringProperty(int prop_id, int index, const String& value){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\012", rid, MsgSetStringPropertyRequest::BEGIN,&prop_id,&index,&value,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgSetStringPropertyResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::GetBinaryProperty(int prop_id, int index, Binary& value){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\013", rid, MsgGetBinaryPropertyRequest::BEGIN,&prop_id,&index,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgGetBinaryPropertyResponse::BEGIN,&value,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::SetBinaryProperty(int prop_id, int index, const Binary& value){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\014", rid, MsgSetBinaryPropertyRequest::BEGIN,&prop_id,&index,&value,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgSetBinaryPropertyResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::RequestKeyFrame(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\015", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgRequestKeyFrameResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::RequestRecoveryFrame(int reserved){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\016", rid, MsgRequestRecoveryFrameRequest::BEGIN,&reserved,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgRequestRecoveryFrameResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::SetBitrate(int bitrate_bytes_sec){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\017", rid, MsgSetBitrateRequest::BEGIN,&bitrate_bytes_sec,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgSetBitrateResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::SetFramerate(int framerate_fps){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\020", rid, MsgSetFramerateRequest::BEGIN,&framerate_fps,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgSetFramerateResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::SetMaxPacketSize(int max_packet_size){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\021", rid, MsgSetMaxPacketSizeRequest::BEGIN,&max_packet_size,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgSetMaxPacketSizeResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::StartPlayback(int decoderfourcc, int width, int height){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\022", rid, MsgStartPlaybackRequest::BEGIN,&decoderfourcc,&width,&height,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgStartPlaybackResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  void SkypeVideoRTPInterfaceClient::RtpPacketReceived(const Binary& data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\023", rid, MsgRtpPacketReceivedRequest::BEGIN,&data,NULL) != Protocol::OK) return;
  }
  void SkypeVideoRTPInterfaceClient::FrameReceived(const Binary& data, int timestamp_ms){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\042", rid, MsgFrameReceivedRequest::BEGIN,&data,&timestamp_ms,NULL) != Protocol::OK) return;
  }
  bool SkypeVideoRTPInterfaceClient::StopPlayback(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\024", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgStopPlaybackResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\025", rid, MsgStartRecordingRequest::BEGIN,&fourcc,&initial_max_packet_size,&initial_bitrate_bytes_sec,&width,&height,&initial_framerate_fps,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgStartRecordingResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::StopRecording(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\026", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgStopRecordingResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::CustomCommand(const String& command, String& result_str){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\027", rid, MsgCustomCommandRequest::BEGIN,&command,NULL) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgCustomCommandResponse::BEGIN,&result_str,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::VideoCallStart(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\030", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgVideoCallStartResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }
  bool SkypeVideoRTPInterfaceClient::VideoCallEnd(){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    bool rv;
    if (m_protocol.wr_call(NULL,4,"ZR\000\031", rid) != Protocol::OK) return 0;
    Protocol::Command cmd;
    if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK) return 0;
    if (m_protocol.rd_value(NULL, rid) != Protocol::OK)  return 0;
    if(m_protocol.rd_parms_lst(NULL, MsgVideoCallEndResponse::BEGIN,&rv,NULL)) return 0;
    return rv;
  }

} // namespace Sid
