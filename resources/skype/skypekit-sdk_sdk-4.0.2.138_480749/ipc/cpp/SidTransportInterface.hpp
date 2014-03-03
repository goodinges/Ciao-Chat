#ifndef TransportInterface_HPP_INCLUDED
#define TransportInterface_HPP_INCLUDED

#include "SidCommonConfig.hpp"
#include "SidPlatform.hpp" // for uint

namespace Sid {
  class CommandInitiator;

  /*** transport abstraction
   * it can be implemented either by a connection (normal usage)
   * or by a file transport (batch debugging)
   * possibly later with by a shared memory?
   * bl_ means blocking call
   * Transport shall manage the concurrent number of writers and the number of readers
   */
  struct TransportInterface {
    enum Status {
      OK,
      ERR_READ,
      ERR_WRITE,
      ERR_CONNECTION
    };
    virtual Status bl_start_decoding(CommandInitiator* curr) = 0;
    // dest can be 0: bytes will be skipped...
    virtual Status bl_read_bytes(CommandInitiator* curr, uint num_bytes, char* dest) = 0;
    virtual Status end_decoding(CommandInitiator* curr) = 0;
    virtual Status end_decoding_with_error(CommandInitiator* curr);

    virtual Status bl_start_encoding(CommandInitiator* curr) = 0;
    // src shall point a linear buffer or at least num_bytes 
    virtual Status bl_write_bytes(CommandInitiator* curr, uint num_bytes, const char* src) = 0;
    virtual Status end_encoding(CommandInitiator* curr, bool immediate_send_hint) = 0;

    virtual void terminate() = 0;

    virtual ~TransportInterface() {};
    TransportInterface() {}
    SKYPEKIT_DISABLE_COPY(TransportInterface);
  };

} // namespace Sid

#endif
