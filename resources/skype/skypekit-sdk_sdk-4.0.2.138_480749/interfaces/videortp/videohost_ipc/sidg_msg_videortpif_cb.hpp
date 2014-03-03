#ifndef __sidg_msgs_videortpif_cb_hpp_included__
#define __sidg_msgs_videortpif_cb_hpp_included__
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
