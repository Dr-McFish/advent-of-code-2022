#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000
#define M_SIZE 20

typedef struct cords {
	int x;
	int y;
} cords_t;

typedef struct cords_list_cell {
	cords_t head;
	struct cords_list_cell* tail;
} cords_list_cell_t;
typedef cords_list_cell_t* cords_list_t;
typedef cords_list_cell_t maillon;

maillon* maillon_creer(cords_t valeur, maillon* suivant) {
	maillon* rt = (maillon*)malloc(sizeof(maillon));
	rt->head = valeur;
	rt->tail = suivant;

	return rt;
}
void maillon_detruire(maillon* m) {
	//assert(m->suivant == NULL);

	free(m);
}

void chaine_detruire(maillon* m) {
	maillon* courant = m;
	while (courant != NULL) {
		maillon* tmp = courant;
		courant = courant->tail;
		maillon_detruire(tmp);
	}
}

int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int abs(int a) {
	return a > 0 ? a : -a;
}

void print_matrix(int visited_matrix[M_SIZE][M_SIZE], cords_list_t rope);

void follow(cords_t target, cords_t* follower_a) {
	cords_t rt;
	cords_t follower = *follower_a;
	int difx = target.x - follower.x;
	int dify = target.y - follower.y;
	int manhatan_dist = abs(difx) + abs(dify);

	if(target.x == follower.x && target.y == follower.y) {
		return;
	}

	if(manhatan_dist == 0) {
		assert(follower.x == target.x);
		assert(follower.y == target.y);
		rt.x = follower.x;
		rt.y = follower.x;
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
			assert(false/*sasasfasd*/);
		}
	} else {
		printf("e:%d\n", manhatan_dist);
		print_matrix(visted_matrix);
		assert(false/*adshfkjl*/);
	}
	
	*follower_a = rt;
}

void rec_follow(cords_list_t rope) {
	if(rope == NULL)
		return;
	cords_t head = rope->head;
	cords_list_t tail = rope->tail;
	if(tail == NULL)
		return;

	follow(head, &tail->head);
	rec_follow(tail);
}


cords_t cords_zero() {
	cords_t rt;
	rt.x = 0;
	rt.y = 0;
	return rt;
}


int main() {
	// ##################### FILENAME #################
	const char* file_name = "in2.txt";


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
		//printf("%d\n", scan_succses);
		//char c = getc(file);
		//if(!(c == '\n' || c == EOF))
		//	printf("%d, %c\n", c, c);
		//assert(c == '\n' || c == EOF);
		i++;
	}}

	// ########################### PROSSES DATA ###########################
	bool visited_matrix[M_SIZE][M_SIZE];
	bool* visited_matrix_semi_centered[M_SIZE];
	for (int i = 0; i < M_SIZE; i++) {
		for (int j = 0; j < M_SIZE; j++) {
			visited_matrix[i][j] = false;
		}
		visited_matrix_semi_centered[i] = visited_matrix[i] + (M_SIZE/2);
	}
	bool** visited_matrix_centered = visited_matrix_semi_centered + (M_SIZE/2);

	cords_list_t rope = maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), maillon_creer(cords_zero(), NULL))))))))));
	cords_list_t tail = rope;
	while (tail->tail != NULL) {
		tail = tail->tail;
	}
	

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
			visited_matrix_centered[tail->head.x][tail->head.y] = true;

			///*
			print_matrix(visited_matrix, rope);
			//*/
			rope->head.x += movement.x;
			rope->head.y += movement.y;
			rec_follow(rope);
			printf("_\n");

		}
		
		
		assert(abs(rope->head.y) < (M_SIZE/2) -1);
		assert(abs(rope->head.x) < (M_SIZE/2) -1);
		
		
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
	chaine_detruire(rope);
	free(mvmt_amplitude);
}

void print_matrix(int visited_matrix[M_SIZE][M_SIZE], cords_list_t rope) {
	for (int y = M_SIZE-1; y >= 0 ; y--) {
		for (int x = 0; x < M_SIZE; x++) {
			char c = visited_matrix[x][y] ? '#': '.';
			if(0 == x + M_SIZE/2 && 0 == y + M_SIZE/2)
				c = 's';
			if(rope->head.x + M_SIZE/2 == x && rope->head.y + M_SIZE/2 == y)
				c = 'H';
			else if(rope->tail->head.x + M_SIZE/2 == x && rope->tail->head.y + M_SIZE/2 == y)
				c = '1';
			else if(rope->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->head.y + M_SIZE/2 == y)
				c = '2';
			else if(rope->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = '3';
			else if(rope->tail->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = '4';
			else if(rope->tail->tail->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = '5';
			else if(rope->tail->tail->tail->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = '6';
			else if(rope->tail->tail->tail->tail->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->tail->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = '7';
			else if(rope->tail->tail->tail->tail->tail->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->tail->tail->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = '8';
			else if(rope->tail->tail->tail->tail->tail->tail->tail->tail->tail->head.x + M_SIZE/2 == x && rope->tail->tail->tail->tail->tail->tail->tail->tail->tail->head.y + M_SIZE/2 == y)
				c = 'T';
			//else if(tail->head.x + M_SIZE/2 == x && tail->head.y + M_SIZE/2 == y)
			//	c = 'T';
			printf("%c", c);
		}
		printf("\n");
	}
}
