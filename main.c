#include "problems/list.h"
#include <stdio.h>
#include <stdlib.h>

void usage(char const program[static 1])
{
	fprintf(stderr, "%s [problem number]\n", program);
}

int main(int argc, char const *argv[argc + 1])
{
	size_t problem = num_problems - 1;
	if (argc == 2) {
		auto parsed = (size_t)atoll(argv[1]);
		if (parsed > 0 && parsed <= num_problems)
			problem = parsed - 1;
		else {
			usage(argv[0]);
			return 1;
		}
	} else if (argc > 1) {
		usage(argv[0]);
		return 1;
	}
	int64_t result = problems[problem]();
	printf("%lld\n", result);

	return 0;
}
