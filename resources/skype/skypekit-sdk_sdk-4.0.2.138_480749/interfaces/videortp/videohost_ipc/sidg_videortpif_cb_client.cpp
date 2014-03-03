#include "sidg_videortpif_cb_client.hpp"

namespace Sid {
using namespace SkypeVideoRTPInterfaceCbMsgs;
  void SkypeVideoRTPInterfaceCbClient::SendRTPPacket(const Binary& data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\001", rid, MsgSendRTPPacketRequest::BEGIN,&data,NULL) != Protocol::OK) return;
  }
  void SkypeVideoRTPInterfaceCbClient::SendFrame(const Binary& data, int timestamp_ms){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\002", rid, MsgSendFrameRequest::BEGIN,&data,&timestamp_ms,NULL) != Protocol::OK) return;
  }

} // namespace Sid
