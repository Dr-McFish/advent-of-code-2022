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
	int64_t range;
} sensor_t;

int count_lines(const char* file_name);
int64_t max(int64_t a, int64_t b);
int64_t min(int64_t a, int64_t b);
int64_t manhantan_distance(cords_t a, cords_t b);


bool is_visible(sensor_t* sensors, int n_sensors, cords_t pos);
bool is_known_beacon(cords_t* beacons, int n_beacons, cords_t pos);
bool is_inbounds(cords_t pos, int xmin, int ymin, int xmax, int ymax);

int main() {
	// ##################### FILENAME #################
	const char* file_name = "in.txt";
	int lines_count = count_lines(file_name) -1;
	sensor_t* sensors = calloc(lines_count, sizeof(sensor_t));
	cords_t* beacons_without_duplicates = calloc(lines_count, sizeof(int64_t));
	int num_beacons = lines_count;

	FILE* file = fopen(file_name, "r");

	int max_cord;
	{int res= fscanf(file, "max_dim=%d\n", &max_cord);
	assert(res == 1);}

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
		sensors[i].range = manhantan_distance(sensors[i].pos, sensors[i].closest_beacon);
		maxrange = max(maxrange, sensors[i].range);
		max_x = max(max_x, sensors[i].pos.x);
		min_x = min(min_x, sensors[i].pos.x);
	}

	//cords_t answer = {14, 11};
	//assert(!is_visible(sensors, lines_count, answer));
	//assert(is_inbounds(answer, 0,0,20,20));
	// for (int i = 0; i < lines_count; i++) {
	// 	printf("%lld\n", manhantan_distance(sensors[i].pos, answer)- sensors[i].range);
	// }

	// ########################### PROSSES DATA ###########################

	for (int sens = 0; sens < lines_count; sens++) {
		cords_t runner = sensors[sens].pos;
		runner.y += sensors[sens].range +1; /*places the runner at the top summet of the sensor, just out of range */
		//printf("rg: %lld\n", sensors[sens].range);

		for (int i = 0; i < 4; i++) {
			assert(runner.y == sensors[sens].pos.y || runner.x == sensors[sens].pos.x);
			assert(1 == manhantan_distance(runner, sensors[sens].pos) - sensors[sens].range);

			cords_t incr;

			if(0 == i) {
				incr.y = -1;
				incr.x = 1;
			} else if(1 == i) {
				incr.y = -1;
				incr.x = -1;
			} else if(2 == i) {
				incr.y = -1;
				incr.x = 1;
			} else if(3 == i) {
				incr.y = 1;
				incr.x = 1;
			}

			int j = 0;
			while ((1 == i%2 && runner.y != sensors[sens].pos.y) || (0 == i%2 && runner.x != sensors[sens].pos.x)) {
				//if(!(1 == manhantan_distance(runner, sensors[sens].pos) - sensors[sens].range))
				//	printf("Hello: i=%d,j=%d, %lld\n", i, j, manhantan_distance(runner, sensors[sens].pos) - sensors[sens].range);
				assert(1 == manhantan_distance(runner, sensors[sens].pos) - sensors[sens].range);
				/* check for answer */
				if(is_inbounds(runner, 0,0, max_cord, max_cord)) {
					if(!is_visible(sensors, lines_count, runner)) {
						printf("ANSWER: x=%lld, y=%lld\n", runner.x, runner.y);
						printf("tuning frequency: %lld\n", (runner.x* 4000000) + runner.y);

						return 0;
					}
				} else {
					/* posibly do optimizations ... */
					/* totaly not needed, glad i didnt do it */
				}
				runner.x += incr.x;
				runner.y += incr.y;

				assert(j < 100000000);
				j++;
			}
				//printf("a");
			
		}
		

	}

	printf("\nwop wop whaahhh!\n");

	// ########################### PROSSES DATA ###########################
	fclose(file);
	free(sensors);
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


bool is_visible(sensor_t* sensors, int n_sensors, cords_t pos) {
	bool res = false;

	for (int i = 0; i < n_sensors; i++) {
		res = res || (manhantan_distance(sensors[i].pos, pos) <= sensors[i].range);
	}
	
	return res;
}

bool is_known_beacon(cords_t* beacons, int n_beacons, cords_t pos) {
	bool res = false;

	for (int i = 0; i < n_beacons; i++) {
		res = res || (beacons[i].x == pos.x && beacons[i].y == pos.y);
	}
	
	return res;
}

bool is_inbounds(cords_t pos, int xmin, int ymin, int xmax, int ymax) {
	return	pos.x >= xmin && pos.x <= xmax &&
			pos.y >= ymin && pos.y <= ymax;
}