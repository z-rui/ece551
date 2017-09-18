#include <stdio.h>
#include <stdlib.h>

int seq3(int, int);
int countEvenInSeq3Range(int, int, int, int);

int naiveCount(int xLow, int xHigh, int yLow, int yHigh)
{
		int x, y;
		int count = 0;

		for (x = xLow; x < xHigh; x++) {
				for (y = yLow; y < yHigh; y++) {
						if (seq3(x, y) % 2 != 0)
								++count;
				}
		}
		return count;
}

int main()
{
	int x, y;

	puts("                    x");
	puts("      -5 -4 -3 -2 -1   0   1   2   3   4   5");
	puts("    +---------------------------------------");
	for (y = -5; y <= 5; y++) {
		printf("%c%2d |", (y == 0) ? 'y' : ' ', y);
		for (x = -5; x <= 5; x++) {
			printf((x < 0) ? "%3d" : "%4d", seq3(x, y));
		}
		putchar('\n');
	}

	for (x = -5; x <= 5; x++) {
		for (y = -5; y <= 5; y++) {
			int x1, y1;
			for (x1 = x; x <= 5; x++) {
				for (y1 = y; y <= 5; y++) {
					int z1, z2;

					z1 = countEvenInSeq3Range(x, x1, y, y1);
					z2 = naiveCount(x, x1, y, y1);
					if (z1 != z2) {
							printf("count(%d,%d,%d,%d) fail !!\n",
									x, x1, y, y1);
							abort();
					}
				}
			}
		}
	}

	return 0;
}

#define main dummy
#include "step3.c"
