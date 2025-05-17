#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "hackem.h"

#define TAB_STOP 145.0f
#define LEFT_X 20.0f
#define TOP_Y 20.0f
#define ROW_Y 30.0f
#define TBOX_W 285.0f
#define TEXT_COLOR GREEN
#define SCR_LEFT_X 480
#define SCR_WIDTH 520
#define SCR_HEIGHT 264
#define X_COLS 512
#define Y_ROWS 256
#define SCR_BASE_ADDR 0x4000
#define SCR_ARR_LEN 0x2000

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

	draw_loop(h, sComp, sDest, sJump, sAsmInst);
}

void draw_loop(Hack *h, char *sComp, char *sDest, char *sJump, char *sAsmInst) {

	// draw loop
	BeginDrawing();
		ClearBackground(BLACK);

		DrawTextEx(h->fontTTF, "PC:",
			(Vector2){ LEFT_X, TOP_Y },
			(float)h->fontTTF.baseSize, 2, TEXT_COLOR);
		DrawTextEx(h->fontTTF, TextFormat("%016b", h->PC),
			(Vector2){LEFT_X + TAB_STOP, TOP_Y},
			(float)h->fontTTF.baseSize, 2, TEXT_COLOR);
		DrawTextEx(h->fontTTF, "A:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 1) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->A),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 1) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, "D:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 2) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->D),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 2) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, "RAM[A]:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 3) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->RAM[h->A]),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 3) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, "ROM[PC]:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 4) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, TextFormat("%016b", h->ROM[h->PC]),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 4) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, "Asm:",
            (Vector2){ LEFT_X, TOP_Y + (ROW_Y * 5) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);
        DrawTextEx(h->fontTTF, TextFormat("%s", sAsmInst),
            (Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * 5) },
            (float)h->fontTTF.baseSize, 2, TEXT_COLOR);

		// draw text input box if RAM is selected
		if(h->iColSelect) {
			Rectangle rRAMinput = {
				SCR_LEFT_X + TAB_STOP,
				TOP_Y + (ROW_Y * 9),
				TBOX_W,
				ROW_Y
				};
			DrawRectangleLinesEx(rRAMinput, 1, TEXT_COLOR);
		}

		// display ROM and RAM lists at bottom of screen
		int k;

		for(k = 0;k < 10;k++) {
			// display ROM
			DrawTextEx(h->fontTTF, TextFormat("%04x", h->iROMstart+k),
				(Vector2){ LEFT_X, TOP_Y + (ROW_Y * (10+k)) },
				(float)h->fontTTF.baseSize, 2, isSelected(h, k, 0));
			DrawTextEx(h->fontTTF, TextFormat("%016b", h->ROM[h->iROMstart+k]),
				(Vector2){LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * (10+k)) },
				(float)h->fontTTF.baseSize, 2, isSelected(h, k, 0));
			
			// display RAM
            DrawTextEx(h->fontTTF, TextFormat("%04x", h->iRAMstart+k),
                (Vector2){ SCR_LEFT_X, TOP_Y + (ROW_Y * (10+k)) },
                (float)h->fontTTF.baseSize, 2, isSelected(h, k, 1));
            DrawTextEx(h->fontTTF, TextFormat("%016b", h->RAM[h->iRAMstart+k]),
                (Vector2){ SCR_LEFT_X + TAB_STOP, TOP_Y + (ROW_Y * (10+k)) },
                (float)h->fontTTF.baseSize, 2, isSelected(h, k, 1));
		}


		// draw contents of screen memory

		int i; // offset for screen address
		int j; // bit being processed
		int x, y; // for tracking x,y coord on Hack screen
		int byte_value;
		int px_value;

		for(i = 0;i < 0x2000;i++) {
			byte_value = h->RAM[SCR_BASE_ADDR + i];

			for(j = 0;j < 16;j++) {
				px_value = (byte_value >> j) & 1;
				x = ((i % 32) * 16) + j;
				y = i / 32;

                switch(px_value) {
                    case 0:
						h->screenArray[y][x] = BLACK;
                        break;
                    case 1:
						h->screenArray[y][x] = GREEN;
                        break;
                }
			}
		}

		UpdateTexture(h->screenTexture, h->screenArray);
		DrawTexturePro(h->screenTexture,
						(Rectangle) { 0, 0, 512, 256 },
                        (Rectangle) { SCR_LEFT_X + 4, 24, 512, 256 },
						(Vector2){0,0},
						0.0f,
						WHITE);

		// draw screen outline
		Rectangle scrBox = { SCR_LEFT_X, TOP_Y, SCR_WIDTH, SCR_HEIGHT };
		DrawRectangleLinesEx(scrBox, 1, TEXT_COLOR);
	EndDrawing();

}

Color isSelected(Hack* h, int row, int col) {
	if(h->iColSelect == col && h->iRowSelect == row) {
		return WHITE;
	}
	else {
		return TEXT_COLOR;
	}
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
