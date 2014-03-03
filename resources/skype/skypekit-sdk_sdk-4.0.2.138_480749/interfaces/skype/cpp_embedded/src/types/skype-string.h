
#ifndef __SKYPE_STRING_H
#define __SKYPE_STRING_H

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#include <skype-thread.h>

/**
 * \class SEString
 * \brief SEString is basic char* based string class.
 *
 * SEString uses implicit sharing (i.e. reference counting).
 * In all of the SEString methods that take const char * parameters, the const char * is
 * interpreted as a classic C-style '\\0'-terminated ASCII string.
 * A SEString that has not been assigned to anything is null, i.e. both the length
 * and data pointer is 0. A SEString that references the empty string ("", a
 * single '\\0' char) is empty.
 * Use if ( !str.isNull() ) to check for a non-null string.
 *
 * This class is not thread-safe. Make sure to use deepCopy() when passing between threads.
 */
class SEString
{
public:
        /** Constructs a null string, i.e. both the length and data pointer are 0.
         * See also isNull().
         */
        inline SEString();

        /** Constructs a string that is a deep copy of str, interpreted as a classic C string.
         * If str is 0, then a null string is created.
         */
        inline SEString(const char* str);

        /**
         * Constructs an implicitly shared copy of str.
         * This is very fast since it only involves incrementing a reference count.
         * \param str is a string to copy
         */
        inline SEString(const SEString& str);

        /** Destroys the string and frees the string's data if this is the last reference to the string. */
        inline ~SEString();

        /** \cond INTERNAL */
//protected:
        /** Allocates a string of the specified size */
        SEString(unsigned int buf);

protected:
        class Data;
        Data* d;
        /** \endcond */

public:
        /** \cond INTERNAL */
        /** The wrapper calls this function when necessary, the UI should NOT use it. */
        SEString escape() const;
        /** The wrapper calls this function when necessary, the UI should NOT use it. */
        SEString unescape() const;
        /** The wrapper calls this function when necessary, the UI should NOT use it. */
        static const SEString keyValue(const SEString& key, const SEString& value);
        /** Marks the string as binary one. Only used by wrapper internals. */
        void markAsBinary();
        /** \endcond */

        /** Returns a pointer to the string storing the data.
         * If the string is null (as defined by isNull() == true), returns an empty string.
         *
         * If you need to keep this string after the SEString object gets deleted, make a copy.
         * In other words, the char* pointer is valid ONLY AS LONG as the corresponding SEString
         * is valid. When the SEString gets deleted, the data get freed and the pointer is invalid.
         *
         * Some examples:
         * <PRE>
         * 	// Works:
         * 	SEString skypeName = me->GetProp(Contact::P_SKYPENAME);
         * 	printf("CONTACT %s\n", (const char*)skypeName);
         *
         * 	// Doesn't work, segmentation fault:
         * 	const char * skypeName = (const char *) me->GetProp(Contact::P_SKYPENAME);
         * 	printf("CONTACT %s\n", skypeName);
         *
         * 	// Doesn't work:
         * 	const char * skypeName;
         * 	if (1) {
         * 		SEString skypeName_1 = me->GetProp(Contact::P_SKYPENAME);
         * 		skypeName = (const char *) skypeName_1;
         * 	} // skypeName_1 deleted here
         * 	printf("CONTACT %s\n", skypeName); // skypeName_1 has been deleted and the correponding data too
         *
         * 	// Works:
         * 	SEString skypeName_1 = me->GetProp(Contact::P_SKYPENAME);
         * 	const char * skypeName = (const char *) skypeName_1;
         * 	printf("CONTACT %s\n", skypeName);
         * </PRE>
         *
         * NB: The example above uses SEObject::GetProp() but in UI it is strongly discouraged to use GetProp().
         * SEObject::GetProps() or Skype::GetMultipleObjectsProps() should be used instead.
         */
        operator const char*() const;
        const char* data() const {
                return operator const char*();
        }

        /** Returns the length of the string.
         * Null strings and empty strings have zero length.
         * See also isNull() and isEmpty().
         */
        size_t size() const {
                return length();
        }
        /** Returns the length of the string.
         * Null strings and empty strings have zero length.
         * See also isNull() and isEmpty().
         */
        size_t length() const;
        /** Returns true if the string is null; otherwise returns false.
         * A null string is always empty.
         * See also isEmpty() and length().
         */
        inline bool isNull() const;
        /** Returns true if the string is empty, i.e. if length() == 0;
         * otherwise returns false. Null strings are also empty.
         *		SEString a("");
         *		a.isEmpty();        // TRUE
         *		a.isNull();         // FALSE
         *
         *		SEString b;
         *		b.isEmpty();        // TRUE
         *		b.isNull();         // TRUE
         *
         * See also isNull() and length().
         */
        inline bool isEmpty() const;
        /** Checks if the string contains binary data */
        bool isBinary() const;

        /** Returns true if the string starts with str; otherwise returns false. */
        bool startWith(const SEString& str) const;
        /** Returns a string that contains the len rightmost characters of the string.
         * If len is greater than the length of the string then the whole string is returned.
         */
        SEString right(unsigned int len) const;
        /** Removes str from the string. */
        SEString trim(const SEString& str) const;
        /** Returns a section of the string.
         * The returned string consists of the characters from position start to position
         * end inclusive. If end is not specified, all fields from position start to the
         * end of the string are included.
         */
        SEString substr(int from, int to) const;
        /** Creates a copy of the string that can be passed between threads. */
        SEString deepCopy() const;
        /** Returns a hashed value of the string. For internal use. */
        unsigned int hash(unsigned int size = 1) const;
        /**  Returns true if str is equal to the string; otherwise returns false.
         * Note that a null string is not equal to a not-null empty string.
         */
        bool equals(const SEString& str) const;
        /**  Returns true if str is equal to the string; otherwise returns false.
         * Note that a null string is not equal to a not-null empty string.
         */
        bool equals(const char* str) const;
        /** Finds the first occurrence of the character.
         * \return  Returns the position of c or -1 if c could not be found.
        */
        int find(char c) const;
        int find(int startpos, char c) const;

        /** Appends str to the string and returns a reference to the string. */
        SEString& operator+=(const SEString &str);
        /** Appends str to the string and returns a reference to the string. */
        SEString& operator+=(const char *str);
        /** Assigns a shallow copy of s to this string and returns a reference to this string.
         * This is very fast because the string isn't actually copied.
         */
        SEString& operator=(const SEString& str);
        /** Assigns a deep copy of str, interpreted as a classic C string to
         * this string and returns a reference to this string.
         * If str is 0, then a null string is created. See also isNull(). */
        SEString& operator=(const char* str);

        typedef unsigned long long uint64;

        /** Converts the string to a boolean (works like atoi).
         * Returns false on null strings.
         */
        bool toBool() const;
        /** DEPRECATED: Use toUInt instead.
         */
        int toInt() const;
        /** Converts the string to an unsigned integer (like strtoul(.. 10).
         * Returns 0 on null strings.
         */
        unsigned int toUInt() const;
        /** Converts the string to an unsigned 64 bits integer (like strtoul(.. 10).
         * Returns 0 on null strings.
         */
        uint64 toUInt64() const;
        /** Converts the string to a binary.
         * \param bin Initialized array to store the binary (use length() to get a big enough array)
         * \return Size of the binary (which is likely to be smaller than length()).
         *		   Returns 0 if the string is null.
         */
        size_t toBinary(char *bin) const;
        /** For binaries returns a string with the hexa representation */
        SEString getHexRepresentation() const;

        void Format(const char *format, va_list arglist);
        void Format(const char *format, ...) {
                va_list args;
                va_start(args, format);
                Format(format, args);
                va_end(args);
        }
public:
        /** Converts an integer into a SEString. */
        static const SEString from(int n);
        /** Converts an integer into a SEString. */
        static const SEString from(uint64 n);
        /** Converts an integer into a SEString using the given base. Only base 10 and 16 are supported. */
        static const SEString from(unsigned int u, unsigned int base = 10);
        /** Converts a boolean into a SEString. */
        static const SEString from(bool b);
        /** Converts a binary into a SEString. */
        static const SEString from(char *bin, unsigned int len);
        static const SEString from(unsigned char chr);
        
        static const SEString Compose(const char *format, ...) {        
                va_list args;
                va_start(args, format);
                SEString temp;
                temp.Format(format, args);
                va_end(args);
                return temp;        
        }
        
        /** \cond INTERNAL */
protected:
        Data* d_ref();
        void  d_unref();
        void  detach();
        char* se_realloc(size_t new_size);
        SEMutex mutex;
        /** \endcond */
};

/** Returns a string which is the result of concatenating the string s1 and the string s2. */
inline const SEString operator+(const SEString& s1, const SEString& s2)
{
        SEString tmp(s1);
        tmp += s2;
        return tmp;
}

/** Returns a string which is the result of concatenating the string s1 and the string s2. */
inline const SEString operator+(const SEString& s1, const char* s2)
{
        SEString tmp(s1);
        tmp += s2;
        return tmp;
}

/** Returns a string which is the result of concatenating the string s1 and the string s2. */
inline const SEString operator+(const char* s1, const SEString& s2)
{
        SEString tmp(s1);
        tmp += s2;
        return tmp;
}

/** Returns true if s1 is equal to s2; otherwise returns false. */
inline bool operator==(const SEString& s1, const SEString& s2)
{
        return s1.equals(s2);
}

/** Returns true if s1 is equal to s2; otherwise returns false. */
inline bool operator==(const SEString& s1, const char* s2)
{
        return s1.equals(s2);
}

/** Returns true if s1 is equal to s2; otherwise returns false. */
inline bool operator==(const char* s1, const SEString& s2)
{
        return s2.equals(s1);
}

/** Returns true if s1 is not equal to s2; otherwise returns false. */
inline bool operator!=(const SEString& s1, const SEString& s2)
{
        return !s1.equals(s2);
}

/** Returns true if s1 is not equal to s2; otherwise returns false. */
inline bool operator!=(const SEString& s1, const char* s2)
{
        return !s1.equals(s2);
}

/** Returns true if s1 is not equal to s2; otherwise returns false. */
inline bool operator!=(const char* s1, const SEString& s2)
{
        return !s2.equals(s1);
}


SEString::SEString()
                :d(0)
{

}

SEString::SEString(const char* str)
                :d(0)
{
        *this = str;
}

SEString::SEString(const SEString& str)
                :d(0)
{
        *this = str;
}

SEString::~SEString()
{
        d_unref();
}

bool SEString::isNull() const
{
        return (0 == d);
}

bool SEString::isEmpty() const
{
        return (0 == length());
}

#endif // __SKYPE_STRING_H
