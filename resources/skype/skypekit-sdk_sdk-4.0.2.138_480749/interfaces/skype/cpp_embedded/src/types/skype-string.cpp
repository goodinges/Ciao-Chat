
#include "skype-string.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#ifndef snprintf
	#define snprintf _snprintf
#endif
#ifndef strtoull
	#define strtoull _strtoui64
#endif
#endif

/** \cond INTERNAL */
// Tune for "malloc count" vs. "total memory allocation" tradeoff
// Affects mostly string concatenation
// 0 means no preallocation
#ifndef SE_STRING_DELTA
#define SE_STRING_DELTA 0
#endif

// Used to adjust the malloc-ed size to the highest multiple of 16
#define CALC_SIZE(x) ((x) / 16 + 1) * 16 + 16 * SE_STRING_DELTA
// NB: original version: #define CALC_SIZE(x) (x) + 25

class SEString::Data
{
public:
        unsigned int ref;
        char* str;
        size_t real_size;
        bool isBin; // when true, the string contains binary data
        SEMutex lock;
};

SEString::SEString(unsigned int buf)
{
        d = new Data();

        d->ref = 1;
        // we don't add DELTA since this constructor is always called with the exact size of the string
        d->str = (char*)malloc(buf + 1);
        d->real_size = buf;
        d->isBin = false;
}

// char    escaped
// "        \"
// ,        \,      (don't forget to escape commas as they are used to mark lists)
// \        \ \     (slash slash)
// 0x00     \0      (all null characters are translated to slash and zero (number zero))

SEString SEString::escape() const
{
        if (isNull())
                return *this;

        size_t new_size = 0;
        size_t mylen = length();

        for (size_t n = 0; n < mylen; n++) {
                if ((',' == d->str[n]) || ('"' == d->str[n]) ||('\\' == d->str[n]))
                        new_size++;
        }

        new_size += mylen;

        SEString str(new_size + 1);

        char* s_from = d->str;
        char* to = str.d->str;

        while (*s_from) {
                if ((',' == *s_from) || ('"' == *s_from) || ('\\' == *s_from)) {
                        *to = '\\';
                        to++;
                }

                *to = *s_from;

                to++;
                s_from++;
        }

        *to = '\0';

        return str;
}

SEString SEString::unescape() const
{
        if (isNull())
                return *this;

        size_t mylen = length();

        SEString str(mylen + 1);

        size_t to = 0;
        size_t cur = 0;
        size_t len = mylen;

        while (cur < len) {
                if ('\\' == d->str[cur]) {
                        cur++;
                }

                str.d->str[to++] = d->str[cur++];
        }

        str.d->str[to++] = '\0';

        return str;
}

// custom function to optimize SEStringDict::format
const SEString SEString::keyValue(const SEString& key, const SEString& value) // static
{
        if (key.isNull())
                return SEString();

        size_t keylen = key.length();
        size_t vallen = value.length();

        SEString str(keylen + 2 + vallen + 3);

        char * ptr = str.d->str;

        strcpy(ptr, key.d->str);
        ptr += keylen;
        strcpy(ptr, "=\"");
        ptr += 2;
        if (!value.isNull()) { // or should we not send the pair at all?
                strcpy(ptr, value.d->str);
                ptr += vallen;
        }
        strcpy(ptr, "\" ");

        return str;
}

void SEString::markAsBinary()
{
        if (d) d->isBin = true;
}
/** \endcond */

SEString::operator const char*() const
{
        static const char* str_null = "";

        if (isNull())
                return str_null;
        else
                return d->str;
}

size_t SEString::length() const
{
        if (isNull())
                return 0;
        else
                return strlen(d->str);
}

bool SEString::isBinary() const
{
        return (d && d->isBin);
}

bool SEString::startWith(const SEString& str) const
{
        if (isNull())
                return false;

        size_t strlen = str.length();

        if (!strlen) // same as (str.isEmpty())
                return true;

        return (0 == strncmp(d->str, str.d->str, strlen));
}

SEString SEString::substr(int s_from, int to) const
{
        if (isNull())
                return *this;

        if (s_from < 0)
                s_from = 0;

        size_t len = length();

        if (to >= int(len))
                to = len - 1 ;

        if (to < 0) {
                to = len + to - 1;
        }

        if (to < s_from) {
                int tmp = to;
                to = s_from;
                s_from = tmp;
        }

        int newl = to - s_from + 1;

        SEString tmp(newl + 1);

        strncpy(tmp.d->str, d->str + s_from, newl);
        tmp.d->str[newl] = 0;

        return tmp;
}

SEString SEString::right(unsigned int len) const
{
        if (isNull())
                return *this;

        size_t mylen = length();
        if (len > mylen)
                len = mylen;

        SEString tmp(len + 1);

        strcpy(tmp.d->str, d->str + mylen - len);

        return tmp;
}

SEString SEString::trim(const SEString& str) const
{
        size_t strlen = str.length();

        if (!strlen || !startWith(str)) // same as (str.isEmpty() || !startWith(str))
                return *this;
        else
                return right(length() - strlen);
}

const SEString SEString::from(int n) // static
{
        char res[12];

        snprintf(res, 12, "%d", n);

        return SEString(res);
}

const SEString SEString::from(uint64 n) // static
{
        char res[24];

        snprintf(res, 24, "%llu", n);

        return SEString(res);
}

const SEString SEString::from(unsigned char chr) // static
{
        char res[2];

        res[0] = chr;
        res[1] = '\0';

        return SEString(res);
}

const SEString SEString::from(unsigned int u, unsigned int base) // static
{
        char res[11];

        if (16 == base)
                snprintf(res, 11, "%X", u);
        else
                snprintf(res, 11, "%u", u);

        return SEString(res);
}

const SEString SEString::from(bool b) // static
{
        char res[2];

        res[1] = '\0';

        if (b)
                res[0] = '1';
        else
                res[0] = '0';

        return SEString(res);
}

const SEString SEString::from(char *bin, unsigned int len) // static
{
        // For binaries, we escape immediately to allow it to survive all the string operations
        size_t new_size = 0;
        unsigned int i;

        for (i = 0; i < len; i++) {
                if (('\0' == bin[i]) || ('\\' == bin[i]) || (',' == bin[i]) || ('"' == bin[i]))
                        new_size++;
        }
        new_size += len;

        SEString str(new_size + 1); // 1 for end of string null char
        str.markAsBinary();

        char* to = str.d->str;

        for (i = 0; i < len; i++) {
                if ('\0' == bin[i]) { // null is escaped differently
                        *to = '\\';
                        to++;
                        *to = '0';
                        to++;
                        continue;
                }
                // normal escaping like in escape()
                if ((',' == bin[i]) || ('"' == bin[i]) || ('\\' == bin[i])) {
                        *to = '\\';
                        to++;
                }
                *to = bin[i];
                to++;
        }

        *to = '\0';

        return str;
}

bool SEString::toBool() const
{
        if (isNull())
                return false;

        return atoi(d->str) != 0;
}

int SEString::toInt() const
{
        if (isNull())
                return 0;

        return atoi(d->str);
}

unsigned int SEString::toUInt() const
{
        if (isNull())
                return 0;

        return strtoul(d->str, NULL, 10);
}

SEString::uint64 SEString::toUInt64() const
{
        if (isNull())
                return 0;

        return strtoull(d->str, NULL, 10);
}

size_t SEString::toBinary(char *bin) const
{
        if (isNull())
                return 0;

        // Check we actually have a binary
        if (!isBinary())
                return 0;

        // For a binary, we didn't unescape in the low-level SEStringDict::parse, but we do it here only
        size_t to = 0;
        size_t cur = 0;
        size_t len = length();

        while (cur < len) {
                if ('\\' == d->str[cur]) {
                        cur++;

                        if ('0' == d->str[cur]) {	// escaped null
                                bin[to++] = '\0';
                                cur++;
                                continue;
                        }
                }

                bin[to++] = d->str[cur++];
        }

        return to;
}

SEString SEString::getHexRepresentation() const
{
        // check we actually have a binary
        if (!isBinary()) return SEString();
        // convert to binary
        char * ps = (char *)malloc(length());
        size_t len = 2 * toBinary(ps);
        // convert to hexa
        static const char hex[17] = "0123456789abcdef";
        SEString str(len);
        unsigned char *ptr = (unsigned char *)ps;
        unsigned int count = 0;
        for (; count < len; ptr++) {
                str.d->str[count++] = hex[*ptr >> 4];
                str.d->str[count++] = hex[*ptr & 0xf];
        }
        str.d->str[count] = 0;
        // free temp buf
        free(ps);
        return str;
}

SEString& SEString::operator+=(const SEString &str)
{
        if (isNull())
                return (*this = str);
        if (str.isNull())
                return (*this);

        detach();

        size_t l = length();

        char* str_new  = se_realloc(l + str.length() + 1);

        if (str_new) {
                strcpy(str_new + l, str.d->str);

                d->str = str_new;
        }

        return *this;
}

SEString& SEString::operator+=(const char* str)
{
        if (isNull())
                return (*this = str);
        if (str == NULL)
                return (*this);

        detach();

        size_t l = length();
        size_t l2 = strlen(str);

        char* str_new = se_realloc(l + l2 + 1);

        if (str_new) {
                strcpy(str_new + l, str);

                d->str = str_new;
        }

        return *this;
}

SEString& SEString::operator=(const SEString& str)
{       
        Data* strd = const_cast<SEString&>(str).d_ref(); // shall be mutable
        d_unref();
        d = strd;

        return *this;
}

SEString& SEString::operator=(const char* str)
{
        d_unref(); 

        if (str) {
                d = new Data();
                d->ref = 1;
                d->real_size = CALC_SIZE(strlen(str) + 1);
                d->str = (char*)malloc(d->real_size);
                strcpy(d->str, str);
                d->isBin = false;
        } else {
                d = 0;
        }

        return *this;
}

/** \cond INTERNAL */
SEString::Data* SEString::d_ref()
{
        SEMutexLock lock_string(mutex);
        if (0 == d)
                return d;
         
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
        return d;
}

void SEString::d_unref()
{
        SEMutexLock lock_string(mutex);
        if (0 == d)
                return;
        
        SEMutex* lock = &d->lock;
        
        lock->Acquire(); 
        
        if (d->ref > 1)
                d->ref--;
        else {
                Data* tmp_d = d;
                d = 0;
                lock->Release();
                free(tmp_d->str);
                delete tmp_d;
                return; 
        }
        
        lock->Release();
}

void SEString::detach()
{
        SEMutexLock lock_string(mutex);
        if (0 == d)
                return;
        
        SEMutex* lock = &d->lock;
        lock->Acquire();
        
        if (1 != d->ref) {

                d->ref--;
                
                size_t sz = CALC_SIZE(strlen(d->str) + 1);
                char* buf = (char*)malloc(sz);
                strcpy(buf, d->str);
                lock->Release();

                Data* d_new = new Data();
                d_new->ref = 1;
                d_new->real_size = sz;
                d_new->str = buf;
                d_new->isBin = d->isBin;
                
                d = d_new;
                return;
        }
        
        lock->Release();
}
/** \endcond */

SEString SEString::deepCopy() const
{
        SEString tmp = *this;

        tmp.detach();

        return tmp;
}

unsigned int SEString::hash(unsigned int hash_size) const
{
        if (isNull())
                return 0;

        const char *ptr;
        unsigned int val;

        val = 0;
        ptr = d->str;

        while (*ptr) {
                int tmp;
                val = (val << 4) + (*ptr);
				tmp = (val & 0xf0000000);
                if (tmp) {
                        val = val ^ (tmp >> 24);
                        val = val ^ tmp;
                }

                ptr++;
        }

        return val % hash_size;
}

bool SEString::equals(const SEString& str) const
{
        if (isNull() && str.isNull())
                return true;

        else if (isNull() || str.isNull())
                return false;

        return (0 == strcmp(d->str, str.d->str));
}

bool SEString::equals(const char* str) const
{
        if (isNull() && (0 == str))
                return true;

        if (isNull() || (0 == str))
                return false;

        return (0 == strcmp(str, d->str));
}

int SEString::find(char c) const
{
        if (isNull())
                return -1;

        char* found = strchr(d->str, c);

        if (0 == found)
                return -1;

        return (found - d->str);
}

int SEString::find(int startpos, char c) const
{
        if (isNull())
                return -1;

        char* found = strchr(d->str+startpos, c);

        if (0 == found)
                return -1;

        return (found - d->str);
}

/** \cond INTERNAL */
char* SEString::se_realloc(size_t new_size)
{
        if (d->real_size >= new_size)
                return d->str;
        else {
                d->real_size = CALC_SIZE(new_size);

                return (char*)::realloc(d->str, d->real_size);
        }
}
/** \endcond */

void SEString::Format(const char *format, va_list arglist)
{
        const unsigned int bufsize = 1024;
        char buffer [bufsize], *buf = buffer;
        unsigned int requested = vsnprintf (buf, bufsize, format, arglist);
        if (requested >= bufsize) {
                buf = new char[requested + 1];
                requested = vsnprintf(buf, requested + 1, format, arglist);
                *this = buf; //copy
                delete [] buf;
        } else {
                *this = buf; //copy
        }
}

#ifdef SE_STRING_TEST_PROGRAM
int main(char **, int)
{
        int i;
        SEString strings[] = {
                "Hello World!",
                "Hel,lo\\ World!",
                "Hel\"lo Wo\\9rld!",
                "Hel,lo Wo\\0rld!",
                SEString()
        };
        for (i = 0; !strings[i].isNull(); i++) {
                SEString escaped = strings[i].escape();
                SEString unescaped = escaped.unescape();
                if (strcmp((const char*)strings[i], (const char*)unescaped))
                        printf("String Failed:\n%s\n%s\n%s\n\n", (const char*)strings[i], (const char*)escaped, (const char*)unescaped);
        }

        // 0x5c = '\\', 0x30 = '0', 0x00 = 0, 0x22 = '"'
        const char bins[] = {
                '1', '2', '3', '4', '5', '6', '7', '8' ,
                '1', '2', 0x5c, 0x30, '5', '6', '7', '8' ,
                '1', '2', 0x5c, 0x30, 0x5c, 0x5c, 0x5c, '8' ,
                '1', '2', 0x5c, 0x30, '5', '6', '7', '8' ,
                '1', '2', 0x00, '4', '5', '6', '7', '8' ,
                '1', 0x5c, 0x00, 0x5c, '5', '6', 0x5c, 0x5c ,
                '1', 0x5c, 0x00, 0x00, '5', 0x00, 0x5c, '8' ,
                0x00, '2', 0x5c, 0x30, '5', 0x22, 0x22, '8' ,
                0x5c, '2', 0x5c, 0x30, '5', 0x22, 0x22, 0x00 ,
                0x00, 0x00, 0x5c, 0x30, '5', 0x22, 0x22, 0x5c ,
                'E'
        };
        size_t bins_len = 8;
        for (i = 0; bins[i] != 'E'; i += bins_len) {
                SEString escaped = SEString::from((char*)(bins + i), bins_len);
                char *unescaped = (char *)malloc(escaped.length());
                size_t unescaped_len = escaped.toBinary(unescaped);
                //printf("%d, escaped.length()=%d unescaped_len=%d\n", i/bins_len, escaped.length(), unescaped_len);
                if ( (memcmp((char*)(bins + i), unescaped, bins_len)) || (unescaped_len != bins_len) )
                        printf("Binary Failed: index %d\n", i/bins_len);
                //printf("%s\n", (const char*)escaped.getHexRepresentation());
                free(unescaped);
        }

}
#endif
