#include "test.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef TESTING
#error // TESTING disabled
#endif

TEST(test_infrastructure_works_correctly)
{
	TEST_COMPLETE();
}

#define TEST_DISPATCH(name) TEST(name);
#include "tests_list.h"
#undef TEST_DISPATCH

struct test {
	const char *name;
	bool (*fun)();
};

constexpr struct test tests[] = {
#define TEST_DISPATCH(test_name)                                               \
	(struct test){.name = #test_name, .fun = &test_##test_name},
#include "tests_list.h"
#undef TEST_DISPATCH
};
constexpr size_t tests_num = sizeof tests / sizeof tests[0];

int main()
{
	size_t successes = 0;
	for (size_t i = 0; i < tests_num; ++i) {
		fprintf(stderr, "Testing %s...", tests[i].name);
		if (tests[i].fun()) {
			fprintf(stderr, "OK\n");
			successes++;
		} else {
			fprintf(stderr, "Failed\n");
		}
	}
	fprintf(stderr, "%zu/%zu successes\n", successes, tests_num);
	return EXIT_SUCCESS;
}
