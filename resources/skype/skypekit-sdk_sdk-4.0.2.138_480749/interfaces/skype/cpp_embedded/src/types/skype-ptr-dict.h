
#ifndef __SKYPE_PTR_DICT_H
#define __SKYPE_PTR_DICT_H

#include <stddef.h>
#include "skype-string.h"
#include <skype-string-list.h>
#include <skype-thread.h>

/** \class SEPtrDict skype-ptr-dict.h "skype-ptr-dict.h"
 *  \brief SEPtrDict is a dictionary class of pointers associated to strings.
 */
class SEPtrDict
{
public:
        /** Creates an empty dictionary of pointers. */
        SEPtrDict();
        /** Constructs an implicitly shared copy of dict. */
        SEPtrDict(const SEPtrDict& dict);
        /** Destroys the dictionary and frees the data if this is the last reference to it. */
        ~SEPtrDict();

        /** \cond INTERNAL */
protected:
        class Element;
        class Data;
        Data* d;
        /** \endcond */

public:
        /** Inserts the key with the value into the dictionary.
         * Multiple items can have the same key, they are not overwritten.
         * You can access them with the offset parameter of the find() function.
         * If key is null, nothing is inserted.
         */
        void insert(const SEString& key, const void* new_value);
        /** Finds the specified key in the dictionary.
         * \param key		Key to search.
         * \param offset 	When the dictionary contains several entries with the same key,
         *					use the offset param to indicate which key you want.
         * \return null if the key is not found.
         */
        const void*& find(const SEString& key = "0", unsigned int offset = 0) const;
        /** Assigns a shallow copy of dict to this dictionary and returns a reference to it.
         * This is very fast because the dictionary isn't actually copied.
         */
        SEPtrDict& operator=(const SEPtrDict& dict);

        /** Returns how many elements are in the dictionary. */
        size_t size() const;

        /** Use this function to iterate through the dictionary.
         * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
         * See also keyAt()
         */
        const void*& operator[](size_t i) const;
        /** Use this function to iterate through the dictionary.
         * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
         * See also operator[]()
         */
        SEString keyAt(size_t i) const;
        
        const void*& value(const SEString& key) { return find(key); }
        SEStringList keys();
        
        /** \cond INTERNAL */

protected:
        void d_ref();
        void d_unref();
        void detach();
        /** \endcond */
};

#endif // __SKYPE_PTR_DICT_H
