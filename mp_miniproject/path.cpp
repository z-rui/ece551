#include "path.h"

#include <string>

/* PathSearcher::setPath records the new value of the PATH variable
 * specified in newpath.
 * This affects all future searches.
 */
void PathSearcher::setPath(const char *newpath)
{
	pathFields.clear();
	path = newpath;

	// parse the ':' seperated path string
	size_t pos = 0;
	while (pos != std::string::npos) {
		size_t save = pos;
		pos = path.find(':', pos);
		if (pos != std::string::npos) {
			path[pos] = '\0';
			pos++;
		}
		pathFields.push_back(&path[save]);
	}
}

#include <sys/stat.h> /* for stat */

/* is_regular_file tells if a file is a regular file.
 */
static bool is_regular_file(const char *pathname)
{
	struct stat st;

	// use stat, because I want to follow symbolic links
	if (stat(pathname, &st) == 0) {
		return S_ISREG(st.st_mode);
	}
	return false;
}

#include <string.h> /* for strchr */

/* PathSearcher::search searches for progname
 * according to the rule in the spec:
 * 1. If progname contains '/', test if it's regular file;
 * 2. Otherwise, search in paths that was previously set by setPath.
 *
 * In either case, if a regular file is found,
 * it return the path that can be used to open the file.
 * Otherwise it returns NULL.
 */
const char *PathSearcher::search(const char *progname)
{
	if (strchr(progname, '/') != NULL) {
		// case 1
		return is_regular_file(progname) ? progname : NULL;
	}

	// case 2. maybe add a cache later
	typename std::vector<const char *>::const_iterator it;

	for (it = pathFields.begin(); it != pathFields.end(); ++it) {
		buf = *it; // directory
		if (buf.size() > 0 && buf[buf.size()-1] != '/') {
			buf += '/'; // separator
		}
		buf += progname;
		if (is_regular_file(buf.c_str())) {
			return buf.c_str();
		}
	}
	return NULL;
}
