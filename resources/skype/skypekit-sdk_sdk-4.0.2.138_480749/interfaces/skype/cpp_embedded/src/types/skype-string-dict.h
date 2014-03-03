
#ifndef __SKYPE_STRING_DICT_H
#define __SKYPE_STRING_DICT_H

#include <stddef.h>
#include <skype-thread.h>
#include "skype-string.h"

class SEStringList;

/** \class SEStringDict skype-string-dict.h "skype-string-dict.h"
 *  \brief SEStringDict is a dictionary class of strings associated to strings.
 */
class SEStringDict
{
public:
        /** Creates an empty dictionary of string/string. */
        SEStringDict();
        /** Constructs an implicitly shared copy of dict. */
        SEStringDict(const SEStringDict& dict);
        /** Destroys the dictionary and frees the data if this is the last reference to it. */
        ~SEStringDict();

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
        void insert(const SEString& key, const SEString& value);
        /** Finds the specified key in the dictionary.
         * \param key		Key to search.
         *					Default is to search for 0, which is the most often used key name
         *					in the Embedded API.
         * \param offset 	When the dictionary contains several entries with the same key,
         *					use the offset param to indicate which key you want.
         * \return null string if the key is not found.
         */
        SEString find(const SEString& key = "0", unsigned int offset = 0) const;
        /** Assigns a shallow copy of dict to this dictionary and returns a reference to it.
         * This is very fast because the dictionary isn't actually copied.
         */
        SEStringDict& operator=(const SEStringDict& dict);

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
        SEString keyAt(size_t i) const;

        /** \cond INTERNAL */
        /** Formats a dictionary according to internal format, i.e. a string with a list
         * of pairs KEY="VALUE" separated by a space.
         */
        SEString format(bool escape_args = true) const;
        /** Creates a list of pairs key-value and parses it with
         * SEStringDict::parse(const SEStringList& str).
         */
        static SEStringDict parse(const SEString& str);
        /** Parses a list of pairs key-value according the internal format, which is
         * 		KEY="VALUE"
         * We assume very strict formatting here, any error in the formatting will make
         * the parsing fail.
         * Warning: The value will be null (SEString::isNull() is true) if the key is not present,
         * but also if the value is an empty string.
         */
        static SEStringDict parse(const SEStringList& str);
        
        SEString value(const SEString& key) { return find(key); }
        
protected:
        void d_ref();
        void d_unref();
        void detach();
        /** \endcond */
};

#endif // __SKYPE_STRING_DICT_H
