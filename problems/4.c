#include "problems.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static size_t int_to_string(int64_t n, char buffer[static 7])
{
	assert(n > 0);
	assert(n < 999999);
	int64_t const input = n;
	size_t len = 0;
	for (int64_t i = 1; i <= n; i *= 10) {
		len++;
	}
	buffer[len] = '\0';
	for (size_t i = len; i > 0; i--) {
		buffer[i - 1] = '0' + n % 10;
		n /= 10;
	}
	assert(len < 7);
	assert(atoll(buffer) == input);
	return len;
}
static bool is_palindrome(int64_t n)
{
	assert(n >= 100 * 100);
	assert(n <= 999 * 999);
	char buffer[7];
	size_t const len = int_to_string(n, buffer);

	bool palindrome = true;
	for (size_t i = 0; i < len; ++i) {
		palindrome = palindrome && buffer[i] == buffer[len - 1 - i];
	}

	return palindrome;
}

static bool is_product_of_two_three_digits_numbers(int64_t n)
{
	for (int64_t i = 100; i <= 999; ++i) {
		if (n % i == 0 && 100 <= (n / i) && (n / i) <= 999)
			return true;
	}
	return false;
}

int64_t problem4()
{
	for (int64_t n = 999 * 999; n >= 100 * 100; --n) {
		if (is_palindrome(n) && is_product_of_two_three_digits_numbers(n))
			return n;
	}
	return 0;
}
