#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char name_t[4];

typedef struct {
	bool knowns_his_num;
	int64_t number;
	char operation;
	int operands[2];
} monkey_t;

int count_lines(const char* file_name);

int name_to_num(name_t* num_to_name, name_t name, int num_names) {
	int res = 0;

	while (0 != strncmp(num_to_name[res], name, 4)){
		res++;
		assert(res < num_names);
	}
	return res;
}

int main() {
	// ##################### FILENAME #################
	const char* file_name = "in.txt";
	int monkey_count = count_lines(file_name);
	
	name_t* num_to_name = calloc(monkey_count, sizeof(name_t));
	monkey_t* monkeys = calloc(monkey_count, sizeof(monkey_t));

	//printf("si:%d\n",monkey_count );

	// ########################### Read DATA ###########################
	{
		FILE* file = fopen(file_name, "r");
		int i = 0;
		while (!feof(file)) {
			int res = fscanf(file, "%c%c%c%c: ", &num_to_name[i][0], &num_to_name[i][1], &num_to_name[i][2], &num_to_name[i][3]);
			assert(4 == res);
			i++;
			char c = getc(file);
			while ('\n' != c && c != EOF)
				c = getc(file);
			

		}
		fclose(file);
	}


	{
		FILE* file = fopen(file_name, "r");
		int i = 0;
		while (!feof(file)) {
			char throw_away[4];
			int res = fscanf(file, "%c%c%c%c: ", &throw_away[0], &throw_away[1], &throw_away[2], &throw_away[3]);
			assert(4 == res);


			char c = getc(file);
			if (isdigit(c)) { /* monkey doesnt have an opperation */
				ungetc(c, file);
				fscanf(file, "%lld\n", &monkeys[i].number);
				monkeys[i].knowns_his_num = true;
			} else {
				ungetc(c, file);
				assert(islower(c));
				monkeys[i].knowns_his_num = false;
				name_t operand1, operand2;

				int res = fscanf(file, "%c%c%c%c ", &operand1[0], &operand1[1], &operand1[2], &operand1[3]);
				assert(res == 4);
				res = fscanf(file, "%c ", &monkeys[i].operation);
				assert(res == 1);
				res = fscanf(file, "%c%c%c%c\n", &operand2[0], &operand2[1], &operand2[2], &operand2[3]);
				assert(res == 4);

				monkeys[i].operands[0] = name_to_num(num_to_name, operand1, monkey_count);
				monkeys[i].operands[1] = name_to_num(num_to_name, operand2, monkey_count);
			}
			
			i++;
		}
		fclose(file);
	}
	const int root = name_to_num(num_to_name, "root", monkey_count);
	const int human = name_to_num(num_to_name, "humn", monkey_count);

	// ########################### PROSSES DATA ###########################

	// for (int i = 0; i < monkey_count; i++) {
	// 	if(monkeys[i].knowns_his_num)
	// 		//printf("monkey #%d: %lld\n", i, monkeys[i].number);
	// 		printf("%.4s: %lld\n", num_to_name[i], monkeys[i].number);
	// 	else
	// 		//printf("monkey #%d: #%d %c #%d\n", i, monkeys[i].operands[0], monkeys[i].operation ,monkeys[i].operands[1]);
	// 		printf("%.4s: %.4s %c %.4s\n", num_to_name[i], num_to_name[monkeys[i].operands[0]], monkeys[i].operation , num_to_name[monkeys[i].operands[1]]);
	// }

	while (!monkeys[root].knowns_his_num) {
		for (int i = 0; i < monkey_count; i++) {
			if(!monkeys[i].knowns_his_num) {
				const int* ops = monkeys[i].operands;
				if(monkeys[ops[0]].knowns_his_num && monkeys[ops[1]].knowns_his_num) {
					//printf("%.4s preforms %.4s %c %.4s. ", num_to_name[i], num_to_name[ops[0]], monkeys[i].operation, num_to_name[ops[1]]);
					//printf("%.4s = %lld %c %lld = ", num_to_name[i], monkeys[ops[0]].number, monkeys[i].operation, monkeys[ops[1]].number);

					switch (monkeys[i].operation)
					{
					case '+':
						//printf("");
						assert(llabs(monkeys[ops[0]].number) < 4611686018427387904LL);
						assert(llabs(monkeys[ops[1]].number) < 4611686018427387904LL);
						monkeys[i].number = monkeys[ops[0]].number + monkeys[ops[1]].number;
						break;
					case '-':
						assert(llabs(monkeys[ops[0]].number) < 4611686018427387904LL);
						assert(llabs(monkeys[ops[1]].number) < 4611686018427387904LL);
						monkeys[i].number = monkeys[ops[0]].number - monkeys[ops[1]].number;
						break;
					case '*':
						//assert(llabs(monkeys[ops[0]].number) < 2147483648LL);
						//assert(llabs(monkeys[ops[1]].number) < 2147483648LL);
						monkeys[i].number = monkeys[ops[0]].number * monkeys[ops[1]].number;
						break;
					case '/':
						assert(0 != monkeys[ops[1]].number);
						monkeys[i].number = monkeys[ops[0]].number / monkeys[ops[1]].number;
						break;
					
					default:
						assert(false);
						break;
					}
					monkeys[i].knowns_his_num = true;
					//printf("%lld\n", monkeys[i].number);
				}
			}
		}
		
	}
	printf("--------\n");

	for (int i = 0; i < monkey_count; i++) {
		if(monkeys[i].knowns_his_num)
			//printf("monkey #%d: %d\n", i, monkeys[i].number);
			printf("#%d %.4s: %lld\n", i, num_to_name[i], monkeys[i].number);
		else
			//printf("monkey #%d: #%d %c #%d\n", i, monkeys[i].operands[0], monkeys[i].operation ,monkeys[i].operands[1]);
			printf("#%d %.4s: %.4s %c %.4s\n", i, num_to_name[i], num_to_name[monkeys[i].operands[0]], monkeys[i].operation, num_to_name[monkeys[i].operands[1]]);
	}

	printf("root's number: %lld\n", monkeys[root].number);


	// ### cleanup
	free(num_to_name);
	free(monkeys);
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
