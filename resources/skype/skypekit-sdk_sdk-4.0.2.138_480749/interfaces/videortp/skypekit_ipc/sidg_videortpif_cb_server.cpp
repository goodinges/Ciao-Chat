#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_videortpif_cb.hpp"

#include "sidg_videortpif_cb_server.hpp"
#include "SidVideoRTPInterface.hpp"

namespace Sid {
  using namespace SkypeVideoRTPInterfaceCbMsgs;
    Protocol::Status SkypeVideoRTPInterfaceCbServer::ProcessCall(int rid, int method)
    {
    switch (method)
    {

      case 1:
      {
        MsgSendRTPPacketRequest* req = &msgs.m_MsgSendRTPPacketRequest;
        if (m_protocol.rd_parms(NULL, req, MsgSendRTPPacketRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterfaceCb* __if = static_cast<SkypeVideoRTPInterfaceCb*>(get_if());
          __if->SendRTPPacket(req->data);
        return Protocol::OK;
      }
      case 2:
      {
        MsgSendFrameRequest* req = &msgs.m_MsgSendFrameRequest;
        if (m_protocol.rd_parms(NULL, req, MsgSendFrameRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypeVideoRTPInterfaceCb* __if = static_cast<SkypeVideoRTPInterfaceCb*>(get_if());
          __if->SendFrame(req->data, req->timestamp_ms);
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
