#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

// given board;
// board stored as character string
void generate(char * b,char place, int start, int max) 
{
   // find empty spot
   for (unsigned int i=start; i<max; ++i)
   {
      char c=b[i];
      if (c!=' ')
         // can't place anything here
         continue;

      b[i]=place;
      // recurse
      puts(b);
      generate(b, (place =='X' ? 'O':'X'),i+1, max);

      // Remove token and try next pos
      b[i]=' ';
   }
}


int main ()
{
   // All boards with X in the middle
char b[]="         ";
generate(b,'O',0,strlen(b));
}

