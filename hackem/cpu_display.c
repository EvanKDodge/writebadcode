#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "hackem.h"

#define TAB_STOP 75.0f
#define LEFT_X 20.0f
#define TOP_Y 20.0f
#define ROW_Y 30.0f

void show_cpu(Hack* h) {
	uint16_t curInst;
    uint16_t T; // instruction type
    uint16_t D; // destination instruction ( A, M, D or any combination)
    uint16_t J; // jump instruction
    uint16_t C; // compute instruction
	char sComp[4];
	char sJump[5];
	char sDest[5];
	char sAsmInst[16];	// same instruction in assembly

	curInst = h->ROM[h->PC];

	T = (0b1000000000000000 & curInst) >> 15;

	switch(T) {
		case 0:
			strcpy(sAsmInst, "");
			sprintf(sAsmInst, "@%d", curInst);
			break;
		case 1:
			strcpy(sAsmInst, "");
			D = (0x0038 & curInst) >> 3;
			if(D) {
				show_dest(D, sDest);
				strcat(sAsmInst, sDest);
				strcat(sAsmInst, "=");
			}

			C = (0x1FC0 & curInst) >> 6;
			show_compute(C, sComp);
			strcat(sAsmInst, sComp);

			J = (0x0007 & curInst);
			show_jump(J, sJump);
			strcat(sAsmInst, sJump);

			break;
	}

	// draw loop
	BeginDrawing();
		ClearBackground(BLACK);

		DrawTextEx(h->fontTTF, "PC:",
			(Vector2){ LEFT_X, TOP_Y },
			(float)h->fontTTF.baseSize, 2, WHITE);
		DrawTextEx(h->fontTTF, TextFormat("%016b", h->PC),
			(Vector2){LEFT_X + TAB_STOP, TOP_Y},
			(float)h->fontTTF.baseSize, 2, WHITE);
		DrawTextEx(h->fontTTF, "A:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 1) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->A),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 1) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, "D:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 2) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->D),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 2) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, "M:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 3) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->RAM[h->A]),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 3) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, "I:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 4) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->ROM[h->PC]),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 4) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, "Asm:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 5) },
            (float)h->fontTTF.baseSize, 2, WHITE);
        DrawTextEx(h->fontTTF, TextFormat("%s", sAsmInst),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 5) },
            (float)h->fontTTF.baseSize, 2, WHITE);
	EndDrawing();
}

void show_compute(uint16_t C, char* sComp) {
	strcpy(sComp, "");
	switch(C) {
		case 0b0101010:
			strcpy(sComp, "0");
			break;
		case 0b0111111:
			strcpy(sComp, "1");
			break;
		case 0b0111010:
			strcpy(sComp, "-1");
			break;
		case 0b0001100:
			strcpy(sComp, "D");
			break;
		case 0b0110000:
			strcpy(sComp, "A");
			break;
		case 0b0001101:
			strcpy(sComp, "!D");
			break;
		case 0b0110001:
			strcpy(sComp, "!A");
			break;
		case 0b0001111:
			strcpy(sComp, "-D");
			break;
		case 0b0110011:
			strcpy(sComp, "-A");
			break;
		case 0b0011111:
			strcpy(sComp, "D+1");
			break;
		case 0b0110111:
			strcpy(sComp, "A+1");
			break;
		case 0b0001110:
			strcpy(sComp, "D-1");
			break;
		case 0b0110010:
			strcpy(sComp, "A-1");
			break;
		case 0b0000010:
			strcpy(sComp, "D+A");
			break;
		case 0b0010011:
			strcpy(sComp, "D-A");
			break;
		case 0b0000111:
			strcpy(sComp, "A-D");
			break;
		case 0b0000000:
			strcpy(sComp, "D&A");
			break;
		case 0b0010101:
			strcpy(sComp, "D|A");
			break;
		case 0b1110000:
			strcpy(sComp, "M");
			break;
		case 0b1110001:
			strcpy(sComp, "!M");
			break;
		case 0b1110011:
			strcpy(sComp, "-M");
			break;
		case 0b1110111:
			strcpy(sComp, "M+1");
			break;
		case 0b1110010:
			strcpy(sComp, "M-1");
			break;
		case 0b1000010:
			strcpy(sComp, "D+M");
			break;
		case 0b1010011:
			strcpy(sComp, "D-M");
			break;
		case 0b1000111:
			strcpy(sComp, "M-D");
			break;
		case 0b1000000:
			strcpy(sComp, "D&M");
			break;
		case 0b1010101:
			strcpy(sComp, "D|M");
			break;
	}
}

void show_dest(uint16_t D, char* sDest) {
	strcpy(sDest, "");
	if(0b001 & D) {
		strcat(sDest, "M");
	}
	if((0b010 & D) >> 1) {
		strcat(sDest, "D");
	}
	if((0b100 & D) >> 2) {
		strcat(sDest, "A");
	}
}

void show_jump(uint16_t J, char* sJump) {
	strcpy(sJump, "");
	switch(J) {
		case 0b000:
			break;
		case 0b001:
			strcpy(sJump, ";JGT");
			break;
		case 0b010:
			strcpy(sJump, ";JEQ");
			break;
		case 0b011:
			strcpy(sJump, ";JGE");
			break;
		case 0b100:
			strcpy(sJump, ";JLT");
			break;
		case 0b101:
			strcpy(sJump, ";JNE");
			break;
		case 0b110:
			strcpy(sJump, ";JLE");
			break;
		case 0b111:
			strcpy(sJump, ";JMP");
			break;
	}
}
