#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 5000

enum facing_e { NORTH = 0,
	            WEST = 1,
	            SOUTH = 2,
	            EAST = 3 };
typedef struct {
	enum { MOVE_FORWARD = 0,
		   RIGHT = 1,
		   LEFT = -1 } type;
	int distance;
} direction_t;

typedef struct {
	int x;
	int y;
} cords_t;

cords_t face_to_incr(enum facing_e facing);
cords_t cube_wrap(cords_t pos, enum facing_e* face);

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

	{
		int i = 0;
		while (!feof(file)) {
			getline(&lines[i], &linecapp, file);
			i++;
		}
	}
	fclose(file);

	char* txt_directions = lines[line_count - 1];
	int max_dir_length = strlen(txt_directions);
	int dir_length = 0;
	direction_t* directions = calloc(max_dir_length, sizeof(direction_t));
	for (size_t i = 0; i < max_dir_length && (isdigit(txt_directions[i]) || isupper(txt_directions[i])); dir_length++) {
		if (isdigit(txt_directions[i])) {
			directions[dir_length].distance = atoi(txt_directions + i);
			directions[dir_length].type = MOVE_FORWARD;
			while (isdigit(txt_directions[i]))
				i++;
		} else {
			if (txt_directions[i] == 'R') {
				directions[dir_length].type = RIGHT;
			} else {
				assert(txt_directions[i] == 'L');
				directions[dir_length].type = LEFT;
			}
			i++;
		}
	}
	line_count -= 2;

	// ########################### PROSSES DATA ###########################

	// for (int i = 0; i < line_count; i++) {
	// 	printf("%s", lines[i]);
	// }
	// //putchar('\n');

	// for (int i = 0; i < dir_length; i++) {
	// 	if(directions[i].type == MOVE_FORWARD) {
	// 		printf("%d", directions[i].distance);
	// 	} else {
	// 		printf("%c", directions[i].type == LEFT ? 'L' : 'R');
	// 	}
	// }
	// putchar('\n');

	enum facing_e facing = WEST;
	cords_t position = {1, 1};
	while (lines[position.y][position.x] != '.')
		position.x++;

	for (int i = 0; i < dir_length; i++) {
		printf("%d, %d", position.x, position.y);
		switch (directions[i].type) {
			case MOVE_FORWARD:
				printf("\t| move forward");
				for (int step = 0; step < directions[i].distance; step++) {
					cords_t inc = face_to_incr(facing);
					cords_t pos_infront = position;
					pos_infront.x += inc.x;
					pos_infront.y += inc.y;

					if(' ' == lines[pos_infront.y][pos_infront.x]) {
						/* wrap arround */
						pos_infront = cube_wrap(position, &facing);
					}

					if('.' == lines[pos_infront.y][pos_infront.x]) {
						/* Move forward */
						position = pos_infront;
					} else if('#' == lines[pos_infront.y][pos_infront.x]) {
						/* Stop, do nothing */
						//continue;
					} else {
						printf("i=%d, c=%d, x=%d, y=%d\n", lines[pos_infront.y][pos_infront.x], pos_infront.x, pos_infront.y);
						assert(false);
					}
				}

				break;

			default:
				printf("\t| %s\t", directions[i].type == LEFT ? "Left" : "Right" );
				assert(directions[i].type == LEFT || directions[i].type == RIGHT);
				facing = (facing + directions[i].type) % 4;
				break;
		}
		printf("\t| facing: %s\n", facing == NORTH ? "north" : (facing == SOUTH ? "south" : (facing == EAST ? "east" : "west")));
	}

	// CALCULATINg password
	int final_password = 0;
	switch (facing)
	{
	case WEST:
		final_password += 0;
		break;
	case SOUTH:
		final_password += 1;
		break;
	case EAST:
		final_password += 2;
		break;
	case NORTH:
		final_password += 3;
		break;
	
	default:
		assert(false);
		break;
	}
	final_password += 4*position.x;
	final_password += 1000*position.y;

	printf("final password: %d\n", final_password);
	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	free(lines);
	free(directions);
}

cords_t face_to_incr(enum facing_e facing) {
	cords_t rt;

	switch (facing)
	{
	case NORTH:
		rt.x = 0;
		rt.y = -1;
		break;
	case SOUTH:
		rt.x = 0;
		rt.y = 1;
		break;
	case WEST:
		rt.x = 1;
		rt.y = 0;
		break;
	case EAST:
		rt.x = -1;
		rt.y = 0;
		break;
	
	default:
		assert(false);
		break;
	}

	return rt;
}



cords_t cube_wrap(cords_t pos, enum facing_e* face){

	return (cords_t)0;
}

