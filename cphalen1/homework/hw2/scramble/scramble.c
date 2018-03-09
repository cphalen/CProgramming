#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 128

int main (int argc, char * args[]) {

	FILE * f;

	// Here we want to ensure that the file being passed from the command line is an actual file
	if(args[1] != NULL) {
		f = fopen(args[1], "r");
	} else {
		printf("ERROR: Command line file passed not found: %s\n", args[1]);
		return EXIT_SUCCESS;
	}

	char c;
	char currentWord[MAX_WORD_LENGTH];
	int letterCounter = 0;
	//This two variables below will be used in our vested for loops
	int x, y;
	//We need these variables to track what our random number is, whether it has already occured in the list, and the actual list which will hold our random numbers
	int currentRandom, unqiueRandom, randoms[MAX_WORD_LENGTH];

	while((c = fgetc(f)) != EOF) {
		if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
			currentWord[letterCounter++] = c;
		}

		// Once we reach a space we want to scramble the letters we have been collecting up until now
		if(c == ' ') {
			putchar('\n');

			// This short loop resets the values in the random so we are not comparing values to that of the last iteration
			for(x = 0; x < letterCounter; x++) {
				randoms[x] = -1;
			}

			// We want to generate a set of random numbers that has the same length as that of the word we are trying to scramble, this list will represent the location of our old letters in our new word
			for(x = 0; x < letterCounter;) {
				currentRandom = rand() % letterCounter;
				unqiueRandom = 1;

				// This loop is necessary to check that each random number we place into our array is unqiue, that way we cannot have repeats, and thus no letter can be allocated to the same space in the new, randomized string
				for(y = 0; y < letterCounter; y++) {
					if(currentRandom == randoms[y]) {
						unqiueRandom = 0;
						break;
					}
				}

				// Only if the random number is unqiue to the earlier numbers in the list will we add it to the list of random numbers
				if(unqiueRandom == 1) {
					randoms[x++] = currentRandom;
				}

			}

			// Once we have created the entire random string we will print out the string, swapping the characters into their new, randomized positions
			for(x = 0; x < letterCounter; x++) {
				putchar(currentWord[randoms[x]]);
			}

			letterCounter = 0;
		}
	}

	return EXIT_SUCCESS;
}
