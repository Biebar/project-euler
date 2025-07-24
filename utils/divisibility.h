#ifndef EULER_UTIL_DIVISIVILITY_H
#define EULER_UTIL_DIVISIVILITY_H

#include <stdint.h>

int64_t smallest_prime_factor(int64_t n);
int64_t largest_prime_factor(int64_t n);
bool is_prime(int64_t n);
int64_t least_common_multiple(int64_t a, int64_t b);
int64_t greatest_common_divisor(int64_t a, int64_t b);
int64_t number_of_factors(int64_t n);

#endif // EULER_UTIL_DIVISIVILITY_H
