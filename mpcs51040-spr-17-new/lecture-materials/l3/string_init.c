#include <stdio.h>
#include <stdlib.h>

// This contains the DECLARATION of the string functions we're using below
#include <string.h>

int main (int argc, char ** args)
{
   const char str[] = "this is NULL terminated!";


   // We're telling printf that we will give it a %u (= unsigned int)
   // but we're giving it the output of strlen(), which is size_t.
   // So we cast to avoid a warning.
   printf("Length of str = %u\n", (unsigned int) strlen(str));

   const char str2[] = "We can manually add a NULL\0 but it will end the string";
   puts(str2);

   printf("String length of str2 = %i, SIZE of str2 = %i\n",
         (int) strlen(str2), (int) sizeof(str2));


   const char str3[] = "1234";
   // TODO: What is the size of str3?? 

   return EXIT_SUCCESS;
}
