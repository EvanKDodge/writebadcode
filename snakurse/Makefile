CC=gcc
LDFLAGS=-lcurses

all:
	$(CC) -o player.o player.c -c
	$(CC) -o snakurse.o snakurse.c -c
	$(CC) -o snakurse.x snakurse.o player.o $(LDFLAGS)

clean:
	rm *.o *.x
