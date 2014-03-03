/*
 * skype-dict.h
 *
 *  Created on: Oct 5, 2010
 *      Author: lauri
 */

#ifndef SKYPEDICT_H_
#define SKYPEDICT_H_

#include "skype-string.h"
#include "skype-list.h"

// specialize for string type
template <class KeyType>
inline bool param_is_empty_string(const KeyType& key) {
	return false;
}

// specialize this for string type
template <class KeyType>
inline int key_to_index(const KeyType & key, int size) {
	return key % size;
}


/** \class SEDict skype-dict.h "skype-dict.h"
 *  \brief SEDict is a dictionary class of KeyType associated to ValueType.
 */
template<class KeyType, class ValueType>
class SEDict {
public:
	/** Creates an empty dictionary of KeyType/ValueType. */
	SEDict(int size) :
		d(0), m_size(size) {
	}
	/** Constructs an implicitly shared copy of dict. */
	SEDict(const SEDict& dict) :
		d(0) {
		*this = dict;
	}
	/** Destroys the dictionary and frees the data if this is the last reference to it. */
	~SEDict() {
		d_unref();
	}

	/** \cond INTERNAL */
protected:
	class Element {
	public:
		Element(const KeyType& k, const ValueType& v) :
			next(0), key(k), value(v) {
		}

	public:
		Element* next;
		KeyType key;
		ValueType value;
	};

	class Data {
	public:
		Data(int size) :
			dirty(true), quick_list(0) {
			dict = new Element*[size];
		}
		~Data() {
			delete[] dict;
		}

	public:
		Element** dict;
		unsigned int ref;
		size_t size;
		bool dirty;
		Element** quick_list;
		SEMutex lock;
	};
	Data* d;
	size_t m_size;
	/** \endcond */

public:
	/** Inserts the key with the value into the dictionary.
	 * Multiple items can have the same key, they are not overwritten.
	 * You can access them with the offset parameter of the find() function.
	 * If key is null, nothing is inserted.
	 */
	void insert(const KeyType& key, const ValueType& value) {

		if (param_is_empty_string<KeyType>(key))
			return;

		detach();

		if (0 == d) {
			d = new Data(m_size);

			d->ref = 1;

			for (size_t n = 0; n < m_size; n++)
				d->dict[n] = 0;
		}

		d->dirty = true;

		unsigned int h = key_to_index<KeyType>(key, m_size);

		Element** e = &(d->dict[h]);

		while (*e)
			e = &((*e)->next);

		*e = new Element(key, value);
	}

	/** Finds the specified key in the dictionary.
	 * \param key		Key to search.
	 *					Default is to search for 0, which is the most often used key name
	 *					in the Embedded API.
	 * \param offset 	When the dictionary contains several entries with the same key,
	 *					use the offset param to indicate which key you want.
	 * \return null string if the key is not found.
	 */
	ValueType find(const KeyType& key, unsigned int offset = 0) const {
		if (0 == d)
			return ValueType();

		unsigned int h = key_to_index<KeyType>(key, m_size);

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

		return ValueType();
	}
	/** Assigns a shallow copy of dict to this dictionary and returns a reference to it.
	 * This is very fast because the dictionary isn't actually copied.
	 */
	SEDict<KeyType, ValueType>& operator=(
			const SEDict<KeyType, ValueType>& dict) {
		const_cast<SEDict<KeyType, ValueType> &> (dict).d_ref();
		d_unref();
		d = dict.d;

		return *this;
	}

	/** Returns how many elements are in the dictionary. */
	size_t size() const {
		if (0 == d)
			return 0;

		if (d->dirty) {
			if (d->quick_list)
				delete[] d->quick_list;

			d->size = 0;

			size_t n;
			for (n = 0; n < m_size; n++) {
				Element** e = &(d->dict[n]);

				while (*e) {
					d->size++;

					e = &((*e)->next);
				}
			}

			d->quick_list = new Element*[d->size];

			size_t c = 0;

			for (n = 0; n < m_size; n++) {
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

	/** Use this function to iterate through the dictionary.
	 * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
	 * See also keyAt()
	 */
	ValueType operator[](size_t i) const {
		if (0 == d)
			return KeyType();

		if (i >= size())
			return KeyType();

		// we have already called size() so we are supposed to be "clean"
		return d->quick_list[i]->value;
	}
	/** Use this function to iterate through the dictionary.
	 * It doesn't make sense to use this functions outside of an iteration (i.e. a loop).
	 * See also operator[]()
	 */
	KeyType keyAt(size_t i) const {
		if (0 == d)
			return KeyType();

		if (i >= size())
			return KeyType();

		// we have already called size() so we are supposed to be "clean"
		return d->quick_list[i]->key;
	}

	ValueType value(const KeyType& key) {
		return find(key);
	}

	bool remove(const KeyType &key) {
		detach();

		if (0 == d) {
			d = new Data(m_size);

			d->ref = 1;

			for (size_t n = 0; n < m_size; n++)
				d->dict[n] = 0;
		}

		d->dirty = true;

		unsigned int h = key_to_index<KeyType>(key, m_size);

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

	SEList<KeyType> keys() {
		SEList<KeyType> result;
		int d_size = size();
		if (d && d->quick_list) {
			for (int i = 0; i < d_size; i++) {
				result.append(d->quick_list[i]->key);
			}
		}
		return result;
	}
protected:
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

			for (size_t n = 0; n < m_size; n++) {
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

	void detach() {
		if (0 == d)
			return;

		d->lock.Acquire();

		if (1 != d->ref) {

			Data* d_new = new Data(m_size);

			d_new->ref = 1;

			size_t n;
			for (n = 0; n < m_size; n++)
				d_new->dict[n] = 0;

			for (n = 0; n < m_size; n++) {
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
};

template<>
inline bool param_is_empty_string<SEString>(const SEString& key) {
	return key.isNull();
}

template <>
inline int key_to_index<SEString>(const SEString & key, int size) {
	return key.hash(size);
}

#endif /* SKYPEDICT_H_ */
