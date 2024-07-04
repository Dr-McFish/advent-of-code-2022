#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>


int max(int a, int b) {
	return a > b ? a : b;
}

int sym_points(char c) {
	return c - 'A' + 1;
}


int rockpaperscisors(char oponent, char outcomme) {
	//oponent += 'A';
	int rt;
	int me;

	
	if (outcomme == 'X') {//lose
		rt = 0;

		if(oponent == 'A') {
			me = 'C';
		} else {
			me = oponent - 1;
		}
	} else if (outcomme == 'Y') {//draw
		rt = 3;

		me = oponent;
	} else if (outcomme == 'Z') {//win
		rt = 6;

		if(oponent == 'C') {
			me = 'A';
		} else {
			me = oponent + 1;
		}		
	}

	rt += sym_points(me);

	printf("%d\n", rt);
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
			if(getchar() != '\n')
				ungetc(EOF, stdin);
		} else if(c == EOF) {
			stop = true;
			break;
		}
	}

	printf("%d\n", point_accumules);
}