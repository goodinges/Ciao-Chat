
// #include "skype-object.h"
#include "skype-obj-dict.h"
#include <stdio.h>

#define SIZE 8123

class SEObject;

/** \cond INTERNAL */
class SEObjDict::Element
{
public:
        Element(const int k, const SEObject* v);

public:
        Element* next;
        int key;
        const SEObject* value;
};

class SEObjDict::Data
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

SEObjDict::Element::Element(const int k, const SEObject* v)
                :next(0),
                key(k),
                value(v)
{

}

SEObjDict::Data::Data()
                :dirty(true),
                quick_list(0)
{

}
/** \endcond */

SEObjDict::SEObjDict()
                :d(0)
{

}

SEObjDict::SEObjDict(const SEObjDict& dict)
                :d(0)
{
        *this = dict;
}

SEObjDict::~SEObjDict()
{
        d_unref();
}

void SEObjDict::insert(const int key, const SEObject* new_value)
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

        while (*e)
                e = &((*e)->next);
        
        *e = new Element(key, new_value);
}

struct CollisionStat {
        CollisionStat() : num_find(0), num_collision(0), num_found(0) {}
        ~CollisionStat() { printf("num find = %d\nnum collision = %d\nnum found = %d\n", num_find, num_collision, num_found); }
        int num_find;
        int num_collision;
        int num_found;
};

const SEObject*& SEObjDict::find(const int key, unsigned int offset) const
{
        static const SEObject* null = 0;

        if (0 == d)
                return null;

        unsigned int h = (unsigned int)key % SIZE;

        Element** e = &(d->dict[h]);

        while (*e) {
                if ((*e)->key == key) {
                        if (0 == offset) {
                                return (*e)->value;
                        } else
                                offset--;
                }
                e = &((*e)->next);
        }

        return null;
}

SEObjDict& SEObjDict::operator=(const SEObjDict& dict)
{
        const_cast<SEObjDict&>(dict).d_ref();

        d_unref();

        d = dict.d;

        return *this;
}

/** \cond INTERNAL */
void SEObjDict::d_ref()
{
        if (0 == d)
                return;
        
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
}

void SEObjDict::d_unref()
{
        if (0 == d)
                return;
        
        SEMutex* lock = &d->lock;
        
        lock->Acquire();         
        
        if (d->ref > 1) {
                d->ref--;
        } else {
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

void SEObjDict::detach()
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

size_t SEObjDict::size() const
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

const SEObject*& SEObjDict::operator[](size_t i) const
{
        static const SEObject* null = 0;

        if (0 == d)
                return null;

        if (i >= size())
                return null;;

        return d->quick_list[i]->value;
}

int SEObjDict::keyAt(size_t i) const
{
        if (0 == d)
                return -1;

        if (i >= size())
                return 0;

        return d->quick_list[i]->key;
}

bool SEObjDict::remove(const int key)
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

        while (*e) {
                if ((*e)->key == key) {
                        Element* tmp = *e;
                        *e = (*e)->next;
                        delete tmp;
                        
                        return true;
                }
                e = &((*e)->next);
        }

        return false;
}

SEIntList SEObjDict::keys() {
        SEIntList result;
        int d_size = size();
        if (d && d->quick_list) {
                for (int i = 0; i < d_size; i++) {
                        result.append(d->quick_list[i]->key);        
                }
        } 
        return result;                
}








