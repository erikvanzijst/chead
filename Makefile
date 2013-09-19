CC=gcc
INCLUDE=`pkg-config --cflags glib-2.0`
CFLAGS=-Wall -g

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

chead: main.o walk.o cset.o ahead.o
	$(CC) `pkg-config --cflags --libs gmodule-no-export-2.0` -o chead main.o walk.o cset.o ahead.o

clean:
	rm -f chead *.o
