#include "arb_int.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void printOutArb(arb_int_t t);

void functionalityCheck() {
	// Function checks to ensure that the arb_int.c implementation is working correctly
	// checks all of the functions and tries to expose edge cases

	arb_int_t x;
	arb_int_t y;
	arb_int_t z;
	arb_from_string(&x, "817");
	arb_from_string(&y, "-1024");
	arb_from_string(&z, "+718");

	arb_int_t w;
	arb_int_t q;
	arb_from_int(&w, 2048);
	arb_from_int(&q, -1920);

	// Ensures that the implementation is working correctly as the arb_int_t value
	// approaches the maximum for what a computer can display in an interger value
	arb_int_t max;
	arb_from_int(&max, 9223372036854775807LL);
	arb_int_t nearMax;
  arb_from_int(&nearMax, 9223372033854775807LL);

	// We will write different values of arb_int_t's to this buffer
	char buffer[256] = {0};

	arb_to_string(x, buffer, 256);
	assert(strncmp(buffer, "817", strlen(buffer)) == 0);

	arb_to_string(y, buffer, 256);
	assert(strncmp(buffer, "-1024", strlen(buffer)) == 0);

	arb_to_string(z, buffer, 256);
	assert(strncmp(buffer, "718", strlen(buffer)) == 0);

	arb_to_string(w, buffer, 256);
	assert(strncmp(buffer, "2048", strlen(buffer)) == 0);

	arb_to_string(q, buffer, 256);
	assert(strncmp(buffer, "-1920", strlen(buffer)) == 0);

	long long int filler = 0;

	arb_to_int(z, &filler);
	assert(718 == filler);

	arb_to_int(w, &filler);
	assert(2048 == filler);

	arb_to_int(q, &filler);
	assert(-1920 == filler);

	arb_to_int(max, &filler);
	assert(9223372036854775807LL == filler);

	arb_to_int(nearMax, &filler);
 	assert(9223372033854775807LL == filler);

	arb_assign(w, q);

	arb_to_int(w, &filler);
  assert(-1920 == filler);

	arb_int_t new;

	arb_duplicate(&new, q);

	arb_to_int(w, &filler);
	assert(-1920 == filler);

	arb_int_t a;
	assert(arb_from_string(&a, "NotAString") != 0);

	arb_int_t add;
	arb_from_int(&add, 100);

	arb_int_t additionValue;
	arb_from_int(&additionValue, 5);

	arb_add(add, additionValue);
	arb_to_int(add, &filler);
	assert(105 == filler);

	arb_int_t carry;
	arb_from_int(&carry, 990);

	arb_int_t carryValue;
	arb_from_int(&carryValue, 110);

	arb_add(carry, carryValue);
	arb_to_int(carry, &filler);
	assert(1100 == filler);

	arb_add(add, carry);
	arb_to_int(add, &filler);
	assert(1205 == filler);

	arb_int_t subtract;
	arb_from_int(&subtract, 110);

	arb_int_t subtractor;
	arb_from_int(&subtractor, 90);

	arb_subtract(subtract, subtractor);
	arb_to_int(subtract, &filler);
	assert(20 == filler);

	arb_free(&subtract);
	arb_from_int(&subtract, -110);
	arb_subtract(subtract, subtractor);
	arb_to_int(subtract, &filler);
	assert(-200 == filler);

	arb_free(&subtractor);
	arb_from_int(&subtractor, -210);
	arb_subtract(subtract, subtractor);
	arb_to_int(subtract, &filler);
	assert(10 == filler);

	arb_subtract(subtract, subtractor);
	arb_to_int(subtract, &filler);
	assert(220 == filler);

	arb_add(add, subtractor);
	arb_to_int(add, &filler);
	assert(995 == filler);

	arb_free(&add);
	arb_from_int(&add, -90);
	arb_add(add, subtract);
	arb_to_int(add, &filler);
	assert(130 == filler);

	arb_int_t value;
	arb_from_int(&value, 10);

	arb_int_t multplier;
	arb_from_int(&multplier, 5);

	arb_multiply(value, multplier);
	arb_to_int(value, &filler);
	assert(50 == filler);

	arb_free(&multplier);
	arb_from_int(&multplier, 15);

	arb_multiply(value, multplier);
	arb_to_int(value, &filler);
	assert(750 == filler);

	arb_free(&multplier);
	arb_from_int(&multplier, -3);

	arb_multiply(value, multplier);
	arb_to_int(value, &filler);
	assert(-2250 == filler);

	// arb_int_t divide;
	// arb_from_int(&divide, 100);
	//
	// arb_int_t dividend;
	// arb_from_int(&dividend, 10);
	//
	// arb_divide(divide, dividend);
	// arb_to_int(divide, &filler);

	// Free all of the arb_int_t values we have initialized
	arb_free(&x);
	arb_free(&y);
	arb_free(&z);
	arb_free(&q);
	arb_free(&w);
	arb_free(&max);
	arb_free(&nearMax);
	arb_free(&new);
	arb_free(&add);
	arb_free(&additionValue);
	arb_free(&carry);
	arb_free(&carryValue);
	arb_free(&subtract);
	arb_free(&subtractor);
	arb_free(&value);
	arb_free(&multplier);
	printf("Diagnostic functionality check complete -- all implementation working as expected\n");
}

int main (int argc, char ** args) {
	if(argc == 1)
		functionalityCheck();
	else if(argc == 4) {
		arb_int_t num1;
		arb_int_t num2;
		// Read in arguments and deny arguments that are not legal
		if(arb_from_string(&num1, args[1]) == -1)
			return EXIT_FAILURE;
		if(arb_from_string(&num2, args[3]) == -1)
			return EXIT_FAILURE;
		// Identify sign
		if(*args[2] == '+')
			arb_add(num1, num2);
		else if(*args[2] == '-')
			arb_subtract(num1, num2);
		else if(*args[2] == '*')
			arb_multiply(num1, num2);
		else
			return EXIT_FAILURE;

		// Read score out and print to command line, then exit program

		long int bufferSize = ((strlen(args[1]) + 1) * (strlen(args[3]) + 1)) * sizeof(char);
		char buffer[bufferSize];
		arb_to_string(num1, buffer, bufferSize);

		printf("Your calculation = %s\n", buffer);
		arb_free(&num1);
		arb_free(&num2);
	}
	return EXIT_SUCCESS;
}
