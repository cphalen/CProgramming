#include <stdlib.h>
#include <stdio.h>
#include "palindrome.h"
#include <ctype.h>

#define MAX_LENGTH 1024

int main() {
	char inputString[MAX_LENGTH];
	bool palindrome;
	int i;
	// This is a do while loop, as we need the program to run at least once so that we have first get a word, and then see how long it is
	do {
		// printf( "Enter a phrase (leave blank to exit program): "); I removed this line beause the homework assignment seems very specific in the way of what we are allowed to output to the screen

		i = 0;
		char c;
		// We run through the characters to save an entire line into the inputString
		while((c = getchar()) != '\n') {
			// Only add a value to the string if it is blank or alphabetical
			if(isalnum(c) || isblank(c))
				inputString[i++] = c;
			else {
				i = -1; // We have to give i a non zero value or else the program will exit when really we want it just to run again
				continue;
			}
		}

		inputString[i] = '\0';
		char * string = &inputString[0];

		palindrome = is_palindrome(string);
		if(palindrome && i != 0) {
				printf("PALINDROME\n");
		} else if(i != 0) {
				printf("NOT PALINDROME\n");
		}
	} while (i != 0); // If the string is of length zero, thus meaning that they just entered the enter key alone, our program will quit

	// printf( "\n-- Exiting program --\n\n"); I removed this line beause the homework assignment seems very specific in the way of what we are allowed to output to the screen

	return EXIT_SUCCESS;
}
