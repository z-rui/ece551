#include "path.h"
#include <string>
#include <iostream>

const char *maybeNULL(const char *s)
{
	return (s == NULL) ? "(null)" : s;
}

int main()
{
	PathSearcher ps;
	const char *s;

	ps.setPath("/bin:/usr/bin:/usr/local/bin");
	s = ps.search("ls");
	std::cout << maybeNULL(s) << std::endl;
	s = ps.search("vim");
	std::cout << maybeNULL(s) << std::endl;
	s = ps.search("evil");
	std::cout << maybeNULL(s) << std::endl;

	return 0;
}
