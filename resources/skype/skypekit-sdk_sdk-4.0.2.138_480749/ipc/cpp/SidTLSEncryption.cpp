#include "SidTLSEncryption.hpp"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <stdio.h>

#ifndef SSL_SUCCESS
#define SSL_SUCCESS 1
#endif 

namespace Sid {

  TLSEncryption::TLSEncryption() : 
        m_ctx(0),
        m_ssl(0)
  {        
        SSL_load_error_strings();
        SSL_library_init();
#ifdef SSL_LIB_CYASSL
#ifdef SSL_LIB_DEBUG        
        CyaSSL_Debugging_ON();        
#endif          
#endif  		
  }

  TLSEncryption::~TLSEncryption() 
  {
        SSL_free(m_ssl);   
        SSL_CTX_free(m_ctx);  
        
        m_ssl = 0;  
        m_ctx = 0;  
        
#ifdef SSL_LIB_CYASSL
#ifdef SSL_LIB_DEBUG        
        CyaSSL_Debugging_OFF();        
#endif        
        FreeCyaSSL();
#endif 
  }

  int TLSEncryption::read(uint num_bytes, char* dest)
  {         
        if (!m_ssl) return -1;
        int result = 0;
        
        Mutex::ScopedLock l(m_exclude_rd_wr);
        while ((result = SSL_read(m_ssl, dest, num_bytes)) == -1) {
                if (SSL_get_error(m_ssl, result) == SSL_ERROR_WANT_READ) {
                        int no_pending_data = SSL_pending(m_ssl) == 0;
// printf("TLSEncryption::read: SSL_ERROR_WANT_READ %s\n", no_pending_data ? "no pending data, allow write": "pending data, call SSL_read again" );
                        if (no_pending_data) return 0;
                } else if (SSL_get_error(m_ssl, result) != SSL_ERROR_WANT_WRITE) {
// printf("TLSEncryption::read: unrecoverable SSL_ERROR, returning -1\n");
                        return -1;
                } else {
// printf("TLSEncryption::read: SSL_ERROR_WANT_WRITE, looping on SSL_read()\n");
                }
                Thread::sleep(1);
        }
        if (result <= 0) return -1;
        
        return result;
  }

  int TLSEncryption::write(uint num_bytes, const char* src)
  {        
        if (!m_ssl) return -1;
        int result = 0;
        
        Mutex::ScopedLock l(m_exclude_rd_wr);
        while ((result = SSL_write(m_ssl, src, num_bytes)) == -1 && 
                 (SSL_get_error(m_ssl, result) == SSL_ERROR_WANT_READ || SSL_get_error(m_ssl, result) == SSL_ERROR_WANT_WRITE)) {
             Thread::Sleep(1);
// printf("TLSEncryption::write: SSL_ERROR_WANT_WRITE or SSL_ERROR_WANT_READ, looping on SSL_write()\n");
        }
        if (result <= 0)  {
// printf("TLSEncryption::write: unrecoverable SSL_ERROR, returning -1\n");
                return -1;
        }        
        return result;
  }
  
  bool TLSEncryption::load_certificate(const char* certificate_buf)
  {
        if (!certificate_buf) return false;
        bool result = false;
         
#if defined(SSL_LIB_OPENSSL)        

        X509 *cert = NULL;
        EVP_PKEY *pkey = NULL;
        BIO *bio_buffer; 
        //create readonly memory BIO
        if (!(bio_buffer = BIO_new_mem_buf((void*)certificate_buf, -1))) return false; //certificate_buf should be null terminated;  
             
        //load PEM cert from buffer
        if(PEM_read_bio_X509(bio_buffer, &cert, 0, NULL))
        {             
                result = (SSL_CTX_use_certificate(m_ctx, cert) == SSL_SUCCESS);                
                X509_free(cert);
                BIO_reset(bio_buffer);
        }                 
        
        //load PEM private key from buffer
        if(result && PEM_read_bio_PrivateKey(bio_buffer, &pkey, 0, NULL)) 
        {
                result = (SSL_CTX_use_PrivateKey(m_ctx, pkey) == SSL_SUCCESS);                                               
                EVP_PKEY_free(pkey);
        }
             
        BIO_free(bio_buffer);   
        
#elif defined(SSL_LIB_CYASSL)   

        uint certificate_buf_len = strlen(certificate_buf);
        if (CyaSSL_CTX_use_certificate_buffer(m_ctx, (const unsigned char*)certificate_buf, 
                        certificate_buf_len, SSL_FILETYPE_PEM) == SSL_SUCCESS) {
                
                result = (CyaSSL_CTX_use_PrivateKey_buffer(m_ctx, (const unsigned char*)certificate_buf, 
                        certificate_buf_len, SSL_FILETYPE_PEM) == SSL_SUCCESS);        
        }      
                 
#endif  

        return result; 
  }
  
//------------------------------------------------------------------------  
  
  TLSEncryptionSocketClient::TLSEncryptionSocketClient(int socket) 
        : TLSEncryption()
  { 
        m_ctx = SSL_CTX_new(TLSv1_client_method());                                                          
        SSL_CTX_set_verify(m_ctx, SSL_VERIFY_NONE, NULL);                                   
        
        m_ssl = SSL_new(m_ctx);       
        SSL_set_fd(m_ssl, socket);
        
#ifdef SSL_LIB_OPENSSL        
        SSL_set_connect_state(m_ssl);                              
#endif       

        //if (m_ctx != NULL && m_ssl != NULL) printf("TLSEncryptionSocketClient setup done\n");     
  }

//------------------------------------------------------------------------  
  
  void TLSEncryptionSocketServer::init(const char* certificate_buf, int socket, ConnectionStateListener* listener) 
  { 
        m_ctx = SSL_CTX_new(TLSv1_server_method());      
          
        if (!load_certificate(certificate_buf)) {
                if (listener) listener->OnError(ConnectionStateListener::SSL_CERTIFICATE_LOAD_ERR);
                return;
        }       
                                                                                                              
        m_ssl = SSL_new(m_ctx);                  
        SSL_set_fd(m_ssl, socket);              
        
#ifdef SSL_LIB_OPENSSL        
        SSL_set_accept_state(m_ssl);      
#endif   

        //if (m_ctx != NULL && m_ssl != NULL) printf("TLSEncryptionSocketServer setup done\n");            
  }
  
//------------------------------------------------------------------------    
  
}
