
#ifndef __SKYPE_INT_DICT_H
#define __SKYPE_INT_DICT_H

#include <stddef.h>
#include <skype-thread.h>

class SEString;
class SEStringList;

/*! \class SEIntDict skype-int-dict.h "skype-int-dict.h"
 *  \brief SEIntDict is a dictionary class of integers associated to strings.
 *
 * It is used to set or retrieve multiple properties at once (see the SEObject class).
 */
class SEIntDict
{
public:
        /** Creates an empty dictionary of integers. */
        SEIntDict();
        /** Constructs an implicitly shared copy of dict. */
        SEIntDict(const SEIntDict& dict);
        /** Destroys the dictionary and frees the data if this is the last reference to it. */
        ~SEIntDict();

        /** \cond INTERNAL */
protected:
        class Element;
        class Data;
        Data* d;
        /** \endcond */

public:
        /** Insert a value into dictionary. If the key already exists, replace.
         * \param key
         * \param value
         * \param replace_only Do not insert, if key is not found, only replace.
         */
        void insert(const int key, const SEString& new_value, bool replace_only = false);
        /** Finds the value correspinding to the key. */
        SEString find(const int key) const;

        /** Assigns a shallow copy of dict to this dictionary and returns a reference to it.
         * This is very fast because the dictionary isn't actually copied.
         */
        SEIntDict& operator=(const SEIntDict& dict);

        /** Returns how many elements are in the dictionary. */
        size_t size() const;

        /** Use this function to iterate through the dictionary.
         * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
         * See also keyAt()
         */
        SEString operator[](size_t i) const;
        /** Use this function to iterate through the dictionary.
         * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
         * See also operator[]()
         */
        int keyAt(size_t i) const;

        /** \cond INTERNAL */
        /** This function is used by the internals of the wrapper. */
        SEString format() const;
        
        SEString value(const int key);

protected:
        void d_ref();
        void d_unref();
        void detach();
        /** \endcond */
};

#endif // __SKYPE_INT_DICT_H
