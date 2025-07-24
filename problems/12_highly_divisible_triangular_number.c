#include "../utils/divisibility.h"
#include <stdint.h>

int64_t problem12_highly_divisible_triangular_number()
{
	constexpr int64_t input = 500;
	int64_t triangular = 1;
	int64_t index = 1;
	while (number_of_factors(triangular) < input) {
		index++;
		triangular += index;
	}
	return triangular;
}
