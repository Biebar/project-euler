#ifndef EULER_TEST_H
#define EULER_TEST_H

struct test_result {
	bool success;
	const char *message;
};

#define TEST(name) struct test_result test_##name()
#define TEST_ASSERT(cond)                                                      \
	do {                                                                        \
		if (!(cond))                                                             \
			return (struct test_result){.success = false, .message = #cond};      \
	} while (false)
#define TEST_COMPLETE()                                                        \
	return (struct test_result)                                                 \
	{                                                                           \
		.success = true, .message = nullptr                                      \
	}

#endif // EULER_TEST_H
