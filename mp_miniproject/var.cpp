#include <assert.h>

#include "var.h"

#define NOT_EXPORTED (~(size_t)0)

#define HASH_INIT_SIZE 64
VarTab::VarTab()
{
	hashtab = new HashSlot[HASH_INIT_SIZE]();
	hashsize = hashfree = HASH_INIT_SIZE;
	exported.push_back(NULL);
}

VarTab::~VarTab()
{
	delete[] hashtab;
}

VarTab::HashSlot *VarTab::lookup(const char *key) const
{
	size_t h = hashKey(key);
	HashSlot *p, *end;
	p = hashtab + (h % hashsize);
	end = hashtab + hashsize;

	while (p->kvPair != NULL && (p->hash != h ||
				!equalKeyKvpair(key, p->kvPair->c_str()))) {
		p++; // linear probing
		if (p == end) {
			p = hashtab; // wrap around
		}
	}
	p->hash = h;
	return p;
}

size_t VarTab::hashKey(const char *key)
{
	size_t h = 6549;
	unsigned char ch;
	while ((ch = *key) != '\0') {
		h = h * 1558 + (ch ^ 233);
		key++;
	}
	return h;
}

bool VarTab::equalKeyKvpair(const char *key, const char *kvPair)
{
	while (*key == *kvPair) {
		assert(*key != '\0' && *key != '='); // not allowed in a key
		key++;
		kvPair++;
	}
	return (*key == '\0' && *kvPair == '=');
}

void VarTab::setVar(const char *key, const char *value)
{
	HashSlot *slot = lookup(key);
	if (slot->kvPair == NULL) {
		newVar(slot, key, value);
	} else {
		changeVar(slot, value);
	}
}

const char *VarTab::getVar(const char *key) const
{
	HashSlot *slot = lookup(key);
	std::string *kvPair = slot->kvPair;
	if (kvPair == NULL) {
		return NULL;
	}
	return kvPair->c_str() + slot->vOffset;
}

void VarTab::newVar(VarTab::HashSlot *slot, const char *key, const char *value)
{
	variables.push_back(std::string());
	std::string *kvPair = &variables.back();

	kvPair->assign(key);
	kvPair->push_back('=');
	slot->vOffset = kvPair->size();
	kvPair->append(value);
	slot->kvPair = kvPair;
	slot->idxExported = NOT_EXPORTED;

	--hashfree;
	maybeRehash();
}

void VarTab::maybeRehash()
{
	if (hashfree < hashsize / 2) {
		size_t newhashsize = hashsize * 2;
		HashSlot *newhashtab = new HashSlot[newhashsize]();
		HashSlot *newhashtabend = newhashtab + newhashsize;
		for (size_t i = 0; i < hashsize; i++) {
			HashSlot *p = &hashtab[i];
			HashSlot *q = &newhashtab[p->hash % newhashsize];
			while (q->kvPair != NULL) {
				q++; // linear probing
				if (q == newhashtabend) {
					q = newhashtab; // wrap around
				}
			}
			*q = *p;
		}
		hashfree += (newhashsize - hashsize);
		hashsize = newhashsize;
		delete[] hashtab;
		hashtab = newhashtab;
	}
}

void VarTab::changeVar(VarTab::HashSlot *slot, const char *value)
{
	std::string *kvPair = slot->kvPair;
	kvPair->replace(kvPair->begin() + slot->vOffset, kvPair->end(), value);

	// pointer to the C string may be invalidated
	if (slot->idxExported != NOT_EXPORTED)
		exported[slot->idxExported] = kvPair->c_str();
}

void VarTab::exportVar(const char *value)
{
	HashSlot *slot = lookup(value);
	if (slot->idxExported != NOT_EXPORTED) { // already exported
		return;
	}
	exported.back() = slot->kvPair->c_str();
	exported.push_back(NULL);
}

const char *const *VarTab::getExported() const
{
	return &exported[0];
}
