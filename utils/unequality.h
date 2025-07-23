#ifndef EULER_UTILS_UNEQUALITY_H
#define EULER_UTILS_UNEQUALITY_H

#include <stdint.h>

inline int64_t min(int64_t a, int64_t b)
{
	return a > b ? b : a;
}
inline int64_t max(int64_t a, int64_t b)
{
	return a > b ? a : b;
}
inline int64_t iabs(int64_t a)
{
	return max(a, -a);
}

#endif // EULER_UTILS_UNEQUALITY_H
