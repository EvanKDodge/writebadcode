#include "raylib.h"
#include <stdio.h>

#define DEF_WIDTH 640
#define DEF_HEIGHT 480
#define SPR_WIDTH 16
#define SPR_HEIGHT 16

typedef struct Tile {
	int x;
	int y;
} Tile;

typedef struct Player {
	int x;
	int y;
	int map_x;
	int map_y;
	int scale_x;
	int scale_y;
	int facing;
} Player;

int main(void) {
	Player dude;
	Texture2D tilemap;

	InitWindow(DEF_WIDTH, DEF_HEIGHT, "The Towers of Eugor");
	SetTargetFPS(60);
	HideCursor();

	tilemap = LoadTexture("assets/Tilemap/tilemap_packed.png");


	dude.x = 20;
	dude.y = 14;
	dude.map_x = 3;
	dude.map_y = 8;
	dude.scale_x = 2;
	dude.scale_y = 2;
	dude.facing = 1;
	
	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(DARKGRAY);

			DrawTexturePro(tilemap,
				(Rectangle) {						// tilemap location and texture size
					dude.map_x*SPR_WIDTH,
					dude.map_y*SPR_HEIGHT,
					SPR_WIDTH * dude.facing,
					SPR_HEIGHT
				},
				(Rectangle) {						// render location and scale
					dude.x*SPR_WIDTH,
					dude.y*SPR_HEIGHT,
					SPR_WIDTH*dude.scale_x,
					SPR_HEIGHT*dude.scale_y
				},
				(Vector2){0,0},						// origin (within texture)
				0.0f,									// rotation
				WHITE);
		EndDrawing();

		if(IsKeyPressed(KEY_UP)) {
			dude.y -= dude.scale_y;
		}
		if(IsKeyPressed(KEY_DOWN)) {
			dude.y += dude.scale_y;
		}
		if(IsKeyPressed(KEY_LEFT)) {
			dude.x -= dude.scale_x;
			dude.facing = -1;
		}
		if(IsKeyPressed(KEY_RIGHT)) {
			dude.x += dude.scale_x;
			dude.facing = 1;
		}
		if(IsKeyPressed(KEY_T)) { ToggleFullscreen(); }
	}

	CloseWindow();

	return 0;
}
