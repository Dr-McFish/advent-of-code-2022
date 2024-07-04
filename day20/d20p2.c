#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

/// @brief remainder of euclidian division
/// @param num numerator
/// @param d denominator
/// @return remainder of num/d
int remaider(int64_t num, int64_t d);
void mix(int64_t* vals, int64_t* indicators, int line_count, int idx);

int main() {
	// ############################ FILENAME ############################
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

	int64_t* vals = (int64_t*)malloc(line_count * sizeof(int64_t));
	int64_t* indicators = (int64_t*)malloc(line_count * sizeof(int64_t));
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		int scan_succses = fscanf(file, "%lld", &vals[i]);
		//printf("%d\n", scan_succses);
		char c = getc(file);
		//if(!(c == '\n' || c == EOF))
		//	printf("%d, %c\n", c, c);
		assert(c == '\n' || c == EOF);
		i++;
	}}
	fclose(file);
	// ########################### PROSSES DATA ###########################


	for (int i = 0; i < line_count; i++) {
		indicators[i] = i;
	}

	const int64_t decription_key = 811589153;
	for (int i = 0; i < line_count; i++) {
		vals[i] *= decription_key;
	}

	for (int i = 0; i < line_count; i++) {
		printf("%lld, ", vals[i]);
	}
	printf("\n\n");
	for (int k = 0; k < 10; k++){
		for (int i = 0; i < line_count; i++) {
			int j = 0;
			while(indicators[j] != i)
				j++;

			/* mix the j_th element */
			mix(vals, indicators, line_count, j);
			
		}
		int cord_of0 = 0;
		while (vals[cord_of0] != 0)
			cord_of0++;
		printf("after %d rounds of mixing\n",k+1);
		for (int i = 0; i < line_count; i++) {
			printf("%lld, ", vals[(i + cord_of0) % line_count]);
		}
		printf("\n\n");
	}
	int cord_of0 = 0;
	while (vals[cord_of0] != 0)
		cord_of0++;

	printf("res: %lld\n", vals[(cord_of0 + 1000) %line_count] +vals[(cord_of0 + 2000) %line_count]+ vals[(cord_of0 + 3000) %line_count]);

	// ########################### PROSSES DATA ###########################

	free(vals);
	free(indicators);
}

int remaider(int64_t num, int64_t d) {
	const int64_t result = num % d;
	return result >= 0 ? result : result + d;
}

void mix(int64_t* vals, int64_t* indicators, int line_count, int idx) {
	int64_t val = vals[idx];
	int64_t indicator = indicators[idx];
	int new_idx = remaider(((int64_t)idx) + val, line_count-1);

	if (idx < new_idx) {
		for (int i = idx; i < new_idx; i++) {
			vals[i] = vals[i + 1];
			indicators[i] = indicators[i + 1];
		}
	} else if (new_idx < idx) {
		for (int i = idx; i > new_idx; i--) {
			vals[i] = vals[i - 1];
			indicators[i] = indicators[i - 1];
		}
	} else {
		/* nothing to do */
		return;
	}

	vals[new_idx] = val;
	indicators[new_idx] = indicator;
	
}
