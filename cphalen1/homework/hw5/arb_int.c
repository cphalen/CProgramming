#include "arb_int.h"
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h> // important for math in changing from string to int and the converse
#include <limits.h>
#ifdef DEBUG_ON
#include <stdio.h>
#endif
#include <stdio.h>

struct arb_int_struct {
	short int * value;
	char sign;
	unsigned int used; // Filled with actual data
	unsigned int allocated; // Allocated for data to fill
};


void arb_free(arb_int_t * i) {
	assert(i);
	free((*i)->value);
	free(*i); // We initialize this pointer so we are allow to free it as well -- the user passes a uninitialized arb_int_t
}

// Allocates memory for an arb_int_struct and returns this memory with default values inserted
// Returns 0 on success and -1 on failure
static int new_arb_internal(arb_int_t * new) {
	if(!new)
		return -1;
	(*new) = (arb_int_t) malloc(sizeof(arb_int_struct));
	(*new)->sign = 0;
	(*new)->used = 0;
	(*new)->allocated = 0;
	(*new)->value = NULL;
 	return 0;
}

// Allocates the proper amount of memory for arb_int_t i as it has not been allocated enough to be copied into
static int arb_realloc_internal(arb_int_t i, size_t allocated) {
	if(!i)
		return -1;

	if(i->used == 0) {
		// If we have nothing from the value of i to copy
		i->value = (short int *) malloc(allocated);
		i->allocated = allocated;
		return 0;
	}

	short int * temp = (short int *) malloc(i->used);
	memcpy(temp, i->value, i->used);
	free(i->value);

	i->value = (short int *) malloc(allocated);
	memcpy(i->value, temp, i->used);
	free(temp);
	i->allocated = allocated;

	return 0;
}

// Takes an arb_int value in a char dataype and returns the actual
// ASCII character value -- 1 becomes '1'
// postcondition: appends a 0-byte to the end of string
static char * tostring_arb_internal(const short int * s, size_t length) {
	char * tostringReturn = (char *) malloc((length + 1) * sizeof(char));
	for(int i = 0; i < length; i++) {
		*(tostringReturn + i) = (char) (*(s + i)) + '0';
	}
	*(tostringReturn + length) = '\0'; // Sets the final byte to be a 0-byte and end the string

	return tostringReturn;
}

// Takes an actual ASCII character value and returns the an arb_int
// value in a char data type -- '1' becomes 1
static short int * fromstring_arb_internal(const char * s, size_t length) {
	short int * tostringReturn = (short int *) malloc((length + 1) * sizeof(short int));
	for(int i = 0; i < length; i++) {
		if(isdigit(*(s + i)))
			*(tostringReturn + i) = (*(s + i)) - '0';
		else {
			// If we run into a non 0-9 character we will not process this string
			free(tostringReturn);
			tostringReturn = NULL;
			break;
		}
	}
	return tostringReturn;
}

// This function takes a pointer and two values, it will
// assign the pointer the value of the units place when
// the two values (which must be 0-9) are multiplied
// the carry from this operation is returned as an integer
// on success. On failure the function will return -1
static int arb_multiply_digits_internal(short int * total, short int valueOne, short int valueTwo, int carry) {
	if(!total)
		return -1;

	short int sumTotal = (valueOne * valueTwo) + carry;
	*total = sumTotal % 10;
	return sumTotal / 10;
}

// This function will take an arb_int_t value and
// then remove all the zeros preceeding the first
// actual number element in the value. This
// function is used for the most part to clean up
// after mulpication
// returns 0 on sucess and -1 on failure
static int arb_clear_leading_zeros(arb_int_t buffer) {
	if(!buffer)
		return -1;
	int i = 0;
	for(;*(buffer->value + i) == 0; i++);
	short int * replacement = (short int *) malloc(buffer->used - (i * sizeof(short int)));
	memcpy(replacement, buffer->value + i, buffer->used - (i * sizeof(short int)));
	free(buffer->value);
	buffer->value = replacement;
	buffer->allocated = buffer->used - (i * sizeof(short int));
	buffer->used = buffer->allocated;
	return 0;
}

int arb_duplicate(arb_int_t * new, const arb_int_t original) {
	if(!new || !original->value)
		return -1;

	new_arb_internal(new);
	arb_realloc_internal(*new, original->allocated);
	memcpy((*new)->value, original->value, (*new)->allocated); // Actually allocates new memory, not assigning to already existing memory
	(*new)->used = original->used;
	(*new)->sign = original->sign;

	return 0;
}

int arb_from_string (arb_int_t * i, const char * s) {
	if(!i || !s)
		return -1;

	arb_int_t swap;
	new_arb_internal(&swap);

	// Identify the sign
	if(*s == '+' || *s == '-') {
		swap->sign = *s;
		s++;
	} else if(isdigit(*s)) {
		swap->sign = '+';
	}

	arb_realloc_internal(swap, (strlen(s) + 1) * sizeof(short int)); // strlen(s) + 1 to accomodate for 0-byte

	short int * temp = fromstring_arb_internal(s, strlen(s)); // Function allocates a short int * that has all the values of a char * expected translated from character format to integer format (+ '0')
	if(!temp) {
		arb_free(&swap);
		return -1;
	}
	memcpy(swap->value, temp, strlen(s) * sizeof(short int));
	swap->used = strlen(s) * sizeof(short int); // Does not include the 0-byte that was written
	free(temp);

	*i = swap; // Finally give the passed value its new memory
	// We are allowed to assume that this *i does not need to be freed, as the user is responsibile for not inputing a arb_int_t value that carrys a value
	return 0;
}

int arb_from_int (arb_int_t * i, signed long long int source) {
	if(!i)
		return -1;

	arb_int_t swap;
	new_arb_internal(&swap);

	// Identify the sign
	if(source < 0) {
		swap->sign = '-';
		source *= -1; // Makes all of the math much easier in the below lines
	} else
		swap->sign = '+';
	int sourceLen = floor(log10(llabs(source))) + 1;
	arb_realloc_internal(swap, sourceLen * sizeof(short int));

	for(int x = 1; x <= sourceLen; x++) {
		// Divides each the long long int by 10^1 - 10^len where len is the length of the long long int
		// (how many digits) and then gives the remainder when divided by 10
		// this provides the value of each digit in order from last to first
		*(swap->value + (x - 1)) = (short int) (((long long int) (source / (long long int) pow(10.0, sourceLen - x))) % 10);
	}

	swap->used = sourceLen * sizeof(short int);

	*i = swap; // Finally give the passed value its new memory
	// We are allowed to assume that this *i does not need to be freed, as the user is responsibile for not inputing a arb_int_t value that carrys a value
	return 0;
}

int arb_to_string (const arb_int_t i, char * buf, size_t max) {
	if(!buf)
		return -1;

	if(!(i->used == sizeof(short int) && *(i->value) == 0))
		arb_clear_leading_zeros(i); // ensure there are no leading zeros

	// Identify the sign
	if(i->sign == '-' && max >= sizeof(char)) {
		*buf = '-';
		buf++; // Make sure to add the '-' character if necessary
		max -= sizeof(char);
	} else if(i->sign && max < sizeof(char)) {
		return -1;
	}

	if(max < (i->used / sizeof(short int) + 1) * sizeof(char)) // The + 1 here accounts for the 0-byte we will have to append at thened of the string
		return -1;

	char * tostringArb = tostring_arb_internal(i->value, i->used / sizeof(short int)); // This function takes a char * and returns a short int * that has the values in the char * translated into short int values (- '0')
	memcpy(buf, tostringArb, (i->used / sizeof(short int) + 1) * sizeof(char));
	free(tostringArb);
	return 0;
}

int arb_to_int (const arb_int_t i, long long int * out) {
	*out = 0; // Set the value to zero, even if a valid pointer is passed

	int llintMaxLength = floor(log10(llabs(LLONG_MAX))) + 1;
	if(!out || i->used / sizeof(short int) > llintMaxLength)
		return -1; // This value will be to large to represent using a long long int
	else if(i->used / sizeof(char) < llintMaxLength) {
		// Whent the value is smaller in length than the max we konw for sure it will be smaller
		for(int x = 1; x <= i->used / sizeof(short int); x++)
			*out += (*(i->value + (x - 1))) * (long long int) pow(10.0, (double) ((i->used / sizeof(short int)) - x));
	} else {
		for(int x = 1; x <= i->used / sizeof(short int); x++) {
			// Here we have to run checks after each interval of digits to check which value is larger
			short int digitPlace = (short int) (((long long int) (LLONG_MAX / (long long int) pow(10.0, llintMaxLength - x))) % 10);
			if(*(i->value + (x - 1)) == digitPlace) {
				// If the value of the interval digits is the same as the max we must carry on to the next interval to see which value is larger
        *out += (long long int) (digitPlace * pow(10.0, llintMaxLength - x));
      } else if(*(i->value + (x - 1)) < digitPlace) {
				// If the value of the interval digits is smaller we can assume the value is smaller than the max
				int y;
				for(y = x; y <= i->used / sizeof(short int); y++) {
					*out += (long long int) (*(i->value + (y - 1)) * (long long int) pow(10.0, (double) (llintMaxLength - y)));
				}
				x = y;
			} else  {
				// If the value is greater than the LLONG_MAX
				// then we cannot represent it using a long long int
				return -1;
			}
		}
	}

	if(i->sign == '-')
		*out *= -1;

	return 0;

}

int arb_assign (arb_int_t x, const arb_int_t y) {
	x->used = 0;
	x->allocated = 0;
	free(x->value);
	if(arb_realloc_internal(x, y->allocated)) // makes use of our realloc function as well
		return -1;
	memcpy(x->value, y->value, y->used);
	x->used = y->used;
	x->sign = y->sign;

	return 0;
}

// All implementation from here on out is for the addition,
// subtraction, multiplication, and divison functionality

// Adds the two short int values in valueOne and valueTwo
// and then returns the sum excluding any carry by reference through the short int
// pointer value sum.
// The function will return 1 if there is carry and 0 if there
// is no carry. Additionally it will return -1 if it encounters
// an error.
static int arb_add_digits_internal(short int * sum, short int valueOne, short int valueTwo) {
	if(!sum)
		return -1;

	short int sumTotal = valueOne + valueTwo;

	*sum = sumTotal % 10;

	return sumTotal > 9;
}

// Subtracts the second short int value in subtraction from the first in  value
// and then returns the value of the expression excluding any carry by reference through
// the short int pointer value total.
// The function will return 1 if there is a necessary pull from the higher digit
// and 0 if there is no pull. Additionally it will return -1 if it encounters
// an error.
static int arb_subtract_digits_internal(short int * total, short int value, short int subtraction) {
	if(!total)
		return -1;

	short int sumTotal = value - subtraction;
	if(sumTotal < 0)
		*total = 10 - abs(sumTotal % 10);
	else
		*total = sumTotal;
	if(*total == 10)
		*total = 0;

	return sumTotal < 0;
}

// Shifts all of the digits in a string of short int values up the line by one
// and pulls out the first value. For example the string of short int values
// 0 1 4 2 5 6 becomes 1 4 2 5 6
// returns 0 on success and -1 on failure
static int arb_shift_digits_internal(short int ** value, size_t currentLength) {
	if(!value)
		return -1;

	short int * shiftedValue = (short int *) malloc(currentLength - sizeof(short int));
	memcpy(shiftedValue, (*value) + 1, currentLength - sizeof(short int));
	free(*value);
	*value = shiftedValue;
	return 0;
}

// This function compares the values of two arb_int_t
// datatypes. It will return -1 if the first value is larger
// 0 if the two values are precisely equal, and 1 if the second
// value is larger. Additionally the function will return -10 on
// failure
static int compare_arb_internal(const arb_int_t x, const arb_int_t y) {
	if(!x || !y)
		return -10;
	if(x->used > y->used)
		return -1;
	else if(x->used < y->used)
		return 1;
	else {
		for(int i = 0; i < x->used / sizeof(short int); i++) {
			if(*(x->value + i) > *(y->value + i))
				return -1;
			else if (*(x->value + i) < *(y->value + i))
				return 1;
		}
		return 0;
	}
}

int arb_add (arb_int_t x, const arb_int_t y) {
	if(!x || !y)
		return -1;

	// This logic string ensures that the sign of values will
	// always be correct upon being returned.
	if(x->sign == '+' && y->sign == '-') {
		y->sign = '+';
		int returnValue = arb_subtract(x, y);
		y->sign = '-';
		return returnValue;
	} else if(x->sign == '-' && y->sign == '+') {
		y->sign = '-';
		int returnValue = arb_subtract(x, y);
		y->sign = '+';
		return returnValue;
	} else if(x->sign == '-' && y->sign == '-') {
		x->sign = '+';
		y->sign = '+';
		int returnValue = arb_subtract(x, y);
		x->sign = '-';
		y->sign = '-';
		return returnValue;
	}

	char larger;
	if(compare_arb_internal(x, y) == -1)
		larger = 'x';
	else
		larger = 'y';
	int additionLength = ((x->used >= y->used) * y->used) + ((x->used < y->used) * x->used);
	int maxPossibleLength = ((x->used <= y->used) * (y->used + sizeof(short int))) + ((x->used > y->used) * (x->used + sizeof(short int)));
	int carry = 0;
	short int * returnValue = (short int *) malloc(maxPossibleLength);
	for(int i = (maxPossibleLength / sizeof(short int)) - 1; i >= 0; i--) {
		if(i == 0) {
			arb_add_digits_internal((returnValue + i), 0 + carry, 0);
				#ifdef DEBUG_ON
				printf("\ninner (%i) %i + %i (+ carry %i) = %hd", i, 0, 0, carry, *(returnValue + i));
				#endif
		} else if(i < (maxPossibleLength / sizeof(short int)) - (additionLength / sizeof(short int))) {
			if(larger == 'x') {
				carry = arb_add_digits_internal((returnValue + i), *(x->value + i - 1) + carry, 0);
				#ifdef DEBUG_ON
				printf("\ninner (%i) %hd + %i (+ carry %i) = %hd", i, *(x->value + i - 1), 0, carry, *(returnValue + i));
				#endif
			} else if(larger == 'y') {
				carry = arb_add_digits_internal((returnValue + i), 0 + carry, *(y->value + i - 1));
				#ifdef DEBUG_ON
				printf("\ninner (%i) %hd + %i (+ carry %i) = %hd", 0, *(y->value + i - 1), 0, carry, *(returnValue + i));
				#endif
			}
				// 0 is included here just to show explicitly that we are substituting
				// 0 in the operation for the out of bounds value of *(x + i)
		} else {
			if(larger == 'x') {
				carry = arb_add_digits_internal((returnValue + i), *(x->value + i - 1) + carry, *(y->value + i - ((maxPossibleLength - additionLength) / sizeof(short int))));
				#ifdef DEBUG_ON
				printf("\nlevel (%i) %hd + %hd (+ carry %i) = %hd", i, *(x->value + i - 1), *(y->value + i - ((maxPossibleLength - additionLength) / sizeof(short int))), carry, *(returnValue + i));
				#endif
			} else if(larger == 'y') {
				carry = arb_add_digits_internal((returnValue + i), *(x->value + i  - ((maxPossibleLength - additionLength) / sizeof(short int))) + carry, *(y->value + i - 1));
				#ifdef DEBUG_ON
				printf("\nlevel (%i) %hd + %hd (+ carry %i) = %hd", i, *(x->value + i - ((maxPossibleLength - additionLength) / sizeof(short int))), *(y->value + i - 1), carry, *(returnValue + i));
				#endif
			}
		}
		if(carry == -1)
			return -1;
	}
	#ifdef DEBUG_ON
	printf("\n\n");
	#endif
	if(carry == 1) {
		free(x->value);
		x->value = returnValue;
		x->allocated = maxPossibleLength;
		x->used = maxPossibleLength;
		return 0;
	} else {
		if(arb_shift_digits_internal(&returnValue, maxPossibleLength) == -1)
			return -1;
		free(x->value);
		x->value = returnValue;
		x->allocated = maxPossibleLength - sizeof(short int);
		x->used = maxPossibleLength - sizeof(short int);
		return 0;
	}
}

int arb_subtract (arb_int_t x, const arb_int_t y) {
	// This long string of logic deals with all of the sign issues when multiplying and additing
	// in essence what it will do is always subtract the small value from the larger value
	// and then swap the values afterwards to achieve the correct sign
	if(x->sign == '+' && y->sign == '-') {
		y->sign = '+';
		int returnValue = arb_add(x, y);
		y->sign = '-';
		return returnValue;
	} else if(x->sign == '-' && y->sign == '+') {
		x->sign = '+';
		int returnValue = arb_add(x, y);
		x->sign = '-';
		return returnValue;
	} else if(x->sign == '-' && y->sign == '-') {
		int returnValue;
		x->sign = '+';
		y->sign = '+';
		if(compare_arb_internal(x, y) == -1) {
			returnValue = arb_subtract(x, y);
			x->sign = '-';
		} else if(compare_arb_internal(x, y) == 1) {
			short int * tempValue = (short int *) malloc(y->allocated * sizeof(short int));
			memcpy(tempValue, y->value, y->used);
			unsigned int allocated = y->allocated;
			unsigned int used = y->used;
			returnValue = arb_subtract(y, x);
			free(x->value);
			x->value = y->value;
			x->allocated = y->allocated;
			x->used = y->used;
			x->sign = '+';
			y->value = tempValue;
			y->allocated = allocated;
			y->used = used;
		} else {
			free(x->value);
			x->value = (short int *) malloc(sizeof(short int));
			*(x->value) = 0;
			x->used = sizeof(short int);
			x->allocated = sizeof(short int);
			returnValue = 0;
		}
		y->sign = '-';
		return returnValue;
	}

	if(compare_arb_internal(x, y) == 0) {
		free(x->value);
		x->value = (short int *) malloc(sizeof(short int));
		*(x->value) = 0;
		x->used = sizeof(short int);
		x->allocated = sizeof(short int);
		return 0;
	} else if(compare_arb_internal(x, y) == 1) {
		// Implicitly will only run when both x->sign and y->sign are postitive
		short int * tempValue = (short int *) malloc(y->allocated * sizeof(short int));
		memcpy(tempValue, y->value, y->used);
		unsigned int allocated = y->allocated;
		unsigned int used = y->used;
		int returnValue = arb_subtract(y, x);
		free(x->value);
		x->value = y->value;
		x->allocated = y->allocated;
		x->used = y->used;
		x->sign = '-';
		y->value = tempValue;
		y->allocated = allocated;
		y->used = used;
		return returnValue;
	}

	char larger;
	if(compare_arb_internal(x, y) == -1) {
		larger = 'x';
	} else if(compare_arb_internal(x, y) == 1) {
		larger = 'y';
	} else {
		free(x->value);
		x->value = (short int *) malloc(sizeof(short int));
		*(x->value) = 0;
		x->used = sizeof(short int);
		x->allocated = sizeof(short int);
		return 0;
	}

	int maxPossibleLength = ((x->used <= y->used) * (y->used)) + ((x->used > y->used) * (x->used));
	int subtractionLength = ((x->used >= y->used) * y->used) + ((x->used < y->used) * x->used);
	int pull = 0;
	short int * returnValue = (short int *) malloc(maxPossibleLength);
	for(int i = maxPossibleLength / sizeof(short int) - 1; i >= 0; i--) {
		if(i < (maxPossibleLength / sizeof(short int)) - (subtractionLength / sizeof(short int))) {
			// if(larger == 'x') {
			pull = arb_subtract_digits_internal((returnValue + i), *(x->value + i) - pull, 0);
			#ifdef DEBUG_ON
			printf("\ninner (%i) %hd - %i (- pull %i) = %hd", i, *(x->value + i), 0, pull, *(returnValue + i));
			#endif
				// 0 is included here just to show explicitly that we are substituting
				// 0 in the operation for the out of bounds value of *(x + i)
		} else {
			if(larger == 'x') {
				pull = arb_subtract_digits_internal((returnValue + i), *(x->value + i) - pull, *(y->value + i - ((maxPossibleLength - subtractionLength) / sizeof(short int))));
				#ifdef DEBUG_ON
				printf("\nlevel (%i) %hd - %hd (- pull %i) = %hd", i, *(x->value + i), *(y->value + i - ((maxPossibleLength - subtractionLength) / sizeof(short int))), pull, *(returnValue + i));
				#endif
			} else if(larger == 'y') {
				pull = arb_subtract_digits_internal((returnValue + i), *(x->value + i  - (maxPossibleLength / sizeof(short int))) - pull, *(y->value + i));
				#ifdef DEBUG_ON
				printf("\nlevel (%i) %hd - %hd (- pull %i) = %hd", i, *(x->value + i - ((maxPossibleLength - subtractionLength) / sizeof(short int))), *(y->value + i), pull, *(returnValue + i));
				#endif
			}
		}

		if(pull == -1)
			return -1;
	}
	#ifdef DEBUG_ON
	printf("\n\n");
	#endif
	if(*(returnValue) != 0) {
		free(x->value);
		x->value = returnValue;
		x->allocated = maxPossibleLength;
		x->used = maxPossibleLength;
		return 0;
	} else {
		if(arb_shift_digits_internal(&returnValue, maxPossibleLength) == -1)
			return -1;
		free(x->value);
		x->value = returnValue;
		x->allocated = maxPossibleLength - sizeof(short int);
		x->used = maxPossibleLength - sizeof(short int);
		return 0;
	}
}

// The goal of this function is to an the number of zeros
// approriate after a given string. For the function of when
// multiplying values making the appending of zeros is easier
// returns 0 on sucess and -1 on failure
static int arb_add_zeros_internal(arb_int_t x, int numZeros) {
	if(!x)
		return -1;
	short int * replacementValue = (short int *) malloc(x->used + (numZeros * sizeof(short int)));
	memcpy(replacementValue, x->value, x->used);
	for(int i = 0; i < numZeros; i++)
		*(replacementValue + (x->used / sizeof(short int)) + i) = 0;
	free(x->value);
	x->value = replacementValue;
	x->used += numZeros * sizeof(short int);
	x->allocated = x->used + (numZeros * sizeof(short int));
	return 0;
}

int arb_multiply (arb_int_t x, const arb_int_t y) {
	arb_clear_leading_zeros(x);
	if(!x || !y)
		return -1;

	// These arb_int_t data structures are made so we can reuse the arb_add funtionality previously implemented to make multiplicaiton easier to impement
	arb_int_t finalSum = (arb_int_t) malloc(sizeof(arb_int_struct));
	finalSum->allocated = ((x->used * (x->used > y->used)) + (y->used * (x->used <= y->used))) * 2;
	finalSum->sign = '+';
	finalSum->used = 0;
	finalSum->value = (short int *) calloc(finalSum->allocated / sizeof(short int), finalSum->allocated); // Maximum length of this multiplication
	*(finalSum->value) = 0;

	arb_int_t current = (arb_int_t) malloc(sizeof(arb_int_struct));
	current->allocated = (x->used / sizeof(short int)) * 2;
	current->sign = '+';
	current->used = 0;
	current->value = (short int *) calloc(current->allocated / sizeof(short int), current->allocated); // Maximum length of any operation in this multiplication
	int carry = 0;

	for(int i = x->used / sizeof(short int) - 1; i >= 0; i--) {

		// Clears the current value
		for(int i = 0; i < current->used / sizeof(short int); i++)
			*(current->value + i) = 0;
		carry = 0;

		for(int j = y->used / sizeof(short int) - 1; j >= 0; j--) {
			carry = arb_multiply_digits_internal(current->value + j + 1, *(x->value + i), *(y->value + j), carry);
			#ifdef DEBUG_ON
			printf("\ninner (%i)(%i) %hd * %hd + (carry = %i) = %hd", i, j, *(x->value + i), *(y->value + j), carry, *(current->value + j + 1));
			#endif
		}
		#ifdef DEBUG_ON
		printf("\n\n");
		#endif

		if(carry != 0) {
			*(current->value) = carry;
			current->used = y->used + sizeof(short int);
			#ifdef DEBUG_ON
			printf("\ncarry + (carry = %i) = %hd", carry, *(current->value));
			#endif
		} else {
			current->used = y->used + sizeof(short int);
		}
		if(current->used > finalSum->used)
			finalSum->used = current->used;
		arb_add_zeros_internal(current, x->used / sizeof(short int) - (i + 1));
		if(arb_add(finalSum, current) == -1)
			return -1;
	}

	// Ensure the sign of our final value is correct
  if((x->sign == '+' && y->sign == '+') || (x->sign == '-' && y->sign == '-'))
          finalSum->sign = '+';
  else
          finalSum->sign = '-';

	arb_free(&current);
	arb_assign(x, finalSum);
	arb_free(&finalSum);
	return 0;
}

// int arb_divide(arb_int_t x, arb_int_t y) {
// 	if(!x || !y)
// 		return -1;
//
// 	if(compare_arb_internal(x, y) == 1)
// 		return 0;
// 	else if(compare_arb_internal(x, y) == 0)
// 		return 1;
//
// 	arb_int_t finalSum = (arb_int_t) malloc(sizeof(arb_int_struct));
// 	finalSum->allocated = x->used - y->used + 1;
// 	finalSum->sign = '+';
// 	finalSum->used = x->used;
// 	finalSum->value = (short int *) calloc(finalSum->allocated / sizeof(short int), finalSum->allocated); // Maximum length of this multiplication
// 	*(finalSum->value) = 0;
//
// 	arb_int_t current = (arb_int_t) malloc(sizeof(arb_int_struct));
// 	current->allocated = (x->used / sizeof(short int)) * 2;
// 	current->sign = '+';
// 	current->used = 0;
// 	current->value = (short int *) calloc(current->allocated / sizeof(short int), current->allocated); // Maximum length of any operation in this multiplication
//
// 	arb_assign(current, y);
//
// 	for(int i = 0; i < finalSum->allocated / sizeof(short int); i++) {
// 		int zerosAppended = 0;
// 		while(compare_arb_internal(x, current) == -1) {
// 			arb_add_zeros_internal(current, 1);
// 			zerosAppended++;
// 		}
// 		x->used -= sizeof(short int);
//
// 		for(int i = 0; i < current->used / sizeof(short int); i++)
// 			printf("\ncurrent value is = %hd", *(current->value + i));
//
// 		int count = 0;
// 		while(*(x->value + i) != 0 && x->sign == '+') {
// 			count++;
// 			for(int i = 0; i < current->used / sizeof(short int); i++)
// 				printf("\ncurrent value is = %hd - at i = %i", *(current->value + i), i);
// 			for(int i = 0; i < x->used / sizeof(short int); i++)
// 				printf("\nx value is = %hd - at i = %i", *(x->value + i), i);
//
// 			arb_subtract(x, current);
// 		}
// 		printf("\nCount = %i -- at i = %i", count, i);
// 		// arb_clear_leading_zeros(x);
// 		for(int i = 0; i < x->used / sizeof(short int); i++)
// 			printf("\n final x value is = %hd", *(x->value + i));
//
//
// 	}
//
// 	arb_assign(x, finalSum);
//
// 	return 0;
// }
