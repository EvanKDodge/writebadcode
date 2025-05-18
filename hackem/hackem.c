#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
			int key = GetKeyPressed();

			// check...most...key input
			keyInput(&hack, key, argv[1]);

			// display CPU data
			show_cpu(&hack);

			if(IsKeyPressed(KEY_SPACE) || !hack.isStepping) {
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
	h->iColSelect = 0;
	h->iRowSelect = 0;

	strcpy(h->cRAMinput, "");
	h->iCharCount = 0;
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

void keyInput(Hack* h, int key, char* s) {
	switch(key) {
		case KEY_S:
			printf("keyInput %d\n", key);
			h->isStepping = !h->isStepping;
			break;
		case KEY_LEFT:
			if(h->iColSelect) { // if RAM is selected
				h->iColSelect--;
				strcpy(h->cRAMinput, "");
				h->iCharCount = 0;
			}
			break;
		case KEY_RIGHT:
			if(!h->iColSelect) { // if ROM is selected
				h->iColSelect++;
			}
			break;
		case KEY_UP:
			if(h->iRowSelect > 0) {
				h->iRowSelect--;
			}
			else {
				if(h->iColSelect) { // RAM col selected
					if(h->iRAMstart > 0) {
						h->iRAMstart--;
					}
				}
				else {			// ROM col selected
					if(h->iROMstart > 0) {
						h->iROMstart--;
					}
				}
			}
			break;
		case KEY_PAGE_UP:
			if(h->iColSelect) { // RAM col selected
				if(h->iRAMstart > 9) {
					h->iRAMstart -= 10;
				}
				else
				{
					h->iRAMstart = 0;
				}
			}
			else {          // ROM col selected
				if(h->iROMstart > 9) {
					h->iROMstart -= 10;
				}
				else
				{
					h->iROMstart = 0;
				}
			}
			break;
		case KEY_HOME:
			if(h->iColSelect) { // RAM
				h->iRAMstart = 0;
			}
			else {  // ROM
				h->iROMstart = 0;
			}
			h->iRowSelect = 0;
			break;
		case KEY_DOWN:
			if(h->iRowSelect < 9) {
				h->iRowSelect++;
			}
			else {
				if(h->iColSelect) { // RAM col selected
					if(h->iRAMstart < (0x3FFF - 9)) {
						h->iRAMstart++;
					}
				}
				else {			// ROM col selected
					if(h->iROMstart < (0x7FFF - 9)) {
						h->iROMstart++;
					}
				}
			}
			break;
		case KEY_PAGE_DOWN:
			if(h->iColSelect) { // RAM col selected
				if(h->iRAMstart < 0x3FFF - 18) {
					h->iRAMstart += 10;
				}
				else
				{
					h->iRAMstart = 0x3FFF - 9;
				}
			}
			else {          // ROM col selected
				if(h->iROMstart < 0x7FFF - 18) {
					h->iROMstart += 10;
				}
				else
				{
					h->iROMstart = 0x7FFF - 9;
				}
			}
			break;
		case KEY_END:
			if(h->iColSelect) { // RAM
				h->iRAMstart = 0x3FFF - 9;
			}
			else {	// ROM
				h->iROMstart = 0x7FFF - 9;
			}
			break;
		case KEY_R:
			init(h);
			loadROM(s, h);

			break;
		case KEY_ZERO:
		case KEY_ONE:
		case KEY_TWO:
		case KEY_THREE:
		case KEY_FOUR:
		case KEY_FIVE:
		case KEY_SIX:
		case KEY_SEVEN:
		case KEY_EIGHT:
		case KEY_NINE:
			if(h->iColSelect) { // if RAM is selected
				if(h->iCharCount < 5) {
					h->cRAMinput[h->iCharCount] = (char)key;
					h->cRAMinput[h->iCharCount+1] = '\0';
					h->iCharCount++;
				}
			}
			break;
		case KEY_BACKSPACE:
			if(h->iColSelect) { // if RAM is selected
				if(h->iCharCount > 0) {
					h->iCharCount--;
					h->cRAMinput[h->iCharCount] = '\0';
				}
			}
			break;
		case KEY_ENTER:
			if(h->iColSelect) { // if RAM is selected
				h->RAM[h->iRAMstart + h->iRowSelect] = atoi(h->cRAMinput);
				strcpy(h->cRAMinput, "");
				h->iCharCount = 0;
			}
			break;
	}
}
