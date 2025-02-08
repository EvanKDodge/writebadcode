#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define COLS 64
#define ROWS 32
#define SIZE 10

typedef struct Chip8_t {
	uint8_t mem[4096];
	uint8_t display[COLS * ROWS];
	uint16_t stack[16];
	uint8_t V[16];
	uint8_t keypad[16];
	uint16_t PC;
	uint16_t I;
	uint8_t SP;
	uint8_t DT;
	uint8_t ST;
} Chip8;

void init(Chip8*);
void loadROM(char*, Chip8*);
void runChip8(Chip8*);
void user_input(Chip8*);
uint8_t getKeyValue(Chip8*);
void draw(Chip8*);
int random_byte();

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

		InitWindow(COLS * SIZE, ROWS * SIZE, "Chip8 Emulator");
		SetTargetFPS(60);

		runChip8(&c8);

		CloseWindow();
	}

	srand(time(NULL));	// seed random number generator with timer

	return 0;
}

void init(Chip8 *c8) {
	int i;
	uint8_t font[] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

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

	// zero out memory and add fonts
	for(i = 0;i < 0x1000;i++) {
		if(i >= 0x50 && i <= 0x9f) {
			c8->mem[i] = font[i - 0x50];
		}
		else {
			c8->mem[i] = 0;
		}
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
	uint8_t tmp8;

	while(!WindowShouldClose()) {
		// fetch
		curInst = c8->mem[c8->PC] << 8 | c8->mem[c8->PC + 1];

		c8->PC += 2;

		// has to happen every frame
		user_input(c8);
		draw(c8);
		if(c8->DT > 0) c8->DT--;
		if(c8->ST > 0) c8->ST--; // should play audio...not yet...
		
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
						c8->SP--;
						c8->PC = c8->stack[c8->SP];
						break;
				}
				break;
			case 1:
				// UNCONDITIONAL JUMP
				c8->PC = NNN;
				break;
			case 2:
				// CALL SUBROUTINE
				c8->stack[c8->SP] = c8->PC;
				c8->SP++;
				c8->PC = NNN;
				break;
			case 3:
				// SKIP if VX = NN
				if(c8->V[X] == NN) c8->PC += 2;
				break;
			case 4:
				// SKIP if VX != NN
				if(c8->V[X] != NN) c8->PC += 2;
				break;
			case 5:
				// SKIP if VX == VY
				if(c8->V[X] == c8->V[Y]) c8->PC += 2;
				break;
			case 6:
				// SET V REGISTER TO VALUE
				c8->V[X] = NN;
				break;
			case 7:
				// ADD VALUE TO V REGISTER
				c8->V[X] += NN;
				break;
			case 8:
				switch(N) {
					case 0:
						// SET VX to the value of VY
						c8->V[X] = c8->V[Y];
						break;
					case 1:
						// SET VX to VX OR VY
						c8->V[X] = c8->V[X] | c8->V[Y];
						break;
					case 2:
						// SET VX to VX AND VY
						c8->V[X] = c8->V[X] & c8->V[Y];
						break;
					case 3:
						// SET VX to VX XOR VY
						c8->V[X] = c8->V[X] ^ c8->V[Y];
						break;
					case 4:
						// SET VX to VX + VY
						tmp = c8->V[X] + c8->V[Y];
						c8->V[0xF] = 0;
						if(tmp > 255) {
							c8->V[0xF] = 1;
						}
						c8->V[X] = tmp;
						break;
					case 5:
						// SET VX to VX - VY
						c8->V[X] = c8->V[X] - c8->V[Y];
						c8->V[0xF] = 0;
						if(c8->V[X] >= c8->V[Y]) {
							c8->V[0xF] = 1;
						}
						break;
					case 6:
						tmp = c8->V[X] & 1; //is rightmost 0 or 1?
						c8->V[X] = c8->V[X] >> 1;
						
						c8->V[0xF] = 0;
						if(tmp) {
							c8->V[0xF] = 1;
						} 
						break;
					case 7:
						// SET VX to VY - VX
						c8->V[X] = c8->V[Y] - c8->V[X];
						c8->V[0xF] = 0;
						if(c8->V[Y] >= c8->V[X]) {
							c8->V[0xF] = 1;
						}
						break;
					case 0xE:
						tmp = (c8->V[X] & 128) >> 7; //is leftmost 0 or 1?
						c8->V[X] = c8->V[X] << 1;
						
						c8->V[0xF] = 0;
						if(tmp) {
							c8->V[0xF] = 1;
						} 

						break;
				}
				break;
			case 9:
				// SKIP if VX != VY
				if(c8->V[X] != c8->V[Y]) c8->PC += 2;
				break;
			case 0xa:
				// SET INDEX REGISTER
				c8->I = NNN;
				break;
			case 0xb:
				// SET INDEX REGISTER to NNN + V0 and Jump to that address
				c8->PC = NNN + c8->V[0];
			case 0xc:
				// genrate random number
				c8->V[X] = random_byte() & NN;
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
			case 0xe:
				switch(NN) {
					case 0x9e:
						if(c8->keypad[c8->V[X]]) c8->PC += 2;
						break;
					case 0xa1:
						if(!c8->keypad[c8->V[X]]) c8->PC += 2;
						break;
				}
				break;
			case 0xf:
				switch(NN) {
					case 0x07:
						// set VX to current value of DT
						c8->V[X] = c8->DT;
						break;
					case 0x15:
						// set delay timer to value of VX
						c8->DT = c8->V[X];
						break;
					case 0x18:
						// set sound timer to value of VX
						c8->ST = c8->V[X];
						break;
					case 0x1e:
						// add the value in VX to I
						c8->I += c8->V[X];
						break;
					case 0x0a:
						// halt until key is pressed
						tmp8 = getKeyValue(c8);
						if(tmp8 != 0) {
							c8->V[X] = tmp8;
						} else {
							c8->PC -= 2;
						}
						break;
					case 0x29:
						// set I to address of corresponding 
						//  hex character (stored in VX) in font memory
						c8->I = c8->V[X] * 5 + 0x50;
						break;
					case 0x33:
						c8->mem[c8->I] = (c8->V[X] - (c8->V[X] % 100)) / 100;
						c8->mem[c8->I + 1] = ((c8->V[X] % 100) - (c8->V[X] % 10)) / 10;
						c8->mem[c8->I + 2] = c8->V[X] % 10;
						c8->I = c8->I + 2;
						break;
					case 0x55:
						// store values in registers V0-VX to I through I+X
						for(i = 0;i <= X;i++) {
							c8->mem[c8->I + i] = c8->V[i];
						}
						break;
					case 0x65:
						// pull values from I to I + X into V0-VX
						for(i = 0;i <= X;i++) {
							c8->V[i] = c8->mem[c8->I + i];
						}
						break;
				}
				break;
		}

	}
}

void user_input(Chip8* c8) {
	int i;

	for(i = 0;i <= 0xF;i++) {
		c8->keypad[i] = 0;
	}

	if(IsKeyPressed(KEY_ONE)) c8->keypad[1] = 1;
	if(IsKeyPressed(KEY_TWO)) c8->keypad[2] = 1;
	if(IsKeyPressed(KEY_THREE)) c8->keypad[3] = 1;
	if(IsKeyPressed(KEY_FOUR)) c8->keypad[0xC] = 1;
	if(IsKeyPressed(KEY_Q)) c8->keypad[4] = 1;
	if(IsKeyPressed(KEY_W)) c8->keypad[5] = 1;
	if(IsKeyPressed(KEY_E)) c8->keypad[6] = 1;
	if(IsKeyPressed(KEY_R)) c8->keypad[0xD] = 1;
	if(IsKeyPressed(KEY_A)) c8->keypad[7] = 1;
	if(IsKeyPressed(KEY_S)) c8->keypad[8] = 1;
	if(IsKeyPressed(KEY_D)) c8->keypad[9] = 1;
	if(IsKeyPressed(KEY_F)) c8->keypad[0xE] = 1;
	if(IsKeyPressed(KEY_Z)) c8->keypad[0xA] = 1;
	if(IsKeyPressed(KEY_X)) c8->keypad[0] = 1;
	if(IsKeyPressed(KEY_C)) c8->keypad[0xB] = 1;
	if(IsKeyPressed(KEY_V)) c8->keypad[0xF] = 1;
}

uint8_t getKeyValue(Chip8* c8) {
	uint8_t i;

	for(i = 0;i <= 0xF;i++) {
		if(c8->keypad[i]) {
			return i;	
		}
	}
	return 0;
}

void draw(Chip8* c8) {
	int x, y;

	BeginDrawing();
		for(x = 0;x < COLS;x++) {
			for(y = 0;y < ROWS;y++) {
				if(c8->display[64 * y + x] == 1) {
					DrawRectangle(x * SIZE, y * SIZE, SIZE, SIZE, WHITE);
				}
				else {
					DrawRectangle(x * SIZE, y * SIZE, SIZE, SIZE, BLACK);
				}
			}
		}
	EndDrawing();
}

int random_byte() {
	int num;

	num = rand() % 256;

	return num;
}
