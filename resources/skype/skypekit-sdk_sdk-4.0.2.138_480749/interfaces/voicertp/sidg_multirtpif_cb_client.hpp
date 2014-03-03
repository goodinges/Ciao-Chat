#ifndef sidg_multirtpif_cb_client_HPPINCLUDED
#define sidg_multirtpif_cb_client_HPPINCLUDED

#include "SidPlatform.hpp"
#include "SidAVClient.hpp"
#include "SidAVServer.hpp"
#include "SidMultiRTPInterface.hpp"
#include "sidg_msg_multirtpif_cb.hpp"

namespace Sid {
  class SkypeMultiRTPCallbackInterfaceClient: public SkypeMultiRTPCallbackInterface, public AVClient
  {

  public:

    SkypeMultiRTPCallbackInterfaceClient() : AVClient(Field::M_SkypeMultiRTPCallbackInterface_fields)
    {
    }

    void SendRTPPacket(int channel_id, const Binary& data);
    Mutex m_Mutex;
  };
}
#endif
