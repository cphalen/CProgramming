#include <stdio.h>
#include <stdlib.h>

unsigned int fibonacci (unsigned int n)
{
   if (!n)
      return 0;
   if (n==1)
      return 1;
   return fibonacci(n-1) + fibonacci(n-2);
}

int main ()
{
   for (unsigned int i=0; i<10; ++i)
   {
      printf("fib(%i)=%i\n", i, fibonacci(i));
   }
   return EXIT_SUCCESS;
}
