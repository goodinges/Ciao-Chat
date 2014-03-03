#ifndef __sidg_pcmif_server_INCLUDED_HPP
#define __sidg_pcmif_server_INCLUDED_HPP

#include "SidAVServer.hpp"
#include "SidPCMInterface.hpp"
#include "sidg_msg_pcmif.hpp"

namespace Sid {

  class SkypePCMInterfaceServer : public AVServer {
  public:
        SkypePCMInterfaceMsgs::pcmifMsgs msgs;
        SkypePCMInterfaceServer() : AVServer(Field::M_SkypePCMInterface_fields){}
    ~SkypePCMInterfaceServer() {}
    Protocol::Status ProcessCall(int rid, int method);
  };
}

#endif
