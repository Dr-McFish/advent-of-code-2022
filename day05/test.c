#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


int main() {

	FILE* f2 = fopen("in2.txt", "r");
	int mv, from, to;
	int res = 1;
	while (!feof(f2) && res != 0)
	{
		res = fscanf(f2, "move %d from %d to %d\n", &mv, &from, &to);
		printf("%d move %d from %d to %d\n", res, mv, from, to);
		/* code */
	}
	

}