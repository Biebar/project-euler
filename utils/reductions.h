#ifndef EULER_UTILS_REDUCTIONS_H
#define EULER_UTILS_REDUCTIONS_H

#include <stddef.h>
#include <stdint.h>

int64_t array_product(
	 size_t len, size_t stride, int64_t const data[(len - 1) * stride + 1]);

int64_t array_max_prod(
	 size_t len,
	 size_t stride,
	 int64_t const data[(len - 1) * stride + 1],
	 size_t window_size);

#endif // EULER_UTILS_REDUCTIONS_H
