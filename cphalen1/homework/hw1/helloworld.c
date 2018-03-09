#include <stdio.h>
#include <stdlib.h>

int printHello(void) {
	char num = 'c';
	printf("Hello world! %i", num);
	return 0;
}

int printLine(void) {
	printf("\n");
	return 0;
}

int main(void) {
	printHello();
	printLine();
	return EXIT_SUCCESS;
}
