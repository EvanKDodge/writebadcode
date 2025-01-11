#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void zeromem(uint8_t*);
void readFile(char*, uint8_t*);

int main(void) {
	uint8_t mem[0xFFF];
	int i;
	FILE* inFile;

	zeromem(mem);
	readFile("IBM-Logo.ch8", mem);

	for(i = 0x0; i < 0x1000; i++) {
		printf("0x%03x\t0x%02x\n", i, mem[i]);
	}

	return 0;
}

void zeromem(uint8_t *mem) {
	int i;

	for(i = 0;i < 0x1000;i++) {
		mem[i] = 0;
	}
}

void readFile(char* s, uint8_t *mem) {
	FILE *inFile;
	int i;

	inFile = fopen(s, "rb");
	if(inFile == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}

	for(i = 0x200; i < 0x1000; i++) {
		fread(&mem[i], 1, 1, inFile);
	}

	fclose(inFile);
}
