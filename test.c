#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TESTING
#error // TESTING disabled
#endif

TEST(test_infrastructure_works_correctly)
{
	TEST_ASSERT(1 == 1);
	TEST_COMPLETE();
}

#define TEST_DISPATCH(name) TEST(name);
#include "tests_list.h"
#undef TEST_DISPATCH

struct test {
	const char *name;
	struct test_result (*fun)();
};

#define VEC_NAME testvec
#define VEC_TYPE struct test
#define VEC_IMPL
#include "utils/vec_template.h"
#undef VEC_IMPl

constexpr struct test all_tests[] = {
#define TEST_DISPATCH(test_name)                                               \
	(struct test){.name = #test_name, .fun = &test_##test_name},
#include "tests_list.h"
#undef TEST_DISPATCH
};
constexpr size_t tests_num = sizeof all_tests / sizeof all_tests[0];

static struct test const *find_test(char const *name)
{
	struct test const *start = &all_tests[0];
	struct test const *end = &all_tests[tests_num];
	while (end - start) {
		struct test const *const midpoint = start + (end - start) / 2;
		int const diff = strcmp(midpoint->name, name);
		if (diff > 0)
			end = midpoint;
		else if (diff < 0)
			start = midpoint + 1;
		else
			return midpoint;
	}
	return nullptr;
}

static struct testvec parse_args(int argc, char const *argv[argc + 1])
{
	struct testvec tests = {};
	if (argc == 2 && strcmp(argv[1], "--all") == 0)
		goto end;
	for (int i = 1; i < argc; ++i) {
		if (strncmp(argv[i], "--test=", 7) != 0) {
			fprintf(stderr, "Unknown argument : %s\n", argv[i]);
			exit(-1);
		}

		struct test const *const test = find_test(&argv[i][7]);
		if (test == nullptr) {
			fprintf(stderr, "Unknown test : %s\n", &argv[i][7]);
			exit(-1);
		}
		*testvec_push(&tests) = *test;
		continue;
	}
end:
	if (tests.len == 0) {
		for (size_t i = 0; i < tests_num; ++i) {
			*testvec_push(&tests) = all_tests[i];
		}
	}
	return tests;
}

int main(int argc, char const *argv[argc + 1])
{
	struct testvec const tests = parse_args(argc, argv);
	int failures = 0;
	for (size_t i = 0; i < tests.len; ++i) {
		fprintf(stderr, "Testing %s...", tests.data[i].name);
		struct test_result const result = tests.data[i].fun();
		if (result.success) {
			fprintf(stderr, "OK\n");
		} else {
			fprintf(stderr, "Failed\n%s\n", result.message);
			failures++;
		}
	}
	return failures;
}
