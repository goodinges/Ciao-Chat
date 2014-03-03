#ifndef Sid_ServerSession_HPP_INCLUDED
#define Sid_ServerSession_HPP_INCLUDED

#include "SidSession.hpp"
#include "SidListener.hpp"
#include "SidServerConnection.hpp" // platform dependant
#include "SidProtocolBinServer.hpp"
#include "SidTLSEncryption.hpp"

namespace Sid {

  class ServerSession : public Session, public CommandInterface {
  public:
    typedef Sid::TcpListener   TcpListener;
    typedef Sid::LocalListener LocalListener;

    class TcpServerConnection : public Sid::TcpServerConnection {
    public:
      TcpServerConnection(Listener* listener, ServerSession* session, int socket, const char* streamlog);
      ~TcpServerConnection();
      Status end_decoding(CommandInitiator*);
      virtual bool handshake() = 0; 
      void attach_thread();
      void detach_thread();
      Protocol::ServerInterface* m_protocol;
      ServerSession*             m_session;
      Listener*                  m_listener;
      int                        m_num_threads;
    };

    class LocalServerConnection : public Sid::LocalServerConnection {
    public:
      LocalServerConnection(Listener* listener, ServerSession* session, int socket, const char* streamlog);
      ~LocalServerConnection();
      Status end_decoding(CommandInitiator*);
      virtual bool handshake() = 0; 
      void attach_thread();
      void detach_thread();
      Protocol::ServerInterface* m_protocol;
      ServerSession*             m_session;
      Listener*                  m_listener;
      int                        m_num_threads;
    };
    
    class TLSTcpServerConnection : public TcpServerConnection, protected TLSEncryptionSocketClient {
    public:
      TLSTcpServerConnection(Listener* listener, ServerSession* session, int socket, const char* streamlog) 
        : TcpServerConnection(listener, session, socket, streamlog), 
        TLSEncryptionSocketClient(socket) {}  
      virtual int recv(uint num_bytes, char* dest, uint){
        return TLSEncryption::read(num_bytes, dest); 
      }
      virtual int send(uint num_bytes, const char* src,bool){
        return TLSEncryption::write(num_bytes, src);
      }       
    };
    
    class TLSLocalServerConnection : public LocalServerConnection, protected TLSEncryptionSocketClient {
    public:
      TLSLocalServerConnection(Listener* listener, ServerSession* session, int socket, const char* streamlog) 
        : LocalServerConnection(listener, session, socket, streamlog), 
        TLSEncryptionSocketClient(socket) {}  
      virtual int recv(uint num_bytes, char* dest, uint){
        return TLSEncryption::read(num_bytes, dest); 
      }
      virtual int send(uint num_bytes, const char* src,bool){
        return TLSEncryption::write(num_bytes, src);
      }       
    };         

    ServerSession(int num_threads, int max_threads_per_connection, bool use_main_thread);
    ~ServerSession();

    bool init(Listener*);
    void start();
    void stop ();

    //bool add_listener(Listener*);
    void del_listener(Listener*);
    void accept_from(Listener*);

    bool add_connection(ServerCommandInterface*);
    void add_waiting(ServerCommandInterface*);
    void add_ready(ServerCommandInterface*);
    void del_connection(ServerCommandInterface*, Listener*);
  
  protected:

    ServerCommandInterface* pop();
    bool has_connection();
    bool poll_waiting();
    void select();
    void process_command(CommandInitiator* thread);
    bool reuse_command_processor(CommandProcessor* command_processor);
    class Queue {
    public:
      Queue();
      bool is_empty() const;
      ServerCommandInterface* pop();
      void push(ServerCommandInterface* q);
      enum { CAPACITY = 64 };
      ServerCommandInterface*  m_queued[CAPACITY];
      int                      m_num_queued;
      int                      m_rd_ptr;
      int                      m_wr_ptr;
    };

    Mutex m_mutex;
    Queue m_waiting_queue;
    Queue m_ready_queue;
    int   m_num_connection;
    bool  m_selecting;
    bool  m_use_main;
  };

} // Sid

#endif

