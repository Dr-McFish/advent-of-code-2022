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
	int width = 0;
	while (lines[0][width] != '\n')
		width++;	
	


	// ########################### PROSSES DATA ###########################
	int my_x;
	int my_y;
	int resiever_x;
	int resiever_y;

	for (int y = 0; y < line_count; y++) {
		for (int x = 0; x < width; x++) {
			if(lines[y][x] == 'S') {
				my_x = x;
				my_y = y;
				lines[y][x] = 'a';
			} else if(lines[y][x] == 'E') {
				resiever_x = x;
				resiever_y = y;
				lines[y][x] = 'z';
			}
		}
	}

	for (int i = 0; i < line_count; i++) {
		printf("%s", lines[i]);
	}
	


	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	free(lines);
}
