#ifndef __sidg_msg_videortpif_hpp_included
#define __sidg_msg_videortpif_hpp_included

#include "SidPlatform.hpp"
#include "SidVideoRTPInterface.hpp"

namespace Sid {
namespace SkypeVideoRTPInterfaceMsgs {
  struct MsgGetInterfaceVersionResponse {
    enum { BEGIN = 0, END = 2 };
    bool rv;
    int version;
  };
  struct MsgGetLastErrorResponse {
    enum { BEGIN = 2, END = 3 };
    int error;
  };
  struct MsgInitResponse {
    enum { BEGIN = 3, END = 4 };
    bool rv;
  };
  struct MsgUninitResponse {
    enum { BEGIN = 4, END = 5 };
    bool rv;
  };
  struct MsgVideoCodec: SkypeVideoRTPInterface::VideoCodec {
    enum { BEGIN = 5, END = 8 };
    MsgVideoCodec() {}
  };
  struct MsgGetCodecCountResponse {
    enum { BEGIN = 8, END = 10 };
    int codec_count;
    bool rv;
  };
  struct MsgGetCodecRequest {
    enum { BEGIN = 10, END = 11 };
    int index;
  };
  struct MsgGetCodecResponse {
    enum { BEGIN = 11, END = 13 };
    bool rv;
    MsgVideoCodec codec;
  };
  struct MsgFixedRes: SkypeVideoRTPInterface::FixedRes {
    enum { BEGIN = 13, END = 16 };
    MsgFixedRes() {}
  };
  struct MsgResCap: SkypeVideoRTPInterface::ResCap {
    enum { BEGIN = 16, END = 21 };
    MsgResCap() {}
  };
  struct MsgGetSenderResolutionCountResponse {
    enum { BEGIN = 21, END = 23 };
    bool rv;
    int resolution_count;
  };
  struct MsgGetSenderResolutionRequest {
    enum { BEGIN = 23, END = 24 };
    int index;
  };
  struct MsgGetSenderResolutionResponse {
    enum { BEGIN = 24, END = 26 };
    bool rv;
    MsgFixedRes resolution;
  };
  struct MsgGetReceiverResolutionCountResponse {
    enum { BEGIN = 26, END = 28 };
    bool rv;
    int resolution_count;
  };
  struct MsgGetReceiverResolutionRequest {
    enum { BEGIN = 28, END = 29 };
    int index;
  };
  struct MsgGetReceiverResolutionResponse {
    enum { BEGIN = 29, END = 31 };
    bool rv;
    MsgResCap resolution;
  };
  struct MsgGetIntPropertyRequest {
    enum { BEGIN = 31, END = 33 };
    int prop_id;
    int index;
  };
  struct MsgGetIntPropertyResponse {
    enum { BEGIN = 33, END = 35 };
    int value;
    bool rv;
  };
  struct MsgSetIntPropertyRequest {
    enum { BEGIN = 35, END = 38 };
    int prop_id;
    int index;
    int value;
  };
  struct MsgSetIntPropertyResponse {
    enum { BEGIN = 38, END = 39 };
    bool rv;
  };
  struct MsgGetStringPropertyRequest {
    enum { BEGIN = 39, END = 41 };
    int prop_id;
    int index;
  };
  struct MsgGetStringPropertyResponse {
    enum { BEGIN = 41, END = 43 };
    bool rv;
    String value;
  };
  struct MsgSetStringPropertyRequest {
    enum { BEGIN = 43, END = 46 };
    int prop_id;
    int index;
    String value;
  };
  struct MsgSetStringPropertyResponse {
    enum { BEGIN = 46, END = 47 };
    bool rv;
  };
  struct MsgH264SenderParameters: SkypeVideoRTPInterface::H264SenderParameters {
    enum { BEGIN = 47, END = 50 };
    MsgH264SenderParameters() {}
  };
  struct MsgH264ReceiverParameters: SkypeVideoRTPInterface::H264ReceiverParameters {
    enum { BEGIN = 50, END = 60 };
    MsgH264ReceiverParameters() {}
  };
  struct MsgH264SenderConstraints: SkypeVideoRTPInterface::H264SenderConstraints {
    enum { BEGIN = 60, END = 70 };
    MsgH264SenderConstraints() {}
  };
  struct MsgGetBinaryPropertyRequest {
    enum { BEGIN = 70, END = 72 };
    int prop_id;
    int index;
  };
  struct MsgGetBinaryPropertyResponse {
    enum { BEGIN = 72, END = 74 };
    Binary value;
    bool rv;
  };
  struct MsgSetBinaryPropertyRequest {
    enum { BEGIN = 74, END = 77 };
    int prop_id;
    int index;
    Binary value;
  };
  struct MsgSetBinaryPropertyResponse {
    enum { BEGIN = 77, END = 78 };
    bool rv;
  };
  struct MsgRequestKeyFrameResponse {
    enum { BEGIN = 78, END = 79 };
    bool rv;
  };
  struct MsgRequestRecoveryFrameRequest {
    enum { BEGIN = 79, END = 80 };
    int reserved;
  };
  struct MsgRequestRecoveryFrameResponse {
    enum { BEGIN = 80, END = 81 };
    bool rv;
  };
  struct MsgSetBitrateRequest {
    enum { BEGIN = 81, END = 82 };
    int bitrate_bytes_sec;
  };
  struct MsgSetBitrateResponse {
    enum { BEGIN = 82, END = 83 };
    bool rv;
  };
  struct MsgSetFramerateRequest {
    enum { BEGIN = 83, END = 84 };
    int framerate_fps;
  };
  struct MsgSetFramerateResponse {
    enum { BEGIN = 84, END = 85 };
    bool rv;
  };
  struct MsgSetMaxPacketSizeRequest {
    enum { BEGIN = 85, END = 86 };
    int max_packet_size;
  };
  struct MsgSetMaxPacketSizeResponse {
    enum { BEGIN = 86, END = 87 };
    bool rv;
  };
  struct MsgStartPlaybackRequest {
    enum { BEGIN = 87, END = 90 };
    int decoderfourcc;
    int width;
    int height;
  };
  struct MsgStartPlaybackResponse {
    enum { BEGIN = 90, END = 91 };
    bool rv;
  };
  struct MsgRtpPacketReceivedRequest {
    enum { BEGIN = 91, END = 92 };
    Binary data;
  };
  struct MsgFrameReceivedRequest {
    enum { BEGIN = 92, END = 94 };
    Binary data;
    int timestamp_ms;
  };
  struct MsgStopPlaybackResponse {
    enum { BEGIN = 94, END = 95 };
    bool rv;
  };
  struct MsgStartRecordingRequest {
    enum { BEGIN = 95, END = 101 };
    int fourcc;
    int initial_max_packet_size;
    int initial_bitrate_bytes_sec;
    int width;
    int height;
    int initial_framerate_fps;
  };
  struct MsgStartRecordingResponse {
    enum { BEGIN = 101, END = 102 };
    bool rv;
  };
  struct MsgStopRecordingResponse {
    enum { BEGIN = 102, END = 103 };
    bool rv;
  };
  struct MsgCustomCommandRequest {
    enum { BEGIN = 103, END = 104 };
    String command;
  };
  struct MsgCustomCommandResponse {
    enum { BEGIN = 104, END = 106 };
    String result_str;
    bool rv;
  };
  struct MsgVideoCallStartResponse {
    enum { BEGIN = 106, END = 107 };
    bool rv;
  };
  struct MsgVideoCallEndResponse {
    enum { BEGIN = 107, END = 108 };
    bool rv;
  };
  struct videortpifMsgs {
    MsgGetInterfaceVersionResponse m_MsgGetInterfaceVersionResponse;
    MsgGetLastErrorResponse m_MsgGetLastErrorResponse;
    MsgInitResponse m_MsgInitResponse;
    MsgUninitResponse m_MsgUninitResponse;
    MsgGetCodecCountResponse m_MsgGetCodecCountResponse;
    MsgGetCodecRequest m_MsgGetCodecRequest;
    MsgGetCodecResponse m_MsgGetCodecResponse;
    MsgGetSenderResolutionCountResponse m_MsgGetSenderResolutionCountResponse;
    MsgGetSenderResolutionRequest m_MsgGetSenderResolutionRequest;
    MsgGetSenderResolutionResponse m_MsgGetSenderResolutionResponse;
    MsgGetReceiverResolutionCountResponse m_MsgGetReceiverResolutionCountResponse;
    MsgGetReceiverResolutionRequest m_MsgGetReceiverResolutionRequest;
    MsgGetReceiverResolutionResponse m_MsgGetReceiverResolutionResponse;
    MsgGetIntPropertyRequest m_MsgGetIntPropertyRequest;
    MsgGetIntPropertyResponse m_MsgGetIntPropertyResponse;
    MsgSetIntPropertyRequest m_MsgSetIntPropertyRequest;
    MsgSetIntPropertyResponse m_MsgSetIntPropertyResponse;
    MsgGetStringPropertyRequest m_MsgGetStringPropertyRequest;
    MsgGetStringPropertyResponse m_MsgGetStringPropertyResponse;
    MsgSetStringPropertyRequest m_MsgSetStringPropertyRequest;
    MsgSetStringPropertyResponse m_MsgSetStringPropertyResponse;
    MsgGetBinaryPropertyRequest m_MsgGetBinaryPropertyRequest;
    MsgGetBinaryPropertyResponse m_MsgGetBinaryPropertyResponse;
    MsgSetBinaryPropertyRequest m_MsgSetBinaryPropertyRequest;
    MsgSetBinaryPropertyResponse m_MsgSetBinaryPropertyResponse;
    MsgRequestKeyFrameResponse m_MsgRequestKeyFrameResponse;
    MsgRequestRecoveryFrameRequest m_MsgRequestRecoveryFrameRequest;
    MsgRequestRecoveryFrameResponse m_MsgRequestRecoveryFrameResponse;
    MsgSetBitrateRequest m_MsgSetBitrateRequest;
    MsgSetBitrateResponse m_MsgSetBitrateResponse;
    MsgSetFramerateRequest m_MsgSetFramerateRequest;
    MsgSetFramerateResponse m_MsgSetFramerateResponse;
    MsgSetMaxPacketSizeRequest m_MsgSetMaxPacketSizeRequest;
    MsgSetMaxPacketSizeResponse m_MsgSetMaxPacketSizeResponse;
    MsgStartPlaybackRequest m_MsgStartPlaybackRequest;
    MsgStartPlaybackResponse m_MsgStartPlaybackResponse;
    MsgRtpPacketReceivedRequest m_MsgRtpPacketReceivedRequest;
    MsgFrameReceivedRequest m_MsgFrameReceivedRequest;
    MsgStopPlaybackResponse m_MsgStopPlaybackResponse;
    MsgStartRecordingRequest m_MsgStartRecordingRequest;
    MsgStartRecordingResponse m_MsgStartRecordingResponse;
    MsgStopRecordingResponse m_MsgStopRecordingResponse;
    MsgCustomCommandRequest m_MsgCustomCommandRequest;
    MsgCustomCommandResponse m_MsgCustomCommandResponse;
    MsgVideoCallStartResponse m_MsgVideoCallStartResponse;
    MsgVideoCallEndResponse m_MsgVideoCallEndResponse;
  };
} // namespace SkypeVideoRTPInterfaceMsgs
} // namespace Sid
#endif
