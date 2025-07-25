#ifndef EULER_UTILS_VEC_H
#define EULER_UTILS_VEC_H

#include <stdint.h>
#define VEC_NAME ivec
#define VEC_TYPE int64_t
typedef struct VEC_NAME VEC_NAME;
#include "vec_template.h"

#include <stddef.h>
#define VEC_NAME uvec
#define VEC_TYPE uint64_t
typedef struct VEC_NAME VEC_NAME;
#include "vec_template.h"

#endif // EULER_UTILS_VEC_H
