
#include "skype-string-dict.h"
#include "skype-string.h"
#include "skype-string-list.h"

#include <stdio.h>

#define SIZE 17

/** \cond INTERNAL */
class SEStringDict::Element
{
public:
        Element(const SEString& k, const SEString& v);

public:
        Element* next;
        SEString key;
        SEString value;
};

class SEStringDict::Data
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

SEStringDict::Element::Element(const SEString& k, const SEString& v)
                :next(0),
                key(k),
                value(v)
{

}

SEStringDict::Data::Data()
                :dirty(true),
                quick_list(0)
{

}
/** \endcond */

SEStringDict::SEStringDict()
                :d(0)
{

}

SEStringDict::SEStringDict(const SEStringDict& dict)
                :d(0)
{
        *this = dict;
}

SEStringDict::~SEStringDict()
{
        d_unref();
}

void SEStringDict::insert(const SEString& key, const SEString& new_value)
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

SEString SEStringDict::find(const SEString& key, unsigned int offset) const
{
        if (0 == d)
                return SEString();

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

        return SEString();
}

/** \cond INTERNAL */
SEString SEStringDict::format(bool escape_args) const
{
        if (0 == d)
                return SEString();

        SEString ret;

        for (size_t n = 0; n < SIZE; n++) {
                Element** e = &(d->dict[n]);

                while (*e) {
                        // we save a bunch of malloc with this one
                        // For binary, escaping has already been done
                        ret += SEString::keyValue((*e)->key, (!escape_args || (*e)->value.isBinary()) ? (*e)->value : (*e)->value.escape());

                        e = &((*e)->next);
                }
        }

        return ret.substr(0, -1);
}
/** \endcond */

SEStringDict& SEStringDict::operator=(const SEStringDict& dict)
{
        const_cast<SEStringDict&>(dict).d_ref();
        d_unref();
        d = dict.d;

        return *this;
}

/** \cond INTERNAL */
void SEStringDict::d_ref()
{
        if (0 == d)
                return;
        
        d->lock.Acquire(); 
        
        d->ref++;
        
        d->lock.Release();
}

void SEStringDict::d_unref()
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

void SEStringDict::detach()
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

SEStringDict SEStringDict::parse(const SEString& str)
{
        // we assume very strict formatting here
        SEStringList sl = SEStringList::split(str, ' ');

        return parse(sl);
}

SEStringDict SEStringDict::parse(const SEStringList& str)
{
        // we assume very strict formatting here
        SEStringList sl = str;

        SEString cur;
        SEStringDict ret;

        while (!(cur = sl.peek()).isNull()) {
                int eq = cur.find('=');

                if (-1 == eq)
                        continue;

                SEString key = cur.substr(0, eq - 1);
                SEString value;

                if ((cur[eq + 1] == 'r') && (cur[eq + 2] == '"')) { // binary are identified with =r""
                        value = cur.substr(eq + 3, -1);
                        value.markAsBinary();
                } else
                        value = cur.substr(eq + 2, -1);

                // Empty string (done before unescaping, otherwise "" is ignored)
                if (('"' == value[0]) && ('"' == value[1]))
                        value = SEString();

                // For binary, don't unescape now, will do it later
                if (!value.isBinary())
                        value = value.unescape();

                ret.insert(key, value);
        }

        return ret;
}
/** \endcond */

size_t SEStringDict::size() const
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

SEString SEStringDict::operator[](size_t i) const
{
        if (0 == d)
                return SEString();

        if (i >= size())
                return SEString();

        // we have already called size() so we are supposed to be "clean"
        return d->quick_list[i]->value;
}

SEString SEStringDict::keyAt(size_t i) const
{
        if (0 == d)
                return SEString();

        if (i >= size())
                return SEString();

        // we have already called size() so we are supposed to be "clean"
        return d->quick_list[i]->key;
}
