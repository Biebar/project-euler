#include "reductions.h"
#include "unequality.h"
#include <assert.h>

int64_t array_product(
	 size_t len, size_t stride, int64_t const data[(len - 1) * stride + 1])

{
	assert(stride > 0);
	size_t const end = len * stride;
	int64_t prod = 1;
	for (size_t i = 0; i < end; i += stride) {
		prod *= data[i];
	}
	return prod;
}

int64_t array_max_prod(
	 size_t len,
	 size_t stride,
	 int64_t const data[(len - 1) * stride + 1],
	 size_t window_size)
{
	assert(stride > 0);
	int64_t max_prod = INT64_MIN;
	if (len + 1 < window_size)
		return max_prod;
	size_t const end = (len - window_size + 1) * stride;
	for (size_t i = 0; i < end; i += stride) {
		max_prod = max(max_prod, array_product(window_size, stride, &data[i]));
	}
	return max_prod;
}
