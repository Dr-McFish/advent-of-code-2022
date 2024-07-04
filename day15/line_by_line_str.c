#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

int main() {
	const char* file_name = "in.txt";
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

	char** lines = (char**)malloc(line_count * sizeof(char*));
	for (int i = 0; i < line_count; i++) {
		lines[i] = (char*)malloc(sizeof(char) * MAX_LINE);
	}
	
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		getline(&lines[i], &linecapp, file);
		i++;
	}}
	fclose(file);

	// ########################### PROSSES DATA ###########################


	for (int i = 0; i < line_count; i++)
	{
		printf("%s", lines[i]);
	}
	


	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	free(lines);
}
