arb_int_t datatype

______________________________

The arb_int_t datatype allows users to handle integers of an arbitrary size, a very powerful
functionality in C, where there are otherwise no integers of arbitrary size.

When initializing an arb_int_t value, do not assign a value, instead use functionality found in
the arb_int.h header file to define values of arb_int_t datatypes. The functionality will allow
users to create arb_int_t values from strings and long long integers (while handling exceeding the max
long long int value). The arb_int_t datatype can also be referenced out into a string or a long long
integer (once again handling how that value will possibly be exceeded);

The arb_int_t datatype can be mutated once defined using the functionality as well. There are
add, subtract, multiply, and divide functions, all of which allow users to interact with their
integers of arbitrary size.

A test file "arb_test.c" is included in this directory, although its principle function is to ensure
that the implementation of the arb_int_t datatype is working as expected, it can also be viewed
as an example function for how to use and interact with the arb_int_t datatype.

______________________________

Campbell Phalen
April 28, 2017
University of Chicago MPCS
C Programming
Dries Kimpe
