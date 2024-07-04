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
	int instruction_count = 0;
	size_t linecapp = MAX_LINE;
	char* buff = (char*)malloc(sizeof(char) * linecapp);

	while (!feof(file)) {
		getline(&buff, &linecapp, file);
		instruction_count++;
	}
	fclose(file);
	printf("%d\n", instruction_count);

	char** instructions = (char**)malloc(instruction_count * sizeof(char*));
	for (int i = 0; i < instruction_count; i++) {
		instructions[i] = (char*)malloc(sizeof(char) * MAX_LINE);
	}
	
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		getline(&instructions[i], &linecapp, file);
		i++;
	}}

	// ########################### PROSSES DATA ###########################
	int signal_strength_sum = 0;

	int crt_line = 0;

	register int x_reg = 1;
	bool is_adding = false;
	int add_value = 0;
	int current_instr = 0;
	for (int cycles_count = 1; current_instr < instruction_count; cycles_count++) {
		//printf("cstart - %d \t%d\n", cycles_count, x_reg);
		if(cycles_count % 40 == 20) {
			int signal_strength = cycles_count * x_reg;
			signal_strength_sum += signal_strength;
			//printf("\n");
			//printf("%d \t%d\t%d\n", signal_strength, cycles_count, x_reg);

		}
		char px = '.';
		int hpos = (cycles_count-1) % 40;
		if (abs(x_reg - hpos) <=1) {
			px = '#';
		}
		printf("%c", px );
		if(cycles_count % 40 == 0) {
			printf("\n");
		}

		if(is_adding) {
			is_adding = false;
			x_reg += add_value;
			current_instr++;
		} else if(strncmp(instructions[current_instr], "noop", 4) == 0) {
			current_instr++;
		} else if(strncmp(instructions[current_instr], "addx", 4) == 0) {
			is_adding = true;
			add_value = atoi(&instructions[current_instr][5]);
		} else {
			assert(-1==0);
		}

		//printf("cENd   - %d \t%d\n\n", cycles_count, x_reg);

		crt_line = (crt_line + 1) %6;
	}
	
	printf("signal strength: %d\n", signal_strength_sum);


	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < instruction_count; i++) {
		free(instructions[i]);
	}
	free(instructions);
}
