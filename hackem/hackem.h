#include <stdint.h>
#include "raylib.h"

#define MAX_LINE_LEN 256

typedef struct Hack_t {
    uint16_t RAM[0x6001]; // RAM (0x0000-0x3FFF)
                          // screen (0x4000-0x5FFF)
                          // keyboard (0x6000)
    uint16_t ROM[0x8000]; // ROM (0x0000-0x7FFF)
    uint16_t PC;
    uint16_t A;
    uint16_t D;
	char strInst[13];

	// state (stepping or running)
	int16_t isStepping;
	int16_t keyMode;

	// Graphics stuff...should probably go elsewhere...but I'm lazy
	Font fontTTF;
	Color screenArray[256][512];
	Texture2D screenTexture;
	uint16_t iRAMstart; // first RAM address in RAM list
	uint16_t iROMstart; // first ROM address is ROM list
	uint16_t iColSelect; // selected column (ROM = 0, RAM = 1)
	uint16_t iRowSelect; // selected row (0..10)
	char cRAMinput[6]; // buffer for RAM input text box
	int iCharCount;
} Hack;

// hackem.c
void init(Hack*);
void initTexture(Hack*);
void loadROM(char*, Hack*);
//void keyInput(Hack*, char*);
void keyInput(Hack*, int, char*);

// cpu.c
void runHack(Hack*);
int16_t compute(uint16_t, Hack*);
void store(Hack*, uint16_t, int16_t);
void jump(uint16_t, int16_t, Hack*);

// cpu_display.c
void show_cpu(Hack*);
void draw_loop(Hack*, char*, char*, char*, char*);
Color isSelected(Hack*, int, int);
void show_compute(uint16_t, char*);
void show_dest(uint16_t, char*);
void show_jump(uint16_t, char*);
