//This file is for Step 3.
//You should do 
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
// 
// Be sure to #include any header files you need!

#include <stdio.h>

int seq3(int x, int y)
{
	/* seq3(x, y) = f(x-1, y) + (y-3)
	 *            = f(x, y-1) + (x+2)
	 * seq3(-2, 3) = 6
	 * => seq3(x, y) = (y-3)*(x+2) + 6
	 */
	return (y-3)*(x+2) + 6;
}

int countEvenInSeq3Range(int xLow, int xHigh, int yLow, int yHigh)
{
	unsigned H, W, E1, E2;

	if (xHigh <= xLow || yHigh <= yLow) {
		return 0;
	}

	// the following calculations won't introduce signed overflow.

	W = (unsigned) xHigh - (unsigned) xLow;
	H = (unsigned) yHigh - (unsigned) yLow;

	E1 = H / 2 + ((H % 2) && (yLow % 2));
	E2 = W / 2 + ((W % 2) && !(xLow % 2));

	// the last one, however, may overflow.

	return (int) (E1 * W + (H-E1) * E2);
}

void check_seq3(int x, int y)
{
	int z;

	z = seq3(x, y);
	printf("seq3(%d, %d) = %d\n", x, y, z);
}

void check_countEvenInSeq3Range(int xLow, int xHigh, int yLow, int yHigh)
{
	int ret;

	ret = countEvenInSeq3Range(xLow, xHigh, yLow, yHigh);
	printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
		xLow, xHigh, yLow, yHigh, ret);
}

int main()
{
	check_seq3(0,0);
	check_seq3(0,1);
	check_seq3(1,1);
	check_seq3(0,2);

	check_countEvenInSeq3Range(0,2,0,3);
	check_countEvenInSeq3Range(0,3,0,3);
	check_countEvenInSeq3Range(0,3,0,4);
	check_countEvenInSeq3Range(0,4,0,3);
	check_countEvenInSeq3Range(0,4,0,4);
	check_countEvenInSeq3Range(1,3,1,3);
	check_countEvenInSeq3Range(1,3,1,4);
	check_countEvenInSeq3Range(1,4,1,3);
	check_countEvenInSeq3Range(1,4,1,4);
	check_countEvenInSeq3Range(0,0,0,1);
	check_countEvenInSeq3Range(0,1,0,0);
	// don't write xHigh - xLow > 0 ...
	check_countEvenInSeq3Range(1,-2147483648,0,0);
	check_countEvenInSeq3Range(0,0,1,-2147483648);
	//check_countEvenInSeq3Range(-2147483648,0,0,1);
	//check_countEvenInSeq3Range(0,53509,0,53509);

	return 0;
}
