# varstring programes makefile
# The argument parameter will allow the user to pass a file that they would like to compile with varstring. Use "argument = ..." when calling make

CFLAGS = -Wall -pedantic -std=c11 -o

varstring: varstring.c varstring.h varstring_test.c
	gcc $(CFLAGS) varstring varstring.c varstring_test.c
	echo Vastring program compiled sucessfully

varstring_arugment: varstring.c varstring.h $(argument)
	gcc $(CFLAGS) varstring varstring.c $(argument)
	echo Vastring program compiled sucessfully

varstring_gflag: varstring.c varstring.h $(argument)
	gcc -g $(CFLAGS) varstring varstring.c $(argument)
	echo Vastring program compiled sucessfully

# This is just a shortcut that will remove out binaries
.PHONY: clean
clean:
	rm varstring
