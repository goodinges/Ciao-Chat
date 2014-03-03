
#ifndef __SKYPE_INT_LIST_H
#define __SKYPE_INT_LIST_H

#include "skype-string.h"
#include <skype-thread.h>

/*! \class SEIntList skype-int-list.h "skype-int-list.h"
 *  \brief SEIntList represents a list of integers.
 *
 * The operator [] is supported, which can be used to go through the list.
 */
class SEIntList
{
public:
        /** Creates an empty list of integers. */
        SEIntList();
        /** Constructs an implicitly shared copy of sl. */
        SEIntList(const SEIntList& sl);
        /** Destroys the list and frees the data if this is the last reference to the list. */
        ~SEIntList();

        /** \cond INTERNAL */
protected:
        class Element;
        class Data;
        Data* d;
        /** \endcond */

public:
        /** Appends an integer at the end of the list. */
        SEIntList& append(const int i);
        void push_back(const int& i) {
                append(i);
        }
        /** Removes the first element from the list and returns it. */
        int peek();
        /** Returns the size of the list. */
        size_t size() const;

        /** Returns the integer specified by its index. */
        int & operator[](unsigned int index);
        int & operator[](unsigned int index) const {
                SEIntList* This = (SEIntList*) this;
                return This->SEIntList::operator [](index);
        }

        /** Assigns a shallow copy of sl to this list and returns a reference to it.
         * This is very fast because the list isn't actually copied.
         */
        SEIntList& operator=(const SEIntList& sl);
        /** Splits a string in a list of integer using the separaror sep. */
        static SEIntList split(const SEString& str, char sep, char esc = '\\');

        bool contains(const int val);
        bool remove_val(const int val);
        bool remove_pos(const unsigned int pos);
        int find_pos(const int val);
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

#endif // __SKYPE_INT_LIST_H
