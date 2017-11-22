#ifndef HASH_H
#define HASH_H

#include <algorithm>

/* A class template for an open address hash table.
 */
template <typename T>
class HashTab {
	struct Slot {
		size_t hash; // hash == 0 is reserved for free slot
		T value;
	};

	Slot *base, *end;
	size_t size, free;

	/* Linear probing.
	 */
	void probe(Slot *&p) const
	{
		p++;
		if (p == end) {
			p = base;
		}
	}

	/* Copy contents from another hash table.
	 *
	 * this == &that is not allowed.
	 * this->size < that.size - that.free will cause an infinite loop.
	 */
	void copyFrom(const HashTab& that)
	{
		for (Slot *p = that.base; p < that.end; p++) {
			if (!p->hash) {
				continue;
			}
			Slot *q = base + (p->hash % size);
			while (q->hash) {
				probe(q);
			}
			*q = *p;
			free--;
		}
	}

	/* Swap contents with another hash table.
	 *
	 * Used in rehashing and the assignment operator.
	 */
	void swap(HashTab& that)
	{
		std::swap(this->base, that.base);
		std::swap(this->end, that.end);
		std::swap(this->size, that.size);
		std::swap(this->free, that.free);
	}

	/* Check the load factor. If >= 0.75, run a rehash.
	 */
	void maybeRehash()
	{
		if (free <= size / 4) {
			HashTab newTab(size * 2);
			newTab.copyFrom(*this);
			swap(newTab);
		}
	}

	/* Look for the slot where value should be.
	 *
	 * It can either be a free slot (value does not exist)
	 * or an occupied slot (otherwise).
	 *
	 * Running this when free == 0 will cause an infinite loop.
	 */
	template <typename U>
	Slot *lookup(size_t hash, const U& value) const
	{
		Slot *p = base + (hash % size);

		if (hash == 0) {
			hash++; // hash == 0 reserved for free slot
		}
		while (p->hash && (p->hash != hash || !(p->value == value))) {
			probe(p);
		}
		return p;
	}
public:
	HashTab(size_t size = 1) : size(size), free(size)
	{
		base = new Slot[size]();
		end = base + size;
	}
	HashTab(const HashTab& that)
	{
		size = free = that.size;
		base = new Slot[size]();
		end = base + size;
		copyFrom(that);
	}
	HashTab& operator= (const HashTab& that)
	{
		if (this != &that) {
			HashTab temp(that);
			swap(temp);
		}
		return *this;
	}
	~HashTab() { delete[] base; }

	/* HashTab::find does a search in the hash table.
	 *
	 * If value is found, the pointer to the T object
	 * in the corresponding slot is returned.
	 * Otherwise NULL is returned.
	 */
	template <typename U>
	T* find(size_t hash, const U& value) const
	{
		Slot *p = lookup(hash, value);
		return (p->hash) ? &p->value : NULL;
	}

	/* HashTab::add tries to add value to the hash table.
	 *
	 * If value does not exist, it returns true,
	 * and *out is set to pointing at a new T object.
	 * In this case, the caller _must_ then change the T object
	 * so that it compares equal with value.
	 *
	 * If value already exists, it returns false.
	 * In this case, the caller can choose to modify
	 * the T object as long as it compares equal with value.
	 */
	template <typename U>
	bool add(size_t hash, const U& value, T **out)
	{
		maybeRehash();
		Slot *p = lookup(hash, value);
		*out = &p->value;
		if (!p->hash) {
			p->hash = hash;
			--free;
			return true;
		}
		return false;
	}
};

extern size_t hashStr(const char *, size_t);

#endif
