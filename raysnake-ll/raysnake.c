#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define LINES 40
#define COLS 64
#define MAX_SEGMENTS 100

typedef struct Piece {
	int r;
	int c;
	char face;
	struct Piece *next;
} Piece;

typedef struct Player {
	//Piece body[MAX_SEGMENTS];
	Piece *head;
	int rvel;
	int cvel;
	int alive;
	int tail;
	Piece food;
} Player;

Player player_init();
void player_input(Player*);
void player_dir_change(Player*, int, int);
void player_update(Player*);
void draw(Player*);
int generate_coord(int, int);
void addItemAtHead(Piece**, int, int, char);
void printList(Piece*);

int main(void) {
	Player user;
	const int scrWidth = 1024; // 64 cols (16 pixels/tile) (0..63)
	const int scrHeight = 640; // 40 rows (16 pixels/tile) (0..39)

	InitWindow(scrWidth, scrHeight, "RaySnake");

	SetTargetFPS(10);

	user = player_init();

	while(!WindowShouldClose()) {
		player_input(&user);

		if(user.alive)
		{
			player_update(&user);
		}

		draw(&user);
	}

	CloseWindow();

	printList(user.head);

	return 0;
}

Player player_init()
{
    Player p;
    int i;

    /* p.body[0].r = LINES / 2;
    p.body[0].c = COLS / 2;
    p.body[0].face = '@'; */

	p.head = NULL;
	addItemAtHead(&p.head, LINES / 2, COLS / 2, '@');

    p.rvel = 0;
    p.cvel = 1;
    p.alive = 1;

    p.food.r = generate_coord(LINES - 2, 2);
    p.food.c = generate_coord(COLS - 2, 1);
    p.food.face = '$';

    p.tail = 0;

    return p;
}

void player_input(Player *p)
{
    if(p->alive)
    {
		if(IsKeyPressed(KEY_UP) && p->rvel != 1)
			player_dir_change(p, -1, 0);
		if(IsKeyPressed(KEY_DOWN) && p->rvel != -1)
			player_dir_change(p, 1, 0);
		if(IsKeyPressed(KEY_LEFT) && p->cvel != 1)
			player_dir_change(p, 0, -1);
		if(IsKeyPressed(KEY_RIGHT) && p->cvel != -1)
			player_dir_change(p, 0, 1);
    }
    else
    {
        if(IsKeyPressed(KEY_R))
        {
            *p = player_init();
        }
	}

	if(IsKeyPressed(KEY_T)) { ToggleFullscreen(); }
}

void player_dir_change(Player *p, int rvel, int cvel)
{
    p->rvel = rvel;
    p->cvel = cvel;
}

// TODO: update for linked-list
void player_update(Player *p)
{
    int i;

    for(i = p->tail;i >= 0;i--)
    {
        if(i == 0)
        {
            p->body[i].r += p->rvel;
            p->body[i].c += p->cvel;

            // if the snake eats food
            if(p->body[i].r == p->food.r && p->body[i].c == p->food.c)
            {
                p->body[p->tail+1].r = p->body[p->tail].r;
                p->body[p->tail+1].c = p->body[p->tail].c;
                p->body[p->tail+1].face = '#';
                p->tail++;

                p->food.r = generate_coord(LINES - 2, 2);
                p->food.c = generate_coord(COLS - 2, 1);
            }

            // if the snake hits a wall, it dies
            if(p->body[i].r < 2 || p->body[i].r > (LINES - 2) ||
                p->body[i].c < 1 || p->body[i].c > (COLS - 2))
            {
                p->alive = 0;
            }
        }
        else
        {
            p->body[i].r = p->body[i-1].r;
            p->body[i].c = p->body[i-1].c;
        }
    }

    // if the snake bites itself, it dies
    for(i = 1;i <= p->tail;i++)
    {
        if(p->body[i].r == p->body[0].r
            && p->body[i].c == p->body[0].c
            && i > 1)
        {
            p->alive = 0;
        }
    }
}
// TODO

void draw(Player *p)
{
    int i, j;
	char s[10];

	BeginDrawing();
		ClearBackground(DARKBLUE);

		if(p->alive)
		{
			//draw the playfield
			for(i = 0;i <= LINES - 1;i++)
			{
				for(j = 0;j <= COLS - 1;j++)
				{
					if(i == 1 || i == LINES - 1)
						DrawRectangle(j * 16, i * 16, 16, 16, WHITE);
					else if(j == 0 || j == COLS - 1)
						DrawRectangle(j * 16, i * 16, 16, 16, WHITE);
				}
			}
			DrawText("Score:", 20, 2, 10, WHITE);
			sprintf(s, "%d", p->tail);
			DrawText(s, 80, 2, 10, WHITE);

			//TODO: update for linked-list
			//draw the snake
			for(i = 0;p->body[i].r > 0 && p->body[i].c > 0;i++)
			{
				DrawRectangle(p->body[i].c * 16, p->body[i].r * 16, 16, 16, GREEN);
			}
			//TODO

			//draw the food
			DrawRectangle(p->food.c * 16, p->food.r * 16, 16, 16, PINK);
		}
		else
		{
			DrawText("Game Over", (COLS / 2) * 16 - (4 * 16), (LINES / 2) * 16, 20, GREEN);
			DrawText("Press r to reset", (COLS / 2) * 16 - (7 * 16), (LINES / 2) * 16 + 32, 20, GREEN);
		}

	EndDrawing();
}

int generate_coord(int high, int low)  // time and stdlib
{
    int num;

    srand(time(NULL));
    num = rand() % (high - low + 1) + low;

    return num;
}

// linked list functions:

void addItemAtHead(Piece **head, int r, int c, char face) {
	Piece *tmp;

	if((tmp = (Piece *)malloc(sizeof(Piece))) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	tmp->r = r;
	tmp->c = c;
	tmp->face = face;
	tmp->next = *head;

	*head = tmp;
}

void printList(Piece *head) {
	Piece *nextNode;
	nextNode = head;

	do {
		printf("%d %d %c\n", nextNode->r, nextNode->c, nextNode->face);
		nextNode = nextNode->next;
	} while(nextNode != NULL);
}
