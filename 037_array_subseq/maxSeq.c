#include <stdlib.h>

size_t maxSeq(int *array, size_t n)
{
	size_t i, j;
	size_t max = 0;

	i = 0;
	for (j = 1; j <= n; j++) {
		if (j == n || array[j] <= array[j-1]) {
			// increasing sequence is array[i..j-1]
			if (j - i > max) {
				max = j - i;
			}
			i = j;
		}
	}
	return max;
}
