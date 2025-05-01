#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "hackem.h"

int main(int argc, char **argv) {
	Hack hack;

    if(argc != 2) {
        printf("Usage: hackem.x romfile\n");
    }
    else {
		init(&hack);
		loadROM(argv[1], &hack);
		hack.RAM[0] = 25;
		hack.RAM[1] = 0;

		// initialize raylib
		InitWindow(1024, 640, "Hack Emulator");
		hack.fontTTF = LoadFontEx("fonts/JuliaMono-Regular.ttf", 32, 0, 250);

		// start the CPU
		runHack(&hack);

		// stop raylib
		CloseWindow();

		return 0;
	}
}

void init(Hack* h) {
	int i;

	h->PC = 0;
	h->A = 0;
	h->D = 0;

	// zero ROM
	for(i = 0;i < 0x8000;i++) {
		h->ROM[i] = 0;
	}

	// zero RAM
	for(i = 0;i <= 0x6000;i++) {
		h->RAM[i] = 0;
	}
}

void loadROM(char* s, Hack* h) {
    int i;
	FILE *inFile;
	char line[MAX_LINE_LEN];

    inFile = fopen(s, "r");
    if(inFile == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for(i = 0;fgets(line, MAX_LINE_LEN, inFile) != NULL;i++)
    {
        h->ROM[i] = strtol(line, NULL, 2);
    }

    fclose(inFile);
}
