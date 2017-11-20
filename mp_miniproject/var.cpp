#include <assert.h>

#include "var.h"

#define NOT_EXPORTED (~(size_t)0)

#define HASH_INIT_SIZE 64
/* Constructor
 */
VarTab::VarTab()
{
	hashTab = new HashSlot[HASH_INIT_SIZE]();
	hashSize = hashFree = HASH_INIT_SIZE;
	exported.push_back(NULL);
}

/* Destructor
 */
VarTab::~VarTab()
{
	delete[] hashTab;
}

/* VarTab::lookup looks for a key in the hash table.
 * 
 * The hash table is open-addressing with linear probing.
 */
VarTab::HashSlot *VarTab::lookup(const char *key) const
{
	size_t h = hashKey(key);
	HashSlot *p, *end;
	p = hashTab + (h % hashSize);
	end = hashTab + hashSize;

	while (p->kvPair != NULL && (p->hash != h ||
				!equalKeyKvpair(key, p->kvPair->c_str()))) {
		p++; // linear probing
		if (p == end) {
			p = hashTab; // wrap around
		}
	}
	p->hash = h;
	return p;
}

/* VarTab::hashKey is the hash function for a string.
 */
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

/* VarTab::equalKeyKvpair compares a key with a key-value pair.
 *
 * Since the key-value pair has the form "key=value",
 * the character '=' is considered the terminator instead of '\0'
 * for the key-value pair.
 */
bool VarTab::equalKeyKvpair(const char *key, const char *kvPair)
{
	while (*key == *kvPair) {
		assert(*key != '\0' && *key != '='); // not allowed in a key
		key++;
		kvPair++;
	}
	return (*key == '\0' && *kvPair == '=');
}

/* VarTab::setVar sets the value of a variable.
 *
 * It either creates a new variable or changes the value of
 * an existing one.
 */
void VarTab::setVar(const char *key, const char *value)
{
	assert(key != NULL && value != NULL);
	HashSlot *slot = lookup(key);
	if (slot->kvPair == NULL) {
		newVar(slot, key, value);
	} else {
		changeVar(slot, value);
	}
}

/* VarTab::getVar gets the value of a variable.
 *
 * If the variable does not exist, NULL is returned.
 */
const char *VarTab::getVar(const char *key) const
{
	assert(key != NULL);
	HashSlot *slot = lookup(key);
	std::string *kvPair = slot->kvPair;
	if (kvPair == NULL) {
		return NULL;
	}
	return kvPair->c_str() + slot->vOffset;
}

/* VarTab::newVar creates a new variable.
 *
 * It is called internally when the variable is found to be nonexistent.
 */
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

	--hashFree;
	maybeRehash();
}

/* VarTab::maybeRehash checks the load factor of the hash table and performs
 * a rehash when necessary.
 *
 * It is called internally when a new element is inserted into the hash table.
 */
void VarTab::maybeRehash()
{
	if (hashFree < hashSize / 2) {
		size_t newSize = hashSize * 2;
		HashSlot *newTab = new HashSlot[newSize]();
		HashSlot *newEnd = newTab + newSize;
		for (size_t i = 0; i < hashSize; i++) {
			HashSlot *p = &hashTab[i];
			HashSlot *q = &newTab[p->hash % newSize];
			while (q->kvPair != NULL) {
				q++; // linear probing
				if (q == newEnd) {
					q = newTab; // wrap around
				}
			}
			*q = *p;
		}
		hashFree += (newSize - hashSize);
		hashSize = newSize;
		delete[] hashTab;
		hashTab = newTab;
	}
}

/* VarTab::changeVar changes the value of a key-value pair.
 *
 * It is called internally when the variable is found to be existent.
 */
void VarTab::changeVar(VarTab::HashSlot *slot, const char *value)
{
	std::string *kvPair = slot->kvPair;
	kvPair->replace(kvPair->begin() + slot->vOffset, kvPair->end(), value);

	// pointer to the C string may be invalidated
	if (slot->idxExported != NOT_EXPORTED)
		exported[slot->idxExported] = kvPair->c_str();
}

/* VarTab::exportVar exports a variable.
 *
 * The exported variable will be put into an array
 * that can be retrieved by calling VarTab::getExported.
 */
void VarTab::exportVar(const char *key)
{
	HashSlot *slot = lookup(key);
	if (slot->kvPair == NULL) { // var does not exist
		newVar(slot, key, "");
		slot = lookup(key); // slot may change after rehashing
	}
	if (slot->idxExported != NOT_EXPORTED) {
		return; // don't export twice
	}
	exported.back() = slot->kvPair->c_str();
	exported.push_back(NULL); // envp ends with NULL
}

/* VarTab::getExported returns an array that contains all exported variables.
 *
 * Each element in the array is a string of the form "key=value",
 * except that the last one is NULL to indicate the end.
 */
const char *const *VarTab::getExported() const
{
	return &exported[0];
}
