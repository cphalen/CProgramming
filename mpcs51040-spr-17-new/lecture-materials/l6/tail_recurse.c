#include <stdio.h>
#include <stdlib.h>

void recurse(int y)
{
   if (!y)
      return;
   int x = y;
   printf("%p\n", (void *) &x);
   recurse(y-1);
}

int main (int argc, char ** args)
{
   recurse(10);
   return EXIT_SUCCESS;
}
