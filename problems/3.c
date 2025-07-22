#include "../utils/factors.h"
#include "problems.h"
#include <stdint.h>

int64_t problem3()
{
	constexpr int64_t input = 600851475143ll;
	return largest_prime_factor(input);
}
