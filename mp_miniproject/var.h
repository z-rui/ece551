#ifndef VAR_H
#define VAR_H

#include <list>
#include <vector>
#include <string>
#include <utility>

#include "hash.h"
#include "debug.h"

class VarTab {
	struct HashItem {
		std::string *kvPair; // "key=value"
		size_t vPos; // position of 'v' (after '=')
		size_t idxExported; // index in exported
		bool operator== (std::pair<const char *, size_t>) const;
	};
	HashTab<HashItem> hashTab;
	std::list<std::string> variables; // list of all stored "key=value"
	std::vector<const char *> exported;

	HashItem *setVar(const char *, size_t, const char *);
	void exportVar(HashItem *);
	void newVar(HashItem *, const char *, size_t, const char *);
public:
	DebugStream debug;

	VarTab();
	void setVar(const char *, const char *);
	const char *getVar(const char *) const;
	void exportVar(const char *);
	const char *const *getExported() const;
	void importExported(const char *const *);
};

#endif /* VAR_H */
