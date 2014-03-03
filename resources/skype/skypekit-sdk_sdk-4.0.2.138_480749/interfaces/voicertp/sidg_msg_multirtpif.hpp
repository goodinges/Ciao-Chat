#ifndef __sidg_msg_multirtpif_hpp_included
#define __sidg_msg_multirtpif_hpp_included

#include "SidPlatform.hpp"
#include "SidMultiRTPInterface.hpp"

namespace Sid {
namespace SkypeMultiRTPInterfaceMsgs {
  struct MsgCodec: SkypeMultiRTPInterface::Codec {
    enum { BEGIN = 0, END = 10 };
    MsgCodec() {}
  };
  struct MsgGetVersionResponse {
    enum { BEGIN = 10, END = 12 };
    String version;
    int rv;
  };
  struct MsgGetLastErrorResponse {
    enum { BEGIN = 12, END = 13 };
    int error;
  };
  struct MsgInitResponse {
    enum { BEGIN = 13, END = 14 };
    int rv;
  };
  struct MsgUninitResponse {
    enum { BEGIN = 14, END = 15 };
    int rv;
  };
  struct MsgCreateChannelRequest {
    enum { BEGIN = 15, END = 17 };
    uint call_member_id;
    uint conversation_id;
  };
  struct MsgCreateChannelResponse {
    enum { BEGIN = 17, END = 19 };
    int channel;
    int rv;
  };
  struct MsgDeleteChannelRequest {
    enum { BEGIN = 19, END = 20 };
    int channel_id;
  };
  struct MsgDeleteChannelResponse {
    enum { BEGIN = 20, END = 21 };
    int rv;
  };
  struct MsgChangeConversationIdRequest {
    enum { BEGIN = 21, END = 24 };
    int channel_id;
    uint conversation_id;
    uint call_member_id;
  };
  struct MsgChangeConversationIdResponse {
    enum { BEGIN = 24, END = 25 };
    int rv;
  };
  struct MsgGetCodecRequest {
    enum { BEGIN = 25, END = 26 };
    uint id;
  };
  struct MsgGetCodecResponse {
    enum { BEGIN = 26, END = 28 };
    MsgCodec codec;
    uint rv;
  };
  struct MsgGetCodecCountResponse {
    enum { BEGIN = 28, END = 30 };
    uint count;
    int rv;
  };
  struct MsgSetSendCodecRequest {
    enum { BEGIN = 30, END = 35 };
    int channel_id;
    String Name;
    int PacketSize;
    int BitRate;
    int PayloadType;
  };
  struct MsgSetSendCodecResponse {
    enum { BEGIN = 35, END = 36 };
    int rv;
  };
  struct MsgInformSendPacketLossRequest {
    enum { BEGIN = 36, END = 41 };
    int channel_id;
    uint packetLossBeforeFEC;
    uint packetLossAfterFEC;
    uint jitterBeforeFEC;
    uint jitterAfterFEC;
  };
  struct MsgStartPlaybackRequest {
    enum { BEGIN = 41, END = 42 };
    int channel_id;
  };
  struct MsgStartPlaybackResponse {
    enum { BEGIN = 42, END = 43 };
    int rv;
  };
  struct MsgStopPlaybackRequest {
    enum { BEGIN = 43, END = 44 };
    int channel_id;
  };
  struct MsgStopPlaybackResponse {
    enum { BEGIN = 44, END = 45 };
    int rv;
  };
  struct MsgStartRecordingRequest {
    enum { BEGIN = 45, END = 46 };
    int channel_id;
  };
  struct MsgStartRecordingResponse {
    enum { BEGIN = 46, END = 47 };
    int rv;
  };
  struct MsgStopRecordingRequest {
    enum { BEGIN = 47, END = 48 };
    int channel_id;
  };
  struct MsgStopRecordingResponse {
    enum { BEGIN = 48, END = 49 };
    int rv;
  };
  struct MsgSubmitRTPPacketRequest {
    enum { BEGIN = 49, END = 51 };
    int channel_id;
    Binary data;
  };
  struct MsgSendDTMFRequest {
    enum { BEGIN = 51, END = 54 };
    int channel_id;
    int eventnr;
    int rtp_type;
  };
  struct MsgSendDTMFResponse {
    enum { BEGIN = 54, END = 55 };
    int rv;
  };
  struct MsgSetOutputAmplificationRequest {
    enum { BEGIN = 55, END = 57 };
    int channel_id;
    int scale;
  };
  struct MsgSetOutputAmplificationResponse {
    enum { BEGIN = 57, END = 58 };
    int rv;
  };
  struct MsgCustomCommandRequest {
    enum { BEGIN = 58, END = 59 };
    String command;
  };
  struct MsgCustomCommandResponse {
    enum { BEGIN = 59, END = 61 };
    String result_str;
    int rv;
  };
  struct multirtpifMsgs {
    MsgGetVersionResponse m_MsgGetVersionResponse;
    MsgGetLastErrorResponse m_MsgGetLastErrorResponse;
    MsgInitResponse m_MsgInitResponse;
    MsgUninitResponse m_MsgUninitResponse;
    MsgCreateChannelRequest m_MsgCreateChannelRequest;
    MsgCreateChannelResponse m_MsgCreateChannelResponse;
    MsgDeleteChannelRequest m_MsgDeleteChannelRequest;
    MsgDeleteChannelResponse m_MsgDeleteChannelResponse;
    MsgChangeConversationIdRequest m_MsgChangeConversationIdRequest;
    MsgChangeConversationIdResponse m_MsgChangeConversationIdResponse;
    MsgGetCodecRequest m_MsgGetCodecRequest;
    MsgGetCodecResponse m_MsgGetCodecResponse;
    MsgGetCodecCountResponse m_MsgGetCodecCountResponse;
    MsgSetSendCodecRequest m_MsgSetSendCodecRequest;
    MsgSetSendCodecResponse m_MsgSetSendCodecResponse;
    MsgInformSendPacketLossRequest m_MsgInformSendPacketLossRequest;
    MsgStartPlaybackRequest m_MsgStartPlaybackRequest;
    MsgStartPlaybackResponse m_MsgStartPlaybackResponse;
    MsgStopPlaybackRequest m_MsgStopPlaybackRequest;
    MsgStopPlaybackResponse m_MsgStopPlaybackResponse;
    MsgStartRecordingRequest m_MsgStartRecordingRequest;
    MsgStartRecordingResponse m_MsgStartRecordingResponse;
    MsgStopRecordingRequest m_MsgStopRecordingRequest;
    MsgStopRecordingResponse m_MsgStopRecordingResponse;
    MsgSubmitRTPPacketRequest m_MsgSubmitRTPPacketRequest;
    MsgSendDTMFRequest m_MsgSendDTMFRequest;
    MsgSendDTMFResponse m_MsgSendDTMFResponse;
    MsgSetOutputAmplificationRequest m_MsgSetOutputAmplificationRequest;
    MsgSetOutputAmplificationResponse m_MsgSetOutputAmplificationResponse;
    MsgCustomCommandRequest m_MsgCustomCommandRequest;
    MsgCustomCommandResponse m_MsgCustomCommandResponse;
  };
} // namespace SkypeMultiRTPInterfaceMsgs
} // namespace Sid
#endif
