#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

int scenic_score(int x_start, int y_start, char** trees, int width, int hight) {
	int trees_visible_north = (y_start == 0) ? 0 : 1;
	int trees_visible_south = (y_start == hight-1) ? 0 : 1;
	int trees_visible_east  = (x_start == 0) ? 0 : 1;
	int trees_visible_west  = (x_start == width-1) ? 0 : 1;

	trees_visible_north = 0;
	trees_visible_south = 0;
	trees_visible_east  = 0;
	trees_visible_west  = 0;
	const int our_tree = trees[y_start][x_start];

	//west
	for (int x = x_start + 1; x < width; x++) {
		trees_visible_west++;
		if(trees[y_start][x] < our_tree)
		{}
		else
			break;
	}

	//east
	for (int x = x_start - 1; x >= 0; x--) {
		trees_visible_east++;
		if(trees[y_start][x] < our_tree)
		{}
		else
			break;
	}

	//South
	for (int y = y_start + 1; y < hight; y++) {
		trees_visible_south++;
		if(trees[y][x_start] < our_tree) 
		{}
		else
			break;
	}

	//North
	for (int y = y_start - 1; y >= 0; y--) {
		trees_visible_north++;
		if(trees[y][x_start] < our_tree)
		{}
		else
			break;
	}
	printf("e:%d | w:%d | n:%d | s:%d |",trees_visible_east , trees_visible_west , trees_visible_north , trees_visible_south);

	return trees_visible_east * trees_visible_west * trees_visible_north * trees_visible_south;	
}

int max(int a, int b) {
	return a > b ? a : b;
}

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

	// ########################### PROSSES DATA ###########################
	int patch_width = strnlen(lines[0], MAX_LINE) -1;

	for (int i = 0; i < line_count; i++)
	{
		for (int j = 0; j < patch_width; j++)
		{
			lines[i][j] -= '0';
			//printf("%d", lines[i][j]);
		}
		//printf("\n");
	}
	//printf("%d", '\n');

	int** is_visible = (int**)malloc(line_count* sizeof(int*));
	for (int i = 0; i < line_count; i++) {
		is_visible[i] = (int*)calloc(patch_width, sizeof(int));
	}

	for (int i = 0; i < line_count; i++) {
		signed char max_visible = -1;
		for (int j = 0; j < patch_width; j++) {
			if(max_visible < lines[i][j]) {
				max_visible = lines[i][j];
				is_visible[i][j] = 1;
			}
		}
	}
	
	for (int i = 0; i < line_count; i++) {
		signed int max_visible = -1;
		for (int j = patch_width-1; j >= 0; j--) {
			if(max_visible < lines[i][j]) {
				max_visible = lines[i][j];
				is_visible[i][j] = 1;
			}
		}
	}
	//* /

	for (int j = 0; j < patch_width; j++) {
		signed char max_visible = -1;
		for (int i = 0; i < line_count; i++) {
			if(max_visible < lines[i][j]) {
				max_visible = lines[i][j];
				is_visible[i][j] = 1;
				if(j == 1)
					printf("A: %d\n", max_visible);
			}
		}
	}
	
	//*
	for (int j = 0; j < patch_width; j++) {	
		signed int max_visible = -1;
		for (int i = line_count-1; i >= 0; i--) {
			if(max_visible < lines[i][j]) {
				max_visible = lines[i][j];
				is_visible[i][j] = 1;
			}
		}
	} //*/

	int visible_count = 0;
	for (int i = 0; i < line_count; i++) {
		for (int j = 0; j < patch_width; j++) {
			visible_count += is_visible[i][j];
			printf("%d", lines[i][j]);
		}
		printf("\n");
	}

	printf("hallo: %d\n", visible_count);

	//*
	int max_scenic_score = 0;
	for (int x = 0; x < patch_width; x++) {
		for (int y = 0; y < line_count; y++) {
			max_scenic_score = max(max_scenic_score, scenic_score(x,y, lines, patch_width, line_count));
		}
	}
	printf("max scene score: %d\n", max_scenic_score);
	//*/
	printf("scene score: %d\n", scenic_score(2,3, lines, patch_width, line_count));

	

	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	free(lines);
}