#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned, unsigned);

void check(unsigned base, unsigned exponent, unsigned expected_result)
{
	if (power(base, exponent) != expected_result) {
		printf("Found a bug in power(%u,%u) !!\n", base, exponent);
		exit(EXIT_FAILURE);
	}
}

int main()
{
	check(0, 0, 1);
	check(0, 1, 0);
	check(1, 0, 1);
	check(2, 0, 1);
	check(3, 3, 27);
	check(5, 4, 625);
	check(4294967295, 1, 4294967295);
	return EXIT_SUCCESS;
}
