#include "SidConnection.hpp"
#include "SidTransportLog.hpp"
#include "SidCommandProcessor.hpp"
#include <string.h>
#include <assert.h>

// The connection class implements the transport interface and as such shall protect the concurrency.
// There is no lock for the readers as it is assumed that only a single reader is active at once.
// But there is a lock for writters taken since the begining of the payload writing as it is assumed several
// concurrent writers. This is coarse grain locking, and it should later on evaluated whether it creates too 
// much contention (with tools such as valgrind drd or mutrace). 
// With this coarse grain locking, the write caching is done inside the connection state.
// A finer grain could be locking at the first write on the actual transport, either whe the write buffer overflows
// or at end of payload. That way several thread can prepare payloads concurrently until it really needs writing.
// This means that the write cache would need being moved from the connection class to the command initiator class.
// this is actually why the transport interface carries a command initiator pointer).
// This may also scale better in term of memory, as there may be more connections than threads (currently it is rather
// the opposite as we mainly have a single connection).
// Then not only have the connection and command initiator code need being modified to make writing the payload reentrant:
// the protocol serialization shall also move its write states to the command initiator too.

namespace Sid {

        Connection::Connection(const char* log_path) 
        {
                m_socket = -1;
                m_rd_buffer_size = m_rd_ptr = m_wr_buffer_size = 0;
                m_log = 0;
                m_connstate_listener = 0;
                if (log_path) m_log = new TransportLog(log_path);
        }

        Connection::~Connection() {
                delete m_log;
        }
//static int reading = 0;
        // Transport Interface
        Connection::Status Connection::bl_start_decoding(CommandInitiator*) 
        {
//              assert(!reading); reading = 1;// printf("Connection::bl_start_decoding from %p\n", current_command_processor);
              return OK;
        }

        Connection::Status 
        Connection::bl_read_bytes(CommandInitiator* current_command_processor, uint nm_bytes, char* dest) 
        {
                int num_bytes = static_cast<int>(nm_bytes);
                if (dest) {
                        if (num_bytes <= m_rd_buffer_size) {
                                memcpy(dest, m_rd_buffer+m_rd_ptr, num_bytes);
                                m_rd_buffer_size -= num_bytes; 
                                m_rd_ptr         += num_bytes;
                        } else {
                                if (m_rd_buffer_size) {
                                        memcpy(dest, m_rd_buffer+m_rd_ptr, m_rd_buffer_size);
                                        dest      += m_rd_buffer_size; 
                                        num_bytes -= m_rd_buffer_size;
                                }
                                while (num_bytes > 1) {
                                        int read;
                                        while ((read = recv(num_bytes - 1, dest, 0)) == 0) { Thread::sleep(1); }
                                        if (read < 0) { terminate(); return ERR_READ; }
                                        if (m_log) m_log->read(current_command_processor,read,dest);
                                        num_bytes -= read;
                                        dest += read;
                                        // if read < num_bytes - 1 and server side can be worth forking from here to keep processing another connection
                                }
                                while ((m_rd_buffer_size = recv(num_bytes, m_rd_buffer, RD_BUFFER_SIZE)) == 0) { Thread::sleep(1); }
                                if (m_rd_buffer_size < 0)  { terminate(); return ERR_READ; }
                                if (m_log) m_log->read(current_command_processor,m_rd_buffer_size,m_rd_buffer);
                                *dest = m_rd_buffer[0];
                                m_rd_buffer_size --; 
                                m_rd_ptr = 1; 
                        } 
                } else {
                        if (num_bytes <= m_rd_buffer_size) {
                                m_rd_buffer_size -= num_bytes; 
                                m_rd_ptr         += num_bytes;
                        } else {
                                char chunk[256];
                                num_bytes -= m_rd_buffer_size;
                                while (num_bytes > 1) {
                                        int read;
                                        while ((read = recv((num_bytes-1) > 256 ? 256 : num_bytes -1, chunk, 0)) == 0) { Thread::sleep(1); }
                                        if (read < 0)  { terminate(); return ERR_READ; }
                                        if (m_log) m_log->read(current_command_processor,read,chunk);
                                        num_bytes -= read;
                                        // if read < to_read and server side can be worth forking from here to keep processing another connection
                                }
                                while ((m_rd_buffer_size = recv(1, m_rd_buffer, RD_BUFFER_SIZE)) == 0) { Thread::sleep(1); }
                                if (m_rd_buffer_size < 0)  { terminate(); return ERR_READ; }
                                if (m_log) m_log->read(current_command_processor,m_rd_buffer_size,m_rd_buffer);
                                m_rd_buffer_size--;
                                m_rd_ptr = 1;
                        }
                }
                return OK;
        }

        Connection::Status 
        Connection::end_decoding(CommandInitiator* current_command_processor) 
        {
//reading = 0; //printf("Connection::end_decoding from %p\n", current_command_processor);
                current_command_processor->fork();
                return OK;
        }

        Connection::Status 
        Connection::end_decoding_with_error(CommandInitiator* current_command_processor) 
        {
//reading = 0;// printf("Connection::end_decoding_with_error from %p\n", current_command_processor);
                if (!is_connected())  return ERR_READ;
                return end_decoding(current_command_processor);
        }

        Connection::Status
        Connection::bl_start_encoding(Sid::CommandInitiator* ) 
        {
                m_encoding_lock.lock();
                *m_wr_buffer ^= *m_wr_buffer; // prefetch write buffer cache line
                return OK;
        }

        Connection::Status
        Connection::bl_write_bytes(CommandInitiator* current_command_processor, uint nm_bytes, const char* src) 
        {
                int num_bytes = static_cast<int>(nm_bytes);
                if ((m_wr_buffer_size + num_bytes) <= WR_BUFFER_SIZE) {
                         memcpy(&m_wr_buffer[m_wr_buffer_size], src, num_bytes);
                         m_wr_buffer_size += num_bytes;
                } else {
                         uint to_write = m_wr_buffer_size;
                         const char* buf = m_wr_buffer;
                         while (to_write > 0) {
                                 int written;
                                 while ((written = send(to_write,buf,false)) == 0) Thread::sleep(1);
                                 if (written < 0) return ERR_WRITE;
                                 if (m_log) m_log->write(current_command_processor, written,buf);
                                 to_write -= written;
                                 buf += written;
                                 // if server and to_write > 0: may fork here
                         }
                         m_wr_buffer_size = 0;
                         buf = src;//const_cast<char*>(src);
                         to_write = num_bytes;
                         do {
                                 int written;
                                 while ((written = send(to_write,buf,false)) == 0) Thread::sleep(1);
                                 if (written < 0) return ERR_WRITE;
                                 if (m_log) m_log->write(current_command_processor, written,buf);
                                 to_write -= written;
                                 buf += written;
                                 // if server and to_write > 0: may fork here
                         } while (to_write > 0);
                }
                return OK;
        }

        Connection::Status
        Connection::end_encoding(CommandInitiator* current_command_processor, bool immediate_send_hint)
        {
                if (m_wr_buffer_size > 0) {
                        uint to_write = m_wr_buffer_size;
                        char* buf = m_wr_buffer;
                        while (to_write > 0) {
                                int written;
                                while ((written = send(to_write,buf,immediate_send_hint)) == 0) Thread::sleep(1);
                                if (written < 0) { m_encoding_lock.unlock(); return ERR_WRITE; }
                                if (m_log) m_log->write(current_command_processor, written,buf);
                                to_write -= written;
                                buf += written;
                                // if server and to_write > 0: may fork here
                        }
                        m_wr_buffer_size = 0;
                }
                m_encoding_lock.unlock();
                return OK;
        }

} // Sid

