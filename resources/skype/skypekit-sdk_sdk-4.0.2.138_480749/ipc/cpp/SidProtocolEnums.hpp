#ifndef Sid_ProtocolEnums_INCLUDED_HPP
#define Sid_ProtocolEnums_INCLUDED_HPP

#include "SidTransportInterface.hpp"

namespace Sid {  
  namespace Protocol {
    enum Command { CMD_RESERVED, 
                   CMD_XCALL     = 'R', 
                   CMD_GET       = 'G', 
                   CMD_SET       = 'A', 
                   CMD_EVENT     = 'E', 
                   CMD_GET_REPLY = 'g', 
                   CMD_SET_REPLY = 'a', 
                   CMD_XREPLY    = 'r',
                   CMD_PROPCHANGE= 'C' }; // shall include a LOGIN command?

    // Error handling principle:
    // - when encoding, the only error is that connection is lost => ERR_ENCODE
    //        this is fatal, so we can immediately (once) call end_encoding() in the protocol stack
    // - when decoding, it may either be invalid stream, or connection loss
    //    - one shall NOT call end_decoding() in the protocol stack but propagate up to it => ERR_DECODE
    //    - then requesting level catches the error and can call resync_decode() only once per request
    //       if resync_decode() again returns ERR_DECODE then connection is lost and requesting level can apply its
    //       connection failure application specific recovery
    //       if it is OK then it was a stream error, one can keep requesting
    // - the protocol enables returning an error from the other part: => ERR_CALL
    //   it means that other part was not able to complete the request but acknowledge it has received it
    enum Status  { OK         = TransportInterface::OK, 
                   ERR_DECODE = TransportInterface::ERR_READ, 
                   ERR_ENCODE = TransportInterface::ERR_WRITE,
                   ERR_CALL,  
                   COMPLETED, // used by Iterators : ie not an error but end of collection reached
                 };

   typedef uint Rid;

  } // Protocol
} // Sid

#endif
