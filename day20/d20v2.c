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
int remaider(int num, int d);
void mix(int* vals, int* indicators, int line_count, int idx);

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

	int* vals = (int*)malloc(line_count * sizeof(int));
	int* indicators = (int*)malloc(line_count * sizeof(int));
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		int scan_succses = fscanf(file, "%d", &vals[i]);
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

	// for (int i = 0; i < line_count; i++) {
	// 	printf("%d, ", vals[i]);
	// }
	//printf("\n\n");
	for (int i = 0; i < line_count; i++) {
		int j = 0;
		while(indicators[j] != i)
			j++;

		//printf("%d(%dth element) moves to ?\n", vals[j], j);
		/* mix the j_th element */
		mix(vals, indicators, line_count, j);
		
		// for (int i = 0; i < line_count; i++) {
		// 	printf("%d, ", vals[i]);
		// }
		// printf("\n\n");
	}

	int cord_of0 = 0;
	while (vals[cord_of0] != 0)
		cord_of0++;

	printf("res: %d\n", vals[(cord_of0 + 1000) %line_count] +vals[(cord_of0 + 2000) %line_count]+ vals[(cord_of0 + 3000) %line_count]);

	// ########################### PROSSES DATA ###########################

	free(vals);
	free(indicators);
}

int remaider(int num, int d) {
	const int result = num % d;
	return result >= 0 ? result : result + d;
}

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void mix(int* vals, int* indicators, int line_count, int idx) {
	int val = vals[idx];
	int indicator = indicators[idx];
	
	for (int i = 0; i < abs(val); i++) {
		if(val > 0) {
			/* swap rightwards */
			if(idx == line_count -1) {
				/* swap with zero */
				swap(&vals[idx], &vals[0]);
				swap(&indicators[idx], &indicators[0]);
				idx = 0;
			} else {
				/* normal */
				swap(&vals[idx], &vals[idx+1]);
				swap(&indicators[idx], &indicators[idx+1]);
				idx++;
			}
		} else {
			/* swap leftwards */
			if(idx == 0) {
				/* swap with line_count -1 */
				swap(&vals[idx], &vals[line_count -1]);
				swap(&indicators[idx], &indicators[line_count -1]);
				idx = line_count -1;
			} else {
				/* normal */
				swap(&vals[idx], &vals[idx-1]);
				swap(&indicators[idx], &indicators[idx-1]);
				idx--;
			}
		}
	}
	
}
