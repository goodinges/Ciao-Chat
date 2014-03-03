#ifndef _ConcurrentCircularBuffer_HPP_INCLUDED
#define _ConcurrentCircularBuffer_HPP_INCLUDED

#include "SidPlatform.hpp"

namespace Sid {

  class CommandInitiator;

  /*** Circular buffer for concurrent produce/consume accesses
   *
   * It creates a circular buffer with a capacity that is a power of two that is intended for stream accesses between a producer and a consumer.
   * If the capacity given to the constructor isn't a power of 2 this is rounded to the next power of two bigger than the requested capacity.
   * nb_produce is a non blocking call, and return the number of written data that can be 0
   * bl_consume is a blocking call that fills the required bytes in a preallocated buffer,
   * and returns only when buffer is filled
   * There is only a protection between 1 producer and 1 consumer, hence one shall ensure
   * - that only a single producer produces at the same time,
   * - and only a single consumer consumes at the same time.
   */
  class ConcurrentCircularBuffer {
  public:
    ConcurrentCircularBuffer(uint capacity);
    ~ConcurrentCircularBuffer();

    uint get_capacity() const         { return m_capacity_mask + 1; }

    uint nb_produce(uint size, const char* src) ;
    // return false if an error occured
    bool bl_consume(uint size, char* dest, CommandInitiator* curr_thread);

    // maybe better discouraging these...
    unsigned short get_size()              { Mutex::ScopedLock exclude(m_rd_wr_lock); return m_size; }

  private:
    void copy_from(const char* src, uint wr_ptr, uint size, uint capacity);
    void copy_to(char* dest, uint rd_ptr, uint size, uint capacity);

    uint             m_capacity_mask;
    char*            m_buffer;
    volatile uint    m_size;
    volatile uint    m_rd_ptr;
    volatile uint    m_wr_ptr;
    volatile uint    m_waited_bytes;
    volatile CommandInitiator* m_blocked_consumer;
    Mutex            m_rd_wr_lock;
  };

} // namespace Sid

#endif
