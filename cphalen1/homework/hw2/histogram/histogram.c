#include <stdio.h>
#include <stdlib.h>

int main() {
	char fileName[] = "histogram-in.txt";

	// Here we declare arrays that will store integers which represent the occurance of each symbol in the file we are reading, the first stores occurance of digits, the second occurance of characters
	int digitCounter[10];
	int characterCounter[26];

	// Populate our two arrays so that they all have an initial value of '0'
	int i;
	for(i = 0; i < 26; i++){
		if(i <= 10) {
			digitCounter[i] = 0;
		}
		characterCounter[i] = 0;
	}

	// Opening our file declared ealrier as fileName
	FILE * f = fopen(fileName, "r");
	char c;

	// Here we read through the file and mark occurances of characters and digitis
	while((c = fgetc(f)) != EOF) {
		if(c >= '0' && c <= '9') {
			digitCounter[c - '0']++;
		} else if(c >= 'a' && c <= 'z') {
			characterCounter[c - 'a']++; // Both capitalzied and lower case characters are treated as the same value
		} else if (c >= 'A' && c <= 'Z') {
			characterCounter[c - 'A']++;
		}
	}

	char toPrint;
	int x, y;
	for(x = 0; x < 10; x++) {
		if(digitCounter[x] == 0) { // Only print out the histogram line if this digit has appeared at least once
			continue;
		}
		toPrint = '0' + x;

		printf("%c: ", toPrint);
		for(y = 0; y < digitCounter[x]; y++) { // Print as many "#" symbols as there are occurances of the digit -- a makeshift histogram
			putchar('#');
		}

		printf(" (%i)\n", digitCounter[x]); // Final endnote shows the numeric value of the digit occurance for readability
	}

	for(x = 0; x < 26; x++) { // Only print out the histogram line if this character has appeared at least once
		if(characterCounter[x] == 0) {
			continue;
		}
		toPrint = 'a' + x;

		printf("%c: ", toPrint);
		for(y = 0; y < characterCounter[x]; y++) { // Print as many "#" symbols as there are occurances of the character -- a makeshift histogram
			putchar('#');
		}

		printf(" (%i)\n", characterCounter[x]); // Final endnote shows the numeric value of the character occurance for readability
	}

	// Cannot forget to close the file once we have opened it
	fclose(f);
	return EXIT_SUCCESS;
}
