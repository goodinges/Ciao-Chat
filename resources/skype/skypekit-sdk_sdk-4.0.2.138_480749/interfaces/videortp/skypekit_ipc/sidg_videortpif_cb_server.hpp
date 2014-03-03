#ifndef __sidg_videortpif_cb_server_INCLUDED_HPP
#define __sidg_videortpif_cb_server_INCLUDED_HPP

#include "SidAVServer.hpp"
#include "SidVideoRTPInterface.hpp"
#include "sidg_msg_videortpif_cb.hpp"

namespace Sid {

  class SkypeVideoRTPInterfaceCbServer : public AVServer {
  public:
        SkypeVideoRTPInterfaceCbMsgs::videortpif_cbMsgs msgs;
        SkypeVideoRTPInterfaceCbServer() : AVServer(Field::M_SkypeVideoRTPInterfaceCb_fields){}
    ~SkypeVideoRTPInterfaceCbServer() {}
    Protocol::Status ProcessCall(int rid, int method);
  };
}

#endif
