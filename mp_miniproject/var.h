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
		// only the pointers are stored,
		// otherwise hashtable makes copies of the strings
		// during rehashing which 1) is slow;
		// 2) invalidates pointers in exported
		std::string *kvPair; // "key=value"
		size_t vPos; // position of 'v' (after '=')
		size_t idxExported; // index in exported
		bool operator== (std::pair<const char *, size_t>) const;
	};
	HashTab<HashItem> hashTab;
	std::list<std::string> variables; // list of all stored "key=value";
					// owns these strings
	std::vector<const char *> exported; // vector of exported variables;
					// can be used as envp directly

	// unfortunately "export" and "new" are both keywords...
	void exportVar(HashItem *);
	void newVar(HashItem *, const char *, size_t, const char *);
	HashItem *setVar(const char *, size_t, const char *);

	// please don't make copies
	VarTab(const VarTab&);
	VarTab& operator=(const VarTab&);
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
