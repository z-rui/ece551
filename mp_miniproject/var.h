#ifndef VAR_H
#define VAR_H

#include <list>
#include <vector>
#include <string>

class VarTab {
	struct HashSlot {
		size_t hash;
		std::string *kvPair;
		size_t vOffset;
		size_t idxExported;
	};
	HashSlot *hashTab;
	size_t hashSize, hashFree;
	std::list<std::string> variables;
	std::vector<const char *> exported;

	static size_t hashKey(const char *);
	static bool equalKeyKvpair(const char *, const char *);
	HashSlot *lookup(const char *) const;
	void maybeRehash();
	void newVar(HashSlot *, const char *, const char *);
	void changeVar(HashSlot *, const char *);
public:
	VarTab();
	~VarTab();
	void setVar(const char *, const char *);
	const char *getVar(const char *) const;
	void exportVar(const char *);
	const char *const *getExported() const;
};

#endif /* VAR_H */
