#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "stack.h"

int main() {
	pile_t test;
	new_stack(&test);

	for (char i = 'A'; i <= 'Z'; i++) {
		push(&test, i);
	}
	
	while (test.nmb_ellemnts != 0)
	{
		putchar(pop(&test));
	}
	assert(test.nmb_ellemnts == 0);
	
	return 0;
}