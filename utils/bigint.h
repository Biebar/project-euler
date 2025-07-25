#ifndef EULER_UTILS_BIGINT_H
#define EULER_UTILS_BIGINT_H

#include "vec.h"

#define BIGINT_DIGIT_MAX UINT64_MAX

typedef uvec bigint;

void bigint_sum(bigint *dest, bigint a, bigint b);
inline bigint bigint_create(uint64_t value)
{
	bigint ret = {};
	*uvec_push(&ret) = value;
	return ret;
}
inline void bigint_destroy(bigint *x)
{
	uvec_destroy(x);
}

#endif // EULER_UTILS_BIGINT_H
