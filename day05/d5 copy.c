#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

#define WIDTH 3
#define HIGHT 100

int main() {

	char stacks[WIDTH][100];
	int stack_size[WIDTH] = {7, 4, 3,8,8,6,7,8,5};
	int instructions[3][600];
	int icount=0;

	const char* file_name = "sample.txt";
	FILE* file = fopen(file_name, "r");
	int line_count = 10;
	size_t linecapp = MAX_LINE;
	char* buff = (char*)malloc(sizeof(char) * linecapp);

	char* lines[11];
	for (int i = 0; i < line_count; i++) {
		lines[i] = (char*)malloc(sizeof(char) * MAX_LINE);
	}
	
	file = fopen(file_name, "r");

	{int i = 0;
	while (i < line_count && i <11) {
		getline(&lines[i], &linecapp, file);
		i++;
	}
	i= 0;
	while (!feof(file)) {
		int res = 0;
		res = fscanf(file, "move %d from %d to %d\n",  &instructions[0][i],  &instructions[1][i],   &instructions[2][i]);
		icount++;
		assert(res == 3);
		printf("move %d from %d to %d\n", instructions[0][i], instructions[1][i], instructions[2][i]);
		i++;
	}}

	// ########################### PROSSES DATA ###########################



	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < 100; j++)
			stacks[i][j] = ' ';
	
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < 7; j++)
			stacks[i][j] = lines[7-j][4*i+1];

	int count = 0;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < WIDTH; i++) {
			printf("[%c] ", stacks[i][7-j]);
			if(isalnum(stacks[i][7-j]))
				count++;
		}
		printf("\n");	
	}
	
	printf("i %d \n", icount);
	//*
	for (int i = 0; i < icount; i++)
	{
		int mv = instructions[0][i];
		int from = instructions[1][i];
		int to = instructions[2][i];
		//printf("move %d from %d to %d| size %d\n", mv, from, to, stack_size[to]);

		for(int l = 0; l < mv; l++) {
			//pop from,
			stacks[to][stack_size[to]] = stacks[from][stack_size[from]-1];
			stacks[from][stack_size[from] +1] = ' ';
			stack_size[to]++;
			stack_size[from]--;
			//push to
			assert(stack_size[from] >= 0);
		}

		
	}//*/
	
	printf("\n");

	for (int i = 0; i < WIDTH; i++) {
		printf("[%d] ", stack_size[i]);
	}
	printf("\n");



	int count2 =0 ;

	for (int j = 0; j < WIDTH; j++) {
		for (int i = 0; i < WIDTH; i++) {
			printf("[%c] ", stacks[i][8-j]);
			if(isalnum(stacks[i][7-j]))
				count2++;
		}
		printf("\n");	
	}

	printf("%d %d\n", count2, count);

	for (int i = 0; i < 8; i++) {
		printf("[%c] ", stacks[i][stack_size[i]-1]);
	}
	printf("\n");

	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	//free(lines);
}
