#include "../utils/divisibility.h"
#include <stdint.h>

int64_t problem10_summation_of_primes()
{
	constexpr int64_t input = 2000000;
	int64_t sum = 0;
	for (int64_t i = 2; i < input; ++i) {
		if (is_prime(i))
			sum += i;
	}
	return sum;
}
