#ifndef __sidg_multirtpif_server_INCLUDED_HPP
#define __sidg_multirtpif_server_INCLUDED_HPP

#include "SidAVServer.hpp"
#include "SidMultiRTPInterface.hpp"
#include "sidg_msg_multirtpif.hpp"

namespace Sid {

  class SkypeMultiRTPInterfaceServer : public AVServer {
  public:
        SkypeMultiRTPInterfaceMsgs::multirtpifMsgs msgs;
        SkypeMultiRTPInterfaceServer() : AVServer(Field::M_SkypeMultiRTPInterface_fields){}
    ~SkypeMultiRTPInterfaceServer() {}
    Protocol::Status ProcessCall(int rid, int method);
  };
}

#endif
