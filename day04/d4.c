#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

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

	int* ints = (int*)malloc(line_count * sizeof(char*));
	file = fopen(file_name, "r");

	int res = 0;
	{int i = 0;
	while (!feof(file)) {
		int inf1, sup1, inf2, d;
		int scan_succses = fscanf(file, "%d-%d,%d-%d", &inf1, &sup1, &inf2, &d);

		if(inf2 <= sup1 && d >= sup1){
			res++;
		} else if(d >= inf1 && d <= sup1){
			res++;
		}
		printf("%d-%d,%d-%d\n", inf1, sup1, inf2, d);
		char cc = getc(file);
		//if(!(cc == '\n' || cc == EOF))
		//	printf("%d, %c\n", cc, cc);
		assert(cc == '\n' || cc == EOF);
		i++;
	}}

	// ########################### PROSSES DATA ###########################



	for (int i = 0; i < line_count; i++) {
		//printf("%d\n", ints[i]);
	}
	


	printf("res : %d\n",res);
	// ########################### PROSSES DATA ###########################

	free(ints);
}
