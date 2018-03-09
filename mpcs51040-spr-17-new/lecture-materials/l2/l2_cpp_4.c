#include <stdio.h>
#include <stdlib.h>

#define MYMAX(a,b) (a > b ? a : b)

#define SQUARE(a) a*a

int somevalue()
{
   puts("call to somevalue");
   return 6;
}

int main(int argc, char ** args)
{
   printf("MYMAX(2,3)=%i\n", MYMAX(2,3));
   printf("MYMAX(1+1, 2+1)=%i\n", MYMAX(1+1,2+1));
   printf("MYMAX(somevalue(),2)=%i\n", MYMAX(somevalue(),2));
   printf("4 squared = %i\n", SQUARE(4));
   printf("4 squared = %i\n", SQUARE(2+2));
   return EXIT_SUCCESS;
}

