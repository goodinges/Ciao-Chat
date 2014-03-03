#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_multirtpif.hpp"

#include "sidg_multirtpif_server.hpp"
#include "SidMultiRTPInterface.hpp"

namespace Sid {
  using namespace SkypeMultiRTPInterfaceMsgs;
    Protocol::Status SkypeMultiRTPInterfaceServer::ProcessCall(int rid, int method)
    {
    switch (method)
    {

      case 1:
      {
        MsgGetVersionResponse* rsp = &msgs.m_MsgGetVersionResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->GetVersion(rsp->version);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetVersionResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 2:
      {
        MsgGetLastErrorResponse* rsp = &msgs.m_MsgGetLastErrorResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->error = __if->GetLastError();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetLastErrorResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 3:
      {
        MsgInitResponse* rsp = &msgs.m_MsgInitResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->Init();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgInitResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 4:
      {
        MsgUninitResponse* rsp = &msgs.m_MsgUninitResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->Uninit();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgUninitResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 10:
      {
        MsgCreateChannelRequest* req = &msgs.m_MsgCreateChannelRequest;
        MsgCreateChannelResponse* rsp = &msgs.m_MsgCreateChannelResponse;
        if (m_protocol.rd_parms(NULL, req, MsgCreateChannelRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->CreateChannel(req->call_member_id, req->conversation_id, rsp->channel);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgCreateChannelResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 11:
      {
        MsgDeleteChannelRequest* req = &msgs.m_MsgDeleteChannelRequest;
        MsgDeleteChannelResponse* rsp = &msgs.m_MsgDeleteChannelResponse;
        if (m_protocol.rd_parms(NULL, req, MsgDeleteChannelRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->DeleteChannel(req->channel_id);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgDeleteChannelResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 12:
      {
        MsgChangeConversationIdRequest* req = &msgs.m_MsgChangeConversationIdRequest;
        MsgChangeConversationIdResponse* rsp = &msgs.m_MsgChangeConversationIdResponse;
        if (m_protocol.rd_parms(NULL, req, MsgChangeConversationIdRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->ChangeConversationId(req->channel_id, req->conversation_id, req->call_member_id);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgChangeConversationIdResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 20:
      {
        MsgGetCodecRequest* req = &msgs.m_MsgGetCodecRequest;
        MsgGetCodecResponse* rsp = &msgs.m_MsgGetCodecResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetCodecRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->GetCodec(req->id, rsp->codec);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetCodecResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 21:
      {
        MsgGetCodecCountResponse* rsp = &msgs.m_MsgGetCodecCountResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->GetCodecCount(rsp->count);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetCodecCountResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 22:
      {
        MsgSetSendCodecRequest* req = &msgs.m_MsgSetSendCodecRequest;
        MsgSetSendCodecResponse* rsp = &msgs.m_MsgSetSendCodecResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetSendCodecRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->SetSendCodec(req->channel_id, req->Name, req->PacketSize, req->BitRate, req->PayloadType);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetSendCodecResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 23:
      {
        MsgInformSendPacketLossRequest* req = &msgs.m_MsgInformSendPacketLossRequest;
        if (m_protocol.rd_parms(NULL, req, MsgInformSendPacketLossRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          __if->InformSendPacketLoss(req->channel_id, req->packetLossBeforeFEC, req->packetLossAfterFEC, req->jitterBeforeFEC, req->jitterAfterFEC);
        return Protocol::OK;
      }
      case 30:
      {
        MsgStartPlaybackRequest* req = &msgs.m_MsgStartPlaybackRequest;
        MsgStartPlaybackResponse* rsp = &msgs.m_MsgStartPlaybackResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStartPlaybackRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->StartPlayback(req->channel_id);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStartPlaybackResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 31:
      {
        MsgStopPlaybackRequest* req = &msgs.m_MsgStopPlaybackRequest;
        MsgStopPlaybackResponse* rsp = &msgs.m_MsgStopPlaybackResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStopPlaybackRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->StopPlayback(req->channel_id);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStopPlaybackResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 32:
      {
        MsgStartRecordingRequest* req = &msgs.m_MsgStartRecordingRequest;
        MsgStartRecordingResponse* rsp = &msgs.m_MsgStartRecordingResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStartRecordingRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->StartRecording(req->channel_id);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStartRecordingResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 33:
      {
        MsgStopRecordingRequest* req = &msgs.m_MsgStopRecordingRequest;
        MsgStopRecordingResponse* rsp = &msgs.m_MsgStopRecordingResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStopRecordingRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->StopRecording(req->channel_id);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStopRecordingResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 40:
      {
        MsgSubmitRTPPacketRequest* req = &msgs.m_MsgSubmitRTPPacketRequest;
        if (m_protocol.rd_parms(NULL, req, MsgSubmitRTPPacketRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          __if->SubmitRTPPacket(req->channel_id, req->data);
        return Protocol::OK;
      }
      case 41:
      {
        MsgSendDTMFRequest* req = &msgs.m_MsgSendDTMFRequest;
        MsgSendDTMFResponse* rsp = &msgs.m_MsgSendDTMFResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSendDTMFRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->SendDTMF(req->channel_id, req->eventnr, req->rtp_type);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSendDTMFResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 50:
      {
        MsgSetOutputAmplificationRequest* req = &msgs.m_MsgSetOutputAmplificationRequest;
        MsgSetOutputAmplificationResponse* rsp = &msgs.m_MsgSetOutputAmplificationResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetOutputAmplificationRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->SetOutputAmplification(req->channel_id, req->scale);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetOutputAmplificationResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 51:
      {
        MsgCustomCommandRequest* req = &msgs.m_MsgCustomCommandRequest;
        MsgCustomCommandResponse* rsp = &msgs.m_MsgCustomCommandResponse;
        if (m_protocol.rd_parms(NULL, req, MsgCustomCommandRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeMultiRTPInterface* __if = static_cast<SkypeMultiRTPInterface*>(get_if());
          rsp->rv = __if->CustomCommand(req->command, rsp->result_str);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgCustomCommandResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
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
