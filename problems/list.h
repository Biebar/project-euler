#ifndef EULER_PROBLEMS_LIST_H
#define EULER_PROBLEMS_LIST_H

#include "problems.h"

static int64_t (*problems[])() = {
#include "problem_func_names.txt"
};
constexpr auto num_problems = sizeof problems / sizeof problems[0];

#endif // EULER_PROBLEMS_LIST_H
