#include <stdio.h>
#include <stdlib.h>

// NOTE: take a look at qsort_r (manpage) and make sure to understand *why*
// there is a need for qqsort_r


// note static
static int compare (const void * a, const void * b)
{
   // a and b are pointers to the array elements
   const int * mya = (const int *) a;
   const int * myb = (const int *) b;

   // Return 0 for a == b, <0 for a<b, >0 for a>b
   return (*mya - *myb);
}

static void printarray (const int a[8], int len)
{
   //printf ("%i\n", (int) sizeof(a));

   for (int i=0; i<len; ++i)
      printf("%i ", a[i]);
   puts("");
}

int main (int argc, char ** args)
{
   int myarray[] = {2, 3, 10, 2, 3, 99, 1, -200};

   printarray(myarray, sizeof(myarray)/sizeof(myarray[0]));

   // note array decay for first argument but not for second argument
   qsort(myarray, sizeof(myarray)/sizeof(myarray[0]), sizeof(myarray[0]),
         compare);

   printarray(myarray, sizeof(myarray)/sizeof(myarray[0]));

   return EXIT_SUCCESS;
}
