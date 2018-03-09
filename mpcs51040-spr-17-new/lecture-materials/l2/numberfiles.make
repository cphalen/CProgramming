# vim: ft=make
# example makefile
# execute using 'make -f numberfiles.make target'

ones:
	echo 11111 > ones

ones_and_twos: ones
	cat ones > ones_and_twos
	echo Hello!
	echo 22222 > ones_and_twos

