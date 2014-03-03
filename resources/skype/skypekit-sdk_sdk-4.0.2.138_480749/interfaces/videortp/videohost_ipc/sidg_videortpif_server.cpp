#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_videortpif.hpp"

#include "sidg_videortpif_server.hpp"
#include "SidVideoRTPInterface.hpp"

namespace Sid {
  using namespace SkypeVideoRTPInterfaceMsgs;
    Protocol::Status SkypeVideoRTPInterfaceServer::ProcessCall(int rid, int method)
    {
    switch (method)
    {

      case 1:
      {
        MsgGetInterfaceVersionResponse* rsp = &msgs.m_MsgGetInterfaceVersionResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetInterfaceVersion(rsp->version);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetInterfaceVersionResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 2:
      {
        MsgGetLastErrorResponse* rsp = &msgs.m_MsgGetLastErrorResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->error = __if->GetLastError();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetLastErrorResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 3:
      {
        MsgInitResponse* rsp = &msgs.m_MsgInitResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->Init();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgInitResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 4:
      {
        MsgUninitResponse* rsp = &msgs.m_MsgUninitResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->Uninit();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgUninitResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 5:
      {
        MsgGetCodecCountResponse* rsp = &msgs.m_MsgGetCodecCountResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetCodecCount(rsp->codec_count);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetCodecCountResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 6:
      {
        MsgGetCodecRequest* req = &msgs.m_MsgGetCodecRequest;
        MsgGetCodecResponse* rsp = &msgs.m_MsgGetCodecResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetCodecRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetCodec(req->index, rsp->codec);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetCodecResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 30:
      {
        MsgGetSenderResolutionCountResponse* rsp = &msgs.m_MsgGetSenderResolutionCountResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetSenderResolutionCount(rsp->resolution_count);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetSenderResolutionCountResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 31:
      {
        MsgGetSenderResolutionRequest* req = &msgs.m_MsgGetSenderResolutionRequest;
        MsgGetSenderResolutionResponse* rsp = &msgs.m_MsgGetSenderResolutionResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetSenderResolutionRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetSenderResolution(req->index, rsp->resolution);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetSenderResolutionResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 32:
      {
        MsgGetReceiverResolutionCountResponse* rsp = &msgs.m_MsgGetReceiverResolutionCountResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetReceiverResolutionCount(rsp->resolution_count);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetReceiverResolutionCountResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 33:
      {
        MsgGetReceiverResolutionRequest* req = &msgs.m_MsgGetReceiverResolutionRequest;
        MsgGetReceiverResolutionResponse* rsp = &msgs.m_MsgGetReceiverResolutionResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetReceiverResolutionRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetReceiverResolution(req->index, rsp->resolution);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetReceiverResolutionResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 7:
      {
        MsgGetIntPropertyRequest* req = &msgs.m_MsgGetIntPropertyRequest;
        MsgGetIntPropertyResponse* rsp = &msgs.m_MsgGetIntPropertyResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetIntPropertyRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetIntProperty(req->prop_id, req->index, rsp->value);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetIntPropertyResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 8:
      {
        MsgSetIntPropertyRequest* req = &msgs.m_MsgSetIntPropertyRequest;
        MsgSetIntPropertyResponse* rsp = &msgs.m_MsgSetIntPropertyResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetIntPropertyRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->SetIntProperty(req->prop_id, req->index, req->value);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetIntPropertyResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 9:
      {
        MsgGetStringPropertyRequest* req = &msgs.m_MsgGetStringPropertyRequest;
        MsgGetStringPropertyResponse* rsp = &msgs.m_MsgGetStringPropertyResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetStringPropertyRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetStringProperty(req->prop_id, req->index, rsp->value);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetStringPropertyResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 10:
      {
        MsgSetStringPropertyRequest* req = &msgs.m_MsgSetStringPropertyRequest;
        MsgSetStringPropertyResponse* rsp = &msgs.m_MsgSetStringPropertyResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetStringPropertyRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->SetStringProperty(req->prop_id, req->index, req->value);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetStringPropertyResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 11:
      {
        MsgGetBinaryPropertyRequest* req = &msgs.m_MsgGetBinaryPropertyRequest;
        MsgGetBinaryPropertyResponse* rsp = &msgs.m_MsgGetBinaryPropertyResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetBinaryPropertyRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->GetBinaryProperty(req->prop_id, req->index, rsp->value);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetBinaryPropertyResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 12:
      {
        MsgSetBinaryPropertyRequest* req = &msgs.m_MsgSetBinaryPropertyRequest;
        MsgSetBinaryPropertyResponse* rsp = &msgs.m_MsgSetBinaryPropertyResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetBinaryPropertyRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->SetBinaryProperty(req->prop_id, req->index, req->value);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetBinaryPropertyResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 13:
      {
        MsgRequestKeyFrameResponse* rsp = &msgs.m_MsgRequestKeyFrameResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->RequestKeyFrame();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgRequestKeyFrameResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 14:
      {
        MsgRequestRecoveryFrameRequest* req = &msgs.m_MsgRequestRecoveryFrameRequest;
        MsgRequestRecoveryFrameResponse* rsp = &msgs.m_MsgRequestRecoveryFrameResponse;
        if (m_protocol.rd_parms(NULL, req, MsgRequestRecoveryFrameRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->RequestRecoveryFrame(req->reserved);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgRequestRecoveryFrameResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 15:
      {
        MsgSetBitrateRequest* req = &msgs.m_MsgSetBitrateRequest;
        MsgSetBitrateResponse* rsp = &msgs.m_MsgSetBitrateResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetBitrateRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->SetBitrate(req->bitrate_bytes_sec);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetBitrateResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 16:
      {
        MsgSetFramerateRequest* req = &msgs.m_MsgSetFramerateRequest;
        MsgSetFramerateResponse* rsp = &msgs.m_MsgSetFramerateResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetFramerateRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->SetFramerate(req->framerate_fps);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetFramerateResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 17:
      {
        MsgSetMaxPacketSizeRequest* req = &msgs.m_MsgSetMaxPacketSizeRequest;
        MsgSetMaxPacketSizeResponse* rsp = &msgs.m_MsgSetMaxPacketSizeResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetMaxPacketSizeRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->SetMaxPacketSize(req->max_packet_size);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetMaxPacketSizeResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 18:
      {
        MsgStartPlaybackRequest* req = &msgs.m_MsgStartPlaybackRequest;
        MsgStartPlaybackResponse* rsp = &msgs.m_MsgStartPlaybackResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStartPlaybackRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->StartPlayback(req->decoderfourcc, req->width, req->height);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStartPlaybackResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 19:
      {
        MsgRtpPacketReceivedRequest* req = &msgs.m_MsgRtpPacketReceivedRequest;
        if (m_protocol.rd_parms(NULL, req, MsgRtpPacketReceivedRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          __if->RtpPacketReceived(req->data);
        return Protocol::OK;
      }
      case 34:
      {
        MsgFrameReceivedRequest* req = &msgs.m_MsgFrameReceivedRequest;
        if (m_protocol.rd_parms(NULL, req, MsgFrameReceivedRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          __if->FrameReceived(req->data, req->timestamp_ms);
        return Protocol::OK;
      }
      case 20:
      {
        MsgStopPlaybackResponse* rsp = &msgs.m_MsgStopPlaybackResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->StopPlayback();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStopPlaybackResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 21:
      {
        MsgStartRecordingRequest* req = &msgs.m_MsgStartRecordingRequest;
        MsgStartRecordingResponse* rsp = &msgs.m_MsgStartRecordingResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStartRecordingRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->StartRecording(req->fourcc, req->initial_max_packet_size, req->initial_bitrate_bytes_sec, req->width, req->height, req->initial_framerate_fps);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStartRecordingResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 22:
      {
        MsgStopRecordingResponse* rsp = &msgs.m_MsgStopRecordingResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->StopRecording();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStopRecordingResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 23:
      {
        MsgCustomCommandRequest* req = &msgs.m_MsgCustomCommandRequest;
        MsgCustomCommandResponse* rsp = &msgs.m_MsgCustomCommandResponse;
        if (m_protocol.rd_parms(NULL, req, MsgCustomCommandRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->CustomCommand(req->command, rsp->result_str);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgCustomCommandResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 24:
      {
        MsgVideoCallStartResponse* rsp = &msgs.m_MsgVideoCallStartResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->VideoCallStart();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgVideoCallStartResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 25:
      {
        MsgVideoCallEndResponse* rsp = &msgs.m_MsgVideoCallEndResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterface* __if = static_cast<SkypeVideoRTPInterface*>(get_if());
          rsp->rv = __if->VideoCallEnd();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgVideoCallEndResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
    default:
      Protocol::Status status;
      status = m_protocol.sk_parms(NULL);
      if (status != Protocol::OK) {
        return status;
      }
    return Protocol::ERR_CALL;  }
  }

} // namespace Sid
