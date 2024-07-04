#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_HEIGHT (1000000)

typedef char row_t[9];

typedef struct {
	int x;
	int y;
} cords_t;


// 3
// 2
// 1
// 0# # # #
//  0 1 2 3
// hieght = 2
typedef struct {
	char outline[4][4]; /* y, x */
	int hieght;
	char id;
} rock_t;


const rock_t flat_rock = {
	{"####","    ","    ","    "},
	1,
	'-'
};
const rock_t plus_rock = {
	{" #  ",
	 "### ",
	 " #  ","    "},
	3,
	'+'
};
const rock_t L_rock = {
	{"### ",
	 "  # ",
	 "  # ","    "},
	3,
	'L'
};
const rock_t I_rock = {
	{"#   ",
	 "#   ",
	 "#   ",
	 "#   "},
	4,
	'|'
};
const rock_t cube_rock = {
	{"##  ",
	 "##  ",
	 "    ",
	 "    "},
	2,
	'C'
};

const bool do_debug_printing = false;
//const char* jets = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
const char* jets = ">><>>><<<>>><<>>>><<<>>>><<><>>><<<<>><<<><<<>><<<<>><<>>>><<>><<<><>><><<<>><<<<>>>><<>>><><<>>><<><<<<>>><<>>><>><<<<>><<<<>><<><<>>>><<<>>><<>>>><<<><><><<>>>><<>><<>><>>><<<>><<>>><<<<>>><><<<><<<<><<<<><<<<>>>><>>>><<<>>>><>>><>>><<<<>>><<<<><<<<>>><>>>><<>>>><<<<>>>><<<><<><<<>>>><>>><<<<>>><<<><<>>><<<<>>>><<<>><>><>><<<<>>>><><>>>><<<<>>>><<><<<<><<<<>>>><<>>><<>><<<><<<><<<<>>><<<>>><<>>>><<<>>>><>>>><<<>><<<>>><<<<>>><<>>><<<<>><<>>>><><<<<>>><><>><>><<<>>><<<>><>>>><<<<>>><<<<>>><<<<>><<<<>>><<>>>><>>>><<<>>><<<<><<<>><><<<<>>>><<<>><<<<>>>><<>><<<><<<<><<<<>>>><<<>>>><<>><><<<>><<>>>><<<>><<<>>>><>>>><><<<<><><<>>><<<>><<>>>><<<>><<>>>><<>>><<<<><<<><<>><<<>><>><<<>>>><><>>>><<<><>>>><<<>>>><<>><<>><<<<><<<>>><<<>><<<<>>><>><<>>><<<<>>>><<<><<><<>><<<<>>><<><<<<>>><>>><<>>><<>><<><><<>>>><<><<<<>>><<<<>>><<<<>>>><<>>>><>><<<<>><<<<>>><>><<<>>>><<><<<>>>><<>>><<>><<<<>><<<><<<<>>><<><<<<>>>><<>>><<<>>>><<<>><<<>>>><>>>><>>><>>><>>><>>><<>>><<><<<<>><<>><<>><>>>><<<<>>><<><<>>>><<<><<>>><<><<<>>><<>><>><<><<<>>><<>>>><<<<>><<><<>><>>>><><<<<>>>><<><<><<>><<<<>><<<><<>>>><<<<>>><>><<>>><>><<<<>><<>>><><<<>>><<<>>>><>>><<<>>><><<<><<<><<<>><>>>><<><<>>><<<<>>>><<>>><<>>>><<<<>>><<>><<<<>><<<<>>><><<><<<>><<>>><><>>>><<<><>>>><<>>>><<<>>><<>><>>>><>><<>>>><<<<>><<<>><<>><>>><<<<>>>><><<<<>>><<<<>>><>>><<<<>>>><>>><<<<>>>><><<>>><<><<<>>><<<>>><>>><<>>>><><<<<>><<>>>><<>>>><<>>><<>><<><>>>><<>>>><<<<>>><<<<><<<<><<>>><<<>><<<>>>><<><<<<>>>><<<<>>>><<<>>><><<<>>><<<>><<<>><<<<>>><<>>><<>><<<>>>><<<<>><><><<<>>>><<<>>><<>>>><<>>>><<<><><<<>>>><<<>>><>><<<>>>><<<<><<<<>>>><<<>>><<>><>>><<>><>>>><<>>><<>><<<>>><><<<>><<<>><><><<<<><<<<><<<<>><>>>><<>>><>><<<>>>><<>><<<>>><<>><<<>>><>>><<<>>>><>><<>><<<>><>><<>>>><>>>><<<><<<<>><>>>><>>>><>>><<<><<>>>><<>>>><<<<><<>>>><<<<><<>>>><<>>><<>><<<>><<>>><><<<<><>>>><>>><<<>><<<<><>>><<<<>>><>>>><><<>>>><<<<>>>><>><>>>><<<<><<<<>>>><>>>><<<>>><><<><<>>>><<<>><<<<><<<<>>><<>>><<<<>>>><<<<>><><<>>><>><<<>>><<>><<>>><><<>><<<<>>>><<<><<<<>>>><<><<<>>>><<<<><<>>>><>>>><<<<>>>><>>>><<<>><><<<>><<<<><<<>><>>><<<<>><<<<>>><<>><>>><><<<<>>><<><<<<>>>><>>><>><<<<><<<>>><>><<<>><<<><<>><>><<<<>>><<<>>><><>>>><><>><<<>>><<><<>>>><<<>>>><><>>><<<><<>><><<<><<>>>><<<>><<><>><<<>><<<<>>><<>>><<>><<<>><><<>>><<>>>><<<>><>><<>>>><<<>>>><<>>><<>>>><<>>><>><>><<<>>><<>>>><>><<>>>><<<<>><<<><<<<>><>>><<>>><<<<><<<>><><<<<>><<><<<>><<<>><>>>><<><>><<<<>>>><>>>><<><><<<<>>>><<<>>>><<<>><<><<<<>>>><<<<>><<<>>>><<<<>>>><<>><<<<>>>><>>><<><<<>>>><<>>><>>>><>><>><>><<>>>><<<><>><<<>>>><<<<>>>><<<<><<<>><><<><<>>>><<<<>><<<<>><><<<<>>>><<<<>><><<>>>><<>><<>>>><<><<><<<><<>>><<><<<><<><<<>>>><<<<>>><><<<<><<>>>><>><<<>>>><<>>><<>>><>>><>>>><<<>>><<>><<>>>><<<>>>><<<<>><<<>><<<><<<><><<<<>>><<>>><<<>><<>>>><><<<<>><<<<>>>><<<<>>><<<<><<>><<>>><<<<><<<><<<<>><<<>><<<><<<>><<<>><<<<>>>><<<>>><>>><<<<>><<<>><<<>>><>>>><<<<>><<<<><<><>><>><<><<<>>>><<<<><<>>><><<<<>><<<>>>><<<<>>><<>>>><<>>>><<<>>><<>>>><<>><>>><<><<<<>>><<<<>><<<<>><<<<>>><<<<><<><><<<><>><<<<>><<<>>>><<<<>>>><<<<>>><<><<><>><<>><<><<<<><<><>><>><<<>><<>>><>>>><<<><<<>><<<>>>><<>>><<>>>><<<<>>><<<<>><<<<>>>><<>><>>><><>><<<<>>>><<<>><<>>><<<>>>><>>>><<<<>>><<>>>><<<><<<<>>>><<><>>>><<<>>>><<><<<<>><<<<>><<><<<<>>>><<<>>>><<<<>><<>><>><<><<>>>><<>>>><<><<>><<<<>>><<<<>>>><>>><<<><>>>><<<>><<<>>><<>>><<<<>>>><><<<<>><>><<>>><<<>>>><<<<>><><>><<<<><<<<>>><<<<>>>><<<>>><<<<>>>><<>>>><<<><<>>><<>>><<<><>><>>>><<<<>>><<<<>>>><<<>><<>>><>>>><<<<>>><<>>>><<<>>>><<<><<<<>><>>><<><><<<>><<<>>><<><<<<>>>><<<<><<<<><<<>>>><<<<><<>><<<<>><<>>>><>><<<<><>>><<<><<<><<>>>><>><<<><<<<><<>>>><<>><<<<>>>><<>><<<<>><<<>><<><>>><>><<><<>>><<><<<<>><><<>>><<<>>><>>><<>>><<<><<<>>>><<<<>>><<<<>>>><<<<>>><<<<>>><<<>>>><<<>><<>><<<<>>>><<<<><<>>>><<>>><<<><<><<>>><<<<>>><<<<><<<>>><>>><<<>>>><>>><<>>><<>>>><>>><<<>>><<<><<<<>><<><<<>>><<<>>><<>>>><<><<>>><<><<<<>>>><<<>><<<<>>><<<<>>><>>>><><<<>><<<<>>>><>><<<>>><<>>>><<<<><<<>>><<<>>><><>>>><>><<<>>><<<<><<<>><<<<>>>><<<<><><<>><><>><<<><<<<><<<>>><<>>>><<<>>><<>>><<>>><<>>><<<>>>><<><<>>>><<<<>>>><<<>><<<><<<<>>><<>>>><<<<>>>><<><<>>><<>><<<>>>><<<>>>><>><<<<>>>><>>>><>>><<<<>><<<<>>><<<><<<>>><<<>><<<>><<<><>>><>><<<<>><<<<>>>><<>><<>><<<>>>><<<>><<<<>>><<>>>><<<<><><<<>>><>>><<<<>>><<>>><<<<>><<<>>><<<<>><>>>><<>>><><<<>>>><<<<>>>><>><<<>>>><<<>>>><>><>>><<<>>>><><>>>><<<<><>>><<<>><<<<>>><<<>>><<><<>>>><<<><<<><<<>>><<<><>><>>><><<<<>>><<<<>>><>>>><<<<>>>><>>><<<>>><>>><>>><<<><<><<><<<<><<<<>>>><<<>>>><<<>>>><<<<><<<><<>>>><>>>><<<>><<<>>><<>><<<<><><>><<><<<>>><<<<>>>><><<><<<>><>>>><<<<>>>><<>>><<<<><<<>>>><<>>><<<<>>>><<<<>><<>>><<<>>>><<<<>><<<><<>><>><<>><>><<>>><<>>>><<<<>><<><<<><<<<>><<<<>><>>><><<<<>><<>><<<<>><<<>>><><<<><<<>>><<><<<<>><<<<>>><<><<>><><<<<><<<>><<<><<<<>>><<<<><<>>><<<>>><<<>><<<<><<>>>><<<<>>><<<<>>><<<<><>>>><<><>><>>><<>>>><><<<><><>>><<<<><<><<<>>><>>><><<<><<>><>>><<<<>><<<>>><<>>>><<>>>><>>>><<<>>><>><<<<>><<<>>><<<>>>><>>><<><<><<<<>>>><<<><>>>><<>>>><<<>>>><<>>>><>><<<<>>>><<><<<<>>>><<>><<<>>>><>><<><<<<>><>>>><<<<>><<>>><<<<>><<<<>>><<<<>>><<>><<<<><><<<>>>><<>>>><<<>>><<<><<<>>><<<>>><<>>><><<<>>><<>><<>>>><><<<<>>><<<>><<><<>>>><<<><<><<<<>><><><<>>>><<<<>>><<<<>><<><<><>>>><<<>>><<<>>>><<<>>><<<<>>>><<<<>>><<<<><<><<<<>>><>>><<<<><>>><>>>><<>>><<>>>><<<<>>><<<>>><<<<><<>>>><>>><<<>>><<<<><><<>>><<<>>><<<>><<<>>>><<<><<>>>><<>>><>><<<>>>><<>>><<>><<<>>>><<><<<<>>>><<>>>><<<<><<>>><<>><>>>><<<>>><<<><<<><<<>>>><<>>>><<>>>><<<<>>><><<<>>>><<<<>><<>>>><><>><<<>><<<<>><>>>><><<<>>>><>><<<<>>><<<<>><<<>><<>>>><>>><<<>>>><>>>><>>>><<>><<<<>>>><<><<<<>>>><><<><>><<<>>><>>>><<>>><<<>><<<>>>><<><<<<><<<>>>><<<<>><<>><<>><<>>>><>>><<<>>><<<><<<<>>>><>>><>><<>>>><>>>><<<<>><<<>>>><<<>><<<<>>><<>><<><<>>>><>><>>><><<>><<><<><<<>>><<<>><>><<<<>>><<<<>>>><<<<><>>>><<>>>><<<<>><<<<>><<<>>>><>><<<>>><<<<>>>><<<<>>><<<><><<><<<<>><<<<><<>><<<<>>>><>><>>>><<<>><<>><<<<>>><<<<><<<><>><>>>><>><>>><<>>>><<<<>><<>>>><<<<>>><<<>><>><<<<>>><<<<>>><<><<<<>>>><<<<>>><<<>>><>><<<><<>>>><<>><<<<>><<<>><><<<>>><<<>><<<<>>>><><<<>>>><<<<>><<<<>>>><<<<>>><<<><>>><<<<>>><>><>>><<>><<><<<>>>><><<<<>>>><<<<>>><<<<>>><<>><>><<>><<<>>><<<<>>><<>>><<<>>><<<<><>>>><>>><<<<>>><<<><>><<>>><<>><<>>>><>>><<<<>><<>><<<>><><<><>><<<<>>><<<<><<>>><>>>><<>><<<>><<>>><<>>>><<<><<<><<<<>><<<>><<>>><><>><<<><<><>>>><<<>>>><<<>>>><>><<><<<<><<<<>>>><<<>>><<><<<<>>><<<>><<>><>>><<>><<<>>>><><><<<>><>>>><<>>>><>>><<<>><>>>><<<<>><<<<><<<<>><<<<>><<<<>>>><<<<>>><<<><<<<>>>><<>>>><<<>><<>>>><<<>>><<<>>><<>><<<>><<<<><<<>><><>>><<><<>>>><<>>><>>><<<>><<><<><<<>>>><<<<><<<><<<<>><<<<><<<>><<>>><<<<>>>><<<>>>><<><<>>>><><<<>><><>>>><<<<>>><><><<<>>>><<<><<<><<>>><<>><<<>><<<>>>><<<>><<>><<<<>><<>>><><<<<><<><<>><<<>>>><<<><<<>>>><<>><<<<>><<<<><<<>>>><<<<>><<<><<<<>>>><>>>><<<><<><><<<><<<>>><<>>><<<<><<>>>><<<>>><<<<>>><<><<>><<>>>><><<<<>>>><><>>><<<>>><<>>><<<<>>>><<<<>><<<>>>><>>><<>>>><>><<<>><<<><<<<>>><>><<<>><<>>>><<><<<>><<><<<><<<<>>>><><<>>>><<<>>><<<<>><>>>><<>><>>><<<<>>><<<<><<<<>><<<<>><<<><<<<><>>>><><<<>>>><<>><<>>><><<<>>><><<<<><>><<<>>>><<><<<<>>>><<<><<<>>>><>>><<<><<<<>>>><<<<><>>><<<>>><<>><<><<<<>>><<<<>>><><>>><<<<><<>>><<>><<<><>><<<<>>><<>><<<>>><<<<>>><<<>>><<<><<<<><<<<>><<><<<>>><<>><<<>><><<<>>>><<><<>>><<<>>>><<>><<<<>>>><>>><<<<><><<<<>><>>><<<>>><<<>>><<<>><>>>><>>><<<<><<<>><>>>><<<>>><><<><>>><<<>>>><>><<<>><<<<><>><<>>><<<><>>><<<>><><<<<>>>><<<><<<>><>><<<>>><>>>><<<>>><<>>>><<<>>>><>>>><<<<>>><<><<>><<<<>>>><>>><<<><<<<>>>><<<>>><<>>><<<>><<<<>>><<<<><>>>><<>>>><<>>><<>><<<<>><<<>>><<<>>>><<<<>>>><>>><><<>>>><<<>>><<><><<>>>><<<>>>><<>><<<<>><<<<>>><<<><<<><<><<<><<<>>>><<<<>>><<<<>><<<<>><<<<>>>><<<><<>><<>>>><<>>><<<<>>>><<<>>>><><>>>><<<<>>>><<<<>><<<>>><<<<>><<<>>><<<<><<<><<<<>>><<<>>><<>>>><>>>><<<<><<<<>><<>>>><<<<><>>><><<>>><>>>><<<>>>><<<<><<><<<<><><<<<><<<<>>>><<<<>>><<<>>><<<<>>>><<<<>><>>>><<>><<>><<>><<<>><>>><>><>><<><>><<<>>><<<>>><>>><<>><<<><>><<>><<<>><<<>><>>>><>>>><<<>><>>>><<>>>><>><<>>>><>><<<<>><<>>><<<<>>><<>><<<>>>><<>><<>><<<<><<<><<<<>>><>>><<>><>><<<>><<<>>><>>>><<>>>><>>><<>>>><><>><<>><<<<><<>>>><<<><<>>>><<>><<<>>>><>><<>>>><>>><><><<<><>><<><>><<<><<<<>>><<>>>><><<>>>><<>>><<>>>><<<<>>>><>>><><>><>>><><<<<>>>><<<<>>><<<<>>>><<>>>><<>>>><<<>>>><<<<>>>><<<>>><<<>>>><<<<><<<>>><<<<><<>>><<>>>><<<<>><>>>><<><><<<<><<<>>><<<>><<><<<<>>>><<>><><<<>>>><<<>>><<<<>>><>>><<<<>>>><<<<>>><<><<>>>><<<><>>>><<<<>>><>><>>>><<<<>><<>>>><>><<<>><<>>><<<<><<<>>><<<><>>><<<><><<<>>><<<>>>><><>>><<<><<<<>>><>>><>><<<><<<<>>><>>>><<<<>>><<<<>>><<<>><<>>><<<<>>>><<>><<<<>><>>><<<>><>><<<<><<<<>><<<><<<>>>><><>><<>>>><<<<><<<>>>><<<<>><<>>><<<><>>>><<<<><><>>>><<>>><<<>>><<<<>>>><<>>><><<<>>><<>>>><<<<>>><<<<>><><<<<>>>><<<<>><<<><<><<<<>>><<<<><<>><<<>>><<<>>><><>>><<<>>><<>><<<>>><<<<>><>>><>>>><>>><<<<>>><>>><<><<<<>>>><<<<>>><<<<>>>><<<>>><>>><>><<<>>>><>>><<<<>>>><<<<>>><<<<>>>><<<>><<<>><<>><>><<<<>>><>>>><<<<>><<<<><<<<>>><>><>><<<>>>><<<><>>><<><<<<>>>><<<<>><<<<>>>><<<>><<<><<<>>>><<<<><<<>><><<<<>>>><<>>>><<<<>>><<>>><>>><<><<<<><<<><<><<<<>>><>>>><<<<><<<<>>>><<<<>>><<>>>><<>>>><>>>><<><<<<><>><<>>>><<<>>><<<<>><<>>><<<>>>><<<>><<<<>><<<><>>><><<<>>><<<>>><<<><<>><<<>><<<>>>><<>><<>>><<<>><>>>><<>>><<<<><><<<>>><<>>><>><<<<>>><<<>>><<>><<<<><<>>><<>>>><<>><>>><<<<><<<<><<<><<<<>>><<>>><<<<>>><<>>>><>>>><<<<>>>><<<>><<<>><<<><>><>>>><<<><<<>>><>>><<<><<>>>><>>>><>>><><<<>>>><<<>>><>><<>>>><<>>><<>>>><<<<>>>><<>>>><<>>><<<>><<<<>>><><<<>>><<<><<<><<<>>>><>>>><><<>>>><<<>><><<<<>>>><<<<>>><<<<>><>><>>>><<<><><>>><<<>>><<<>>>><<>>>><<<<>>><><<>>>><>>><<><<<>>>><<<>>>><<<>>>><>>><>>>><>>><>>><>>><<<<><>>><<<>>><>><<<>>><>>>><<<<>>><<>>>><<><<>>><<><<<<>>><<>>><<><<<<>>><<>><<>>>><<<>>>><<<><><<<<><<<><<><>><<<<>><<>><<<<>><<>>>><<<>>>><<<<>>>><>>>><<<<>><<<>>>><<>>>><<><<>>><<<<><>>>><<<>>>><<<<><<<<>><>>><<<>>><<<<><>><<<><<>>><<>>>><<<><<<>><<<>>><<<>>><<<<>>><<<<>>>><<<>>>><<<><<<<>><<<<>>><<<>><<<>>><>>><<>><>>>><>>>><<<>>><<<>>>><<>>><<<><><<<>>>><<<><<<<>>><<<>>>><><<>>><<>>><<<>>><<<<>>><<<>>>><<<>>>><>>>><<<><<><>>>><<>>>><<<<>><<<><<<<>>><>";
int jets_period;

int max(int a, int b) {return a > b ? a : b;}


/// @brief fall rock into chamber
/// @return highest rock after fall
int fall_rock(row_t* chamber, int highest_rock, int* jet_phase, const rock_t rock);
void print_chamber(row_t* chamber, int ystart);

int main() {
	jets_period = strlen(jets);
	row_t* chamber = (row_t*)calloc(MAX_HEIGHT, sizeof(row_t));
	for (int i = 0; i < MAX_HEIGHT; i++)
		strncpy(chamber[i], "|       |", 9);	
	int highest_rock = 0;
	int jet_phase = 0;

	row_t* memory = (row_t*)calloc(30, sizeof(row_t));
	int jet_phase_mem = (row_t*)calloc(30, sizeof(row_t));

	/* Do stuff */
	for (int64_t i = 0; i < 1000000000000ll; i++) {
		highest_rock = fall_rock(chamber, highest_rock, &jet_phase, flat_rock);
		highest_rock = fall_rock(chamber, highest_rock, &jet_phase, plus_rock);
		highest_rock = fall_rock(chamber, highest_rock, &jet_phase, L_rock);
		highest_rock = fall_rock(chamber, highest_rock, &jet_phase, I_rock);
		highest_rock = fall_rock(chamber, highest_rock, &jet_phase, cube_rock);

		if(i == 1000) {
			/* save to memory */
		} else if (i > 1000) {
			/* try to recall from memory */
		}
	}
	highest_rock = fall_rock(chamber, highest_rock, &jet_phase, flat_rock);
	highest_rock = fall_rock(chamber, highest_rock, &jet_phase, plus_rock);

	print_chamber(chamber, highest_rock+3);

	printf("Tower hieght: %d\n", highest_rock);
	return 0;
}



//(((((((((((((((((((((((((((((((((((((((((((.)))))))))))))))))))))))))))))))))))))))))))


void print_chamber(row_t* chamber, int ystart) {
	for (int y = ystart; y >= 0; y--) {
		for (int x = 0; x < 9; x++) {
			printf("%c", chamber[y][x]);
		}
		printf("\n");
	}
	
	printf("+-------+\n");
}

void print_chamber_rock(row_t* chamber, int ystart, const rock_t rock, cords_t rock_pos) {
	for (int y = ystart; y >= 0; y--) {
		for (int x = 0; x < 9; x++) {
			char c = chamber[y][x];

			if(x >= rock_pos.x && x < rock_pos.x +4
			&& y >= rock_pos.y && y < rock_pos.y +4) {
				if('#' == rock.outline[y -rock_pos.y][x - rock_pos.x]) 
					c = '@';
			}
			
			printf("%c", c);
		}
		printf("\n");
	}
	
	printf("+-------+\n");
}


bool is_coliding(row_t* chamber, const rock_t rock, cords_t rock_pos) {
	if(rock_pos.y < 0)
		return true;

	bool res = false;

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if(rock.outline[y][x] == '#') {
				res = res || chamber[rock_pos.y +y][rock_pos.x + x] != ' ';
			}
		}
	}

	return res;
}

void place_rock(row_t* chamber, const rock_t rock, cords_t rock_pos) {
	assert(!is_coliding(chamber, rock, rock_pos));

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if(rock.outline[y][x] == '#') {
				chamber[rock_pos.y +y][rock_pos.x + x] = '#';
			}
		}
	}
	
}


int fall_rock(row_t* chamber, int highest_rock, int* jet_phase, const rock_t rock) {
	cords_t rock_pos = {/*x*/ 3, highest_rock + 3};

	if(do_debug_printing) {
		printf("%c Begins falling: \n", rock.id);
		print_chamber_rock(chamber, highest_rock + 6, rock, rock_pos);
		printf("\n");
	}
	assert(!is_coliding(chamber, rock, rock_pos));

	while (true) {

		{ /* get pushed by the jet stream if posible */
			char push_direction = jets[*jet_phase % jets_period];
			(*jet_phase)++;

			cords_t attempted_position = rock_pos;

			if(push_direction == '<') {
				attempted_position.x--;
			} else {
				assert('>' == push_direction);
				attempted_position.x++;
			}

			if(! is_coliding(chamber, rock, attempted_position)) {
				rock_pos = attempted_position;
			}


			if(do_debug_printing) {
				printf("%c jet: \n", push_direction);
				print_chamber_rock(chamber, highest_rock + 6, rock, rock_pos);
				printf("\n");
			}
			assert(!is_coliding(chamber, rock, rock_pos));

		}
		{ /* fall down */
			cords_t attempted_position = rock_pos;
			attempted_position.y--;

			if(is_coliding(chamber, rock, attempted_position)) {
				place_rock(chamber, rock, rock_pos);
				
				if(do_debug_printing) {
					printf("Rock comes to rest\n");
					print_chamber(chamber, highest_rock + 6);
					printf("\n");
				}
				return max(rock_pos.y + rock.hieght, highest_rock);
			} else {
				rock_pos = attempted_position;

				if(do_debug_printing) {
					printf("Rock falls 1 unit\n");
					print_chamber_rock(chamber, highest_rock + 6, rock, rock_pos);
					printf("\n");
				}
			}
		}
	}
}
