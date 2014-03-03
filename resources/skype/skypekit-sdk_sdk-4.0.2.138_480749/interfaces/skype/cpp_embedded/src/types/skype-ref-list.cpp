
#include "skype-ref-list.h"
#include "skype-object.h"

SEReference empty;

/** \cond INTERNAL */
class SERefList::Element
{
public:
        Element();

public:
        SEReference reference;
        Element* next;
};

class SERefList::Data
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

SERefList::Element::Element()
                :next(0)
{

}
/** \endcond */

SERefList::SERefList()
                :d(0)
{
}

SERefList::SERefList(const SERefList& sl)
                :d(0)
{
        *this = sl;
}

SERefList::~SERefList()
{
        d_unref();
}

/** \cond INTERNAL */
void SERefList::init()
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

SERefList& SERefList::append(const SEReference& ref)
{
        detach();

        if (0 == d) {
                init();

                d->list = new Element();
                d->list->reference = ref;
                d->last = d->list;
        } else {
                d->dirty = true;
                d->last->next = new Element();
                d->last->next->reference = ref;
                d->last = d->last->next;
        }

        return *this;
}

SEReference& SERefList::peek()
{
        detach();

        if (0 == d)
                return empty;

        d->dirty = true;

        SEReference& ret = d->list->reference;

        Element* next = d->list->next;

        delete d->list;

        d->list = next;

        if (0 == d->list) {
                delete d;
                d = 0;
        }

        return ret;
}

size_t SERefList::size() const
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

SEReference &SERefList::operator[](size_t n) const
{
        if ((0 == d) || (n >= size()))
                return const_cast<SEReference &>(empty);

        return d->quick_list[n]->reference;
}

SERefList& SERefList::operator=(const SERefList& sl)
{
        d_unref();
        
        d = sl.d;

        d_ref();

        return *this;
}

/** \cond INTERNAL */
void SERefList::d_ref()
{
        if (0 == d)
                return;
        
        d->lock.Acquire();
        
        d->ref++;
        
        d->lock.Release();
}

void SERefList::d_unref()
{
        if (0 == d)
                return;

        SEMutex* lock = &d->lock;
        
        lock->Acquire(); 

        if (d->ref > 1)
                d->ref--;
        else {
                Element* tmp = d->list;

                while (tmp) {
                        Element* next = tmp->next;

                        delete tmp;

                        tmp = next;
                }

                if (d->quick_list)
                        delete[] d->quick_list;

		Data* keep = d;
                d = 0;
                lock->Release();
                delete keep;
                
                
                
                return;
        }
        
        lock->Release();
}

void SERefList::detach()
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
                d_new->list->reference = d->list->reference;
        
                Element* from = d->list->next;
                Element* to = d_new->list;
        
                while (from) {
                        to->next = new Element;
                        to->next->reference = from->reference;
        
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

void SERefList::resize(const unsigned int new_size)
{
        if (size() == new_size) return;   
        if (new_size == 0) { 
                clear();
                return;     
        }       
        
        detach();      
        uint cursize = size(); //updates quick_list
        
        if (cursize > new_size) { //shrink
                
                Element* elem_last = d->quick_list[new_size-1]; 
                elem_last->next = 0;
                d->last = elem_last;                
                
                for (uint n = new_size; n < cursize; n++) {
                        delete d->quick_list[n];
                }                
                                                
        } else {   //grow
        
                while (cursize < new_size) {
                        if (0 == d) {
                                init();
                                d->list = new Element();
                                d->list->reference = SEReference();
                                d->last = d->list;
                        } else {
                                d->last->next = new Element();
                                d->last->next->reference = SEReference();
                                d->last = d->last->next;
                        }
                        
                        cursize++;
                }
        }
        
        d->dirty = true;
}

int SERefList::find_pos(const SEReference & val)
{
        int len = size();
        for (int n = 0; n < len; n++) {
                if (d->quick_list[n]->reference == val) {
                        return n;
                }
        }
        
        return -1;
}

bool SERefList::contains(const SEReference & val)
{
        return (find_pos(val) >= 0);
}

bool SERefList::remove_val(const SEReference & val)
{
        int pos = find_pos(val);
        if (pos >= 0) 
                return remove_pos(pos);

        return false;
}

bool SERefList::remove_pos(const unsigned int pos)
{          
        if (0 == d || pos >= size()) return false; 
                                
        detach();         
        uint cursize = size();//updates quick_list         
        
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

void SERefList::clear()
{
        if (0 == d) {
                return;
        }
        
        detach();

        for (uint n = 0; n < size(); n++) {
                delete d->quick_list[n]; 
        }
               
        if (d->quick_list)
                delete[] d->quick_list;

        delete d;
        
        d = 0;        
}
