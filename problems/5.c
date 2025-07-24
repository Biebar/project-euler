#include "../utils/divisibility.h"
#include <stdint.h>

int64_t problem5()
{
	int64_t lcm = 1;
	for (int64_t i = 1; i <= 20; ++i)
		lcm = least_common_multiple(lcm, i);
	return lcm;
}
