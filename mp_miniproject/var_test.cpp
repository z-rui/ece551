#include <iostream>
#include <sstream>

#include "var.h"

static void printVar(VarTab& vtab, const char *key)
{
	const char *value = vtab.getVar(key);
	std::cout << (value ? value : "(none)") << std::endl;
}

static void printExported(VarTab& vtab)
{
	const char *const *p = vtab.getExported();
	while (*p != NULL) {
		std::cout << *p << std::endl;
		p++;
	}
}

int main()
{
	VarTab vtab;

	printVar(vtab, "xxx");
	vtab.setVar("xxx", "yyy");
	printVar(vtab, "xxx");
	vtab.setVar("xxx", "zzz");
	printVar(vtab, "xxx");

	printExported(vtab);
	vtab.exportVar("xxx");
	printExported(vtab);

	for (int i = 0; i < 100; i++) {
		std::stringstream ss;
		ss << "var" << i;
		vtab.setVar(ss.str().c_str(), "dummy");
		if (i % 2 != 0) {
			vtab.exportVar(ss.str().c_str());
		}
	}
	printExported(vtab);
	return 0;
}
