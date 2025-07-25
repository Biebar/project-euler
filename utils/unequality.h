#ifndef EULER_UTILS_UNEQUALITY_H
#define EULER_UTILS_UNEQUALITY_H

#include <stddef.h>
#include <stdint.h>

#ifdef UNEQUALITY_IMPL
#define MINMAX_EXTERN extern
#else
#define MINMAX_EXTERN
#endif
#define MINMAXDEF(name, type, operator)                                        \
	MINMAX_EXTERN inline type name(type a, type b)                              \
	{                                                                           \
		return a operator b ? a : b;                                             \
	}
MINMAXDEF(imin, int64_t, <)
MINMAXDEF(imax, int64_t, >)
MINMAXDEF(smin, size_t, <)
MINMAXDEF(smax, size_t, >)
#define min(a, b) _Generic((a), int64_t: imin, size_t: smin)(a, b)
#define max(a, b) _Generic((a), int64_t: imax, size_t: smax)(a, b)
#undef MINMAXDEF
#undef MINMAX_EXTERN
inline int64_t iabs(int64_t a)
{
	return max(a, -a);
}

#endif // EULER_UTILS_UNEQUALITY_H
