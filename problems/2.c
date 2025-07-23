#include <stdint.h>

typedef struct vec2 vec2;
struct vec2 {
	int64_t first, second;
};

static inline vec2 next(vec2 f)
{
	return (vec2){f.second, f.first + f.second};
}

int64_t problem2()
{
	int64_t sum = 0;
	for (vec2 f = (vec2){0, 1}; f.second < 4000000; f = next(f)) {
		if (f.second % 2 == 0)
			sum += f.second;
	}

	return sum;
}
