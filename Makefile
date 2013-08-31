CC=gcc
CFLAGS=-Wall -g

main: main.o walk.o cset.o

clean:
	rm -f main *.o
