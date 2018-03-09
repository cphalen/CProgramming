# trie program makefile
# The argument parameter will allow the user to pass a file that they would like to compile with trie. Use "argument = ..." when calling make

CFLAGS = -Wall -pedantic -std=c11 -o

all: trie.c trie.h $(argument)
	gcc $(CFLAGS) trie trie.c $(argument)

test: trie.c trie_test.c trie.h $(argument)
	gcc $(CFLAGS) trie_test trie.c trie_test.c -lcunit

# This is just a shortcut that will remove out binaries
.PHONY: clean
clean:
	-rm trie
	-rm trie_test
