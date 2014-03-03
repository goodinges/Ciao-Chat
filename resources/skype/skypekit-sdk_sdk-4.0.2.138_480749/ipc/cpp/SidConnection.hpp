#ifndef _Sid_Connection_HPP_INCLUDED
#define _Sid_Connection_HPP_INCLUDED

#include "SidTransportInterface.hpp"
#include "SidConnectionStateListenerInterface.hpp"

namespace Sid {

class Transport;
class TransportLog;
struct CommandInterface;

class Connection : public TransportInterface {
public:
        Connection(const char* log_path = 0);
        ~Connection();

        bool is_connected() const { return m_socket != -1; }
        int  get_id() const       { return m_socket; }
        bool has_more() const     { return m_rd_buffer_size > 0; }
          
        // Transport Interface
        virtual Status bl_start_decoding(Sid::CommandInitiator* current_command_processor);
        virtual Status bl_read_bytes(Sid::CommandInitiator* current_command_processor, uint num_bytes, char* dest);
        virtual Status end_decoding(Sid::CommandInitiator* current_command_processor);
        virtual Status end_decoding_with_error(Sid::CommandInitiator* current_command_processor);
        virtual Status bl_start_encoding(Sid::CommandInitiator* current_command_processor);
        virtual Status bl_write_bytes(Sid::CommandInitiator* current_command_processor, uint num_bytes, const char* src);
        virtual Status end_encoding(Sid::CommandInitiator* current_command_processor, bool immediate_send_hint);
//        virtual void terminate() = 0; // also part of TransportInterface
protected:
        // delegated to actual implementation
        virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead) = 0;
        virtual int send(uint num_bytes, const char* src, bool immediate_send_hint) = 0;
        
        void set_connstate_listener(ConnectionStateListener* listener) { m_connstate_listener = listener; }
        ConnectionStateListener* get_connstate_listener() { return m_connstate_listener; }; 
        
        enum {
                RD_BUFFER_SIZE = 192,
                WR_BUFFER_SIZE = 192
        };

private:
        TransportLog*            m_log;
protected:
        int                      m_socket; // if socket, else flag for connection on/off (-1)
private:
        uint                     m_rd_ptr;
        int                      m_rd_buffer_size;
        char                     m_rd_buffer[RD_BUFFER_SIZE];
        Mutex                    m_encoding_lock;
        int                      m_wr_buffer_size;
        char                     m_wr_buffer[WR_BUFFER_SIZE];
        ConnectionStateListener* m_connstate_listener; //or list of listeners        
        SKYPEKIT_DISABLE_COPY(Connection);
};

} // Sid

#endif

