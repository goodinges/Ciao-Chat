#ifndef ConnectionStateListener_HPP_INCLUDED
#define ConnectionStateListener_HPP_INCLUDED

#include "SidPlatform.hpp" // for uint
#include "SidCommonConfig.hpp"

namespace Sid {

  struct ConnectionStateListener {
    
        enum CONNECTION_ERROR {
                NONE = 0,
                SSL_CERTIFICATE_LOAD_ERR,
        };
    
        virtual void Connecting() = 0;    
        virtual void Connected() = 0;
        virtual void Disconnected() = 0;        
        virtual void OnError(CONNECTION_ERROR err) = 0;
                
        virtual ~ConnectionStateListener() {};
        ConnectionStateListener() {}
        SKYPEKIT_DISABLE_COPY(ConnectionStateListener);
  };

} // namespace Sid

#endif
