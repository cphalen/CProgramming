# Palindrome programes makefile

palindrome: palindrome.c palindrome_driver.c palindrome.h
	gcc -Wall -pedantic -std=c11 -o palindrome palindrome.c palindrome_driver.c
	echo Palindrome program complied successfully

# This is just a shortcut that will remove out binaries
.PHONY: clean
clean:
	rm palindrome
