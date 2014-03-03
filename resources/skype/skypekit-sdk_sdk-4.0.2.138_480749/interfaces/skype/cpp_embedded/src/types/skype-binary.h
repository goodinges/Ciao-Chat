
#ifndef __SKYPE_BINARY_H
#define __SKYPE_BINARY_H

#include <stdlib.h> //size_t
#include <string.h>

/*! \class SEBinary skype-binary.h "skype-binary.h"
 *  \brief SEBinary Represents a BLOB. This used, for example, for avatar images.
 */
class SEBinary
{
public:
        SEBinary() : ref(0), buffer(0), len(0) {}
        SEBinary(const SEBinary &b) : ref(0), buffer(0), len(0) {
                set(b.buffer, b.len);
        }
        SEBinary(unsigned char * buf, size_t length) : ref(0), buffer(0), len(0) {
                set(buf, length);
        }
        SEBinary(char * buf, size_t length) : ref(0), buffer(0), len(0) {
                set(buf, length);
        }        
        ~SEBinary() {
                free(buffer);
        }
        void set(const void *buf, unsigned int new_size);
        void resize(unsigned int new_size);
        void reserve(unsigned int new_size) {
                resize (new_size);
        }
        void set_at_offset(unsigned int offset, const void *buf, unsigned int new_size);
        size_t getSize() const {
                return len;
        }
        size_t size() const {
                return len;
        }
        operator const char *(void) const {
                return (const char *)buffer;
        }
        const char *data(void) const {
                return (const char *)buffer;
        }     
        char *data(void) {
                return (char *)buffer;
        }          
        operator char *(void) {
                return (char *)buffer;
        }
        bool operator ==(const SEBinary &b) const;
        bool operator !=(const SEBinary &b) const {
                return !operator==(b);
        }
        const SEBinary & operator=(const SEBinary &b);
private:
        unsigned int ref;
        unsigned char* buffer;
        size_t len;
};

#endif
