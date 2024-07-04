#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

typedef struct cords {
	int x;
	int y;
} cords_t;

typedef struct cords_list_cell {
	cords_t head;
	struct cords_list_cell* tail;
} cords_list_cell_t;
typedef cords_list_cell_t* cords_list_t;


int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int abs(int a) {
	return a > 0 ? a : -a;
}

cords_t follow(cords_t target, cords_t follower) {
	cords_t rt;
	int difx = target.x - follower.x;
	int dify = target.y - follower.y;
	int manhatan_dist = abs(difx) + abs(dify);

	if(target.x == follower.x && target.y == follower.y) {
		return follower;
	}

	if(manhatan_dist == 0) {
		assert(follower.x == target.x);
		assert(follower.y == target.y);
		rt.x = follower.x;
		rt.y = follower.x;
		printf("aaaaa");
	} else if(manhatan_dist == 1 || manhatan_dist == 2) {
		rt.x = max(min(follower.x, target.x + 1), target.x - 1);
		rt.y = max(min(follower.y, target.y + 1), target.y - 1);
	} else if(manhatan_dist == 3) {
		if(abs(difx) == 2) {
			assert(abs(dify) == 1);
			rt.x = max(min(follower.x, target.x + 1), target.x - 1);
			rt.y = target.y;
		} else if(abs(dify) == 2) {
			rt.x = target.x;
			rt.y = max(min(follower.y, target.y + 1), target.y - 1);
			assert(abs(difx) == 1);
		} else {
			assert(0/*sasasfasd*/);
		}
	} else {
		printf("e:%d\n", manhatan_dist);
		assert(false/*adshfkjl*/);
	}
	
	return rt;
}



cords_t cords_zero() {
	cords_t rt;
	rt.x = 0;
	rt.y = 0;
	return rt;
}

int main() {
	// ##################### FILENAME #################
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
	free(buff);

	int* mvmt_amplitude = (int*)malloc(line_count * sizeof(int));
	char* directions = (char*)malloc(line_count * sizeof(char));
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		int scan_succses = fscanf(file, "%c %d\n", &directions[i], &mvmt_amplitude[i]);
		printf("%d\n", scan_succses);
		//char c = getc(file);
		//if(!(c == '\n' || c == EOF))
		//	printf("%d, %c\n", c, c);
		//assert(c == '\n' || c == EOF);
		i++;
	}}

	// ########################### PROSSES DATA ###########################
#define M_SIZE 1000
	bool visited_matrix[M_SIZE][M_SIZE];
	bool* visited_matrix_semi_centered[M_SIZE];
	for (int i = 0; i < M_SIZE; i++) {
		for (int j = 0; j < M_SIZE; j++) {
			visited_matrix[i][j] = false;
		}
		visited_matrix_semi_centered[i] = visited_matrix[i] + (M_SIZE/2);
	}
	bool** visited_matrix_centered = visited_matrix_semi_centered + (M_SIZE/2);

	cords_list_t cord_list = malloc(sizeof(cords_list_cell_t));
	cord_list->tail = malloc(sizeof(cords_list_cell_t));

	cord_list->head = cords_zero();
	cord_list->tail->head = cords_zero();

	for (int i = 0; i < line_count; i++) {
		printf("~~~~~~~~~~~~~~~ %c %d ~~~~~~~~~~~~~~~\n", directions[i], mvmt_amplitude[i]);

		for (int j = 0; j < mvmt_amplitude[i]; j++)
		{
			cords_t movement  = cords_zero();
			switch (directions[i])
			{
			case 'L':
				movement.x--;
				break;
			case 'R':
				movement.x++;
				break;
			case 'D':
				movement.y--;
				break;
			case 'U':
				movement.y++;
				break;
			default:
				assert(false);
			}
			visited_matrix_centered[cord_list->tail->head.x][cord_list->tail->head.y] = true;

			/*
			for (int y = M_SIZE-1; y >= 0 ; y--) {
				for (int x = 0; x < M_SIZE; x++) {
					char c = visited_matrix[x][y] ? '#': '.';
					if(0 == x && 0 == y)
						c = 's';
					if(cord_list->head.x == x && cord_list->head.y == y)
						c = 'H';
					if(cord_list->tail->head.x == x && cord_list->tail->head.y == y)
						c = 'T';
					printf("%c", c);
				}
				printf("\n");
			}
			//*/
			cord_list->head.x += movement.x;
			cord_list->head.y += movement.y;
			//if(cord_list->head.y != cord_list->tail->head.y || cord_list->head.x != cord_list->tail->head.x)
				cord_list->tail->head = follow(cord_list->head, cord_list->tail->head);
			printf("_\n");

		}
		
		
		assert(abs(cord_list->head.y) < (M_SIZE/2) -1);
		assert(abs(cord_list->head.x) < (M_SIZE/2) -1);
		
		
	}
	
	int res = 0;
	for (int i = 0; i < M_SIZE; i++) {
		for (int j = 0; j < M_SIZE; j++) {
			if(visited_matrix[i][j])
				res++;
		}
	}
	printf("res %d\n", res);
	// ########################### PROSSES DATA ###########################

	free(mvmt_amplitude);
}
