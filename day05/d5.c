#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "stack.h"

#define MAX_LINE 5000

int main() {

	pile_t stacks[9];
	for (int i = 0; i < 9; i++)
		new_stack(&stacks[i]);
	
	//int stack_size[9] = {2,3,1,0,0,0,0,0,0};
	
	//int stack_size[9] = {7, 4, 3,8,8,6,7,8,5};
	
	int instructions[3][600];
	int icount=0;

	const char* file_name = "in.txt";
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
		//printf("move %d from %d to %d\n", instructions[0][i], instructions[1][i], instructions[2][i]);
		i++;
	}}

	// ########################### PROSSES DATA ###########################

	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			char c = lines[7-j][4*i+1];
			if(isupper(c))
				push(&stacks[i], c);
		}
	}

	printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");	
	int count = 0;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 9; i++) {
			printf("[%c] ", stacks[i].donnes[7-j]);
			if(isalnum(stacks[i].donnes[7-j]))
				count++;
		}
		printf("\n");	
	}
	for (int i = 0; i < 9; i++) {
		printf("[%d] ", stacks[i].nmb_ellemnts);
	}
	
	printf("i %d \n", icount);

	//*
	pile_t rev_pile;
	new_stack(&rev_pile);
	for (int i = 0; i < icount; i++)
	{
		int mv = instructions[0][i];
		int from = instructions[1][i]-1;
		int to = instructions[2][i]-1;
		printf("move %d from %d to %d\n", mv, from, to);

		for(int l = 0; l < mv; l++) {
			//pop from,
			push(&rev_pile, pop(&stacks[from]));
		}
		for(int l = 0; l < mv; l++) {
			//pop from,
			push(&stacks[to], pop(&rev_pile));
		}
		assert(rev_pile.nmb_ellemnts == 0);
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) {
				printf("[%c] ", stacks[i].donnes[8-j]);
			}
			printf("\n");	
		}
		for (int i = 0; i < 9; i++) {
			printf("[%d] ", stacks[i].nmb_ellemnts);
		}
		printf("\n");	
	}//*/
	
	printf("\n");

	for (int i = 0; i < 9; i++) {
		printf("[%d] ", stacks[i].nmb_ellemnts);
	}
	printf("\n");


	/*
	int count2 =0;

	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			printf("[%c] ", stacks[i].donnes[8-j]);
			if(isalnum(stacks[i].donnes[7-j]))
				count2++;
		}
		printf("\n");	
	}

	printf("%d %d\n", count2, count);
	*/
	for (int i = 0; i < 9; i++) {
		//printf("AAA\n");

		printf("%c", stacks[i].donnes[stacks[i].nmb_ellemnts-1]);
	}
	printf("\n");

	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	//free(lines);
}
