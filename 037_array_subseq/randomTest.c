#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maxSeq myMaxSeq
#include "maxSeq.c"
#undef maxSeq

extern size_t maxSeq(int *, size_t);

int array[8192];

void dotest()
{
	int i, n;

	n = rand() % 8192;
	for (i = 0; i < n; i++) {
		array[i] = rand();
	}
	if (maxSeq(array, n) != myMaxSeq(array, n)) {
		printf("found a bug !!\n");
		abort();
	}
}

int main() {
	int N = 3000;

	srand(time(0));
	while (N--) {
		dotest();
	}
	return 0;
}
