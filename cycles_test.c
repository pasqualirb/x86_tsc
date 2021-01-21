// 2019-06-19
//
// Usage: cmd [<loop_iterations>]

#include <stdint.h> // uint64_t
#include <stdio.h>  // printf()
#include <stdlib.h> // malloc()
#include <string.h> // memset()

#include <unistd.h> // usleep()

#include "get_cycles.h"

int
main(int argc, char **argv)
{
	int total;
	uint64_t *array;

	int i;

	total = argc > 1 ? atoi(argv[1]) : 1;

	array = malloc(total * sizeof(*array));
	if (!array)
		return 1;
	memset(array, 0, total * sizeof(*array));

	for (i = 0; i < total; i++) {
		uint64_t start, end;

		start = get_cycles_start();
		usleep(1000000);
		end   = get_cycles_end();

		array[i] = end - start;
	}

	for (i = 0; i < total; i++)
		printf("%lld, %lld\n", i, array[i]);

	return 0;
}
