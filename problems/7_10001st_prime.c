#include "../utils/divisibility.h"
#include <stdint.h>

int64_t problem7_10001st_prime()
{
	constexpr int64_t input = 10001;
	int64_t prime_count = 0;
	int64_t current = 1;
	while (prime_count < input) {
		current++;
		if (is_prime(current))
			prime_count++;
	}
	return current;
}
