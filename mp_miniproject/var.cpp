#include <utility>

#include <assert.h>
#include <string.h>

#include "var.h"

#define NOT_EXPORTED (~(size_t)0)

/* Constructor
 */
VarTab::VarTab()
{
	exported.push_back(NULL); // envp ends with NULL
}

/* VarTab::setVar sets the value of a variable.
 *
 * It either creates a new variable or changes the value of
 * an existing one.
 */
void VarTab::setVar(const char *key, const char *value)
{
	assert(key != NULL && value != NULL);
	setVar(key, strlen(key), value);
}

/* VarTab::getVar gets the value of a variable.
 *
 * If the variable does not exist, NULL is returned.
 */
const char *VarTab::getVar(const char *key) const
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	size_t hash = hashStr(key, key_len);
	const HashItem *it = hashTab.find(hash, std::make_pair(key, key_len));
	debug << "VarTab: get: \"";
	if (it == NULL) {
		debug << key << "\" undefined\n";
		return NULL;
	}
	debug << *it->kvPair << "\"\n";
	return it->kvPair->c_str() + it->vPos;
}


/* VarTab::exportVar exports a variable.
 *
 * The exported variable will be put into an array
 * that can be retrieved by calling VarTab::getExported.
 */
void VarTab::exportVar(const char *key)
{
	assert(key != NULL);
	size_t key_len = strlen(key);
	size_t hash = hashStr(key, key_len);
	HashItem *it;
	if (hashTab.add(hash, std::make_pair(key, key_len), &it)) {
		// var did not exist; create an empty one
		newVar(it, key, key_len, "");
	} // otherwise keep the old value
	exportVar(it);
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

/* VarTab::importExported imports several variables at once and exports them.
 *
 * Useful for initializing the environment variables.
 */
void VarTab::importExported(const char *const *environ)
{
	assert(environ != NULL);
	for (; *environ != NULL; environ++) {
		const char *p = strchr(*environ, '=');
		if (p == NULL) {
			// a weird entry that does not have a '='
			continue;
		}
		HashItem *it = setVar(*environ, p - *environ, p + 1);
					/* key, key_len, value */
		exportVar(it);
	}
}


/* Internal version of VarTab::setVar.
 *
 * It requires the length of the key.
 * This is useful for VarTab::importExported.
 * Because the strings in environ array are like "key=value",
 * It's fine to pass that string with length = (length of "key")
 * and thus avoids a copy.
 *
 * It also returns the HashItem pointer.
 * Because new variables are unexported, importExported
 * can then export it by calling the internal version
 * of exportVar.
 */
VarTab::HashItem *VarTab::setVar(const char *key, size_t key_len,
		const char *value)
{
	size_t hash = hashStr(key, key_len);
	HashItem *it;
	if (hashTab.add(hash, std::make_pair(key, key_len), &it)) {
		newVar(it, key, key_len, value);
	} else {
		std::string& kv = *it->kvPair;
		debug << "VarTab: set: \"" << kv;
		kv.replace(kv.begin() + it->vPos, kv.end(), value);
		debug << "\" changed to \"" << kv << "\"\n";
		if (it->idxExported != NOT_EXPORTED) {
			// pointer may be invalidated
			exported[it->idxExported] = kv.c_str();
		}
	}
	return it;
}

/* VarTab::newVar initializes an HashItem.
 */
void VarTab::newVar(HashItem *it, const char *key, size_t key_len,
		const char *value)
{
	// "variables" owns all the memory of the strings
	variables.push_back(std::string());
	std::string& kv = variables.back();
	kv.assign(key, key_len);
	kv.push_back('=');
	kv.append(value);
	it->kvPair = &kv;
	it->vPos = key_len + 1;
	it->idxExported = NOT_EXPORTED;
	debug << "VarTab: new: \"" << kv << "\"\n";
}

/* Internal version of VarTab::exportVar.
 *
 * It takes a HashItem pointer from a previous hash search.
 */
void VarTab::exportVar(HashItem *it)
{
	if (it->idxExported != NOT_EXPORTED) {
		return; // don't export twice
	}
	exported.back() = it->kvPair->c_str();
	it->idxExported = exported.size() - 1;
	exported.push_back(NULL); // envp ends with NULL
	debug << "VarTab: export: \"" << *it->kvPair << "\"\n";
}

/* All methods in this class only use std::pair<const char *, size_t>
 * as the key when doing hash search.
 *
 * Therefore only one == operator for HashItem is given.
 */
bool VarTab::HashItem::operator==
	(std::pair<const char *, size_t> pair) const
{
	return kvPair->compare(0, vPos - 1, pair.first, pair.second) == 0;
}
