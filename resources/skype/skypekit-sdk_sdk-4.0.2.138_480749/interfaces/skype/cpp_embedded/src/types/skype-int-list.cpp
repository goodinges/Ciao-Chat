
#include "skype-int-list.h"

/** \cond INTERNAL */
class SEIntList::Element
{
public:
        Element();

public:
        int i;
        Element* next;
};

class SEIntList::Data
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

SEIntList::Element::Element()
                :next(0)
{

}
/** \endcond */

SEIntList::SEIntList()
                :d(0)
{

}

SEIntList::SEIntList(const SEIntList& sl)
                :d(0)
{
        *this = sl;
}

SEIntList::~SEIntList()
{
        d_unref();
}

/** \cond INTERNAL */
void SEIntList::init()
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

SEIntList& SEIntList::append(const int i)
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
SEIntList& SEIntList::prepend(const int i)
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

int SEIntList::peek()
{
        detach();

        if (0 == d)
                return 0;

        d->dirty = true;

        int ret = d->list->i;

        Element* next = d->list->next;

        delete d->list;

        d->list = next;

        if (0 == d->list) {
                delete d;
                d = 0;
        }

        return ret;
}

size_t SEIntList::size() const
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

int & SEIntList::operator [](unsigned int index)
{
        static int null = 0;
        Element* cur = d->list;
        unsigned int i = 0;
        while ((cur)) {
                if (i == index) return cur->i;
                i++;
                cur = cur->next;
        }

        return null;
}

SEIntList& SEIntList::operator=(const SEIntList& sl)
{
        const_cast<SEIntList&>(sl).d_ref();

        d_unref();

        d = sl.d;

        return *this;
}

/** \cond INTERNAL */
void SEIntList::d_ref()
{
        if (0 == d)
                return;
                
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
}

void SEIntList::d_unref()
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

void SEIntList::detach()
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

SEIntList SEIntList::split(const SEString& str, char sep, char esc)
{
        SEIntList ret;
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
                        ret.append(str.substr(from, cur - 1).toInt());

                        break;
                }

                if (str[cur] == sep) {
                        // here we may go out of bounds (handled above)
                        if (esc && (str[cur - 1] == esc)) {
                                cur++;

                                continue;
                        }

                        ret.append(str.substr(from, cur - 1).toInt());

                        while ((str[cur] == sep) && (cur < len)) cur++;

                        if (cur >= (len - 1))
                                break;

                        from = cur++;

                        continue;
                }

                if (cur == (len - 1)) {
                        ret.append(str.substr(from, cur).toInt());

                        break;
                }

                cur++;
        }

        return ret;
}

int SEIntList::find_pos(const int val)
{
        int len = size();
        for (int n = 0; n < len; n++) {
                if (d->quick_list[n]->i == val) {
                        return n;
                }
        }
        
        return -1;
}

bool SEIntList::contains(const int val)
{
        return (find_pos(val) >= 0);
}

bool SEIntList::remove_val(const int val)
{
        int pos = find_pos(val);
        if (pos >= 0) return remove_pos(pos);

        return false;
}

bool SEIntList::remove_pos(const unsigned int pos)
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

void SEIntList::resize(const unsigned int new_size)
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

void SEIntList::clear()
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












