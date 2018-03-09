//
// Demonstrates problem if headers can be included (indirectly) multiple times
// in the same C file.
//
// Also demonstrates string access
//

#include "mylib.h"
#include "mylib2.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//
// args[0] is the name of the program
// args[1] (available only if argc>1) is the first command line argument
//
int main (int argc, char ** args)
{
   if (argc != 2)
   {
      puts("Need command line argument!");
      return EXIT_FAILURE;
   }

   // OK, we have one command line argument
   // Copy it into our own buffer
   char mybuffer[16];
   if (strlen(args[1])>=sizeof(mybuffer))
   {
      puts("Command line argument too long!");
      return EXIT_FAILURE;
   }

   // OK, room for string + 0-terminating byte
   strcpy(mybuffer, args[1]);

   // Now call our own function to make the string lowercase
   char mybuffer2[100];
   copy_and_lower(mybuffer2, mybuffer, sizeof(mybuffer2));

   printf("I received '%s' and turned it into '%s'!\n", mybuffer, mybuffer2);
   return EXIT_SUCCESS;
}

