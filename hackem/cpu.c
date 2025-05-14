#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include "raylib.h"
#include "hackem.h"

void runHack(Hack* h) {
	int i;
	uint16_t curInst;
	uint16_t T; // instruction type
	uint16_t D; // destination instruction ( A, M, D or any combination)
	uint16_t J; // jump instruction
	uint16_t C; // compute instruction
	int16_t compVal; // computed value
	uint16_t dest; // destination

	// fetch next instruction
	curInst = h->ROM[h->PC];

	// increment to next instruction
	h->PC += 1;

	// decode instruction type: A (0)  or C (1)
	T = (0b1000000000000000 & curInst) >> 15;
	
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

			// compute value
			compVal = compute(C, h);

			// place computed value in destination(s)
			store(h, D, compVal);

			// calculate jump
			jump(J, compVal, h);

			break;
	}
}

int16_t compute(uint16_t C, Hack *h) {
	int16_t compVal;

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
	return compVal;
}

void store(Hack *h, uint16_t D, int16_t compVal) {
	// place computed value in destination(s)
	if(0b001 & D) {
		h->RAM[h->A] = compVal;
	}
	if((0b010 & D) >> 1) {
		h->D = compVal;
	}
	if((0b100 & D) >> 2) {
		h->A = compVal;
	}
}

void jump(uint16_t J, int16_t compVal, Hack *h) {
	switch(J) {
		case 0b000:
			break;
		case 0b001:
			if(compVal > 0) h->PC = h->A;
			break;
		case 0b010:
			if(compVal == 0) h->PC = h->A;
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
}
