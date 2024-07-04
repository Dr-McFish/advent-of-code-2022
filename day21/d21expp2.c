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
	uint64_t number;
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

bool addition_overflow(uint64_t a, uint64_t b);
bool substraction_overflow(uint64_t a, uint64_t b);
bool multiplication_overflow(uint64_t a, uint64_t b);

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

	monkeys[root].operation = '=';
	monkeys[human].operation = 'h';
	monkeys[human].knowns_his_num = false;

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
		bool has_done_stuff = false;
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
						assert((monkeys[ops[0]].number) < 4611686018427387904LL);
						assert((monkeys[ops[1]].number) < 4611686018427387904LL);
						monkeys[i].number = monkeys[ops[0]].number + monkeys[ops[1]].number;
						has_done_stuff = true;
						break;
					case '-':
						assert((monkeys[ops[0]].number) < 4611686018427387904LL);
						assert((monkeys[ops[1]].number) < 4611686018427387904LL);
						monkeys[i].number = monkeys[ops[0]].number - monkeys[ops[1]].number;
						has_done_stuff = true;
						break;
					case '*':
						//assert((monkeys[ops[0]].number) < 2147483648LL);
						//assert((monkeys[ops[1]].number) < 2147483648LL);
						monkeys[i].number = monkeys[ops[0]].number * monkeys[ops[1]].number;
						has_done_stuff = true;
						break;
					case '/':
						assert(0 != monkeys[ops[1]].number);
						monkeys[i].number = monkeys[ops[0]].number / monkeys[ops[1]].number;
						has_done_stuff = true;
						break;
					
					case 'h':
					case '=':
						break;
					
					default:
						assert(false);
						break;
					}
					if(monkeys[i].operation != 'h' && monkeys[i].operation != '=')
						monkeys[i].knowns_his_num = true;
					//printf("%lld\n", monkeys[i].number);
				}
			}
		}
		if(!has_done_stuff)
		break;
		
	}
	printf("--------\n");

	for (int i = 0; i < monkey_count; i++) {
		if(!monkeys[i].knowns_his_num) {
			if(monkeys[i].knowns_his_num)
				//printf("monkey #%d: %d\n", i, monkeys[i].number);
				printf("#%d %.4s: %lld\n", i, num_to_name[i], monkeys[i].number);
			else
				//printf("monkey #%d: #%d %c #%d\n", i, monkeys[i].operands[0], monkeys[i].operation ,monkeys[i].operands[1]);
				printf("#%d %.4s: %.4s %c %.4s\n", i, num_to_name[i], num_to_name[monkeys[i].operands[0]], monkeys[i].operation, num_to_name[monkeys[i].operands[1]]);
			if(i != human)
				/* The puzzle didn't tell me this!!! */
				assert(monkeys[monkeys[i].operands[0]].knowns_his_num || monkeys[monkeys[i].operands[1]].knowns_his_num);
		}
	}

	int dependent_monkey = monkeys[monkeys[root].operands[0]].knowns_his_num ?
						   monkeys[root].operands[1]: monkeys[root].operands[0];

	uint64_t desired_result = monkeys[monkeys[root].operands[0]].knowns_his_num 
							? monkeys[monkeys[root].operands[0]].number 
							: monkeys[monkeys[root].operands[1]].number;

	int i = 0;
	while (dependent_monkey != human) {
		i++;
		char operation = monkeys[dependent_monkey].operation;
		
		uint64_t known_operand;
		int unkown_opperand_monkey;
		if(monkeys[monkeys[dependent_monkey].operands[0]].knowns_his_num) {
			known_operand =  monkeys[monkeys[dependent_monkey].operands[0]].number;
			unkown_opperand_monkey = monkeys[dependent_monkey].operands[1];
		} else {
			known_operand =  monkeys[monkeys[dependent_monkey].operands[1]].number;
			unkown_opperand_monkey = monkeys[dependent_monkey].operands[0];
		}

		dependent_monkey = unkown_opperand_monkey;

		/* preform the insverse operation */
		switch (operation)
		{
		case '+':
			//assert(!substraction_overflow(desired_result, known_operand));
			desired_result -= known_operand;
			break;
		case '-':
			//assert(!addition_overflow(desired_result, known_operand));
			desired_result += known_operand;
			break;
		case '*':
			assert(known_operand != 0);
			int des_res2 = desired_result / known_operand;
			if(des_res2 * known_operand != desired_result)
				printf("%d\n", i);
			assert(des_res2 * known_operand == desired_result);

			desired_result = des_res2;
			break;
		case '/':
			//assert(!multiplication_overflow(desired_result, known_operand));
			desired_result *= known_operand;
			break;

		default:
			assert(false);
			break;
		}
	}
	
	printf("humn yells: %lld", desired_result);
	// ### cleanup ###
	free(num_to_name);
	free(monkeys);
}

bool addition_overflow(uint64_t a, uint64_t b) {
	a = a >> 32;
	b = b >> 32;
	return (a + b) > 4294967000LL || (a + b) < -4294967000LL;
}
bool substraction_overflow(uint64_t a, uint64_t b) {
	a = a >> 32;
	b = b >> 32;
	return (a - b) > 4294967000LL || (a - b) < -4294967000LL;
}
bool multiplication_overflow(uint64_t a, uint64_t b) {
	a = a >> 33;
	b = b >> 33;

	return (a * b) > 4611686014132420000LL || (a * b) < -4611686014132420000LL;
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
