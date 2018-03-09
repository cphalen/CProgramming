#include <stdlib.h>
#include <stdio.h>
#include "palindrome.h"
#include <ctype.h>

int main() {
	char inputString[1024];
	bool palindrome;
	int i;
	do {
		printf( "Enter a word (leave blank to exit program): ");

		i = 0;
		char c;
		while((c = getchar()) != '\n') {
			if(isalpha(c) || isblank(c))
				inputString[i++] = c;
		}

		inputString[i] = '\0';
		char * string = &inputString[0];

		palindrome = is_palindrome(string);
		if(palindrome) {
				printf("This phrase is palindromic\n\n");
		} else {
				printf("This phrase is NOT palindromic\n\n");
		}
	} while (i != 0);

	printf( "\n-- Exiting program --\n");

	return EXIT_SUCCESS;
}
