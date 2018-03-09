#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char ** args)
{
   int * ip = (int *)  malloc (sizeof(*ip));

   // NOTE: ip is NOT INITIALIZED to 0 for you
   *ip = 0;

   printf ("My integer = %i\n", *ip);

   // (A) Try uncommenting the line below and checking with valgrind...
   //ip = 0;

   // (B) Or try this one (with or without line C)
   //ip -= 9;

   // (C) Or try uncommenting the line below (but not A or B above)
   // and check the difference
   // free (ip);


   return EXIT_SUCCESS;
}
