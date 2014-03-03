#include "sidg_multirtpif_cb_client.hpp"

namespace Sid {
using namespace SkypeMultiRTPCallbackInterfaceMsgs;
  void SkypeMultiRTPCallbackInterfaceClient::SendRTPPacket(int channel_id, const Binary& data){
    Mutex::ScopedLock l(m_Mutex);
    uint rid;
    if (m_protocol.wr_call_lst(NULL,4,"ZR\000\001", rid, MsgSendRTPPacketRequest::BEGIN,&channel_id,&data,NULL) != Protocol::OK) return;
  }

} // namespace Sid
