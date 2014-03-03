#ifndef AVIpcTransport_HPP_INCLUDED
#define AVIpcTransport_HPP_INCLUDED
#include "SidPlatform.hpp"
#include "AVTransport/AVTransport.hpp"
#include "SidTransportInterface.hpp"
#include "SidSession.hpp"

namespace Sid {

  /*** Connection transport message above server socket
   * ensure that only one CommandInitiator can atomically write a message
   * ensures that only one CommandInitiator can atomically read a message
   * write and read access can be concurrent
   * Creates the protocol handler and register it to the CommandInitiator::Pool
   */
  class AVTransportWrapper : public TransportInterface {
  public:
    AVTransportWrapper();
    ~AVTransportWrapper();

    Status bl_start_decoding(CommandInitiator* curr);
    Status bl_read_bytes(CommandInitiator* curr, uint num_bytes, char* dest);
    Status end_decoding(CommandInitiator* curr);

    Status bl_start_encoding(CommandInitiator* curr);
    Status bl_write_bytes(CommandInitiator* curr, uint num_bytes, const char* src);
    Status end_encoding(CommandInitiator* curr, bool immediate_hint);

    int Connect(const char* key, bool server_mode, int block_timeout_ms);
    void terminate();
    int IsConnected();

  private:
    AVTransport *m_Transport;
    bool m_Connected;
  };

} // namespace Sid

#endif
