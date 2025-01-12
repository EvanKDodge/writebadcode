#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Chip8_t {
	uint8_t mem[4096];
	uint8_t display[64][32];
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
		runChip8(&c8);
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

	while(c8->PC <= 0x228) {
		// fetch
		curInst = c8->mem[c8->PC] << 8 | c8->mem[c8->PC + 1];

		printf("%04x\t%04x\n", c8->PC, curInst);

		c8->PC += 2;

		// decode
		T = (0xF000 & curInst) >> 12;
		X = (0x0F00 & curInst) >> 8;
		Y = (0x00F0 & curInst) >> 4;
		N = (0x000F & curInst);
		NN = (0x00FF & curInst);
		NNN = (0x0FFF & curInst);

		// execute
		switch(T) {
			case 0:
				printf("Clear screen or return from subroutine.\n\n");
				break;
			case 1:
				c8->PC = NNN;
				break;
			case 6:
				c8->V[X] = NN;
				break;
			case 7:
				c8->V[X] += NN;
				break;
			case 0xa:
				c8->I = NNN;
				break;
			case 0xd:
				printf("Draw sprite to screen\n\n");
				break;
			default:
		}
	}
}
