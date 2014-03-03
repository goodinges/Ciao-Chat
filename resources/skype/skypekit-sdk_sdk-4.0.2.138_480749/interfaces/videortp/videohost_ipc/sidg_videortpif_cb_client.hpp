#ifndef sidg_videortpif_cb_client_HPPINCLUDED
#define sidg_videortpif_cb_client_HPPINCLUDED

#include "SidPlatform.hpp"
#include "SidAVClient.hpp"
#include "SidAVServer.hpp"
#include "SidVideoRTPInterface.hpp"
#include "sidg_msg_videortpif_cb.hpp"

namespace Sid {
  class SkypeVideoRTPInterfaceCbClient: public SkypeVideoRTPInterfaceCb, public AVClient
  {

  public:

    SkypeVideoRTPInterfaceCbClient() : AVClient(Field::M_SkypeVideoRTPInterfaceCb_fields)
    {
    }

    void SendRTPPacket(const Binary& data);
    void SendFrame(const Binary& data, int timestamp_ms);
    Mutex m_Mutex;
  };
}
#endif
