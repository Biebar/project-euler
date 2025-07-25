#include <stddef.h>

#undef CONCAT
#undef CONCAT_
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

#define VEC_FUN(fun) CONCAT(VEC_NAME, CONCAT(_, fun))

struct VEC_NAME {
	VEC_TYPE *data;
	size_t len;
	size_t cap;
};

void VEC_FUN(grow)(struct VEC_NAME *vec);
inline VEC_TYPE *VEC_FUN(push)(struct VEC_NAME *vec)
{
	if (vec->len == vec->cap)
		VEC_FUN(grow)(vec);
	return &vec->data[vec->len++];
}
void VEC_FUN(destroy)(struct VEC_NAME *vec);

#ifdef VEC_IMPL
#include "unequality.h"
#include <stdlib.h>
#include <string.h>

void VEC_FUN(grow)(struct VEC_NAME *vec)
{
	if (vec->cap > SIZE_MAX / 2)
		goto err;
	size_t const new_cap = max(vec->cap, 4) * 2;

	VEC_TYPE *new_data = malloc((sizeof vec->data[0]) * new_cap);
	if (!new_data)
		goto err;

	if (vec->len)
		memcpy(new_data, vec->data, vec->len * sizeof vec->data[0]);
	if (vec->cap && vec->data)
		free(vec->data);

	vec->data = new_data;
	vec->cap = new_cap;
	return;
err:
	exit(EXIT_FAILURE);
}
extern VEC_TYPE *VEC_FUN(push)(struct VEC_NAME *vec);
void VEC_FUN(destroy)(struct VEC_NAME *vec)
{
	free(vec->data);
	*vec = (struct VEC_NAME){};
}
#endif
#undef VEC_FUN
#undef VEC_NAME
#undef VEC_TYPE
