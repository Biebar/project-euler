#include "problems.h"

int64_t problem1()
{
	int64_t sum = 0;
	for (int64_t n = 0; n < 1000; ++n) {
		if (n % 3 == 0 || n % 5 == 0)
			sum += n;
	}
	return sum;
}
