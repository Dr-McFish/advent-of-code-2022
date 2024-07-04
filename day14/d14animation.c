#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void delay(int number_of_miliseconds);

typedef struct {
	int x;
	int y;
} cords_t;
cords_t cord_max(cords_t a, cords_t b);
cords_t cord_min(cords_t a, cords_t b);

#define CAVE_LENGTH 1000
#define ROCK '#'
#define SAND 'o'
#define AIR '.'

/// @brief adds rock to cave in a straight line from cords begin to end
/// @param cave table of chars to add rock to
/// @param begin start of the rock line
/// @param end end of the rock line
void fill_rock(char cave[CAVE_LENGTH][CAVE_LENGTH], cords_t begin, cords_t end);

/// @brief adds and falls sand into cave from the top
/// @param cave table of chars to add sand to
/// @return true - if the sand reaches equilibrium
///			| false - if sand falls into the void OR the sand sourse becomes blocked
bool add_sand(char cave[CAVE_LENGTH][CAVE_LENGTH]);

void add_path(char cave[CAVE_LENGTH][CAVE_LENGTH]);

void print_cave(char cave[CAVE_LENGTH][CAVE_LENGTH],
                int start_x, int end_x, int start_y, int end_y);

// arg1: filename
int main(int argc, char* argv[]) {
	// ##################### FILENAME #################
	const char* file_name = (argc > 1) ? argv[1] : "in.txt";
	FILE* file = fopen(file_name, "r");

	// ##################### ARGV #####################
	const enum { PART_1,
		         PART_2 } part = (argc > 2 && 0 == strncmp(argv[2], "-p1", 3)) ? PART_1 : PART_2;
	if (argc > 2) {
		assert(0 == strncmp(argv[2], "-p1", 3) || 0 == strncmp(argv[2], "-p2", 3));
	}

	// ############################ READ DATA #############################
	char cave[CAVE_LENGTH][CAVE_LENGTH];
	memset(cave, AIR, CAVE_LENGTH * CAVE_LENGTH);

	cords_t mins = {CAVE_LENGTH, CAVE_LENGTH};
	cords_t maxs = {0, 0};

	while (!feof(file)) {
		int res;
		char c;
		cords_t cords_prev;
		res = fscanf(file, "%d,%d", &cords_prev.x, &cords_prev.y);
		assert(2 == res);

		mins = cord_min(mins, cords_prev);
		maxs = cord_max(maxs, cords_prev);
		while (' ' == (c = getc(file))) {
			cords_t cords_next;
			res = fscanf(file, "-> %d,%d", &cords_next.x, &cords_next.y);
			assert(2 == res);

			mins = cord_min(mins, cords_next);
			maxs = cord_max(maxs, cords_next);

			fill_rock(cave, cords_prev, cords_next);
			cords_prev = cords_next;
		}
		assert('\n' == c || EOF == c);
	}

	if (part == PART_2) {
		// Add floor
		const cords_t floor_start = {0, maxs.y + 2};
		const cords_t floor_end = {CAVE_LENGTH - 1, maxs.y + 2};
		fill_rock(cave, floor_start, floor_end);
	}

	// ########################### PROSSES DATA ###########################
	int print_start_x;
	int print_end_x;
	const int print_end_y = maxs.y + 3;
	if (part == PART_1) {
		print_start_x = mins.x - 1;
		print_end_x = maxs.x + 1;
	} else if (part == PART_2) {
		print_start_x = (mins.x + maxs.x) / 2 - maxs.y - 5;
		print_end_x = (mins.x + maxs.x) / 2 + maxs.y + 5;
	}

	int scroll = 0;
	int sand_count = 0;
	while (add_sand(cave)) {
		sand_count++;
		print_cave(cave, print_start_x, print_end_x, scroll, scroll + 30);
		delay(35000);
		printf("\n");

		if (scroll <= print_end_y - 30){		
			if (scroll < 82){
				if (0 == sand_count % 5)
					scroll++;
			} else if(scroll < 114) {
				if (0 == sand_count % 2)
					scroll++;
			} else 
				if (0 == sand_count % 4)
					scroll++;
		}
	}

	add_path(cave);
	print_cave(cave, print_start_x, print_end_x, 0, print_end_y);
	printf("Sand Count: %d\n", sand_count);

	// ############################## CLEANUP #############################
	fclose(file);
}

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void fill_rock(char cave[CAVE_LENGTH][CAVE_LENGTH], cords_t begin, cords_t end) {
	assert(begin.x >= 0);
	assert(begin.y >= 0);
	assert(end.x >= 0);
	assert(end.y >= 0);
	assert(begin.x < CAVE_LENGTH);
	assert(begin.y < CAVE_LENGTH);
	assert(end.x < CAVE_LENGTH);
	assert(end.y < CAVE_LENGTH);

	cords_t difference = {end.x - begin.x, end.y - begin.y};
	assert((0 == difference.x) || (0 == difference.y));

	if (difference.y < 0 || difference.x < 0) {
		swap(&begin.x, &end.x);
		swap(&begin.y, &end.y);
	}
	if (0 == difference.x) {
		for (int y_i = begin.y; y_i <= end.y; y_i++) {
			const int x = begin.x /* = end.x */;
			cave[x][y_i] = ROCK;
		}
	} else /* 0 == difference.y */ {
		for (int x_i = begin.x; x_i <= end.x; x_i++) {
			const int y = begin.y /* = end.x */;
			cave[x_i][y] = ROCK;
		}
	}
}

const cords_t SAND_SOURCE = {500 /* x */, 0 /* y */};
bool add_sand(char cave[CAVE_LENGTH][CAVE_LENGTH]) {
	if (SAND == cave[SAND_SOURCE.x][SAND_SOURCE.y])
		/* Source is blocked */
		return false;

	cords_t sand_particle = SAND_SOURCE;
	bool is_moving = true;

	while (CAVE_LENGTH + 2 > sand_particle.y && is_moving) {
		const cords_t below_snd = {sand_particle.x, sand_particle.y + 1};
		const cords_t below_left = {sand_particle.x - 1, sand_particle.y + 1};
		const cords_t below_right = {sand_particle.x + 1, sand_particle.y + 1};

		if (AIR == cave[below_snd.x][below_snd.y]) {
			sand_particle = below_snd;
		} else if (AIR == cave[below_left.x][below_left.y]) {
			sand_particle = below_left;
		} else if (AIR == cave[below_right.x][below_right.y]) {
			sand_particle = below_right;
		} else {
			is_moving = false;
			cave[sand_particle.x][sand_particle.y] = SAND;
		}
	}

	return !is_moving; /* at equilibrium, the particle is not moving*/
}

void add_path(char cave[CAVE_LENGTH][CAVE_LENGTH]) {
	if (SAND == cave[SAND_SOURCE.x][SAND_SOURCE.y])
		/* Source is blocked */
		return;

	cords_t sand_particle = SAND_SOURCE;
	bool is_moving = true;

	while (CAVE_LENGTH + 2 > sand_particle.y && is_moving) {
		const cords_t below_snd = {sand_particle.x, sand_particle.y + 1};
		const cords_t below_left = {sand_particle.x - 1, sand_particle.y + 1};
		const cords_t below_right = {sand_particle.x + 1, sand_particle.y + 1};

		cave[sand_particle.x][sand_particle.y] = '~';
		if (AIR == cave[below_snd.x][below_snd.y]) {
			sand_particle = below_snd;
		} else if (AIR == cave[below_left.x][below_left.y]) {
			sand_particle = below_left;
		} else if (AIR == cave[below_right.x][below_right.y]) {
			sand_particle = below_right;
		} else {
			is_moving = false;
		}
	}
}

void print_cave(char cave[CAVE_LENGTH][CAVE_LENGTH],
                const int start_x, const int end_x, const int start_y, const int end_y) {
	for (int y = start_y; y <= end_y; y++) {
		for (int x = start_x; x <= end_x; x++) {
			char c = (500 == x && 0 == y) ? '+' : cave[x][y];
			printf("%c", c);
		}
		printf("\n");
	}
}

int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

cords_t cord_min(cords_t a, cords_t b) {
	cords_t rt = {min(a.x, b.x), min(a.y, b.y)};
	return rt;
}

cords_t cord_max(cords_t a, cords_t b) {
	cords_t rt = {max(a.x, b.x), max(a.y, b.y)};
	return rt;
}

void delay(int number_of_miliseconds) {
	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + number_of_miliseconds)
		;
}