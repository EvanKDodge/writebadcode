#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "raylib.h"

#define COLS 64
#define ROWS 32

typedef struct Chip8_t {
	uint8_t mem[4096];
	uint8_t display[COLS * ROWS];
	uint16_t stack[16];
	uint8_t V[16];
	uint16_t PC;
	uint16_t I;
	uint8_t SP;
	uint8_t DT;
	uint8_t ST;
} Chip8;

void init(Chip8*);
void loadROM(char*, Chip8*);
void runChip8(Chip8*);
void draw(Chip8*);

int main(int argc, char **argv) {
	Chip8 c8;
	int i;
	FILE* inFile;

	if(argc != 2) {
		printf("Usage: c8em.x romfile\n");
	}
	else {
		init(&c8);
		loadROM(argv[1], &c8);

		InitWindow(COLS * 10, ROWS * 10, "Chip8 Emulator");
		SetTargetFPS(60);

		runChip8(&c8);

		CloseWindow();
	}

	return 0;
}

void init(Chip8 *c8) {
	int i;

	// init registers
	c8->PC = 0x200;
	c8->SP = 0;
	c8->DT = 0;
	c8->ST = 0;
	c8->I = 0;

	// zero V registers
	for(i = 0;i <= 0xF;i++) {
		c8->V[i] = 0;
	}

	// zero out memory
	for(i = 0;i < 0x1000;i++) {
		c8->mem[i] = 0;
	}

	// display test pattern
	for(i = 0;i < (COLS * ROWS);i++) {
		if(i % 2 == 0) {
			c8->display[i] = 0;
		} else {
			c8->display[i] = 1;
		}
	}
}

void loadROM(char* s, Chip8* c8) {
	FILE *inFile;
	int i = 0x200;	// we start loading at mem[512]

	inFile = fopen(s, "rb");
	if(inFile == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	while((fread(&c8->mem[i], 1, 1, inFile) && i < 0x1000))
	{
		i++;
	}

	fclose(inFile);
}

void runChip8(Chip8* c8) {
	uint16_t curInst;
	uint16_t T, X, Y, N;
	uint16_t NN, NNN;
	int i, j, x, y, tmp;

	while(!WindowShouldClose()) {
		// fetch
		curInst = c8->mem[c8->PC] << 8 | c8->mem[c8->PC + 1];

		c8->PC += 2;

		// decode
		T = (0xF000 & curInst) >> 12;	// instruction type
		X = (0x0F00 & curInst) >> 8;	// 2nd nibble
		Y = (0x00F0 & curInst) >> 4;	// 3rd nibble
		N = (0x000F & curInst);			// 4th nibble
		NN = (0x00FF & curInst);		// last 2 nibbles
		NNN = (0x0FFF & curInst);		// last 3 nibbles		

		// execute
		switch(T) {
			case 0:
				switch(NN) {
					case 0xe0:
						// CLEAR SCREEN
						for(i = 0;i < (COLS * ROWS);i++) {
							c8->display[i] = 0;
						}
						break;
					case 0xee:
						// RETURN FROM 2NNN
						c8->PC = c8->stack[c8->SP];
						c8->SP--;
						break;
				}
				break;
			case 1:
				// UNCONDITIONAL JUMP
				c8->PC = NNN;
				break;
			case 6:
				// SET V REGISTER TO VALUE
				c8->V[X] = NN;
				break;
			case 7:
				// ADD VALUE TO V REGISTER
				c8->V[X] += NN;
				break;
			case 0xa:
				// SET INDEX REGISTER
				c8->I = NNN;
				break;
			case 0xd:
				// DRAW SPRITE TO SCREEN
				y = c8->V[Y] & 31;
				c8->V[0xF] = 0;

				for(i = c8->I;i < (c8->I + N);i++) {
					x = c8->V[X] & 63;
					for(j = 7;j >= 0;j--) {
						tmp = (c8->mem[i] >> j) & 1;
						if(tmp == 1 && c8->display[64 * y + x] == 1) {
							c8->display[64 * y + x] = 0;
							c8->V[0xF] = 1;
						}
						if(tmp == 1 && c8->display[64 * y + x] == 0) {
							c8->display[64 * y + x] = 1;
						}
						if(x > (COLS - 1)) break;
						x++;
					}
					y++;
					if(y > (ROWS - 1)) break;
				}
				break;
			default:
		}

		draw(c8);
	}
}

void draw(Chip8* c8) {
	int x, y;

	BeginDrawing();
		for(x = 0;x < COLS;x++) {
			for(y = 0;y < ROWS;y++) {
				if(c8->display[64 * y + x] == 1) {
					DrawRectangle(x * 10, y * 10, 10, 10, WHITE);
				}
				else {
					DrawRectangle(x * 10, y * 10, 10, 10, BLACK);
				}
			}
		}
	EndDrawing();
}
