#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getOutput (char * buf, const char * opt)
{
   sprintf(buf, "the option was: '%s'\n", opt);
}

int main (int argc, char ** args)
{
   if (!strcmp(args[1], "option"))
   {
      puts("You passed option 'option'!");
   }
   else if (argc == 2)
   {
      char buffer[20];
      getOutput(buffer, args[1]);
      puts(buffer);
   }
   else
   {
      printf("Please call this program using '%s option'...\n",
            args[0]);
   }

   return 0;
}
