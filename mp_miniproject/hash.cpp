#include "hash.h"

/* hashStr hashes a string */
size_t hashStr(const char *s, size_t len)
{
	size_t h = 6549;
	for (; len > 0; len--) {
		h = h * 1558 + ((unsigned char) *s ^ 233);
		s++;
	}
	return h;
}
