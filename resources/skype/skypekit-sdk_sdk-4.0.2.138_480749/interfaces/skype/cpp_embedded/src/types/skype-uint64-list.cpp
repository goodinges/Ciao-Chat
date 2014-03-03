
#include "skype-uint64-list.h"

/** \cond INTERNAL */
class SEUInt64List::Element
{
public:
        Element();

public:
        SEUInt64List::uint64 i;
        Element* next;
};

class SEUInt64List::Data
{
public:
        unsigned int ref;
        Element* list;
        Element* last;
        bool dirty;
        Element** quick_list;
        size_t size;
        SEMutex lock;
};

SEUInt64List::Element::Element()
                :next(0)
{

}
/** \endcond */

SEUInt64List::SEUInt64List()
                :d(0)
{

}

SEUInt64List::SEUInt64List(const SEUInt64List& sl)
                :d(0)
{
        *this = sl;
}

SEUInt64List::~SEUInt64List()
{
        d_unref();
}

/** \cond INTERNAL */
void SEUInt64List::init()
{
        if (d)
                return;

        d = new Data();
        d->ref = 1;
        d->list = 0;
        d->dirty = true;
        d->size = 0;
        d->quick_list = 0;
}
/** \endcond */

SEUInt64List& SEUInt64List::append(const SEUInt64List::uint64 i)
{
        detach();

        if (0 == d) {
                init();

                d->list = new Element();
                d->list->i = i;
                d->last = d->list;
        } else {
                d->dirty = true;
                d->last->next = new Element();
                d->last->next->i = i;
                d->last = d->last->next;
        }
//         printf("added to intlist %d\n",i);
        return *this;
}

#if 0
SEUInt64List& SEUInt64List::prepend(const SEUInt64List::uint64 i)
{
        detach();

        if (0 == d) {
                init();

                d->list = new Element();
                d->list->i = i;
                d->last = d->list;
        } else {
                d->dirty = true;

                Element* n = new Element();

                n->i = i;
                n->next = d->list;
                d->list = n;
        }

        return *this;
}
#endif

SEUInt64List::uint64 SEUInt64List::peek()
{
        detach();

        if (0 == d)
                return 0;

        d->dirty = true;

        uint64 ret = d->list->i;

        Element* next = d->list->next;

        delete d->list;

        d->list = next;

        if (0 == d->list) {
                delete d;
                d = 0;
        }

        return ret;
}

size_t SEUInt64List::size() const
{
        if (0 == d)
                return 0;

        if (!d->dirty)
                return d->size;

        if (d->quick_list)
                delete[] d->quick_list;

        // first pass - count elements
        d->size = 0;

        Element* e = d->list;

        do {
                d->size++;
                e = e->next;
        } while (e);

        // second pass - fill quick access vector
        d->quick_list = new Element*[d->size];

        e = d->list;
        size_t n = 0;

        while (e) {
                d->quick_list[n++] = e;

                e = e->next;
        }

        d->dirty = false;

        return d->size;
}

SEUInt64List::uint64 & SEUInt64List::operator [](unsigned int index)
{
        static uint64 null = 0;
        Element* cur = d->list;
        unsigned int i = 0;
        while ((cur)) {
                if (i == index) return cur->i;
                i++;
                cur = cur->next;
        }

        return null;
}

SEUInt64List& SEUInt64List::operator=(const SEUInt64List& sl)
{
        const_cast<SEUInt64List&>(sl).d_ref();

        d_unref();

        d = sl.d;

        return *this;
}

/** \cond INTERNAL */
void SEUInt64List::d_ref()
{
        if (0 == d)
                return;
                
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
}

void SEUInt64List::d_unref()
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

                Element* tmp = tmp_d->list;

                while (tmp) {
                        Element* next = tmp->next;

                        delete tmp;

                        tmp = next;
                }

                if (tmp_d->quick_list)
                        delete[] tmp_d->quick_list;

                delete tmp_d;
                
                return; 
        }
        
        lock->Release();
}

void SEUInt64List::detach()
{
        if (0 == d)
                return;
        
        d->lock.Acquire();
        
        if (1 != d->ref) {

                Data* d_new = new Data();
        
                d_new->ref = 1;
                d_new->dirty = true;
                d_new->quick_list = 0;
        
                d_new->list = new Element;
                d_new->list->i = d->list->i;
        
                Element* from = d->list->next;
                Element* to = d_new->list;
        
                while (from) {
                        to->next = new Element;
                        to->next->i = from->i;
        
                        to = to->next;
                        from = from->next;
                }
        
                d_new->last = to;
                
                d->lock.Release();
                
                d_unref();
        
                d = d_new;
                        
                return;        
        }
        
        d->lock.Release();
}
/** \endcond */

SEUInt64List SEUInt64List::split(const SEString& str, char sep, char esc)
{
        SEUInt64List ret;
        size_t from = 0;
        size_t cur = 0;
        size_t len = str.length();

        // skip trailing separators
        while (str[cur] == sep) {
                from++;
                cur++;
        }

        cur++;

        while (cur <= len) {
                // the case when we go out of bounds is when we have escaped separator
                // before end of the string
                if (cur == len) {
                        ret.append(str.substr(from, cur - 1).toUInt64());

                        break;
                }

                if (str[cur] == sep) {
                        // here we may go out of bounds (handled above)
                        if (esc && (str[cur - 1] == esc)) {
                                cur++;

                                continue;
                        }

                        ret.append(str.substr(from, cur - 1).toUInt64());

                        while ((str[cur] == sep) && (cur < len)) cur++;

                        if (cur >= (len - 1))
                                break;

                        from = cur++;

                        continue;
                }

                if (cur == (len - 1)) {
                        ret.append(str.substr(from, cur).toUInt64());

                        break;
                }

                cur++;
        }

        return ret;
}

int SEUInt64List::find_pos(const SEUInt64List::uint64 val)
{
        int len = size();
        for (int n = 0; n < len; n++) {
                if (d->quick_list[n]->i == val) {
                        return n;
                }
        }
        
        return -1;
}

bool SEUInt64List::contains(const SEUInt64List::uint64 val)
{
        return (find_pos(val) >= 0);
}

bool SEUInt64List::remove_val(const SEUInt64List::uint64 val)
{
        int pos = find_pos(val);
        if (pos >= 0) return remove_pos(pos);

        return false;
}

bool SEUInt64List::remove_pos(const unsigned int pos)
{          
        if (0 == d || pos >= size()) return false; 
                                
        detach();         
        unsigned int cursize = size();//updates quick_list         
        
        Element* elem_del = d->quick_list[pos];         
        Element* elem_next = elem_del->next; 
        Element* elem_prev = pos == 0 ? 0 : d->quick_list[pos-1]; 
        
        if (pos == 0) {//first
                delete elem_del;
                d->list = elem_next;               
        } else if (pos == cursize -1) { //last
                delete elem_del;
                elem_prev->next = 0;
                d->last = elem_prev;                
        } else {
                delete elem_del;
                elem_prev->next = elem_next;        
        }         
                 
        d->dirty = true; 
        if (0 == d->list) {
                delete d;
                d = 0;
        }                 

        return true;
}

void SEUInt64List::resize(const unsigned int new_size)
{
        if (size() == new_size) return;   
        if (new_size == 0) { 
                clear();
                return;     
        }       
        
        detach();      
        unsigned int cursize = size(); //updates quick_list
        
        if (cursize > new_size) { //shrink
                
                Element* elem_last = d->quick_list[new_size-1]; 
                elem_last->next = 0;
                d->last = elem_last;                
                
                for (unsigned int n = new_size; n < cursize; n++) {
                        delete d->quick_list[n];
                }                
                                                
        } else {   //grow
        
                while (cursize < new_size) {
                        if (0 == d) {
                                init();
                                d->list = new Element();
                                d->list->i = 0;
                                d->last = d->list;
                        } else {
                                d->last->next = new Element();
                                d->last->next->i = 0;
                                d->last = d->last->next;
                        }
                        
                        cursize++;
                }
        }
        
        d->dirty = true;
}

void SEUInt64List::clear()
{
        if (0 == d) {
                return;
        }
        
        detach();

        for (unsigned int n = 0; n < size(); n++) {
                delete d->quick_list[n]; 
        }
               
        if (d->quick_list)
                delete[] d->quick_list;

        delete d;
        
        d = 0;        
}












