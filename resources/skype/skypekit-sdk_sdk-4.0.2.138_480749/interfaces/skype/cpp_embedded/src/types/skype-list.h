/*
 * skype-list.h
 *
 *  Created on: Oct 5, 2010
 *      Author: lauri
 */

#ifndef SKYPELIST_H_
#define SKYPELIST_H_

#include <stdio.h>
#include <skype-string.h>


/** \cond INTERNAL */

template<class ElementType>
class SEList {
public:
	/** Creates an empty list. */
	SEList() :
		d(0), empty(ElementType()) {
	}

	/** Constructs an implicitly shared copy of sl. */
	SEList(const SEList<ElementType>& sl) :
		d(0), empty(ElementType()) {
		*this = sl;
	}
	/** Destroys the list and frees the data if this is the last reference to the list. */
	~SEList() {
		d_unref();
	}

	/** \cond INTERNAL */
protected:
	class Element {
	public:
		Element() :
			next(0) {
		}
		;

	public:
		ElementType value;
		Element* next;
	};
	class Data {
	public:
		unsigned int ref;
		Element* list;
		Element* last;
		bool dirty;
		Element** quick_list;
		size_t size;
		SEMutex lock;
	};
	Data* d;
	ElementType empty;
	/** \endcond */

public:
	/** Appends a string at the end of the list. */
	void push_back(const ElementType& str) {
		append(str);
	}
	// specialize for string type
	inline bool append_empty_string_if_param_isnull(const ElementType& param) {
		return false;
	}
	/** Appends a element at the end of the list. */
	SEList<ElementType>& append(const ElementType& param) {
		if (append_empty_string_if_param_isnull(param))
			return *this;

		detach();
		if (0 == d) {
			init();

			d->list = new Element();
			d->list->value = param;
			d->last = d->list;
		} else {
			d->dirty = true;
			d->last->next = new Element();
			d->last->next->value = param;
			d->last = d->last->next;
		}

		return *this;
	}
	/** Removes the first element from the list and returns it. */
	ElementType peek() {
		detach();

		if (0 == d)
			return ElementType();

		d->dirty = true;

		ElementType ret = d->list->value;

		Element* next = d->list->next;

		delete d->list;

		d->list = next;

		if (0 == d->list) {
			delete d;
			d = 0;
		}

		return ret;
	}
	/** Returns the size of the list. */
	size_t size() const {
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
		} while ((e = e->next));

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
	/** Returns the string specified by its index. */
	/*	ElementType operator[](size_t n) {
	 if ((0 == d) || (n >= size()))
	 return ElementType();

	 return d->quick_list[n]->param;
	 }*/

	/** Returns a string reference specified by its index. */
	const ElementType &operator[](size_t n) const {
		if ((0 == d) || (n >= size()))
			return empty;

		// we have already called size() so we are supposed to be "clean"
		return d->quick_list[n]->value;
	}
	/** Assigns a shallow copy of sl to this list and returns a reference to it.
	 * This is very fast because the list isn't actually copied.
	 */
	SEList<ElementType>& operator=(const SEList<ElementType>& sl) {
		const_cast<SEList&> (sl).d_ref();
		d_unref();

		d = sl.d;

		return *this;

	}
	/** Creates a string out of the list of strings by joining them.
	 * \param sep String to use as a separator between each joined string.
	 * \param escape_args Should the parameters be escaped before joining them.
	 */
	ElementType join(const ElementType& sep, bool escape_args = true) const {
		ElementType ret;

		if (!size())
			return ret;

		ret += escape_args ? operator[](0).escape() : operator[](0);

		for (size_t n = 1; n < size(); n++)
			ret += sep + (escape_args ? operator[](n).escape() : operator[](n));

		return ret;
	}

	bool contains(const ElementType& val) {
		return (find_pos(val) >= 0);
	}

	bool remove_val(const ElementType& val) {
		int pos = find_pos(val);
		if (pos >= 0)
			return remove_pos(pos);

		return false;
	}

	bool remove_pos(const unsigned int pos) {
		if (0 == d || pos >= size())
			return false;

		detach();
		unsigned int cursize = size();//updates quick_list

		Element* elem_del = d->quick_list[pos];
		Element* elem_next = elem_del->next;
		Element* elem_prev = pos == 0 ? 0 : d->quick_list[pos - 1];

		if (pos == 0) {//first
			delete elem_del;
			d->list = elem_next;
		} else if (pos == cursize - 1) { //last
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

	int find_pos(const ElementType& val) {
		int len = size();
		for (int n = 0; n < len; n++) {
			if (d->quick_list[n]->value == val) {
				return n;
			}
		}

		return -1;
	}

	void resize(const unsigned int new_size) {
		if (size() == new_size)
			return;
		if (new_size == 0) {
			clear();
			return;
		}

		detach();
		unsigned int cursize = size(); //updates quick_list

		if (cursize > new_size) { //shrink

			Element* elem_last = d->quick_list[new_size - 1];
			elem_last->next = 0;
			d->last = elem_last;

			for (unsigned int n = new_size; n < cursize; n++) {
				delete d->quick_list[n];
			}

		} else { //grow

			while (cursize < new_size) {
				if (0 == d) {
					init();
					d->list = new Element();
					d->list->value = ElementType();
					d->last = d->list;
				} else {
					d->last->next = new Element();
					d->last->next->value = ElementType();
					d->last = d->last->next;
				}

				cursize++;
			}
		}

		d->dirty = true;
	}
	void clear() {
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
	/** \cond INTERNAL */
protected:
	void init() {
		if (d)
			return;

		d = new Data();
		d->ref = 1;
		d->list = 0;
		d->dirty = true;
		d->size = 0;
		d->quick_list = 0;
	}
	void d_ref() {
		if (0 == d)
			return;

		d->lock.Acquire();

		d->ref++;

		d->lock.Release();
	}
	void d_unref() {
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
	void detach() {
		if (0 == d)
			return;

		d->lock.Acquire();

		if (1 != d->ref) {

			Data* d_new = new Data();

			d_new->ref = 1;
			d_new->dirty = true;
			d_new->quick_list = 0;

			d_new->list = new Element;
			d_new->list->value = d->list->value;

			Element* from = d->list->next;
			Element* to = d_new->list;

			while (from) {
				to->next = new Element;
				to->next->value = from->value;

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
};

template<>
inline bool SEList< SEString >::append_empty_string_if_param_isnull(const SEString& elem) {
	if (elem.isNull()) {
		append("");
		return true;
	}
	return false;
}

#endif /* SKYPELIST_H_ */
