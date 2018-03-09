#include <stdio.h>
#include <stdlib.h>


// New type: struct tag1
// NOTE: tag1 is *not* a type;
struct tag1
{
   char c;
   int i;
};


struct tag1 tag1struct;
// FAILS:
// tag1 tag1struct


// Since struct tag1 is a type, I can typedef it
typedef struct tag1 Tag1Struct;

// Now this works
Tag1Struct example;

// More common:
typedef struct tag1 tag1;

// Now both 'tag1' and 'struct tag1' refer to the same struct type

// A struct lays out the members of the struct in order in memory, one after oneother.
// Note: the compiler is free to insert 'padding' in between the members to satisfy
// architecture limitations (for example: 4 byte integers need to start in memory
// on an address which is a multiple of 4).

// You can obtain the location of the member within the struct using the
// offsetof macro (which is defined in stddef.h)
#include <stddef.h>

// A union is like a struct, except that members are placed on top of eachother
// in memory (instead of one after oneother).
// NOTE: union and struct share the same tag space, so can't have the same tagname for both.
union tag2
{
   char c;
   int i;
};

int main ()
{
   printf ("Size of struct tag1 = %lu bytes\n", sizeof(tag1));
   printf ("  member c starts at offset: %lu bytes from the start of the struct\n",
         offsetof(struct tag1, c));
   printf ("  member i starts at offset: %lu bytes from the start of the struct\n",
         offsetof(struct tag1, i));

   printf ("Size of union tag2 = %lu bytes\n", sizeof(union tag2));
   printf ("  member c starts at offset: %lu bytes from the start of the struct\n",
         offsetof(union tag2, c));
   printf ("  member i starts at offset: %lu bytes from the start of the struct\n",
         offsetof(union tag2, i));

   return EXIT_SUCCESS;
}
