#ifndef EULER_UTILS_BIGINT_H
#define EULER_UTILS_BIGINT_H

#include "vec.h"

#define BIGINT_DIGIT_MAX UINT64_MAX

typedef uvec bigint;

void bigint_normalize(bigint *a);
int bigint_cmp(bigint a, bigint b);
void bigint_sum(bigint *dest, bigint a, bigint b);
void bigint_diff(bigint *dest, bigint a, bigint b);
void bigint_copy(bigint *dest, bigint source);
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
