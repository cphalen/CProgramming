#include <stdio.h>
#include <stdlib.h>

#define SHOWME(varname) \
    printf("var " #varname " size=%lu address=%p type=<no introspection> value=%llu\n", \
          (long unsigned) sizeof(varname), (void*) &varname, (long long unsigned) varname);
         

void example1 ()
{

   // a is a variable; it has:
   // 1- a size (sizeof(a)) -- depends on system but fixed at compile time
   // 2- an address (&a): depends on execution(runtime)
   // 3- a 'domain' (or set of possible values): signed integers in a certain range
   //     (exact range depends on (1))
   // 4- a specific value (runtime) -- an element from (3); 6 in this case.

   unsigned int a = 6;

   SHOWME(a);

   // b is a variable; it has:
   // 1- a size (sizeof(a)) -- depends on system but fixed at compile time
   // 2- an address (&a): depends on execution(runtime)
   // 3- a 'domain' (or set of possible values): signed integers in a certain range
   //     (exact range depends on (1))
   // 4- a specific value (runtime) -- an element from (3); 6 in this case.

   // What is type of 23? (Explains the warning...)
   unsigned int * b = 23;

   SHOWME(b);


   // Same for c:
   // 4- value of c = address of b... But nothing special about that!

   // What is type of &a ?
   unsigned int * c = &a;

   SHOWME(c);
}


void example2 ()
{
   int a = 2;
   int b = 30;

   int * ptr = &a;

   printf("Value of a=%i\n", a);
   *ptr = 3;
   printf("Value of a=%i\n", a);

   int ** ptr2 = &ptr;

   printf("Value of ptr=%p\n", (void*)ptr);

   // TODO: Calculate the types on both sides and show that they match!
   *ptr2 = &b;
   printf("Value of ptr=%p\n", (void*)ptr);

   printf("Value of b=%i\n", b);
   // TODO: Calculate the types on both sides and show that they match!
   *ptr = 6;
   printf("Value of b=%i\n", b);

   // TODO: Calculate the types on both sides and show that they match!
   **ptr2 = 10;
   printf("Value of b=%i\n", b);


   int * localvarptr = 0;
   {
      int localvar = 10;
      localvarptr = &localvar;
   }
   // localvarptr is now a 'dangling pointer'; it points to an invalid memory location
   // (other than 0 or NULL) since localvar is no longer 'alive' (it died after the
   // scope in which it was defined ended).
   // However, the pointer localvarptr is still in scope and still alive...

   // 0x -> hex literal
   // TODO: Run this with valgrind...
   // Does it detect the problem?
   *localvarptr = 0xdead;
}

void example3()
{
   typedef struct X { int a; } X;

   X structexample;
   
   X * ptr_to_struct = &structexample;

   // TODO: Do the type validation...
   int * ptr_to_int = &structexample.a;

   *ptr_to_int = 6;
   printf("structexample.x = %i\n", structexample.a);
   ptr_to_struct->a = 7;
   printf("structexample.x = %i\n", structexample.a);


}

int main ()
{
   example1 ();
   example2 ();
   example3 ();

   return EXIT_SUCCESS;
}
