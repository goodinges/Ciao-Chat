#ifndef __sidg_msgs_multirtpif_cb_hpp_included__
#define __sidg_msgs_multirtpif_cb_hpp_included__
#include "SidMultiRTPInterface.hpp"

namespace Sid {
namespace SkypeMultiRTPCallbackInterfaceMsgs {
  struct MsgSendRTPPacketRequest {
    enum { BEGIN = 0, END = 2 };
    int channel_id;
    Binary data;
  };
  struct multirtpif_cbMsgs {
    MsgSendRTPPacketRequest m_MsgSendRTPPacketRequest;
  };
} // namespace SkypeMultiRTPCallbackInterfaceMsgs
} // namespace Sid
#endif
