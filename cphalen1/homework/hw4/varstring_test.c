#include "varstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void testFunctionality() {
  // This function tests all of the methods we programmed to agument and utilize
  // varstrings, we can call this program with no parameters from the command line
  // to quickly run this function and ensure all of the proper functionality is running correctly

  varstring_handle handle = varstring_new();
  varstring_append_str(handle, "Hello");
  assert(strncmp(varstring_access(handle), "Hello", varstring_length(handle)) == 0);
  varstring_append_char(handle, ' ');
  varstring_append_str(handle, "World");
  varstring_append_char(handle, '\0');
  assert(strncmp(varstring_access(handle), "Hello World", varstring_length(handle)) == 0);

  varstring_handle other = varstring_from("Hello UChicago");
  assert(strncmp(varstring_access(other), "Hello UChicago", varstring_length(other)) == 0);
  assert(varstring_length(other) == 14);
  varstring_set_char(other, 5, '\0');
  char * accessString = varstring_convert(other);
  assert(strncmp(accessString, "Hello", strlen(accessString)) == 0);
  assert(*varstring_access(other) == 'H');
  assert(*(varstring_access(other)+ 5) == '\0');
  assert(*(varstring_access(other) + 6) == 'U');

  varstring_set_string(other, "Hello C Programming");
  assert(strncmp(varstring_access(other), "Hello C Programming", varstring_length(other)) == 0);

  varstring_reverse(other);
  assert(strncmp(varstring_access(other), "gnimmargorP C olleH", varstring_length(other)) == 0);
  varstring_reverse(other);

  varstring_handle greeting = varstring_from("helloworld");
  assert(strncmp(varstring_access(greeting), "helloworld", varstring_length(greeting)) == 0);
  varstring_insert_str(greeting, 5, " big wide ");
  assert(strncmp(varstring_access(greeting), "hello big wide world", varstring_length(greeting)) == 0);

  varstring_append_varstring(other, greeting);
  assert(strncmp(varstring_access(other), "Hello C Programminghello big wide world", varstring_length(other)) == 0);
  varstring_insert_str(other, 19, " -- ");
  assert(strncmp(varstring_access(other), "Hello C Programming -- hello big wide world", varstring_length(other)) == 0);

  printf("\nThe functionality test ran without encountering issues\n\n");

  // We have to ensure that we clear all of the memoryspace we allocated calling varstring functions
  varstring_free(handle);
  varstring_free(greeting);
  varstring_free(other);
  free(other);
  free(accessString);
  free(handle);
  free(greeting);
}

int main (int argc, char ** args) {
  if(argc == 1) {
    testFunctionality();
    return EXIT_SUCCESS;
  }

  // When called with arguments from the commandline this function will append each argument to be part of a single varstring

  varstring_handle handle = varstring_new();

  int i;
  for(i = 0; i < argc; i++) {
    varstring_append_str(handle, *(++args));
  }

  // We print each of the letters in our new varstring
  putchar('\n');
  for(i = 0; i < varstring_length(handle); i++) {
    putchar(*(varstring_access(handle) + i));
  }
  putchar('\n');
  putchar('\n');

  // We then clear this varstring to ensure that no memory is left in use
  varstring_free(handle);
  free(handle);
}
