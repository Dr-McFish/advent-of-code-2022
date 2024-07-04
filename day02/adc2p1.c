#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>



int max(int a, int b) {
	return a > b ? a : b;
}

int rockpaperscisors(char oponent, char me) {
	oponent += 'X' - 'A';
	int rt;
	if(me == 'X') {
		rt = 1;
	} else if(me == 'Y') {
		rt = 2;
	} else if(me == 'Z') {
		rt = 3;
	}

	if(oponent == me) {// draw
		rt += 3;
	} else if(oponent == 'X') {
		if (me == 'Y')
		{
			rt += 6;
		} 
		
	} else if(oponent == 'Y') {
		if (me == 'Z')
		{
			rt += 6;
		} 
	} else if(oponent == 'Z') {
		if (me == 'X')
		{
			rt += 6;
		} 
	}
	return rt;
}

int main() {
	bool stop = 1;
	int res = 1;

	int point_accumules = 0;

	while (stop) {
		char c = getc(stdin);
		if(isupper(c)) {
			ungetc(c, stdin);
			char oponent, me;
			int ligne;
			oponent = getchar();
			assert(getchar() == ' ');
			me = getchar();
			int points = rockpaperscisors(oponent, me);
			point_accumules += points;
			if(getchar() == EOF)
				ungetc(EOF, stdin);
		} else if(c == EOF) {
			stop = true;
			break;
		}
	}

	printf("%d\n", point_accumules);
}