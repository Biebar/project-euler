#include "bigint.h"
#include "vec.h"
#include <assert.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdlib.h>

static inline void swap(bigint *a, bigint *b)
{
	bigint tmp = *a;
	*a = *b;
	*b = tmp;
}

struct mult_result {
	uint64_t carry, result;
};

struct mult_result mult(uint64_t a, uint64_t b)
{
	struct mult_result ret = {};
	constexpr uint64_t middle = BIGINT_DIGIT_MAX / 2;
	uint64_t const a1 = a % middle;
	uint64_t const a2 = a / middle;
	uint64_t const b1 = b % middle;
	uint64_t const b2 = b / middle;

	//    a2 a1
	//  * b2 b1
	//
	//    p2 p1
	// p4 p3

	uint64_t const p1 = a1 * b1;
	uint64_t const p2 = a2 * b1;
	uint64_t const p3 = a1 * b2;
	uint64_t const p4 = a2 * b2;

	uint64_t carry = ckd_add(&ret.result, p1, (p2 % middle) * middle) ? 1 : 0;
	carry += ckd_add(&ret.result, ret.result, (p3 % middle) * middle) ? 1 : 0;
	ret.carry = (p2 - p2 % middle) + (p3 - p3 % middle) + p4 + carry;
	return ret;
}

void bigint_normalize(bigint *a)
{
	for (size_t i = a->len - 1; i != SIZE_MAX && !a->data[i]; --i) {
		--a->len;
	}
}
static void bigint_lshift(bigint *x, int n)
{
	assert(n >= 0);
	assert(n < 64);
	uint64_t carry = 0;
	for (size_t i = 0; i < x->len; ++i) {
		uint64_t old = x->data[i];
		x->data[i] = (old << n) | carry;
		carry = old >> (64 - n);
	}
	if (carry)
		*uvec_push(x) = carry;
}
static inline uint64_t bigint_nth_binary_digit(bigint x, size_t n)
{
	bool const is_one = x.data[n / 64] & ((uint64_t)1 << (n % 64));
	return is_one;
}
static inline void
bigint_set_nth_binary_digit(bigint *x, size_t n, uint64_t val)
{
	size_t const i = n / 64;
	size_t const d = n % 64;
	if (val) {
		for (size_t j = x->len; j < i + 1; ++j)
			*uvec_push(x) = 0;
		x->data[i] |= (size_t)1 << d;
	} else {
		if (x->len > i) {
			x->data[i] &= ~((size_t)1 << d);
			bigint_normalize(x);
		}
	}
}
void bigint_sum(bigint *dest, bigint a, bigint b)
{
	bigint output = *dest;
	output.len = 0;
	if (a.len > b.len)
		swap(&a, &b);
	bool carry = false;
	size_t i = 0;
	for (; i < a.len; ++i) {
		uint64_t sum = 0;
		carry = ckd_add(&sum, a.data[i], carry ? 1 : 0);
		carry = carry || ckd_add(&sum, sum, b.data[i]);
		*uvec_push(&output) = sum;
	}
	for (; i < b.len; ++i) {
		if (carry) {
			uint64_t sum = 0;
			carry = ckd_add(&sum, b.data[i], 1);
			*uvec_push(&output) = sum;
		} else {
			*uvec_push(&output) = b.data[i];
		}
	}
	if (carry) {
		*uvec_push(&output) = 1;
	}

	*dest = output;
}
static void bigint_diff_inplace(bigint *a, bigint b)
{
	assert(bigint_cmp(*a, b) >= 0);
	uint64_t carry = 0;
	for (size_t i = 0; i < b.len; ++i) {
		uint64_t res;
		carry = ckd_sub(&res, a->data[i], carry);
		carry += ckd_sub(&res, res, b.data[i]);
		a->data[i] = res;
	}
	for (size_t i = b.len; i < a->len; ++i) {
		uint64_t res;
		carry = ckd_sub(&res, a->data[i], carry);
		a->data[i] = res;
	}
	assert(!carry);
	bigint_normalize(a);
}
void bigint_diff(bigint *dest, bigint a, bigint b)
{
	bigint_copy(dest, a);
	bigint_diff_inplace(dest, b);
}
void bigint_add_monome(bigint *a, uint64_t b, size_t power)
{
	if (b == 0)
		return;
	while (a->len <= power)
		*uvec_push(a) = 0;
	uint64_t carry = b;
	size_t i = power;
	while (carry != 0) {
		if (i == a->len)
			*uvec_push(a) = 0;
		carry = ckd_add(&a->data[i], a->data[i], carry) ? 1 : 0;
		i++;
	}
}
void bigint_mult_monome(bigint *dest, bigint a, uint64_t b, size_t power)
{
	bigint output = *dest;
	output.len = 0;
	if (!b)
		return;
	for (size_t i = 0; i < power; ++i)
		*uvec_push(&output) = 0;
	uint64_t carry = 0;
	for (size_t i = power; i < a.len; ++i) {
		struct mult_result const monome_mult = mult(a.data[i], b);
		carry = ckd_add(uvec_push(&output), monome_mult.result, carry) ? 1 : 0;
		carry += monome_mult.carry;
	}
	if (carry)
		*uvec_push(&output) = carry;
	*dest = output;
}

int bigint_cmp(bigint a, bigint b)
{
	if (a.len < b.len)
		return -1;
	if (a.len > b.len)
		return 1;
	for (size_t i = a.len - 1; i != SIZE_MAX; --i) {
		if (a.data[i] < b.data[i])
			return -1;
		else if (a.data[i] > b.data[i])
			return 1;
	}
	return 0;
}
void bigint_div(bigint *quotient, bigint *remainder, bigint a, bigint b)
{
	assert(b.len > 0);
	quotient->len = 0;
	remainder->len = 0;
	size_t const num_bin_digits = a.len * 64;
	for (size_t i = num_bin_digits - 1; i != BIGINT_DIGIT_MAX; --i) {
		bigint_lshift(remainder, 1);
		bigint_set_nth_binary_digit(remainder, 0, bigint_nth_binary_digit(a, i));
		if (bigint_cmp(*remainder, b) >= 0) {
			bigint_diff_inplace(remainder, b);
			bigint_set_nth_binary_digit(quotient, i, 1);
		}
	}
}

void bigint_copy(bigint *dest, bigint source)
{
	dest->len = 0;
	for (size_t i = 0; i < source.len; ++i)
		*uvec_push(dest) = source.data[i];
}
extern inline bigint bigint_create(uint64_t value);
extern inline void bigint_destroy(bigint *x);

#ifdef TESTING
#include "../test.h"
constexpr uint64_t half_num_digits = BIGINT_DIGIT_MAX / 2 + 1;

TEST(adding_zero_to_bigint_does_nothing)
{
	bigint res = {};
	bigint a = bigint_create(1);
	bigint b = {};
	bigint_sum(&res, a, b);
	TEST_ASSERT(res.len == 1);
	TEST_ASSERT(res.data[0] == 1);
	TEST_COMPLETE();
}
TEST(adding_one_digit_bigints_without_carry_gives_one_digit)
{
	bigint res = {};
	bigint a = bigint_create(2);
	bigint b = bigint_create(3);
	bigint_sum(&res, a, b);
	TEST_ASSERT(res.len == 1);
	TEST_ASSERT(res.data[0] == a.data[0] + b.data[0]);
	TEST_COMPLETE();
}
TEST(adding_one_digit_bigints_with_carry_gives_two_digits)
{
	bigint res = {};
	bigint a = bigint_create(BIGINT_DIGIT_MAX);
	bigint b = bigint_create(BIGINT_DIGIT_MAX);
	bigint_sum(&res, a, b);
	TEST_ASSERT(res.len == 2);
	TEST_ASSERT(res.data[0] == BIGINT_DIGIT_MAX - 1);
	TEST_ASSERT(res.data[1] == 1);
	TEST_COMPLETE();
}
TEST(adding_one_to_a_bigint_can_carry_over_multiple_digits)
{
	bigint res = {};
	bigint a = bigint_create(1);
	bigint b = bigint_create(BIGINT_DIGIT_MAX);
	*uvec_push(&b) = BIGINT_DIGIT_MAX;
	*uvec_push(&b) = BIGINT_DIGIT_MAX;
	bigint_sum(&res, a, b);
	TEST_ASSERT(res.len == 4);
	TEST_ASSERT(res.data[0] == 0);
	TEST_ASSERT(res.data[1] == 0);
	TEST_ASSERT(res.data[2] == 0);
	TEST_ASSERT(res.data[3] == 1);
	TEST_COMPLETE();
}
TEST(adding_rank_0_monome_to_a_bigint_0_gives_one_digit)
{
	bigint a = {};
	bigint_add_monome(&a, 1, 0);
	TEST_ASSERT(a.len == 1);
	TEST_ASSERT(a.data[0] == 1);
	TEST_COMPLETE();
}
TEST(adding_rank_1_monome_to_a_bigint_0_gives_two_digit)
{
	bigint a = {};
	bigint_add_monome(&a, 1, 1);
	TEST_ASSERT(a.len == 2);
	TEST_ASSERT(a.data[0] == 0);
	TEST_ASSERT(a.data[1] == 1);
	TEST_COMPLETE();
}
TEST(adding_monome_to_bigint_can_carry_over_multiple_digits)
{
	bigint a = bigint_create(BIGINT_DIGIT_MAX);
	*uvec_push(&a) = BIGINT_DIGIT_MAX;
	*uvec_push(&a) = BIGINT_DIGIT_MAX;
	bigint_add_monome(&a, 1, 0);
	TEST_ASSERT(a.len == 4);
	TEST_ASSERT(a.data[0] == 0);
	TEST_ASSERT(a.data[1] == 0);
	TEST_ASSERT(a.data[2] == 0);
	TEST_ASSERT(a.data[3] == 1);
	TEST_COMPLETE();
}
TEST(multiplying_by_zero_monome_gives_zero)
{
	bigint res = {};
	bigint a = bigint_create(1);
	bigint_mult_monome(&res, a, 0, 0);
	TEST_ASSERT(res.len == 0);
	TEST_COMPLETE();
}
TEST(multiplying_one_digit_bigint_with_monome_1_is_identity)
{
	bigint res = {};
	bigint a = bigint_create(1);
	*uvec_push(&a) = 2;
	*uvec_push(&a) = 3;
	bigint_mult_monome(&res, a, 1, 0);
	TEST_ASSERT(res.len == 3);
	TEST_ASSERT(res.data[0] == 1);
	TEST_ASSERT(res.data[1] == 2);
	TEST_ASSERT(res.data[2] == 3);
	TEST_COMPLETE();
}
TEST(multiplying_two_small_digits_doesnt_carry)
{
	struct mult_result res = mult(2, 3);
	TEST_ASSERT(res.result == 6);
	TEST_ASSERT(res.carry == 0);
	TEST_COMPLETE();
}
TEST(multiplying_half_num_digit_by_two_carries_over)
{
	struct mult_result res = mult(half_num_digits, 2);
	TEST_ASSERT(res.result == 0);
	TEST_ASSERT(res.carry == 1);
	TEST_COMPLETE();
}
TEST(multiplying_bigint_by_monome_can_carry_over_multiple_digits)
{
	bigint res = {};
	bigint a = bigint_create(half_num_digits);
	*uvec_push(&a) = half_num_digits;
	*uvec_push(&a) = half_num_digits;
	bigint_mult_monome(&res, a, 2, 0);
	TEST_ASSERT(res.len == 4);
	TEST_ASSERT(res.data[0] == 0);
	TEST_ASSERT(res.data[1] == 1);
	TEST_ASSERT(res.data[2] == 1);
	TEST_ASSERT(res.data[3] == 1);
	TEST_COMPLETE();
}
TEST(sub_bigint_by_zero_does_nothing)
{
	bigint res = {};
	bigint a = bigint_create(1);
	bigint zero = {};
	bigint_diff(&res, a, zero);
	TEST_ASSERT(res.len == a.len);
	TEST_ASSERT(res.data[0] == a.data[0]);
	TEST_COMPLETE();
}
TEST(sub_bigint_by_itself_returns_zero)
{
	bigint res = {};
	bigint a = bigint_create(1);
	bigint_diff(&res, a, a);
	TEST_ASSERT(res.len == 0);
	TEST_COMPLETE();
}
TEST(sub_bigint_may_carry)
{
	bigint res = {};
	bigint a = {};
	*uvec_push(&a) = 0;
	*uvec_push(&a) = 1;
	bigint b = bigint_create(1);
	bigint_diff(&res, a, b);
	TEST_ASSERT(res.len == 1);
	TEST_ASSERT(res.data[0] == BIGINT_DIGIT_MAX);
	TEST_COMPLETE();
}
TEST(sub_bigint_may_carry_over_multiple_digits)
{
	bigint res = {};
	bigint a = {};
	*uvec_push(&a) = 0;
	*uvec_push(&a) = 0;
	*uvec_push(&a) = 1;
	bigint b = bigint_create(1);
	bigint_diff(&res, a, b);
	TEST_ASSERT(res.len == 2);
	TEST_ASSERT(res.data[0] == BIGINT_DIGIT_MAX);
	TEST_ASSERT(res.data[1] == BIGINT_DIGIT_MAX);
	TEST_COMPLETE();
}
TEST(normalizing_normalized_bigint_does_nothing)
{
	bigint n = {};
	*uvec_push(&n) = 0;
	*uvec_push(&n) = 1;
	bigint_normalize(&n);
	TEST_ASSERT(n.len == 2);
	TEST_COMPLETE();
}
TEST(normalized_bigint_zero_is_empty_vector)
{
	bigint n = bigint_create(0);
	bigint_normalize(&n);
	TEST_ASSERT(n.len == 0);
	TEST_COMPLETE();
}
TEST(bigint_lshift_zero_gives_zero)
{
	bigint n = {};
	bigint_lshift(&n, 1);
	TEST_ASSERT(n.len == 0);
	TEST_COMPLETE();
}
TEST(bigint_div_zero_by_one_is_zero)
{
	bigint a = {};
	bigint b = bigint_create(1);
	bigint quot = {}, rem = {};
	bigint_div(&quot, &rem, a, b);
	TEST_ASSERT(quot.len == 0);
	TEST_ASSERT(rem.len == 0);
	TEST_COMPLETE();
}
TEST(bigint_div_one_by_one_is_one)
{
	bigint a = bigint_create(1);
	bigint b = bigint_create(1);
	bigint quot = {}, rem = {};
	bigint_div(&quot, &rem, a, b);
	TEST_ASSERT(quot.len == 1 && quot.data[0] == 1);
	TEST_ASSERT(rem.len == 0);
	TEST_COMPLETE();
}
TEST(bigint_div_4_by_2_is_2)
{
	bigint a = bigint_create(4);
	bigint b = bigint_create(2);
	bigint quot = {}, rem = {};
	bigint_div(&quot, &rem, a, b);
	TEST_ASSERT(quot.len == 1 && quot.data[0] == 2);
	TEST_ASSERT(rem.len == 0);
	TEST_COMPLETE();
}
TEST(bigint_div_5_by_2_is_2_plus_1)
{
	bigint a = bigint_create(5);
	bigint b = bigint_create(2);
	bigint quot = {}, rem = {};
	bigint_div(&quot, &rem, a, b);
	TEST_ASSERT(quot.len == 1 && quot.data[0] == 2);
	TEST_ASSERT(rem.len == 1 && rem.data[0] == 1);
	TEST_COMPLETE();
}
TEST(bigint_div_onezero_by_two_is_halfdigit)
{
	bigint a = {};
	*uvec_push(&a) = 0;
	*uvec_push(&a) = 1;
	bigint b = bigint_create(2);
	bigint quot = {}, rem = {};
	bigint_div(&quot, &rem, a, b);
	TEST_ASSERT(quot.len == 1 && quot.data[0] == half_num_digits);
	TEST_ASSERT(rem.len == 0);
	TEST_COMPLETE();
}
TEST(bigint_div_2_by_onezero_is_0_plus_2)
{
	bigint a = bigint_create(2);
	bigint b = {};
	*uvec_push(&b) = 0;
	*uvec_push(&b) = 1;
	bigint quot = {}, rem = {};
	bigint_div(&quot, &rem, a, b);
	TEST_ASSERT(quot.len == 0);
	TEST_ASSERT(rem.len == 1 && rem.data[0] == 2);
	TEST_COMPLETE();
}
#endif // TESTING
