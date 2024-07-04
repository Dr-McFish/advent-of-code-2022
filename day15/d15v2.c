#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
	int64_t max_row_x = max_x + maxrange + 10;
	int64_t min_row_x = min_x - maxrange - 10;

	int* d_from_sens_range = calloc(lines_count, sizeof(int));

	const int row_of_interest = 2000000; /* y = */
	printf("max_row_x: %lld\n", max_row_x);
	printf("min_row_x: %lld\n", min_row_x);

	int n_where_the_beacon_isn_t = 0;
	for (int64_t x = min_row_x; x < max_row_x; x++) {
		cords_t crds = {x, row_of_interest};

		int max_sens_range = manhantan_distance(sensors[0].pos, crds) - ranges[0];
		sensor_t sensor_of_that_range = sensors[0];

		for (int sens = 1; sens < lines_count; sens++) {
			d_from_sens_range[sens] = manhantan_distance(sensors[sens].pos, crds) - ranges[sens];
			if(max_sens_range < d_from_sens_range[sens]) {
				sensor_of_that_range = sensors[sens];
				max_sens_range = d_from_sens_range[sens];
			}
		}
		int64_t dif = llabs(crds.y, )

		if(max_sens_range > 0) {
			n_where_the_beacon_isn_t++;
		} else {

		}
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

			/* 		The missile knows where it is at all times. It knows this because it knows where it isn't. 
				By subtracting where it is from where it isn't, or where it isn't from where it is (whichever is greater),
				it obtains a difference, or deviation. The guidance subsystem uses deviations to generate corrective
				commands to drive the missile from a position where it is to a position where it isn't, and arriving
				at a position where it wasn't, it now is. Consequently, the position where it is, is now the position that
				it wasn't, and it follows that the position that it was, is now the position that it isn't.
					In the event that the position that it is in is not the position that it wasn't, the system has acquired
				a variation, the variation being the difference between where the missile is, and where it wasn't.
				If variation is considered to be a significant factor, it too may be corrected by the GEA. However,
				the missile must also know where it was.
					The missile guidance computer scenario works as follows. Because a variation has modified some of the
				information the missile has obtained, it is not sure just where it is. However, it is sure where it isn't,
				within reason, and it knows where it was. It now subtracts where it should be from where it wasn't, or
				vice-versa, and by differentiating this from the algebraic sum of where it shouldn't be, and where it was,
				it is able to obtain the deviation and its variation, which is called error. */
		}
	}

	printf("n_where_the_beacon_isn_t: %d\n", n_where_the_beacon_isn_t);

	// ########################### PROSSES DATA ###########################
	fclose(file);
	free(sensors);
	free(d_from_sens_range);
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
