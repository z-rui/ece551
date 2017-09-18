//This file is for Step 1.
//You should do 
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
// 
// Be sure to #include any header files you need!

#include <stdio.h>

int seq1(int x)
{
	return 4 * x - 3;
}

void printSeq1Range(int low, int high)
{
	int i;

	for (i = low; i < high; i++) {
		printf((i == low) ? "%d" : ",%d", seq1(i));
	}
	putchar('\n');
}

void check_seq1(int x)
{
	int y;

	y = seq1(x);
	printf("seq1(%d) = %d\n", x, y);
}

void check_printSeq1Range(int low, int high)
{
	printf("printSeq1Range(%d,%d)\n", low, high);
	printSeq1Range(low, high);
}

int main()
{
	check_seq1(0);
	check_seq1(1);
	check_seq1(10);
	check_seq1(-4);
	check_seq1(-536870911);

	check_printSeq1Range(1, 5);
	check_printSeq1Range(-3, 5);
	check_printSeq1Range(6, 4);
	check_printSeq1Range(0, 0);
	check_printSeq1Range(-536870911, -536870900);
	// don't write for (i = low; i <= high - 1; i++) ...
	check_printSeq1Range(-2147483647, -2147483648);

	return 0;
}
