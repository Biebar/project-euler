#include "factors.h"
#include <assert.h>
#include <stdint.h>

typedef struct int_and_square int_and_square;
struct int_and_square {
	int64_t val, square;
};

static int_and_square inc_int_and_square(int_and_square x)
{
	assert(x.square == x.val * x.val);
	assert(x.val >= 0);
	assert(x.val <= INT64_MAX - 1);
	assert((x.val + 1) < INT64_MAX / (x.val + 1));

	int_and_square ret = {};
	if (x.square > INT64_MAX - 2 * (x.val + 1) - 1)
		ret = (int_and_square){INT64_MAX, INT64_MAX};
	else
		ret = (int_and_square){.val = x.val + 1,
		                       .square = x.square + 2 * x.val + 1};

	assert(ret.val == x.val + 1);
	assert(ret.square == ret.val * ret.val);
	return ret;
}

int64_t smallest_prime_factor(int64_t n)
{
	assert(n != INT64_MAX);
	int64_t ret = 0;
	for (int_and_square i = {2, 4}; i.square <= n; i = inc_int_and_square(i)) {
		if (n % i.val == 0) {
			ret = i.val;
			break;
		}
	}
	if (ret == 0)
		ret = n;
	assert(n % ret == 0);
	return ret;
}

int64_t largest_prime_factor(int64_t n)
{
	assert(n != INT64_MAX);
	int64_t ret = n;
	for (int64_t factor = 1; factor != ret;
	     factor = smallest_prime_factor(ret)) {
		ret = ret / factor;
	}
	assert(n % ret == 0);
	return ret;
}
