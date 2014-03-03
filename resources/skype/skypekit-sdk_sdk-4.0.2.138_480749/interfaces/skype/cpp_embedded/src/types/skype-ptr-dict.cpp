
#include "skype-ptr-dict.h"
#include <stdio.h>

#define SIZE 17

/** \cond INTERNAL */
class SEPtrDict::Element
{
public:
        Element(const SEString& k, const void* v);

public:
        Element* next;
        SEString key;
        const void* value;
};

class SEPtrDict::Data
{
public:
        Data();

public:
        Element* dict[SIZE];
        unsigned int ref;
        size_t size;
        bool dirty;
        Element** quick_list;
        SEMutex lock;
};

SEPtrDict::Element::Element(const SEString& k, const void* v)
                :next(0),
                key(k),
                value(v)
{

}

SEPtrDict::Data::Data()
                :dirty(true),
                quick_list(0)
{

}
/** \endcond */

SEPtrDict::SEPtrDict()
                :d(0)
{

}

SEPtrDict::SEPtrDict(const SEPtrDict& dict)
                :d(0)
{
        *this = dict;
}

SEPtrDict::~SEPtrDict()
{
        d_unref();
}

void SEPtrDict::insert(const SEString& key, const void* new_value)
{
        if (key.isNull())
                return;

        detach();

        if (0 == d) {
                d = new Data();

                d->ref = 1;

                for (size_t n = 0; n < SIZE; n++)
                        d->dict[n] = 0;
        }

        d->dirty = true;

        unsigned int h = key.hash(SIZE);

        Element** e = &(d->dict[h]);

        while (*e)
                e = &((*e)->next);

        *e = new Element(key, new_value);
}

const void*& SEPtrDict::find(const SEString& key, unsigned int offset) const
{
        static const void* null = 0;

        if (0 == d)
                return null;

        unsigned int h = key.hash(SIZE);

        Element** e = &(d->dict[h]);

        while (*e) {
                if ((*e)->key == key) {
                        if (0 == offset)
                                return (*e)->value;
                        else
                                offset--;
                }

                e = &((*e)->next);
        }

        return null;
}

SEPtrDict& SEPtrDict::operator=(const SEPtrDict& dict)
{
        const_cast<SEPtrDict&>(dict).d_ref();

        d_unref();

        d = dict.d;

        return *this;
}

/** \cond INTERNAL */
void SEPtrDict::d_ref()
{
        if (0 == d)
                return;
        
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
}

void SEPtrDict::d_unref()
{
        if (0 == d)
                return;

        SEMutex* lock = &d->lock;
        
        lock->Acquire(); 

        if (d->ref > 1)
                d->ref--;
        else {
                Data* tmp_d = d;                
                d = 0;
                
                lock->Release();

                for (size_t n = 0; n < SIZE; n++) {
                        while (tmp_d->dict[n]) {
                                Element* tmp = tmp_d->dict[n]->next;

                                delete tmp_d->dict[n];

                                tmp_d->dict[n] = tmp;
                        }
                }

                if (tmp_d->quick_list)
                        delete[] tmp_d->quick_list;

                delete tmp_d;
                
                return; 
        }
        
        lock->Release();
}

void SEPtrDict::detach()
{
        if (0 == d)
                return;

        if (1 == d->ref)
                return;

        Data* d_new = new Data();

        d_new->ref = 1;

        size_t n;
        for (n = 0; n < SIZE; n++)
                d_new->dict[n] = 0;

        for (n = 0; n < SIZE; n++) {
                Element** from = &(d->dict[n]);
                Element** to = &(d_new->dict[n]);

                while (*from) {
                        *to = new Element((*from)->key, (*from)->value);

                        from = &((*from)->next);
                        to = &((*to)->next);
                }
        }

        d_unref();

        d = d_new;
}

/** \endcond */

size_t SEPtrDict::size() const
{
        if (0 == d)
                return 0;

        if (d->dirty) {
                if (d->quick_list)
                        delete[] d->quick_list;

                d->size = 0;

                size_t n;
                for (n = 0; n < SIZE; n++) {
                        Element** e = &(d->dict[n]);

                        while (*e) {
                                d->size++;

                                e = &((*e)->next);
                        }
                }

                d->quick_list = new Element*[d->size];

                size_t c = 0;

                for (n = 0; n < SIZE; n++) {
                        Element** e = &(d->dict[n]);

                        while (*e) {
                                d->quick_list[c++] = *e;

                                e = &((*e)->next);
                        }
                }

                d->dirty = false;
        }

        return d->size;
}

const void*& SEPtrDict::operator[](size_t i) const
{
        static const void* null = 0;

        if (0 == d)
                return null;

        if (i >= size())
                return null;;

        return d->quick_list[i]->value;
}

SEString SEPtrDict::keyAt(size_t i) const
{
        if (0 == d)
                return SEString();

        if (i >= size())
                return SEString();

        return d->quick_list[i]->key;
}

SEStringList SEPtrDict::keys() {
        SEStringList result;
        int d_size = size();
        if (d && d->quick_list) {
                for (int i = 0; i < d_size; i++) {
                        result.append(d->quick_list[i]->key);        
                }
        } 
        return result;                
}
