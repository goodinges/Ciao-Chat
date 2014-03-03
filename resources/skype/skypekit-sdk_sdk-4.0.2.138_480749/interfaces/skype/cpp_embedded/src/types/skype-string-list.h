
#ifndef __SKYPE_STRING_LIST_H
#define __SKYPE_STRING_LIST_H

#include "skype-string.h"
#include <skype-thread.h>

/*! \class SEStringList skype-string-list.h "skype-string-list.h"
 *  \brief SEStringList represents a list of strings (SEString objects).
 *
 * The operator [] is supported, which can be used to go through the list.
 */
class SEStringList
{
public:
        /** Creates an empty list of strings. */
        SEStringList();
        /** Constructs an implicitly shared copy of sl. */
        SEStringList(const SEStringList& sl);
        /** Destroys the list and frees the data if this is the last reference to the list. */
        ~SEStringList();

        /** \cond INTERNAL */
protected:
        class Element;
        class Data;
        Data* d;
        SEString empty;
        /** \endcond */

public:
        /** Appends a string at the end of the list. */
        void push_back(const SEString& str) {
                append(str);
        }
        /** Appends a string at the end of the list. */
        SEStringList& append(const SEString& str);
        /** Removes the first element from the list and returns it. */
        SEString peek();
        /** Returns the size of the list. */
        size_t size() const;
        /** Returns the string specified by its index. */
        SEString operator[](size_t n);
        /** Returns a string reference specified by its index. */
        const SEString &operator[](size_t n) const;
        /** Assigns a shallow copy of sl to this list and returns a reference to it.
         * This is very fast because the list isn't actually copied.
         */
        SEStringList& operator=(const SEStringList& sl);
        /** Creates a string out of the list of strings by joining them.
         * \param sep String to use as a separator between each joined string.
         * \param escape_args Should the parameters be escaped before joining them.
         */
        SEString join(const SEString& sep, bool escape_args = true) const;
        /** Splits a string in a list of string using the separaror sep. */
        static SEStringList split(const SEString& str, char sep, char esc = '\\');

        bool contains(const SEString& val);
        bool remove_val(const SEString& val);
        bool remove_pos(const unsigned int pos);
        int find_pos(const SEString& val);

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

#endif // __SKYPE_STRING_LIST_H
