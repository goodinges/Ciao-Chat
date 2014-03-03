
#include "skype-binary.h"
#include <stdio.h>

void SEBinary::set(const void *buf, unsigned int new_size)
{
        resize(new_size);
        
        if (new_size > 0) {
                set_at_offset(0, buf, new_size);
        }
}

void SEBinary::resize(unsigned int new_size)
{
        if (len == new_size) return;
        
        if (new_size == 0) {
                free(buffer);
                buffer = 0;
        } else {
                if (buffer != 0) {
                        buffer = (unsigned char*) realloc(buffer, new_size);
                } else {
                        buffer = (unsigned char*) malloc(new_size);
                }
        }

        len = new_size;
}

void SEBinary::set_at_offset(unsigned int offset, const void *buf, unsigned int new_size)
{
        if (buf && new_size) {
                if (buf != buffer) {
                
                        if (offset + new_size > len) {
                                resize(offset + new_size);
                        }
                        
                        memmove(buffer + offset, buf, new_size);
                }
        } 
}

bool SEBinary::operator ==(const SEBinary &b) const
{
        return (b.len == len && !memcmp(b.buffer, buffer, len));
}

const SEBinary & SEBinary::operator=(const SEBinary &b)
{
        set(b.buffer, b.len);

        return *this;
}



