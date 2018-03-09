#include <stdio.h>
#include <stdlib.h>



void size_after_decay(char array[])
{
   // NOTE: Such a common error that gcc 5.4 and newer start warning about this!
   printf("The size of array is: %u\n", (unsigned int) sizeof(array));
}

void size_after_decay2(char * array)
{
   printf("The size of array is: %u\n", (unsigned int) sizeof(array));
}

#define SHOW_AND_RUN(a) do { puts("Running '" #a "'"); a; } while(0);

int main (int argc, char ** args)
{
   char array[] = "1234";

   // application of sizeof to resulting type of 'array + 0' DECAY!
   printf("The size of array+0 is: %u\n", (unsigned int) sizeof(array+0));

   // direct application of sizeof to array: no decay
   printf("The size of array is: %u\n", (unsigned int) sizeof(array));


   // Use of array in expression: decay!
   SHOW_AND_RUN(size_after_decay(array));
   SHOW_AND_RUN(size_after_decay2(array));
   SHOW_AND_RUN(size_after_decay(array + 0));


   return EXIT_SUCCESS;
}
