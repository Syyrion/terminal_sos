CC=gcc
CFLAGS=-I.

main: main.o board.o search.o behavior.o
	$(CC) $(CFLAGS) -o sos main.o board.o search.o behavior.o

board.o: board.c
	$(CC) $(CFLAGS) -c board.c

search.o: search.c board.h
	$(CC) $(CFLAGS) -c search.c

behavior.o: behavior.c board.h
	$(CC) $(CFLAGS) -c behavior.c

main.o: main.c boardinterface.h search.h behavior.h
	$(CC) $(CFLAGS) -c main.c


clean:
	rm -f *.o sos