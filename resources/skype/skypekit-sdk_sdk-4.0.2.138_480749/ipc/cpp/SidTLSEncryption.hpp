#ifndef _Sid_TLSEncryption_HPP_INCLUDED_
#define _Sid_TLSEncryption_HPP_INCLUDED_

#include <openssl/ssl.h>
#include "SidConnectionStateListenerInterface.hpp"

namespace Sid {

        class TLSEncryption {
        public:
                TLSEncryption();
                virtual ~TLSEncryption();

                virtual int read(uint num_bytes, char* dest);
                virtual int write(uint num_bytes, const char* src);                        
        protected:
                virtual bool load_certificate(const char* certificate_buf);     
                SSL_CTX* m_ctx;
                SSL*     m_ssl;
                Mutex    m_exclude_rd_wr;
        };   
        
        class TLSEncryptionSocketClient : protected TLSEncryption {
        public:        
                TLSEncryptionSocketClient(int socket);
                virtual ~TLSEncryptionSocketClient() {}       
        };
        
        class TLSEncryptionSocketServer : public TLSEncryption {
        public:
                TLSEncryptionSocketServer() {}
                void init(const char* certificate_buf, int socket, ConnectionStateListener* listener = 0);
                virtual ~TLSEncryptionSocketServer() {}        
        };
        
}

#endif
