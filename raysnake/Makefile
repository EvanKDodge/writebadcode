CC=gcc
RAYLIB_INST=/home/hexboy/local
CFLAGS=-I$(RAYLIB_INST)/include
LDFLAGS=-L$(RAYLIB_INST)/lib -lraylib -Wl,-R$(RAYLIB_INST)/lib

all:
	$(CC) -o raysnake.x raysnake.c $(CFLAGS) $(LDFLAGS)
clean:
	rm *.x
