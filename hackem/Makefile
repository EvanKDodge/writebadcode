CC=gcc
#RAYLIB_INST=/home/hexboy/local/raylib-5.0_linux_amd64
#CFLAGS=-I$(RAYLIB_INST)/include
#LDFLAGS=-L$(RAYLIB_INST)/lib -lraylib -Wl,-R$(RAYLIB_INST)/lib
LDFLAGS=-lncurses

all:
	$(CC) -o hackem.x hackem.c cpu.c cpu_display.c $(LDFLAGS)

clean:
	rm *.x
