
#ifndef __SKYPE_PTR_VECTOR_H
#define __SKYPE_PTR_VECTOR_H

#include <stdlib.h>
#include "skype-int-list.h"

/*! \class SEPtrVector skype-ptr-vector.h "skype-ptr-vector.h"
 *  \brief Vector is an array that is reference counted.
 *
 * Its size is set at creation, it doesn't grow automatically.
 *
 * SEPtrVector makes no use of the C++ templates, therefore you have to
 * explicitly cast its elements to appropriate pointer type (SEPtrVector uses void*).
 */
class SEPtrVector
{
public:
        /** Creates an empty vector object. */
        SEPtrVector();
        /** Creates a vector object of specified size. */
        SEPtrVector(size_t s);
        SEPtrVector(const SEPtrVector& pv);
        ~SEPtrVector();

        /** \cond INTERNAL */
protected:
        class Data;
        Data* d;
        /** \endcond */

public:
        /** Resizes and resets the vector */
        void setnewsize(size_t s);
        /** Returns how many elements are in the vector. */
        size_t size() const;
        /** Returns the item at position i, or NULL if there is no item at that position. */
        void*& operator[](size_t n) const;
        /** Assigns a shallow copy of pv to this vector and returns a reference to it.
         * This is very fast because the vector isn't actually copied. */
        SEPtrVector& operator=(const SEPtrVector& pv);

        /** \cond INTERNAL */
protected:
        void d_ref();
        void d_unref();
        /** \endcond */
};

#endif // __SKYPE_PTR_VECTOR_H
