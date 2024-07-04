#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

int count_lines(const char* file_name);

int main() {
	// ##################### FILENAME #################
	const char* file_name = "in2.txt";
	FILE* file = fopen(file_name, "r");

	// ########################### PROSSES DATA ###########################

	while (!feof(file)) {
		/* Do something */
	}


	// ########################### PROSSES DATA ###########################
	fclose(file);

}

int count_lines(const char* file_name) {
	FILE* file = fopen(file_name, "r");
	int res = 1;

	while (!feof(file)) {
		if ('\n' == getc(file))
			res++;
	}

	fclose(file);

	return res;
}
