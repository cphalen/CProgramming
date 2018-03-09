#include "varstring.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** args)
{
   varstring_handle s = varstring_from("my first varstring");
   varstring_append_str(s, " was very easy to create!");
   puts(varstring_access(s));
   varstring_free(s);
   return EXIT_SUCCESS;
}


