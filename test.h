#ifndef EULER_TEST_H
#define EULER_TEST_H

#define TEST(name) bool test_##name()
#define TEST_ASSERT(cond)                                                      \
	do {                                                                        \
		if (!(cond))                                                             \
			return false;                                                         \
	}
#define TEST_COMPLETE() return true

#endif // EULER_TEST_H
