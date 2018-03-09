# mnist database program makefile
# The argument parameter will allow the user to pass a file that they would like to compile with mnist. Use "argument = ..." when calling make

CFLAGS = -Wall -pedantic -std=c11 -o

all: mnist.c mnist2pgm.c
	gcc $(CFLAGS) mnist2pgm mnist.c mnist2pgm.c
	gcc $(CFLAGS) test_mnist mnist.c test_mnist.c -lcunit

test: mnist.c test_mnist.c
	gcc $(CFLAGS) test_mnist mnist.c test_mnist.c -lcunit
	./test_mnist

name: mnist.c $(argument)
	gcc $(CFLAGS) $(argument) mnist.c $(argument).c

# This is just a shortcut that will remove out binaries
.PHONY: clean
clean:
	rm test_mnist
	rm mnist2pgm
