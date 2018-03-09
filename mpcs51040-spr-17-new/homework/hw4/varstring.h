// NOTE the header protection here! (protect against double inclusion!)
#pragma once

#include <stdbool.h>

// varstring_t: better C string functionality!

// The goal is to provide a library (a header and corresponding .c file)
// which simplifies working with C character strings.
//
// It has the following improved features over traditional C strings;
//
//   - can store *any* character, including 0-bytes as part of the string.
//   - variable length: no need to decide on a fixed length.
//   - conversion to and from traditional C strings for easy integration
//     with existing code.
//     (Note that 

// This is a *declaration* of a struct only. The definition should go in your 
// .c file.
struct varstring_t;

typedef struct varstring_t varstring_t;


// ---- Below are the prototypes for the functions provided by our library.
// ---- Implement them in the corresponding .c file.

// Instead of passing a varstring_t * to every function, we will instead
// define a handle representing the varstring.
//
// (This is similar to how an open file is represented by a FILE * type, but
// instead of typing varstring_t * everywhere, we simply call 'varstring_t *'
// varstring_handle.
//
typedef varstring_t * varstring_handle;

// We also want to have a const representation
// Question: why is this not the same as:
//   typedef const varstring_handle varstring_const_handle;
typedef const varstring_t * varstring_const_handle;



// Create a new empty (length 0) varstring.
// The returned handle needs to be freed with varstring_free when no longer
// needed.
varstring_handle varstring_new();

// Create a new varstring and initialize it with the contents of the passed in
// C string.
// The returned handle needs to be freed with varstring_free when no longer
// needed.
//
// If str is NULL, then an empty varstring is created.
// The 0-byte marker indicating the end of the C style string is NOT added to
// the varstring.
//
// So: varstring_from("1234") returns a varstring for which varstring_length()
// returns 4.
varstring_handle varstring_from(const char * str);



// can include 0 bytes in contents
varstring_handle varstring_from_raw(const char * str, size_t bytes);

// Free a varstring
void varstring_free (varstring_handle h);


// Return the length of the varstring
unsigned int varstring_length (varstring_const_handle h);

// Return the Nth character of the varstring as an unsigned character, or
// returns -1 if the access is invalid (for example, the string doesn't have
// that many characters).
// The first character has position 0.
int varstring_get_char (varstring_const_handle h, unsigned int position);


// Change the position'th character in the string to c. Returns false
// if the position is invalid, true otherwise.
bool varstring_set_char (varstring_handle h, unsigned int position, char c);

// The following function convers a varstring into a regular C string.
// Note that *NO NEW MEMORY IS ALLOCATED*, instead the a pointer pointing
// into varstring memory is returned. In other words, there is no need to free
// the return value of this function.
//
// If the varstring contains any 0-bytes (which can't be represented as a
// traditional C string), the function returns a pointer to a C string
// containing up to (but not including) the first 0-byte character in the
// varstring.
const char * varstring_access (varstring_const_handle handle);


// Unlike the access function above, this function allocates a NEW traditional
// C string with the same contents as the varstring. The caller of this
// function is responsible for freeing the returned char * pointer.
//
// The function *always* returns a valid pointer, even if the varstring is
// empty (in which case a pointer to a single 0-character is returned).
char * varstring_convert (varstring_const_handle handle);


// Append a character to the end of the varstring.
void varstring_append_char (varstring_handle h, char c);

// Append a varstring to the end of another varstring.
// h += other
//
// IMPORTANT: Your function should work properly even if h and other are
// the same varstring...
void varstring_append_varstring (varstring_handle h, varstring_const_handle other);

// Append a traditional C string to the end of another varstring
// if c == NULL, it should be treated as an empty string.
void varstring_append_str(varstring_handle h, const char * c);


// Reverse the varstring
// i.e. if the string was 12345, after the call, it should be 54321.
void varstring_reverse (varstring_handle h);

// Replace the contents of the varstring with the given C string.
// Note that this does not create a new varstring, it only changes
// the contents of the existing one...
// 
// If c == NULL, it should be treated as an empty string.
// 
// This makes a COPY of c -- after the call returns c can be modified
// or freed without affecting the the contents of h.
void varstring_set_string (varstring_handle h, const char * c);

// Insert a string at the given position.
// Note that while varstring_set_string replaces the string,
// this function *inserts* in the existing string at the given position,
// shifting the existing string if needed.
//
// *It is valid to specify the first position past the end of the string,
// for which the effect of the call should be to append the string.
//
// Positions beyond that, the function should return false. For all positions
// from [0, length], the function should return true.
//
// For example, if h is "12346" then h should be "16782345" after executing:
// varstring_insert_str(h, 1, "678") -> true
bool varstring_insert_str(varstring_handle h, unsigned int position,
      const char * str);



