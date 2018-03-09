#include <stdio.h>
#include <stdlib.h>

// TODO: try running the program with and without command line arguments.
// Also try passing arguments containing spaces
int main (int argc, char * args[])
{
   printf ("argc=%i:\n", argc);
   for (int i=0; i<argc; ++i)
   {
      printf("Argument %i: '%s'\n", i, args[i]);
   }
   return EXIT_SUCCESS;
}
