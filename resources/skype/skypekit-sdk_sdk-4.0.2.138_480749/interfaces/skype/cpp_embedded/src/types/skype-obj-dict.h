
#ifndef __SKYPE_OBJ_DICT_H
#define __SKYPE_OBJ_DICT_H

#include <stddef.h>
#include <skype-int-list.h>
#include <skype-thread.h>

class SEObject;

/** \class SEObjDict skype-obj-dict.h "skype-obj-dict.h"
 *  \brief SEObjDict is a dictionary class of SEObject pointers associated to integers.
 */
class SEObjDict
{
public:
        /** Creates an empty dictionary of pointers. */
        SEObjDict();
        /** Constructs an implicitly shared copy of dict. */
        SEObjDict(const SEObjDict& dict);
        /** Destroys the dictionary and frees the data if this is the last reference to it. */
        ~SEObjDict();

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
        void insert(const int key, const SEObject* value);
        /** Finds the specified key in the dictionary.
         * \param key		Key to search.
         * \param offset 	When the dictionary contains several entries with the same key,
         *					use the offset param to indicate which key you want.
         * \return null if the key is not found.
         */
        const SEObject*& find(const int key, unsigned int offset = 0) const;
        /** Assigns a shallow copy of dict to this dictionary and returns a reference to it.
         * This is very fast because the dictionary isn't actually copied.
         */
        SEObjDict& operator=(const SEObjDict& dict);

        /** Returns how many elements are in the dictionary. */
        size_t size() const;

        /** Use this function to iterate through the dictionary.
         * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
         * See also keyAt()
         */
        const SEObject*& operator[](size_t i) const;
        /** Use this function to iterate through the dictionary.
         * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
         * See also operator[]()
         */
        int keyAt(size_t i) const;

        bool remove(const int key);
        
        const SEObject*& value(const int key) { return find(key); }
        
        SEIntList keys();
        
        /** \cond INTERNAL */

protected:
        void d_ref();
        void d_unref();
        void detach();
        /** \endcond */
};

#endif // __SKYPE_OBJ_DICT_H
