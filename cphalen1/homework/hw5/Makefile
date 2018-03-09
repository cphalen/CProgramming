# arb_int programes makefile
# The argument parameter will allow the user to pass a file that they would like to compile with arb_int. Use "argument = ..." when calling make

CFLAGS = -Wall -pedantic -std=c11 -o

arb_int: arb_int.c arb_int.h arb_test.c
	gcc $(CFLAGS) arb_int arb_int.c -lm  arb_test.c

arb_int_arugment: arb_int.c arb_int.h $(argument)
	gcc $(CFLAGS) arb_int arb_int.c -lm $(argument)

arb_int_gflag: arb_int.c arb_int.c $(argument)
	gcc -g $(CFLAGS) arb_int arb_int.c -lm $(argument)

# This is just a shortcut that will remove out binaries
.PHONY: clean
clean:
	rm arb_int
