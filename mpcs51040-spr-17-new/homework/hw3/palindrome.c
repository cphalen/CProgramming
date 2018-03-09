#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "palindrome.h"

bool is_palindrome(const char * string) {
	int stringLength = strlen(string);

	for(int i = 0; i < (stringLength / 2); i++) {
		if(string[i] != string[(stringLength - 1) - i]) {
			return false;
		}
	}

	return true;
}
