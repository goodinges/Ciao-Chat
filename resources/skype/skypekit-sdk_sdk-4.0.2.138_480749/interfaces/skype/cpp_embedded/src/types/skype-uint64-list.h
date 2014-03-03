
#ifndef __SKYPE_UINT64_LIST_H
#define __SKYPE_UINT64_LIST_H

#include "skype-string.h"
#include <skype-thread.h>

/*! \class SEUInt64List skype-uint64-list.h "skype-uint64-list.h"
 *  \brief SEUInt64List represents a list of integers.
 *
 * The operator [] is supported, which can be used to go through the list.
 */
class SEUInt64List
{
public:
        typedef unsigned long long uint64;

        /** Creates an empty list of integers. */
        SEUInt64List();
        /** Constructs an implicitly shared copy of sl. */
        SEUInt64List(const SEUInt64List& sl);
        /** Destroys the list and frees the data if this is the last reference to the list. */
        ~SEUInt64List();

        /** \cond INTERNAL */
protected:
        class Element;
        class Data;
        Data* d;
        /** \endcond */

public:
        /** Appends an integer at the end of the list. */
        SEUInt64List& append(const uint64 i);
        void push_back(const uint64& i) {
                append(i);
        }
        /** Removes the first element from the list and returns it. */
        uint64 peek();
        /** Returns the size of the list. */
        size_t size() const;

        /** Returns the integer specified by its index. */
        uint64 & operator[](unsigned int index);
        uint64 & operator[](unsigned int index) const {
                SEUInt64List* This = (SEUInt64List*) this;
                return This->SEUInt64List::operator [](index);
        }

        /** Assigns a shallow copy of sl to this list and returns a reference to it.
         * This is very fast because the list isn't actually copied.
         */
        SEUInt64List& operator=(const SEUInt64List& sl);
        /** Splits a string in a list of integer using the separaror sep. */
        static SEUInt64List split(const SEString& str, char sep, char esc = '\\');

        bool contains(const uint64 val);
        bool remove_val(const uint64 val);
        bool remove_pos(const unsigned int pos);
        int find_pos(const uint64 val);
        void resize(const unsigned int new_size);
        void clear();
        
        /** \cond INTERNAL */
protected:
        void init();
        void d_ref();
        void d_unref();
        void detach();
        /** \endcond */
};

#endif // __SKYPE_UINT64_LIST_H

