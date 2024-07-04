#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct {
	enum { ADD,
		   MULTIPLY,
		   SQUARE } op_type;
	int64_t amount;
} operation_t;
int64_t apply_operation(operation_t opp, int64_t val);

struct monkey {
	queue_t* items;
	operation_t operation;
	int64_t test_divisible_by;
	int if_true_thow_to;
	int if_false_thow_to;
	int items_inspected;
};
typedef struct monkey monkey_t;
/* has no items */
monkey_t* create_mokey(operation_t operation, int div_test, int true_throw_to, int false_throw_to);
void kill_mokey(monkey_t* monkey);
void monkey_catch(monkey_t* monkey, int64_t item);
int64_t monkey_grab(monkey_t* monkey);                       // assumes the monkey  has stuff
void monkey_throw_to(monkey_t* thrower, monkey_t* catcher);  // assumes the thrower has stuff
bool monkey_has_stuff(monkey_t* monkey);
void monkey_do_turn(monkey_t* monkeys[], int monkey_idx, int64_t monkey_mod);
monkey_t* text_to_monkey(FILE* fd);

int max(int a, int b) { return a > b ? a : b; }

int main() {
	// ############################## INPUT ##############################
	const char* file_name = "in.txt";

	// <line_count>
	FILE* file = fopen(file_name, "r");
	int line_count = 1;

	while (!feof(file)) {
		if ('\n' == getc(file))
			line_count++;
	}
	fclose(file);
	printf("line count: %d\n", line_count);
	// </lincount>

	int monkey_count = (line_count + 1) / 7;
	monkey_t** monkeys = calloc(monkey_count, sizeof(monkey_t*));

	file = fopen(file_name, "r");
	for (int i = 0; i < monkey_count; i++) {
		monkeys[i] = text_to_monkey(file);
		if (i != (monkey_count - 1))
			assert('\n' == getc(file));
	}
	fclose(file);

	int monkey_mod = 1;
	for (int i = 0; i < monkey_count; i++)
		monkey_mod *= monkeys[i]->test_divisible_by;

		// ########################### PROSSES DATA ##########################

#define NUM_ROUNDS 10000
	printf("Before round %d, the monkeys are holding items with these worry levels:\n", 1);
	for (int i = 0; i < monkey_count; i++) {
		monkey_t* monkey = monkeys[i];
		printf("Monkey %d: ", i);
		queue_print(monkey->items);
		// printf("\n");
	}
	for (int round = 1; round <= NUM_ROUNDS; round++) {
		for (int i = 0; i < monkey_count; i++) {
			monkey_do_turn(monkeys, i, monkey_mod);
		}

		if (round == 1 || round == 20 || 0 == round % 1000) {
			printf("== After round %d ==\n", round);
			for (int i = 0; i < monkey_count; i++) {
				monkey_t* monkey = monkeys[i];
				printf("Monkey %d inspected items %d times.\n", i, monkey->items_inspected);
			}
			printf("\n");
		}
	}

	int most_items_inspected = 0;
	for (int i = 0; i < monkey_count; i++) {
		most_items_inspected = max(most_items_inspected, monkeys[i]->items_inspected);
		printf("%d, ", monkeys[i]->items_inspected);
	}
	int scnd_most_items_inspected = 0;
	for (int i = 0; i < monkey_count; i++) {
		int itms_inspected = monkeys[i]->items_inspected;

		if (itms_inspected != most_items_inspected) {
			scnd_most_items_inspected = max(scnd_most_items_inspected, itms_inspected);
		}
	}

	printf("Monkey buissnes: %lld\n %d, %d\n",
	       ((int64_t)most_items_inspected)* ((int64_t)scnd_most_items_inspected),
	       most_items_inspected, scnd_most_items_inspected);

	// ############################# CLEANUP #############################
	// genocide
	for (int i = 0; i < monkey_count; i++) {
		kill_mokey(monkeys[i]);
	}
	free(monkeys);
}

// ******************************** Functions ********************************

#define MAX_64_SQRT (4294967296 - 1000)

int64_t apply_operation(operation_t opp, int64_t val) {
	int64_t ret;

	assert(val < MAX_64_SQRT);
	switch (opp.op_type) {
		case ADD:
			ret = val + opp.amount;
			break;
		case MULTIPLY:
			ret = val * opp.amount;
			break;
		case SQUARE:
			ret = val * val;
			break;
	}

	return ret;
}

// ****************************** Mokey Functions ******************************

#define QUEUE_CAP 100

monkey_t* create_mokey(operation_t operation, int div_test, int true_throw_to, int false_throw_to) {
	monkey_t* monkey = (monkey_t*)malloc(sizeof(monkey_t));
	assert(monkey != NULL);
	monkey->items = queue_create(QUEUE_CAP);

	monkey->operation = operation;
	monkey->test_divisible_by = div_test;
	monkey->if_true_thow_to = true_throw_to;
	monkey->if_false_thow_to = false_throw_to;
	monkey->items_inspected = 0;

	return monkey;
}

monkey_t* sythesize_mokey(queue_t* items, operation_t operation, int div_test, int true_throw_to, int false_throw_to) {
	monkey_t* monkey = (monkey_t*)malloc(sizeof(monkey_t));
	assert(monkey != NULL);

	monkey->items = items;
	monkey->operation = operation;
	monkey->test_divisible_by = div_test;
	monkey->if_true_thow_to = true_throw_to;
	monkey->if_false_thow_to = false_throw_to;
	monkey->items_inspected = 0;

	return monkey;
}

void kill_mokey(monkey_t* monkey) {
	queue_free(monkey->items);
	free(monkey);
}

void monkey_catch(monkey_t* monkey, int64_t item) {
	enqueue(monkey->items, item);
}

int64_t monkey_grab(monkey_t* monkey) {
	return dequeue(monkey->items);
}

void monkey_throw_to(monkey_t* thrower, monkey_t* catcher) {
	monkey_catch(catcher, monkey_grab(thrower));
}

bool monkey_has_stuff(monkey_t* monkey) {
	return !queue_is_empty(monkey->items);
}

#define MONKEY_MOD = 9699690
/* = 7*11*13*3*17*2*5*19 */
/* product of all divisibility tests */

void monkey_do_turn(monkey_t* monkeys[], int monkey_idx, int64_t monkey_mod) {
	monkey_t* monkey = monkeys[monkey_idx];
	// printf("Monkey %d:\n", monkey_idx);

	while (monkey_has_stuff(monkey)) {
		int64_t held_item = monkey_grab(monkey);

		/* inspect */
		// printf("  Monkey inspects an item with a worry level of %lld.\n", held_item);
		held_item = apply_operation(monkey->operation, held_item);
		// printf("    Worry level is %s",
		//        monkey->operation.op_type == ADD ? "inceased" : "multiplied");
		// if (monkey->operation.op_type == SQUARE)
		//	printf(" by itself");
		// else
		//	printf(" by %lld", monkey->operation.amount);
		// printf(" to %lld.\n", held_item);
		monkey->items_inspected++;

		/* wasn't dammaged relief */
		held_item %= monkey_mod;

		// printf("    Monkey gets bored with item. Worry level is divided by 3 to %lld.\n", held_item);

		/* throw */
		bool test = 0 == held_item % monkey->test_divisible_by;
		// printf("    Current worry level is%s divisible by %lld.\n",
		//        test ? "" : " not",
		//        monkey->test_divisible_by);
		if (test) {
			//	printf("    Item with worry level %lld is thrown to monkey %d.\n", held_item, monkey->if_true_thow_to);
			monkey_catch(monkeys[monkey->if_true_thow_to], held_item);
		} else {
			//	printf("    Item with worry level %lld is thrown to monkey %d.\n", held_item, monkey->if_false_thow_to);
			monkey_catch(monkeys[monkey->if_false_thow_to], held_item);
		}
	}
	// printf("\n");
}

monkey_t* text_to_monkey(FILE* fd) {
	queue_t* items = queue_create(30);
	operation_t operation;
	int64_t test_divisible_by;
	int if_true_thow_to;
	int if_false_thow_to;
	int monkey_num;

	int res;

	res = fscanf(fd, "Monkey %d:\n", &monkey_num);
	assert(1 == res);
	{
		int item;
		char comma;
		res = fscanf(fd, "  Starting items: %d%c", &item, &comma);
		assert(2 == res);
		printf("%d\n", monkey_num);
		enqueue(items, item);

		while (comma == ',') {
			assert(' ' == getc(fd));
			fscanf(fd, "%d%c", &item, &comma);
			enqueue(items, item);
		}
		assert(comma == '\n');
	}
	{
		char op, tp;
		int amount;
		fscanf(fd, "  Operation: new = old %c ", &op);
		tp = getc(fd);
		if (isdigit(tp)) {
			ungetc(tp, fd);
			res = fscanf(fd, "%d\n", &amount);
			assert(1 == res);
			operation.amount = amount;
			operation.op_type = (op == '+') ? ADD : MULTIPLY;
			printf((op == '+') ? "ADD" : "MULTIPLY");
			assert((op == '+') || (op == '*'));
		} else {
			ungetc(tp, fd);
			res = fscanf(fd, "old\n");
			assert(0 == res);
			assert('*' == op);
			operation.op_type = SQUARE;
			operation.amount = 0;
			printf("SQUARE");
		}
	}

	fscanf(fd, "  Test: divisible by %lld\n", &test_divisible_by);
	fscanf(fd, "    If true: throw to monkey %d\n", &if_true_thow_to);
	fscanf(fd, "    If false: throw to monkey %d", &if_false_thow_to);
	getc(fd);  // '\n'

	assert(!queue_is_empty(items));
	return sythesize_mokey(items, operation, test_divisible_by,
	                       if_true_thow_to, if_false_thow_to);
}
