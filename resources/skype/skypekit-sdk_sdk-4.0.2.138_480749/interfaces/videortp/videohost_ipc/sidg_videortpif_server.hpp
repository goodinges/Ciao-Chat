#ifndef __sidg_videortpif_server_INCLUDED_HPP
#define __sidg_videortpif_server_INCLUDED_HPP

#include "SidAVServer.hpp"
#include "SidVideoRTPInterface.hpp"
#include "sidg_msg_videortpif.hpp"

namespace Sid {

  class SkypeVideoRTPInterfaceServer : public AVServer {
  public:
        SkypeVideoRTPInterfaceMsgs::videortpifMsgs msgs;
        SkypeVideoRTPInterfaceServer() : AVServer(Field::M_SkypeVideoRTPInterface_fields){}
    ~SkypeVideoRTPInterfaceServer() {}
    Protocol::Status ProcessCall(int rid, int method);
  };
}

#endif
