
#ifndef __SKYPE_REF_LIST_H
#define __SKYPE_REF_LIST_H

#include <stdio.h>
#include <skype-thread.h>

class SEReference;

/*! \class SERefList skype-ref-list.h "skype-ref-list.h"
 *  \brief SERefList represents a list of SEReference objects.
 *
 * The operator [] is supported, which can be used to go through the list.
 */
class SERefList
{
public:
        /** Creates an empty list of references. */
        SERefList();
        /** Constructs an implicitly shared copy of sl. */
        SERefList(const SERefList& sl);
        /** Destroys the list and frees the data if this is the last reference to the list. */
        ~SERefList();

        /** \cond INTERNAL */
protected:
        class Element;
        class Data;
        Data* d;
        /** \endcond */
public:
        /** Appends a ref at the end of the list. */
        void push_back(const SEReference & ref) {
                append(ref);
        }
        /** Appends a ref at the end of the list. */
        SERefList& append(const SEReference & ref);
        /** Removes the first element from the list and returns it. */
        SEReference& peek();
        /** Returns the size of the list. */
        size_t size() const;
        /** Returns the ref specified by its index. */
        SEReference& operator[](size_t n) const;
        /** Assigns a shallow copy of sl to this list and returns a reference to it.
         * This is very fast because the list isn't actually copied.
         */
        SERefList& operator=(const SERefList& sl);

        bool contains(const SEReference & val);
        bool remove_val(const SEReference & val);
        bool remove_pos(const unsigned int pos);
        int find_pos(const SEReference & val);

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

#endif // __SKYPE_REF_LIST_H
