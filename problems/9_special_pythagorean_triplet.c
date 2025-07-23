#include <stddef.h>
#include <stdint.h>

static inline bool is_triplet(int64_t a, int64_t b, int64_t c)
{
	return a * a + b * b == c * c;
}

int64_t problem9_special_pythagorean_triplet()
{
	constexpr int64_t input = 1000;
	for (int64_t a = 1; a < input; ++a) {
		int64_t const max_b = input - a;
		for (int64_t b = a + 1; b < max_b; ++b) {
			int64_t const c = input - a - b;
			if (is_triplet(a, b, c))
				return a * b * c;
		}
	}
	return 0;
}
