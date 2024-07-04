#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>


int max(int a, int b) {
	return a > b ? a : b;
}
int main() {
	bool stop = 1;
	int res = 1;
	size_t read_size1 = 1;
	size_t read_size2 = 1;
	size_t read_size3 = 1;
	int prio_total = 0;
	char* buffer1 = (char *)malloc(1000 * sizeof(char));;
	char* buffer2 = (char *)malloc(1000 * sizeof(char));;
	char* buffer3 = (char *)malloc(1000 * sizeof(char));;
	size_t lc = 1000;
	while (!feof(stdin)) {
		read_size1 = getline(&buffer1, &lc, stdin) -1;
		read_size2 = getline(&buffer2, &lc, stdin) -1;
		read_size3 = getline(&buffer3, &lc, stdin) -1;
		

		for (int i = 0; i < read_size1; i++) {
			for (int j = 0; j < read_size2; j++) {
				if(buffer1[i] == buffer2[j]){
					char c = buffer1[i];
					for (int k = 0; k < read_size3; k++)
					{
						if(c == buffer3[k]) {
							int points = 0;
							
							if(islower(c)) {
								points = c -'a' +1;
							} else if (isupper(c)) {
								points = c -'A' +27 ;
							} else if (c == EOF) {
								goto FIN;
							}
							prio_total += points;
							goto END_BOUCLE;
						}
					}
				}
			}
		}
		END_BOUCLE:
		;
	}
	FIN:

	free(buffer1);
	free(buffer2);
	free(buffer3);
	printf("\n%d\n", prio_total);
}