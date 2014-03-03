#include <string.h>
#include "SidConcurrentCircularBuffer.hpp"
#include "SidCommandProcessor.hpp"
#include <stdio.h>

namespace Sid { 

ConcurrentCircularBuffer::ConcurrentCircularBuffer(uint capacity)
{
  uint capacity_pow2;
  for (capacity_pow2 = 1; capacity_pow2 < capacity; capacity_pow2 <<= 1); 
  m_rd_ptr = m_wr_ptr = m_size = m_waited_bytes = 0;
  m_blocked_consumer = 0;
  m_capacity_mask = capacity_pow2 - 1;      
  m_buffer = new char[capacity_pow2];
}

ConcurrentCircularBuffer::~ConcurrentCircularBuffer() 
{ 
  m_rd_wr_lock.lock();
  CommandInitiator* thread = (CommandInitiator*) m_blocked_consumer; // cast away volatile...
  if (thread) // notify current reader that it can't read
    thread->resume(false);
  m_rd_wr_lock.unlock(); 
  // ensure current read thread completes if it was just resumed
  Thread::sleep(1); // force a reschedule
  m_rd_wr_lock.lock(); 
  delete[] m_buffer; 
  m_rd_wr_lock.unlock(); 
}

uint ConcurrentCircularBuffer::nb_produce(uint  size, const char* src) 
{
  // assert(size>0);
  uint  capacity = get_capacity();
  uint   wr      = m_wr_ptr; 
  m_rd_wr_lock.lock();
  if ((m_size+size) > capacity) {
    size = capacity - m_size; //reduce size to what is available
    if (size == 0) { // buffer is full
      m_rd_wr_lock.unlock();
      return size;
    }
  }
  copy_from(src, wr, size, capacity);
  m_size += size;
  if (m_blocked_consumer && m_waited_bytes <= m_size) {
    CommandInitiator* thread = (CommandInitiator*) m_blocked_consumer; // cast away volatile...
    m_blocked_consumer = 0;
    thread->resume(true);
  }
  m_wr_ptr = (wr+size) & m_capacity_mask; 
  m_rd_wr_lock.unlock();
  return size; 
}

bool ConcurrentCircularBuffer::bl_consume(uint size, char* dest, CommandInitiator* curr_thread) 
{
  uint  capacity_mask = m_capacity_mask;
  uint  capacity      = get_capacity();
  uint  chunk;
  while (size > 0) {
    chunk = size;
    if (chunk > capacity) chunk = capacity;
    uint  rd = m_rd_ptr;
    m_rd_wr_lock.lock();
    if (m_size < chunk) {
      m_blocked_consumer = curr_thread;
      m_waited_bytes = chunk;
      m_rd_wr_lock.unlock();
      if (!curr_thread->yield()) return false;
      m_rd_wr_lock.lock();
    }
    copy_to(dest, rd, chunk, capacity);
    m_size -= chunk;
    dest   += chunk;
    m_rd_ptr = (rd+chunk) & capacity_mask; // single reader ensured:no need to protect...
    size -= chunk;
    m_rd_wr_lock.unlock();
  }
  return true;
}

inline void ConcurrentCircularBuffer::copy_from(const char* src, uint wr_ptr, uint bytes_to_copy, uint buffer_capacity) 
{
  // assert(bytes_to_copy <= buffer_capacity);
  if ((wr_ptr + bytes_to_copy) > buffer_capacity) {
    uint bytes_until_end_of_buffer = buffer_capacity - wr_ptr;
    // copy last src bytes to [0,bytes_until_end_of_buffer]
    memcpy(&m_buffer[0], &src[bytes_until_end_of_buffer], bytes_to_copy - bytes_until_end_of_buffer);
    bytes_to_copy = bytes_until_end_of_buffer;
  }
  // copy first src bytes to [wr_ptr,wr_ptr+bytes_to_copy]
  memcpy(&m_buffer[wr_ptr], src, bytes_to_copy);
}

inline void ConcurrentCircularBuffer::copy_to(char* dest, uint rd_ptr, uint bytes_to_copy, uint buffer_capacity) 
{
  // assert(bytes_to_copy <= buffer_capacity);
  if ((rd_ptr + bytes_to_copy) > buffer_capacity) {
    uint bytes_until_end_of_buffer = buffer_capacity - rd_ptr;
    memcpy(&dest[bytes_until_end_of_buffer], &m_buffer[0], bytes_to_copy - bytes_until_end_of_buffer);
    bytes_to_copy = bytes_until_end_of_buffer;
  }
  memcpy(dest, &m_buffer[rd_ptr], bytes_to_copy);
}

} // namespace

