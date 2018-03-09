// Double inclusion protection
# pragma once

#include <stdlib.h>

// Will be defined in implementation
struct arb_int_struct;
/* {
        short int * value;
        char sign;
        unsigned int used;
        unsigned int allocated;
};*/

// This is mainly for internal convinience
typedef struct arb_int_struct arb_int_struct;

// This typedef allows the implementation user to acess arb_int structs without
// ever having to interact with the struct. The implementation will handle
// that side of the data management for the user.
typedef arb_int_struct * arb_int_t;

// Frees an arb_int from memory
void arb_free (arb_int_t * i);

// Takes a pointer to a new arb_int_t that will be allocated memory that is
// identical to the memory in the original arb_int_t, but it is imperative
// that the user know the data is being copied, as opposed to the new
// arb_int just being set to point to the originial arb_int
// returns 0 on success and non-zero on failure.
int arb_duplicate (arb_int_t * new, const arb_int_t original);

// Takes a pointer to an art_int_t and a const char pointer to char s.
// The values in s will be treated as numbers unless non-numerical with the
// exclusion of "+" and "-" in the initial slot to indecate sign
// returns - on success and non-zero on failure. 
int arb_from_string (arb_int_t * i, const char * s);

// Takes a pointer to an arb_int_t and a signed long long integer, which
// is the longest possible integer value attainable in C.
// The arb_int_t pointer will be assigned the value of source.
// returns zero on success and non-zero on failure .
int arb_from_int (arb_int_t * i, signed long long int source);

// Takes a pointer to a const arb_int_t pointer and will print out the value of the arb_int_t that our pointer points to by assigned that value to the
//  character pointer value. A size_t max is included so the function can
// quickly ensure that enough memory is allocated for the character pointer.
// negative numbers are returned in a string with a leading "-" character.
// returns zero on success and non-zero on failure 
int arb_to_string (const arb_int_t i, char * buf, size_t max);

// Takes a constant arb_int_t and a long long integer that will
// be given the value of the arb_int_t if possible.
// If the size of the arb_int_t exceeds the value that a long long int is
// capable of possesing the function will fail.
// returns zero on success and non-zero on failure.
int arb_to_int (const arb_int_t i, long long int * out);

// Takes an arb_int_t and a const arb_int_t. The value of the const 
// art_int_t is written to the arb_int_t,  but arb_int_t does not
// simply point to the value contained within the const arb_int_t,
// instead new memory is allocated and the of the const arb_int_t
// is copied.
// returns zero on success and non-zero on failure.
int arb_assign (arb_int_t x, const arb_int_t y);

// Relatively straightfoward, takes two arb_int_t values and
// adds the value of the const arb_int_t to the value of the
// arb_int_t/
// returns zero on success and non-zero on failure.
int arb_add (arb_int_t x, const arb_int_t y);

// Also somewhat self explanatory. Takes a arb_int_t and a
// const arb_int_t, the value of the latter is substracted from
// the value of the former and saved to the value of the formed.
// returns zero on success and non-zeor on failure.
int arb_subtract (arb_int_t x, const arb_int_t y);

// This is slightly more difficult. Parameters are arb_int_t and a
// const arb_int_t, the const arb_int_t is the multplier for the
// arb_int_t value. After multiplication the product is saved to the
// arb_int_t parameter.
// returns zero on success and non-zero on failure
int arb_multiply (arb_int_t x, const arb_int_t y);

// Also on the more difficult to understand side. Takes both 
// an arb_int_t and a const arb_int_t in that order. The arb_int_t 
// value is divided by the const arb_int_t value and saved to the
// arb_int_t variable.
// returns zero on success and non-zero on failure.
int arb_divide (arb_int_t x, const arb_int_t y);

// This is a comparision function that takes two const arb_int_t values
// does the operation to compare which value is larger. If the first
// parameter is of a larger value the function returns -1, if the two
// parameters are of equal value the function returns 0, and finally
// if the second parameter is of a larger size the function returns 1.
int arb_compare (const arb_int_t x, const arb_int_t y);
