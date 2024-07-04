#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>



int max(int a, int b) {
	return a > b ? a : b;
}

int main() {
	bool stop = false;
	int elf_max = 0;

	while (!stop)
	{
		int elf = 0;
		int res = 1;
		while (res) {
			char c = getc(stdin);
			if(isdigit(c)) {
				ungetc(c, stdin);
				int ligne;
				res = scanf("%d", &ligne);
				elf += ligne;
			} else if(c == '\n') {
				char c2;
				if ((c2 = getc(stdin)) == '\n')
					break;
				else
					ungetc(c2, stdin);
			} else if(c == EOF) {
				stop = true;
				break;
			}
		}

		elf_max = max(elf_max, elf);
	}
	
	printf("%d\n", elf_max);
}