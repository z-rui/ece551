//This file is for Step 2.
//You should do 
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sumSeq2
//  Step 2 (D): add test cases to main to test sumSeq2
// 
// Be sure to #include any header files you need!

#include <stdio.h>

int seq2(int x)
{
	/* seq2(n+1) - seq2(n) = 2*n + 4
	 * seq2(0) = 1
	 * => seq2(n) = n*(n-1) + 4*n + 1
	 */
	return x*(x+3) + 1;
}

int sumSeq2(int low, int high)
{
	int s = 0;
	int i;

	for (i = low; i < high; i++) {
		s += seq2(i);
	}
	return s;
}

void check_seq2(int x)
{
	int y;

	y = seq2(x);
	printf("seq2(%d) = %d\n", x, y);
}

void check_sumSeq2(int low, int high)
{
	int sum;

	sum = sumSeq2(low, high);
	printf("sumSeq2(%d, %d) = %d\n", low, high, sum);
}

int main()
{
	check_seq2(0);
	check_seq2(1);
	check_seq2(14);
	check_seq2(-4);

	check_sumSeq2(1, 3);
	check_sumSeq2(2, 5);
	check_sumSeq2(0, 0);
	check_sumSeq2(32766, 32768);
	check_sumSeq2(-2147483647, -2147483648);

	return 0;
}
