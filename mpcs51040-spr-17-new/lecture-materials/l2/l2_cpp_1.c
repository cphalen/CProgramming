#include <stdio.h>
#include <stdlib.h>

// Macro expands to nothing
// #define SAY_GOODBYE

// Macro expands to "Rob" (inluding ")
#define NAME "Rob"


#define printf(a,b) printf("Hello Class!")


int x = 10;


int main (int argc, char ** args)
{
#ifdef SAY_GOODBYE
   puts("Goodbye!\n");
#else
   printf("Hello %s!\n", NAME);
#endif
   return EXIT_SUCCESS;
}
