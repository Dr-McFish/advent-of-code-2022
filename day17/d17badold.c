#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_HEIGHT (5256 + 20)
typedef char row_t[7];

typedef struct {
	int x;
	int y;
} cords_t;

const char* jets = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
int jets_period;

int max(int a, int b) {return a > b ? a : b;}
// 
// ###o
//
int fall_flat_rock(row_t* chamber, int highest_rock, int* jet_phase);

// .#.
// #o#
// .#.
int fall_plus_rock(row_t* chamber, int highest_rock, int* jet_phase);

// ..#
// ..#
// #o#
int fall_L_rock(row_t* chamber, int highest_rock, int* jet_phase);

// #
// #
// #
// o
int fall_I_rock(row_t* chamber, int highest_rock, int* jet_phase);

// 
// ##
// #o
int fall_cube_rock(row_t* chamber, int highest_rock, int* jet_phase);

void print_chamber(row_t* chamber, int ystart);
void print_chamber_rock(row_t* chamber, int ystart, char rock, cords_t rock_cords, char rdisp);

int main() {
	jets_period = strlen(jets);
	row_t* chamber = (row_t*)calloc(BUFF_HIGHT, sizeof(row_t));
	memset(chamber, ' ', sizeof(char) * 7 * BUFF_HIGHT);
	int highest_rock = 0;
	int jet_phase = 0;

	/* Do stuff */
	for (int i = 0; i < 404; i++)
	{
		highest_rock = max(highest_rock, fall_flat_rock(chamber, highest_rock, &jet_phase));
		//prifntf("Tower hieght: %d\n", highest_rock);
		highest_rock = max(highest_rock, fall_plus_rock(chamber, highest_rock, &jet_phase));
		//prifntf("Tower hieght: %d\n", highest_rock);
		highest_rock = max(highest_rock, fall_L_rock(chamber, highest_rock, &jet_phase));
		//prifntf("Tower hieght: %d\n", highest_rock);
		highest_rock = max(highest_rock, fall_I_rock(chamber, highest_rock, &jet_phase));
		//prifntf("Tower hieght: %d\n", highest_rock);
		highest_rock = max(highest_rock, fall_cube_rock(chamber, highest_rock, &jet_phase));
		/* code */
	}
	highest_rock = max(highest_rock, fall_flat_rock(chamber, highest_rock, &jet_phase));
	highest_rock = max(highest_rock, fall_plus_rock(chamber, highest_rock, &jet_phase));
	

	print_chamber(chamber, highest_rock+3);

	printf("Tower hieght: %d\n", highest_rock);
	return 0;
}



//(((((((((((((((((((((((((((((((((((((((((((.)))))))))))))))))))))))))))))))))))))))))))

int fall_flat_rock(row_t* chamber, int highest_rock, int* jet_phase) {
	cords_t rock_pos = {/*x*/ 5, highest_rock + 3};
	//prifntf("- Begins falling: \n");
	print_chamber_rock(chamber, highest_rock + 6, '-', rock_pos, '@');
	//prifntf("\n");

	while (true) {

		{ /* get pushed by the jet stream if posible */
			char push_direction = jets[*jet_phase % jets_period];
			(*jet_phase)++;
			assert(push_direction == '<' || '>' == push_direction);

			if(push_direction == '<') {
				if(rock_pos.x > 3 && chamber[rock_pos.y][rock_pos.x -4] == ' ') {
					rock_pos.x--;
				}
			} else /* push_direction == '<' */ {
				if(rock_pos.x < 6 && chamber[rock_pos.y][rock_pos.x +1] == ' ') {
					rock_pos.x++;
				}
			}
			//prifntf("%c jet: \n", push_direction);
			print_chamber_rock(chamber, highest_rock + 6, '-', rock_pos, '@');
			//prifntf("\n");
		}
		{ /* fall down */
			if(rock_pos.y == 0 || (chamber[rock_pos.y - 1][rock_pos.x] != ' '
								   || chamber[rock_pos.y - 1][rock_pos.x-1] != ' '
								   || chamber[rock_pos.y - 1][rock_pos.x-2] != ' '
								   || chamber[rock_pos.y - 1][rock_pos.x-3] != ' ')) {
				/* stop falling*/
				chamber[rock_pos.y][rock_pos.x] = '#';
				chamber[rock_pos.y][rock_pos.x-1] = '#';
				chamber[rock_pos.y][rock_pos.x-2] = '#';
				chamber[rock_pos.y][rock_pos.x-3] = '#';
				
				//prifntf("Rock comes to rest\n");
				print_chamber_rock(chamber, highest_rock + 6, '-', rock_pos, '#');
				//prifntf("\n");

				return rock_pos.y+1;
			} else {
				rock_pos.y--;

				//prifntf("Rock falls 1 unit\n");
				print_chamber_rock(chamber, highest_rock + 6, '-', rock_pos, '@');
				//prifntf("\n");
			}
		}
	}
}

int fall_plus_rock(row_t* chamber, int highest_rock, int* jet_phase) {
	cords_t rock_pos = {/*x*/ 3, highest_rock + 3 + 1};
	//prifntf("+ Begins falling: \n");
	print_chamber_rock(chamber, highest_rock + 6, '+', rock_pos, '@');
	//prifntf("\n");

	while (true) {

		{ /* get pushed by the jet stream if posible */
			char push_direction = jets[*jet_phase % jets_period];
			(*jet_phase)++;
			assert(push_direction == '<' || '>' == push_direction);

			if(push_direction == '<') {
				if(rock_pos.x > 1 && chamber[rock_pos.y+1][rock_pos.x -1] == ' '
								  && chamber[rock_pos.y  ][rock_pos.x -2] == ' '
								  && chamber[rock_pos.y-1][rock_pos.x -1] == ' ') {
					rock_pos.x--;
				}
			} else /* push_direction == '<' */ {
				if(rock_pos.x < 5 && chamber[rock_pos.y+1][rock_pos.x +1] == ' '
								  && chamber[rock_pos.y  ][rock_pos.x +2] == ' '
								  && chamber[rock_pos.y-1][rock_pos.x +1] == ' ') {
					rock_pos.x++;
				}
			}
			//prifntf("%c jet: \n", push_direction);
			print_chamber_rock(chamber, highest_rock + 6, '+', rock_pos, '@');
			//prifntf("\n");
		}
		{ /* fall down */
			if(rock_pos.y == 0 ||(chamber[rock_pos.y - 1][rock_pos.x + 1] != ' '
							   || chamber[rock_pos.y - 2][rock_pos.x 	] != ' '
							   || chamber[rock_pos.y - 1][rock_pos.x - 1] != ' ')) {
				/* stop falling*/
				chamber[rock_pos.y][rock_pos.x] = '#';
				chamber[rock_pos.y-1][rock_pos.x] = '#';
				chamber[rock_pos.y+1][rock_pos.x] = '#';
				chamber[rock_pos.y][rock_pos.x-1] = '#';
				chamber[rock_pos.y][rock_pos.x+1] = '#';
				
				//prifntf("Rock comes to rest\n");
				print_chamber_rock(chamber, highest_rock + 6, '+', rock_pos, '#');
				//prifntf("\n");

				return rock_pos.y+2;
			} else {
				rock_pos.y--;

				//prifntf("Rock falls 1 unit\n");
				print_chamber_rock(chamber, highest_rock + 6, '+', rock_pos, '@');
				//prifntf("\n");
			}
		}
	}
}

int fall_L_rock(row_t* chamber, int highest_rock, int* jet_phase) {
	cords_t rock_pos = {/*x*/ 3, highest_rock + 3};
	//prifntf("L Begins falling: \n");
	print_chamber_rock(chamber, highest_rock + 6, 'L', rock_pos, '@');
	//prifntf("\n");

	while (true) {

		{ /* get pushed by the jet stream if posible */
			char push_direction = jets[*jet_phase % jets_period];
			(*jet_phase)++;
			assert(push_direction == '<' || '>' == push_direction);

			if(push_direction == '<') {
				if(rock_pos.x > 1 && chamber[rock_pos.y+2][rock_pos.x] == ' '
								  && chamber[rock_pos.y+1][rock_pos.x] == ' '
								  && chamber[rock_pos.y  ][rock_pos.x - 2] == ' ') {
					rock_pos.x--;
				}
			} else /* push_direction == '<' */ {
				if(rock_pos.x < 5 && chamber[rock_pos.y+2][rock_pos.x+2] == ' '
								  && chamber[rock_pos.y+1][rock_pos.x+2] == ' '
								  && chamber[rock_pos.y  ][rock_pos.x+2] == ' ') {
					rock_pos.x++;
				}
			}
			//prifntf("%c jet: \n", push_direction);
			print_chamber_rock(chamber, highest_rock + 6, 'L', rock_pos, '@');
			//prifntf("\n");
		}
		{ /* fall down */
			if(rock_pos.y == 0 ||(chamber[rock_pos.y - 1][rock_pos.x + 1] != ' '
							   || chamber[rock_pos.y - 1][rock_pos.x 	] != ' '
							   || chamber[rock_pos.y - 1][rock_pos.x - 1] != ' ')) {
				/* stop falling*/
				chamber[rock_pos.y][rock_pos.x-1] = '#';
				chamber[rock_pos.y][rock_pos.x] = '#';
				chamber[rock_pos.y][rock_pos.x+1] = '#';
				chamber[rock_pos.y+1][rock_pos.x+1] = '#';
				chamber[rock_pos.y+2][rock_pos.x+1] = '#';
				
				//prifntf("Rock comes to rest\n");
				print_chamber_rock(chamber, highest_rock + 6, 'L', rock_pos, '#');
				//prifntf("\n");

				return rock_pos.y+3;
			} else {
				rock_pos.y--;

				//prifntf("Rock falls 1 unit\n");
				print_chamber_rock(chamber, highest_rock + 6, 'L', rock_pos, '@');
				//prifntf("\n");
			}
		}
	}
}

int fall_I_rock(row_t* chamber, int highest_rock, int* jet_phase) {
	cords_t rock_pos = {/*x*/ 2, highest_rock + 3};
	//prifntf("| Begins falling: \n");
	print_chamber_rock(chamber, highest_rock + 6, '|', rock_pos, '@');
	//prifntf("\n");

	while (true) {

		{ /* get pushed by the jet stream if posible */
			char push_direction = jets[*jet_phase % jets_period];
			(*jet_phase)++;
			assert(push_direction == '<' || '>' == push_direction);

			if(push_direction == '<') {
				if(rock_pos.x > 1	&& chamber[rock_pos.y  ][rock_pos.x -1] == ' '
									&& chamber[rock_pos.y+1][rock_pos.x -1] == ' '
									&& chamber[rock_pos.y+2][rock_pos.x -1] == ' '
									&& chamber[rock_pos.y+3][rock_pos.x -1] == ' ') {
					rock_pos.x--;
				}
			} else /* push_direction == '<' */ {
				if(rock_pos.x < 6	&& chamber[rock_pos.y  ][rock_pos.x +1] == ' '
									&& chamber[rock_pos.y+1][rock_pos.x +1] == ' '
									&& chamber[rock_pos.y+2][rock_pos.x +1] == ' '
									&& chamber[rock_pos.y+3][rock_pos.x +1] == ' ') {
					rock_pos.x++;
				}
			}
			//prifntf("%c jet: \n", push_direction);
			print_chamber_rock(chamber, highest_rock + 6, '|', rock_pos, '@');
			//prifntf("\n");
		}
		{ /* fall down */
			if(rock_pos.y == 0 || (chamber[rock_pos.y - 1][rock_pos.x] != ' ')) {
				/* stop falling*/
				chamber[rock_pos.y][rock_pos.x] = '#';
				chamber[rock_pos.y+1][rock_pos.x] = '#';
				chamber[rock_pos.y+2][rock_pos.x] = '#';
				chamber[rock_pos.y+3][rock_pos.x] = '#';
				
				//prifntf("Rock comes to rest\n");
				print_chamber_rock(chamber, highest_rock + 6, '|', rock_pos, '#');
				//prifntf("\n");

				return rock_pos.y+3;
			} else {
				rock_pos.y--;

				//prifntf("Rock falls 1 unit\n");
				print_chamber_rock(chamber, highest_rock + 6, '|', rock_pos, '@');
				//prifntf("\n");
			}
		}
	}
}

int fall_cube_rock(row_t* chamber, int highest_rock, int* jet_phase) {
	cords_t rock_pos = {/*x*/ 3, highest_rock + 3 + 1};
	//prifntf("C Begins falling: \n");
	print_chamber_rock(chamber, highest_rock + 6, 'C', rock_pos, '@');
	//prifntf("\n");

	while (true) {

		{ /* get pushed by the jet stream if posible */
			char push_direction = jets[*jet_phase % jets_period];
			(*jet_phase)++;
			assert(push_direction == '<' || '>' == push_direction);

			if(push_direction == '<') {
				if(rock_pos.x > 1 && chamber[rock_pos.y+1][rock_pos.x -2] == ' '
								  && chamber[rock_pos.y  ][rock_pos.x -2] == ' ') {
					rock_pos.x--;
				}
			} else /* push_direction == '<' */ {
				if(rock_pos.x < 6 && chamber[rock_pos.y+1][rock_pos.x +1] == ' '
								  && chamber[rock_pos.y  ][rock_pos.x +1] == ' ') {
					rock_pos.x++;
				}
			}
			//prifntf("%c jet: \n", push_direction);
			print_chamber_rock(chamber, highest_rock + 6, 'C', rock_pos, '@');
			//prifntf("\n");
		}
		{ /* fall down */
			if(rock_pos.y == 0 ||(chamber[rock_pos.y - 1][rock_pos.x - 1] != ' '
							   || chamber[rock_pos.y - 1][rock_pos.x] != ' ')) {
				/* stop falling*/
				assert(' ' == chamber[rock_pos.y][rock_pos.x]);
				assert(' ' == chamber[rock_pos.y][rock_pos.x-1]);
				assert(' ' == chamber[rock_pos.y+1][rock_pos.x]);
				assert(' ' == chamber[rock_pos.y+1][rock_pos.x-1]);

				chamber[rock_pos.y][rock_pos.x] = '#';
				chamber[rock_pos.y][rock_pos.x-1] = '#';
				chamber[rock_pos.y+1][rock_pos.x] = '#';
				chamber[rock_pos.y+1][rock_pos.x-1] = '#';
				
				//prifntf("Rock comes to rest\n");
				print_chamber_rock(chamber, highest_rock + 6, 'C', rock_pos, '#');
				//prifntf("\n");

				return rock_pos.y+2;
			} else {
				rock_pos.y--;

				//prifntf("Rock falls 1 unit\n");
				print_chamber_rock(chamber, highest_rock + 6, 'C', rock_pos, '@');
				//prifntf("\n");
			}
		}
	}
}

void print_chamber(row_t* chamber, int ystart) {
	for (int y = ystart; y >= 0; y--) {
		//prifntf("|");
		for (int x = 0; x < 7; x++) {
			//prifntf("%c", chamber[y][x]);
		}
		//prifntf("|\n");
	}
	
	//prifntf("+-------+\n");
}

void print_chamber_rock(row_t* chamber, int ystart, char rock, cords_t rock_cords, char rdisp) {
	for (int y = ystart; y >= 0; y--) {
		//prifntf("|");
		for (int x = 0; x < 7; x++) {
			char c = chamber[y][x];

			if(x == rock_cords.x && y == rock_cords.y)
				c = rdisp;
			switch (rock)
			{
			case '-':
				if( (x == rock_cords.x-1 && y == rock_cords.y) ||
					(x == rock_cords.x-2 && y == rock_cords.y) ||
					(x == rock_cords.x-3 && y == rock_cords.y))
					c = rdisp;
				break;
			case '+':
				if( (x == rock_cords.x-1 && y == rock_cords.y) ||
					(x == rock_cords.x+1 && y == rock_cords.y) ||
					(x == rock_cords.x && y == rock_cords.y - 1) ||
					(x == rock_cords.x && y == rock_cords.y + 1))
					c = rdisp;
				break;
			case 'L':
				if( (x == rock_cords.x-1 && y == rock_cords.y) ||
					(x == rock_cords.x+1 && y == rock_cords.y) ||
					(x == rock_cords.x+1 && y == rock_cords.y + 1) ||
					(x == rock_cords.x+1 && y == rock_cords.y + 2))
					c = rdisp;
				break;
			case '|':
				if( (x == rock_cords.x && y == rock_cords.y + 1) ||
					(x == rock_cords.x && y == rock_cords.y + 2) ||
					(x == rock_cords.x && y == rock_cords.y + 3))
					c = rdisp;
				break;
			case 'C':
				if( (x == rock_cords.x-1&& y == rock_cords.y) ||
					(x == rock_cords.x	&& y == rock_cords.y + 1) ||
					(x == rock_cords.x-1&& y == rock_cords.y + 1))
					c = rdisp;
				break;
			
			default:
				assert(NULL);
				break;
			}
			//prifntf("%c", c);
		}
		//prifntf("|\n");
	}
	
	//prifntf("+-------+\n");
}