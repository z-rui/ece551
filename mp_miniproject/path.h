#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>

class PathSearcher {
	std::string path;
	std::string buf;
	std::vector<const char *> pathFields;
public:
	void setPath(const char *);
	const char *search(const char *);
};

#endif /* PATH_H */
