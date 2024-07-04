#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int64_t x;
	int64_t y;
} cords_t;

typedef struct {
	cords_t pos;
	cords_t closest_beacon;
} sensor_t;

int count_lines(const char* file_name);
int64_t max(int64_t a, int64_t b);
int64_t min(int64_t a, int64_t b);
int64_t manhantan_distance(cords_t a, cords_t b);

int main() {
	// ##################### FILENAME #################
	const char* file_name = "in.txt";
	int lines_count = count_lines(file_name);
	sensor_t* sensors = calloc(lines_count, sizeof(sensor_t));
	int64_t* ranges = calloc(lines_count, sizeof(int64_t));
	cords_t* beacons_without_duplicates = calloc(lines_count, sizeof(int64_t));
	int num_beacons = lines_count;

	FILE* file = fopen(file_name, "r");

	{
		int i = 0;
		while (!feof(file)) {
			sensor_t* s = &sensors[i];
			int res = fscanf(file, "Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld",
			                 &s->pos.x, &s->pos.y, &s->closest_beacon.x, &s->closest_beacon.y);
			assert(4 == res);

			beacons_without_duplicates[i].x = s->closest_beacon.x;
			beacons_without_duplicates[i].y = s->closest_beacon.y;

			getc(file); /* \n */
			i++;
		}
	}

	/* remove duplicates */
	{

		for (int i = 0; i < num_beacons; i++) {
			for (int j = i+1; j < num_beacons; j++) {
				if(beacons_without_duplicates[i].x == beacons_without_duplicates[j].x
				&& beacons_without_duplicates[i].y == beacons_without_duplicates[j].y) {
					beacons_without_duplicates[j] = beacons_without_duplicates[num_beacons-1];
					num_beacons--;
					j--;
				}
			}
		}

		for (int i = 0; i < num_beacons; i++) {
			//printf("(%lld, %lld)\n", beacons_without_duplicates[i].x, beacons_without_duplicates[i].y);
			for (int j = i+1; j < num_beacons; j++) {
				assert(beacons_without_duplicates[i].x != beacons_without_duplicates[j].x
					|| beacons_without_duplicates[i].y != beacons_without_duplicates[j].y);
			}
		}
		
		
	}

	int64_t maxrange = 0;
	int64_t max_x = 0;
	int64_t min_x = 0;
	for (int i = 0; i < lines_count; i++) {
		ranges[i] = manhantan_distance(sensors[i].pos, sensors[i].closest_beacon);
		maxrange = max(maxrange, ranges[i]);
		max_x = max(max_x, sensors[i].pos.x);
		min_x = min(min_x, sensors[i].pos.x);
	}

	// ########################### PROSSES DATA ###########################
	/* calculate max row x = maximum x cordinate of sensor + maximum range */
	int64_t max_row_x = 4000000;
	int64_t min_row_x = 0;

	const int row_of_interest = 2000000; /* y = */
	printf("max_row_x: %lld\n", max_row_x);
	printf("min_row_x: %lld\n", min_row_x);

	
	int n_where_the_beacon_isn_t = 0;
	for (int x = min_row_x; x < max_row_x; x++) {
		cords_t crds = {x, row_of_interest};
		bool is_scanned = false;

		for (int sens = 0; sens < lines_count; sens++) {
			is_scanned = is_scanned || manhantan_distance(sensors[sens].pos, crds) <= ranges[sens];
		}
		if(is_scanned)
			n_where_the_beacon_isn_t++;
	}

	for (int i = 0; i < num_beacons; i++) {
		if(beacons_without_duplicates[i].y == row_of_interest) {
			bool is_scanned = false;
			cords_t crds = beacons_without_duplicates[i];
			
			for (int sens = 0; sens < lines_count; sens++) {
				is_scanned = is_scanned || manhantan_distance(sensors[sens].pos, crds) <= ranges[sens];
			}
			if(is_scanned)
				n_where_the_beacon_isn_t--; /* because that is where it is */
		}
	}

	printf("n_where_the_beacon_isn_t: %d\n", n_where_the_beacon_isn_t);

	// ########################### PROSSES DATA ###########################
	fclose(file);
	free(sensors);
	free(ranges);
	free(beacons_without_duplicates);
}

int64_t max(int64_t a, int64_t b) {
	return a > b ? a : b;
}
int64_t min(int64_t a, int64_t b) {
	return a < b ? a : b;
}

int64_t manhantan_distance(cords_t a, cords_t b) {
	return llabs(a.x - b.x) + llabs(a.y - b.y);
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
