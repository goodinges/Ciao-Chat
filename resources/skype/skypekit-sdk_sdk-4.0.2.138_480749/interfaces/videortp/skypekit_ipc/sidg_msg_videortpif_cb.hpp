#ifndef __sidg_msg_videortpif_cb_hpp_included
#define __sidg_msg_videortpif_cb_hpp_included

#include "SidPlatform.hpp"
#include "SidVideoRTPInterface.hpp"

namespace Sid {
namespace SkypeVideoRTPInterfaceCbMsgs {
  struct MsgSendRTPPacketRequest {
    enum { BEGIN = 0, END = 1 };
    Binary data;
  };
  struct MsgSendFrameRequest {
    enum { BEGIN = 1, END = 3 };
    Binary data;
    int timestamp_ms;
  };
  struct videortpif_cbMsgs {
    MsgSendRTPPacketRequest m_MsgSendRTPPacketRequest;
    MsgSendFrameRequest m_MsgSendFrameRequest;
  };
} // namespace SkypeVideoRTPInterfaceCbMsgs
} // namespace Sid
#endif
