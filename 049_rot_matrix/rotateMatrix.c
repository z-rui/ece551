#include <stdio.h>
#include <stdlib.h>

#define COLS 10
#define ROWS 10

void error(const char *msg)
{
	fprintf(stderr, "Invalid input: %s\n", msg);
	exit(EXIT_FAILURE);
}

void read_line(char line[], FILE *f)
{
	int i;
	int ch;

	i = 0;
	for (;;) {
		ch = fgetc(f);
		switch (ch) {
			case '\n':
				if (i == COLS) {
					line[COLS] = '\0';
					return;
				} else {
					error("line is too short");
				}
				break;
			case EOF:
				if (i == COLS) {
					error("unexpected EOF");
				} else {
					error("line is too short");
				}
				break;
			default:
				if (i < COLS) {
					line[i++] = (char) ch;
				} else {
					error("line is too long");
				}
				break;
		}
	}
}

void print_rotated(const char M[ROWS][COLS])
{
	int i, j;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			/* let M' be the rotated matrix,
			 * then M'(i, j) = M(COLS-1-j, i) */
			putchar(M[COLS-1-j][i]);
		}
		putchar('\n');
	}
}

int main(int argc, char *argv[])
{
	char matrix[ROWS][COLS];
	FILE *f;
	int i, ch;

	if (argc != 2) {
		fprintf(stderr, "Usage: rotateMatrix input\n");
		exit(EXIT_FAILURE);
	}
	if ((f = fopen(argv[1], "r")) == 0) {
		fprintf(stderr, "Could not open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < ROWS; i++) {
		read_line(matrix[i], f);
	}
	/* test for EOF */
	if ((ch = fgetc(f)) != EOF) {
		fprintf(stderr, "Invalid input: file is too long (read %d instead of EOF)\n", ch);
		exit(EXIT_FAILURE);
	}
	fclose(f);

	print_rotated(matrix);

	return 0;
}
