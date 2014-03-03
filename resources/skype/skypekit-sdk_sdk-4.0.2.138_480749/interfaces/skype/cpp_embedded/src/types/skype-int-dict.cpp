
#include "skype-int-dict.h"
#include "skype-string.h"

#define SIZE 17

/** \cond INTERNAL */
class SEIntDict::Element
{
public:
        Element(const int k, const SEString& v);

public:
        Element* next;
        int key;
        SEString value;
};

class SEIntDict::Data
{
public:
        Data();

public:
        Element* dict[SIZE];
        unsigned int ref;
        size_t size;
        Element** quick_list;
        bool dirty;
        SEMutex lock;
};

SEIntDict::Data::Data()
                :quick_list(false),
                dirty(true)
{

}

SEIntDict::Element::Element(const int k, const SEString& v)
                :next(0),
                key(k),
                value(v)
{

}
/** \endcond */

SEIntDict::SEIntDict()
                :d(0)
{
}

SEIntDict::SEIntDict(const SEIntDict& dict)
                :d(0)
{
        *this = dict;
}

SEIntDict::~SEIntDict()
{
        d_unref();
}

void SEIntDict::insert(const int key, const SEString& new_value, bool replace_only)
{
        detach();

        if (0 == d) {
                d = new Data();

                d->ref = 1;

                for (size_t n = 0; n < SIZE; n++)
                        d->dict[n] = 0;
        }

        d->dirty = true;

        unsigned int h = (unsigned int)key % SIZE;

        Element** e = &(d->dict[h]);

        while (*e && ((*e)->key != key)) {
                e = &((*e)->next);
        }

        if (0 == *e) {
                if (replace_only)
                        return;
                else
                        *e = new Element(key, new_value);
        } else
                (*e)->value = new_value;
}

SEString SEIntDict::find(const int key) const
{
        if (0 == d)
                return SEString();

        unsigned int h = (unsigned int)key % SIZE;

        Element** e = &(d->dict[h]);

        while (*e) {
                if ((*e)->key == key)
                        return (*e)->value;

                e = &((*e)->next);
        }

        return SEString();
}

/** \cond INTERNAL */
// TODO can save many mallocs if SEString has sprintf()
SEString SEIntDict::format() const
{
        if (0 == d)
                return SEString();

        SEString ret;

        for (size_t n = 0; n < SIZE; n++) {
                Element** e = &(d->dict[n]);

                while (*e) {
                        ret+= SEString::from((*e)->key) + "=\"" + (*e)->value + "\" ";

                        e = &((*e)->next);
                }
        }

        return ret.substr(0, -1);
}
/** \endcond */

SEIntDict& SEIntDict::operator=(const SEIntDict& dict)
{
        const_cast<SEIntDict&>(dict).d_ref();

        d_unref();

        d = dict.d;

        return *this;
}

/** \cond INTERNAL */
void SEIntDict::d_ref()
{
        if (0 == d)
                return;
        
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
}

void SEIntDict::d_unref()
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

void SEIntDict::detach()
{
        if (0 == d)
                return;
        
        d->lock.Acquire();
        
        if (1 != d->ref) {

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
                
                d->lock.Release();
                
                d_unref();
        
                d = d_new;
                
                return; 
        }
        
        d->lock.Release();
}
/** \endcond */

size_t SEIntDict::size() const
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

SEString SEIntDict::operator[](size_t i) const
{
        if (0 == d)
                return SEString();

        if (i >= size())
                return SEString();

        // we have already called size() so we are supposed to be "clean"
        return d->quick_list[i]->value;
}

int SEIntDict::keyAt(size_t i) const
{
        if (0 == d)
                return -1;

        if (i >= size())
                return 0;

        // we have already called size() so we are supposed to be "clean"
        return d->quick_list[i]->key;
}

SEString SEIntDict::value(const int key) 
{ 
        return find(key); 
}
