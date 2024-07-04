#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MAPSIZE 25

void delay(int number_of_miliseconds);
int abs(int a) {return a > 0 ? a : -a;}
int max(int a, int b) {return a > b ? a : b;}


void recursive_fill(char map[MAPSIZE][MAPSIZE][MAPSIZE], int x_start, int y_start, int z_start);

int main() {
	// ##################### FILENAME #################
	const char* file_name = "in.txt";
	FILE* file = fopen(file_name, "r");

	// ############################ READ DATA ############################
	char map[MAPSIZE][MAPSIZE][MAPSIZE];
	memset(map, 6, MAPSIZE* MAPSIZE * MAPSIZE);

	int max_dim = 0;
	while (!feof(file)) {
		int x, y, z;
		int res = fscanf(file, "%d,%d,%d", &x, &y, &z);
		assert(3 == res);
		getc(file);

		max_dim = max(max_dim, max(x, max(y, z)));

		map[x+1][y+1][z+1] = 1;
	}

	fclose(file);
	printf("maxdim: %d\n", max_dim);
	//return 0;
	// ########################### PROSSES DATA ###########################


	/* Part 2 prosessing */
	recursive_fill(map, 0, 0, 0);
	for (int x = 0; x < MAPSIZE; x++)
		for (int y = 0; y < MAPSIZE; y++)
			for (int z = 0; z < MAPSIZE; z++)
				if(6 == map[x][y][z])
					map[x][y][z] = 1;
	/* end */

	char map_dx[MAPSIZE-1][MAPSIZE][MAPSIZE];
	char map_dy[MAPSIZE][MAPSIZE-1][MAPSIZE];
	char map_dz[MAPSIZE][MAPSIZE][MAPSIZE-1];
	for (int x = 0; x < MAPSIZE-1; x++)
		for (int y = 0; y < MAPSIZE; y++)
			for (int z = 0; z < MAPSIZE; z++)
				map_dx[x][y][z] = map[x+1][y][z] - map[x][y][z];
	
	for (int x = 0; x < MAPSIZE; x++)
		for (int y = 0; y < MAPSIZE-1; y++)
			for (int z = 0; z < MAPSIZE; z++)
				map_dy[x][y][z] = map[x][y+1][z] - map[x][y][z];

	for (int x = 0; x < MAPSIZE; x++)
		for (int y = 0; y < MAPSIZE; y++)
			for (int z = 0; z < MAPSIZE-1; z++)
				map_dz[x][y][z] = map[x][y][z+1] - map[x][y][z];


	int num_sides= 0;
	for (int x = 0; x < MAPSIZE-1; x++)
		for (int y = 0; y < MAPSIZE; y++)
			for (int z = 0; z < MAPSIZE; z++)
				num_sides += abs(map_dx[x][y][z]);
	
	for (int x = 0; x < MAPSIZE; x++)
		for (int y = 0; y < MAPSIZE-1; y++)
			for (int z = 0; z < MAPSIZE; z++)
				num_sides += abs(map_dy[x][y][z]);

	for (int x = 0; x < MAPSIZE; x++)
		for (int y = 0; y < MAPSIZE; y++)
			for (int z = 0; z < MAPSIZE-1; z++)
				num_sides += abs(map_dz[x][y][z]);

	printf("%d\n", num_sides);

	// for (int z = 0; z < 7; z++){
	// 	for (int y = 0; y < 4; y++){
	// 		for (int x = 0; x < 4; x++){
	// 			printf("%c", map[x][y][z] ? '#' : '.' );
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("\n\n\n\n");
	// 	delay(500000);
	// }


	// ########################### PROSSES DATA ###########################
}

void delay(int number_of_miliseconds) {
	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + number_of_miliseconds)
		;
}

void recursive_fill(char map[MAPSIZE][MAPSIZE][MAPSIZE], int x_start, int y_start, int z_start) {
	if(x_start < 0 || y_start < 0 || z_start < 0 ||
	   x_start >= MAPSIZE || y_start >= MAPSIZE || z_start >= MAPSIZE) 
		return;
	if(map[x_start][y_start][z_start] == 1 || map[x_start][y_start][z_start] == 0)
		return;
	assert(map[x_start][y_start][z_start] == 6);

	map[x_start][y_start][z_start] = 0;

	recursive_fill(map, x_start+1, y_start, z_start);
	recursive_fill(map, x_start, y_start+1, z_start);
	recursive_fill(map, x_start, y_start, z_start+1);
	recursive_fill(map, x_start-1, y_start, z_start);
	recursive_fill(map, x_start, y_start-1, z_start);
	recursive_fill(map, x_start, y_start, z_start-1);
}
