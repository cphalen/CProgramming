#include <stdio.h>
#include <stdlib.h>

// TODO: predict the output... (then compile and run to verify)
int main ()
{

   for (unsigned int i=0; i<20; ++i)
   {
      if (i==2)
         continue;

      if (i==12)
         break;

      printf("Iteration: i=%u\n", i);

      if (i==6)
         i=9;
   }

   // i is out of scope here

   return EXIT_SUCCESS;
}
