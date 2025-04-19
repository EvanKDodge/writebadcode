#include <stdio.h>
#include <stdint.h>

typedef struct Hack_t {
	uint16_t RAM[0x6001]; // RAM (0x0000-0x3FFF)
						  // screen (0x4000-0x5FFF)
						  // keyboard (0x6000)
	uint16_t ROM[0x8000]; // ROM (0x0000-0x7FFF)
	uint16_t PC;
	uint16_t A;
	uint16_t D;
} Hack;

void init(Hack*);
void runHack(Hack*);

int main(int argc, char **argv) {
	Hack hack;

	init(&hack);
	runHack(&hack);
	printf("Here's the answer: %d\n", hack.RAM[0]);
	printf("...again in binary: %016b\n", hack.RAM[0]);
	return 0;
}

void init(Hack* h) {
	int i;

	uint16_t ADDHACK[6] = {
		0x7000,
		0xEC10,
		0x7FFF,
		0xE090,
		0x0000,
		0xE308
	};

	h->PC = 0;
	h->A = 0;
	h->D = 0;

	// zero ROM and add code (temporary)
	for(i == 0;i < 0x8000;i++) {
		if(i < 6) {
			h->ROM[i] = ADDHACK[i];
		}
		else {
			h->ROM[i] = 0;
		}
	}

	// zero RAM
	for(i == 0;i <= 0x6000;i++) {
		h->RAM[i] = 0;
	}
}

void runHack(Hack* h) {
	int i;
	uint16_t curInst;
	uint16_t T; // instruction type
	uint16_t D; // destination instruction ( A, M, D or any combination)
	uint16_t J; // jump instruction
	uint16_t C; // compute instruction
	uint16_t compVal; // computed value
	uint16_t dest; // destination

	for(i = 0;i < 6;i++) {
		// fetch next instruction
		printf("PC: %d\n", h->PC);
		curInst = h->ROM[h->PC];

		h->PC += 1;

		// decode instruction type: A (0)  or C (1)
		T = (0b1000000000000000 & curInst) >> 15;
		printf("Type: %d\n", T);
		switch(T) {
			case 0:
				// set the A register to the value
				// given in the instruction
				// Value can be used for:
				// 1) a constant
				// 2) a RAM address (M in following C instruction)
				// 3) a ROM address if a JMP instruction follows
				h->A = curInst;
				break;
			case 1:
				C = (0x1FC0 & curInst) >> 6;
				J = (0x0007 & curInst);
				D = (0x0038 & curInst) >> 3;

				printf("Compute: %07b Dest: %03b Jump: %03b\n", C, D, J);

				// compute value
				switch(C) {
					case 0b0101010:
						compVal = 0;	// 0
						break;
					case 0b0111111:
						compVal = 1;	// 1
						break;
					case 0b0111010:
						compVal = -1;	// -1
						break;
					case 0b0001100:
						compVal = h->D;	// D
						break;
					case 0b0110000:
						compVal = h->A;	// A
						break;
					case 0b0001101:
						compVal = !h->D;	// !D
						break;
					case 0b0110001:
						compVal = !h->A;	// !A
						break;
					case 0b0001111:
						compVal = -h->D;	// -D
						break;
					case 0b0110011:
						compVal = -h->A;	// -A
						break;
					case 0b0011111:
						compVal = h->D + 1;	// D+1
						break;
					case 0b0110111:
						compVal = h->A + 1;	// A+1
						break;
					case 0b0001110:
						compVal = h->D - 1;	// D-1
						break;
					case 0b0110010:
						compVal = h->A - 1;	// A-1
						break;
					case 0b0000010:
						compVal = h->D + h->A;	// D+A
						break;
					case 0b0010011:
						compVal = h->D - h->A;	// D-A
						break;
					case 0b0000111:
						compVal = h->A - h->D;	// A-D
						break;
					case 0b0000000:
						compVal = h->D & h->A;	// D&A
						break;
					case 0b0010101:
						compVal = h->D | h->A;	// D|A
						break;
					case 0b1110000:
                        compVal = h->RAM[h->A]; // M
                        break;
					case 0b1110001:
                        compVal = !h->RAM[h->A];    // !M
                        break;
					case 0b1110011:
                        compVal = -h->RAM[h->A];    // -M
                        break;
					case 0b1110111:
                        compVal = h->RAM[h->A] + 1; // M+1
                        break;
					case 0b1110010:
                        compVal = h->RAM[h->A] - 1; // M-1
                        break;
					case 0b1000010:
                        compVal = h->D + h->RAM[h->A];  // D+M
                        break;
					case 0b1010011:
                        compVal = h->D - h->RAM[h->A];  // D-M
                        break;
					case 0b1000111:
                        compVal = h->RAM[h->A] - h->D;  // M-D
                        break;
					case 0b1000000:
                        compVal = h->D & h->RAM[h->A];  // D&M
                        break;
					case 0b1010101:
                        compVal = h->D | h->RAM[h->A];  // D|M
                        break;
				}

				// place computed value in destination(s)
				switch(D) {
					case 0b000:
						break;
					case 0b001:
						h->RAM[h->A] = compVal;
						break;
					case 0b010:
						h->D = compVal;
						break;
					case 0b011:
						h->RAM[h->A] = compVal;
						h->D = compVal;
						break;
					case 0b100:
						h->A = compVal;
						break;
					case 0b101:
						h->RAM[h->A] = compVal;
						h->A = compVal;
						break;
					case 0b110:
						h->A = compVal;
						h->D = compVal;
						break;
					case 0b111:
						h->D = compVal;
						h->RAM[h->A] = compVal;
						h->A = compVal;
						break;
				}

				switch(J) {
					case 0b000:
						break;
					case 0b001:
						if(compVal > 0) h->PC = h->A;
						break;
					case 0b010:
						if(compVal = 0) h->PC = h->A;
						break;
					case 0b011:
						if(compVal >= 0) h->PC = h->A;
						break;
					case 0b100:
						if(compVal < 0) h->PC = h->A;
						break;
					case 0b101:
						if(compVal != 0) h->PC = h->A;
						break;
					case 0b110:
						if(compVal <= 0) h->PC = h->A;
						break;
					case 0b111:
						h->PC = h->A;
						break;
				}

				break;
		}
	}
}
