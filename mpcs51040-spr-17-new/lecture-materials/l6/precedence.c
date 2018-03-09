#include <stdio.h>
#include <stdlib.h>

//int main (int argc, char * args[])
int main (int argc, char ** args)
{


   {
      char t[] = "1234\n";


      // PREFIX ++ lower priority than [] so:
      // ++t[1] --> ++(t[1])
      printf("value = %c\n", ++t[1]);
      puts(t);

   }

   typedef struct MyStruct
   {
      char * str;
   } MyStruct;

   // Array of pointers to structs

   MyStruct t;
   t.str = "test\n";

   MyStruct * s[] = { &t };

   // s[0]->str : [] and -> associativity is left-to-right:
   // s[0]->str -> ((s[0])->str)
   // NOT: (s([0]->str))
   puts(s[0]->str);

   // & and *: associativity right-to-left:
   const char * str2 = "test2";
   printf("Value = %c\n", *++str2); // *++str2 -> (*(++str2))



   return EXIT_SUCCESS;
}
