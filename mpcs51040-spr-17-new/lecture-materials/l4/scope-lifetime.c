#include <stdio.h>
#include <stdlib.h>

int a = 10;

int global_variable()
{
   ++a;
   printf("Value of a (global variable) = %i\n", a);
   return a;
}

int local_variable()
{
   int a = 10;
   ++a;
   printf("Value of a (local variable) = %i\n", a);
   return a;
}

int local_static_variable()
{
   static int a = 10;
   ++a;
   printf("Value of a (local static variable) = %i\n", a);
   return a;
}

// NOTE: why do we need static local variables? Why not make everything global if we want to retain the value?

int main (int argc, char ** args)
{
   global_variable();
   global_variable();
   local_variable();
   local_variable();
   local_static_variable();
   local_static_variable();
   return EXIT_SUCCESS;
}
