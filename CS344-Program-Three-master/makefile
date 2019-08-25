default: smallsh

smallsh.o: smallsh.c smallsh.h
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -c smallsh.c

smallsh: smallsh.o main.c
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -o smallsh smallsh.o main.c

clean:
	rm smallsh.o

cleanall: clean
	rm smallsh
