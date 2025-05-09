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
		hack.RAM[0] = 16;
		hack.RAM[1] = 0;

		// initialize raylib, set up render texture, and font
		InitWindow(1024, 640, "Hack Emulator");
		initTexture(&hack);
		hack.fontTTF = LoadFontEx("fonts/JuliaMono-Regular.ttf", 32, 0, 250);

		while(!WindowShouldClose()) {
			if(IsKeyPressed(KEY_S)) {
				hack.isStepping = !hack.isStepping;
			}
			else if(IsKeyPressed(KEY_R)) {
				init(&hack);
				loadROM(argv[1], &hack);
			}

			// display CPU data
			show_cpu(&hack);

			if(IsKeyPressed(KEY_ENTER) || !hack.isStepping) {
				// run a CPU cycle
				runHack(&hack);
			}
		}

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

	h->isStepping = 1;

	// zero ROM
	for(i = 0;i < 0x8000;i++) {
		h->ROM[i] = 0;
	}

	// zero RAM
	for(i = 0;i <= 0x6000;i++) {
		h->RAM[i] = 0;
	}

	// reset RAM and ROM starting addresses
	h->iRAMstart = 0;
	h->iROMstart = 0;
}

void initTexture(Hack* h) {
	int x, y;

	// set up graphics
	for(x = 0;x < 512;x++) {
		for(y = 0;y < 256;y++) {
			h->screenArray[y][x] = BLACK;
		}
	}

	Image screenImage = {
		.data = h->screenArray,
		.width = 512,
		.height = 256,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.mipmaps = 1
	};

	h->screenTexture = LoadTextureFromImage(screenImage);
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
