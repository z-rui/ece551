#include "hash.h"

size_t hashStr(const char *s, size_t len)
{
	size_t h = 6549;
	for (; len > 0; len--) {
		h = h * 1558 + ((unsigned char) *s ^ 233);
	}
	return h;
}
