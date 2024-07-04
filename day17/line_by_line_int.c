#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

int main() {
	// ##################### FILENAME #################
	const char* file_name = "in2.txt";


	FILE* file = fopen(file_name, "r");
	int line_count = 0;
	size_t linecapp = MAX_LINE;
	char* buff = (char*)malloc(sizeof(char) * linecapp);

	while (!feof(file)) {
		getline(&buff, &linecapp, file);
		line_count++;
	}
	fclose(file);
	printf("%d\n", line_count);

	int* ints = (int*)malloc(line_count * sizeof(char*));
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		int scan_succses = fscanf(file, "%d", &ints[i]);
		printf("%d\n", scan_succses);
		char c = getc(file);
		//if(!(c == '\n' || c == EOF))
		//	printf("%d, %c\n", c, c);
		assert(c == '\n' || c == EOF);
		i++;
	}}
	fclose(file);
	// ########################### PROSSES DATA ###########################



	for (int i = 0; i < line_count; i++) {
		printf("%d\n", ints[i]);
	}
	


	// ########################### PROSSES DATA ###########################

	free(ints);
}
