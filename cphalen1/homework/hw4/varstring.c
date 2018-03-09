#include "varstring.h"
#include <string.h>
#include <stdlib.h>

struct varstring_t
{
  char * data;
  unsigned int length;
};

varstring_handle varstring_new() {
  varstring_handle current = (varstring_t *) malloc(sizeof(varstring_t));
  // Initialize data to be empty and length to be 0
  current->data = NULL;
  current->length = 0;
  return current;
}

varstring_handle varstring_from(const char * str) {
  varstring_handle current = varstring_new();
  if(str == NULL)
    return current;
  // We allocate the 0-byte to avoid errors from the compiler, but do not account for the 0-bytes in the length, so in essence they are ignored
  current->data = (char *)malloc(sizeof(char) * (strlen(str) + 1));
  memcpy(current->data, str, sizeof(char) * (strlen(str) + 1));
  current->length = strlen(str);

  return current;
}

void varstring_free (varstring_handle h) {
  // We do not free the pointer passed, we only set it to null, as the user may want to reuse
  free(h->data);
  h = NULL;
}

unsigned int varstring_length (varstring_const_handle h) {
  return h->length;
}

int varstring_get_char (varstring_const_handle h, unsigned int position) {
  if(position >= h->length) // Ensure the call is to a character within the range of the varstring's length
    return -1;
  return (*(h->data + (position * sizeof(char))));
}

bool varstring_set_char (varstring_handle h, unsigned int position, char c) {
  if(position >= h->length) // Ensure the call is to a character within the range of the varstring's length
    return false;
  *(h->data + (position * sizeof(char))) = c;
  return true;
}

const char * varstring_access (varstring_const_handle handle) {
  return handle->data;
}

char * varstring_convert (varstring_const_handle handle) {
  char * current = (char *)malloc((strlen(handle->data) + 1) * sizeof(char)); // Once again we include the 0-byte here as the string we return will need a 0-byte
  memcpy(current, handle->data, (strlen(handle->data) + 1) * sizeof(char));
  return current;
}

void varstring_append_char (varstring_handle h, char c) {
  char * temp = h->data; // The variable stores the data we will write to our new varstring
  h->data = (char *)malloc((h->length + 1) * sizeof(char)); // We include an extra character size here as we intent to set to be the character passed in the parameters
  memcpy(h->data, temp, h->length * sizeof(char)); // Copy over everything leading up to the last character we will append
  (h->data)[h->length++] = c;
  free(temp);
}

void varstring_append_varstring (varstring_handle h, varstring_const_handle other) {
  char * temp = h->data; // The variable stores the data we will write to our new varstring
  h->data = (char *)malloc((h->length + other->length) * sizeof(char));
  memcpy(h->data, temp, h->length * sizeof(char));
  memcpy(h->data + h->length, other->data, other->length * sizeof(char));
  h->length += other->length;
  free(temp);
}

void varstring_append_str(varstring_handle h, const char * c) {
  varstring_handle other = varstring_from(c);
  varstring_append_varstring(h, other);
  varstring_free(other); // We have to free this pointer within our varstring leftover space
  free(other);
}

void varstring_reverse (varstring_handle h) {
  for(int i = 0; i < h->length / 2; i++) {
    char temp = h->data[i]; // Temp stores one value as we switch the other so we can do our second switch without losing a value
    h->data[i] = h->data[h->length - i - 1]; // This additional -1 is here as the length of the varstring would be one past the final element as the varstrings are 0-indexed
    h->data[h->length - i - 1] = temp;
  }
}

void varstring_set_string (varstring_handle h, const char * c) {
  varstring_handle new = varstring_new();
  varstring_append_str(new, c);
  varstring_free(h); // We are inserting a new varstring for this pointer to point to, so we must first clear the varstring that it was pointing to originally to avoid leftover memory space
  *h = *new;
  free(new);
}

bool varstring_insert_str(varstring_handle h, unsigned int position, const char * str) {
  if(position >= h->length)
    return false;

  char * temp = h->data; // temp stores the pointer to our original data so we can copy it over
  h->data = (char *)malloc((h->length + strlen(str)) * sizeof(char));
  memcpy(h->data, temp, position * sizeof(char)); // Copy information before the break
  memcpy(h->data + position, str, (strlen(str) + 1) * sizeof(char)); // Insert the new information after the break
  memcpy(h->data + position + strlen(str), temp + position, (h->length - position) * sizeof(char)); // Copy over all other information after the break
  h->length += strlen(str);
  free(temp);
  return true;
}
