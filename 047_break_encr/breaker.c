#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int findMaxIdx(int array[], int n)
{
	int maxIdx = 0;
	int i;

	for (i = 0; i < n; i++) {
		if (array[i] > array[maxIdx]) {
			maxIdx = i;
		}
	}
	return maxIdx;
}

int getkey(FILE *f)
{
	int freq[26] = {0};
	int ch;
	int maxIdx;

	while ((ch = fgetc(f)) != EOF) {
		if (isalpha(ch)) {
			freq[tolower(ch) - 'a']++;
		}
	}
	maxIdx = findMaxIdx(freq, 26);
	/* transform is 4 (e) -> maxIdx */
	/* or x -> (x + (maxIdx - 4)) % 26 */
	return (maxIdx + 22) % 26;
}

int main(int argc, char *argv[])
{
	FILE *f;
	int key;

	if (argc != 2) {
		fprintf(stderr, "usage %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	f = fopen(argv[1], "r");
	if (f == NULL) {
		fprintf(stderr, "failed to open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	key = getkey(f);
	printf("%d\n", key);

	fclose(f);
	return 0;
}
