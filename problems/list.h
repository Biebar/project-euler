#ifndef EULER_PROBLEMS_LIST_H
#define EULER_PROBLEMS_LIST_H

#include <stdint.h>

#define PROBLEM_DISPATCH(name) int64_t problem##name();
#include "problems.h"
#undef PROBLEM_DISPATCH

static int64_t (*problems[])() = {
#define PROBLEM_DISPATCH(name) &problem##name,
#include "problems.h"
#undef PROBLEM_DISPATCH
};
constexpr auto num_problems = sizeof problems / sizeof problems[0];

#endif // EULER_PROBLEMS_LIST_H
