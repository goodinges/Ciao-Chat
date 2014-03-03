
#include "skype-ptr-vector.h"

/** \cond INTERNAL */
class SEPtrVector::Data
{
public:
        unsigned int ref;
        void** list;
        size_t size;
};
/** \endcond */

SEPtrVector::SEPtrVector()
                :d(0)
{

}

SEPtrVector::SEPtrVector(size_t s)
{
        if (0 == s) {
                d = 0;
                return;
        }

        d = new Data();

        d->size = s;
        d->list = new void*[s];
        d->ref = 1;
}

SEPtrVector::SEPtrVector(const SEPtrVector& pv)
                :d(0)
{
        *this = pv;
}

SEPtrVector::~SEPtrVector()
{
        d_unref();
}

void SEPtrVector::setnewsize(size_t s)
{
// 	d_unref();

        void** tmp = new void*[s];
        unsigned int i = 0;
        while (i < size() && i < s) {
                tmp[i] = d->list[i];
                i++;
        }

        if (d) {
                delete[] d->list;
                delete d;
        }

        d = new Data();
        d->size = s;
        d->list = tmp;

// 	d_ref();
}

size_t SEPtrVector::size() const
{
        if (0 == d)
                return 0;

        return d->size;
}

void*& SEPtrVector::operator[](size_t n) const
{
        static void* null = 0;

        if ((0 == d) || (n >= size()))
                return null;

        return d->list[n];
}

SEPtrVector& SEPtrVector::operator=(const SEPtrVector& pv)
{
        const_cast<SEPtrVector&>(pv).d_ref();
        d_unref();

        d = pv.d;


        return *this;
}

/** \cond INTERNAL */
void SEPtrVector::d_ref()
{
        if (0 == d)
                return;

        d->ref++;
}

void SEPtrVector::d_unref()
{
        if (0 == d)
                return;

        if (d->ref > 1)
                d->ref--;
        else {
                delete[] d->list;

                delete d;

                d = 0;
        }
}
/** \endcond */
