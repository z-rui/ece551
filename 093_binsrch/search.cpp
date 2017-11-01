#include "function.h"
#include <iostream>

int binarySearchForZero(Function<int, int> * f, int low, int high)
{
	int original_low = low;

	while (low < high) {
		int mid = low + (high - low) / 2;
		int value = f->invoke(mid);
		if (value <= 0) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	// I don't understand why return low when all positive...
	return (low == original_low) ? low : low - 1;
}
