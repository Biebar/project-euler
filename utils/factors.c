#include "factors.h"
#include "unequality.h"
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

bool is_prime(int64_t n)
{
	return smallest_prime_factor(n) == n;
}

static inline void swap(int64_t *a, int64_t *b)
{
	int64_t tmp = *a;
	*a = *b;
	*b = tmp;
}
int64_t greatest_common_divisor(int64_t a, int64_t b)
{
	if (a < 0)
		a *= -1;
	if (b < 0)
		b *= -1;
	if (a < b)
		swap(&a, &b);
	while (b != 0) {
		int64_t rem = a % b;
		a = b;
		b = rem;
	}
	return a;
}
int64_t least_common_multiple(int64_t a, int64_t b)
{
	int64_t gcd = greatest_common_divisor(a, b);
	if (gcd == 0)
		return 1;
	int64_t lcm = iabs((a / gcd) * b);
	return lcm;
}
