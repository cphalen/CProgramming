#include <stdio.h>
#include <stdlib.h>

#define PRINTINFO(msg,var) printf("%s: Address of " #var " is: %p, size is: %lu\n", \
      msg, (void *) &var, (long unsigned) sizeof(var))

// void means no value is returned
void func(int a)
{
   PRINTINFO("in func a", a);
}

int globalvar = 0;

int main (int argc, char ** args)
{
   int a = 10;

   double d = 0.00023;

   PRINTINFO("first a", a);

   func(a);

   {
      long unsigned int a;
      PRINTINFO("local a", a);

   }

   PRINTINFO("first d", d);

   PRINTINFO("global", globalvar);


   return EXIT_SUCCESS;
}
