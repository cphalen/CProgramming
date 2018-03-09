#include <stdio.h>
#include <stdlib.h>

/*
 * For this example, pay attention to:
 *   - evaluation order and the risks of preprocessor macros
 *   - argument parsing rules for preprocessor macros
 */


// Works only for signed integer
int min(int a, int b)
{
   // Ternary operator ?:
   return (a < b ? a : b);
}

// Works only for float
// Can't reuse name 'min'
float minf(float a, float b)
{
   // Ternary operator ?:
   return (a < b ? a : b);
}

// How to do this using preprocessor macro?
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// Try:
//int xx = MIN([2,3],2);
//int yy = MIN()
//int zz = MIN(,)

int main (int argc, char ** args)
{
   int x = 2;
   int y = 3;
   printf ("The minimum of variables x and y is: %i\n", min(x,y));
   double f1 = 0.2;
  double f2 = 0.3;
   printf ("The minimum of variables f1 and f2 is: %f\n", minf(f1,f2));
   printf ("The minimum of x&1 and y&1 is: %i\n", MIN(x&1,y&1));
   printf ("The minimum of ++y and x is: %i\n", MIN(++x,y));
   return EXIT_SUCCESS;
}
