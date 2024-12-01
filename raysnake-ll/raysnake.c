#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "raylib.h"

#define LINES 40
#define COLS 64
#define MAX_SEGMENTS 100

typedef struct Piece {
	int r;
	int c;
	struct Piece *next;
} Piece;

typedef struct Player {
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
void addItemAtHead(Piece**, int, int);
void addItemAtTail(Piece *, int, int);
void deleteItemAtHead(Piece **);
bool eatsFood(Piece *, int, int);
Piece getItemAtHead(Piece *);
bool hitsWall(Piece*);
bool biteSelf(Piece*);

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

	return 0;
}

Player player_init()
{
    Player p;
    int i;

	p.head = NULL;
	addItemAtHead(&p.head, LINES / 2, COLS / 2);

    p.rvel = 0;
    p.cvel = 1;
    p.alive = 1;

    p.food.r = generate_coord(LINES - 2, 2);
    p.food.c = generate_coord(COLS - 2, 1);

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

void player_update(Player *p)
{
	// basic snake movement
	addItemAtTail(p->head, p->rvel, p->cvel);
	deleteItemAtHead(&p->head);

	// if snake eats food, it gets bigger
	if(eatsFood(p->head, p->food.r, p->food.c)) {
		Piece new = getItemAtHead(p->head);
		addItemAtHead(&p->head, new.r, new.c);
		p->tail++;

		p->food.r = generate_coord(LINES - 2, 2);
		p->food.c = generate_coord(COLS - 2, 1);
	}

	// if snake hits a wall, it dies
	if(hitsWall(p->head)) {
		p->alive = 0;
	}

	// if snake bites itself, it dies
	if(biteSelf(p->head)) {
		p->alive = 0;
	}
}

void draw(Player *p)
{
    int i, j;	// loop vars (duh)
	char s[10]; // score
	Piece *tmp;

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

			//draw the snake
			tmp = p->head;
			while(tmp != NULL) {
				DrawRectangle(tmp->c * 16, tmp->r * 16, 16, 16, GREEN);
				tmp = tmp->next;
			}

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

void addItemAtHead(Piece **head, int r, int c) {
	Piece *tmp;

	if((tmp = (Piece *)malloc(sizeof(Piece))) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	tmp->r = r;
	tmp->c = c;
	tmp->next = *head;

	*head = tmp;
}

void deleteItemAtHead(Piece **head)
{
	Piece *tmp;

	tmp = *head;

	if(tmp != NULL) {
		*head = tmp->next;
		free(tmp);
	}
}

void addItemAtTail(Piece *head, int r, int c) {
	// we're just adding a piece to the end of the linked list (which is actually the head of the snake)
	Piece *nextNode;
	Piece *tmp;
	nextNode = head;

	while(nextNode->next != NULL) {
		nextNode = nextNode->next;
	}

	if((tmp = (Piece *)malloc(sizeof(Piece))) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	tmp->r = nextNode->r + r;
	tmp->c = nextNode->c + c;
	tmp->next = NULL;
	nextNode->next = tmp;
}

bool eatsFood(Piece *head, int r, int c) {
	bool eatsFood = false;
	Piece *nextNode;
	nextNode = head;

	while (nextNode->next != NULL) {
		nextNode = nextNode->next;
	}

	if(nextNode->r == r && nextNode->c == c) {
		eatsFood = true;
	}

	return eatsFood;
}

bool hitsWall(Piece *head) {
	bool hitsWall = false;
	Piece *nextNode;
	nextNode = head;

	while (nextNode->next != NULL) {
		nextNode = nextNode->next;
	}

	if(nextNode->r < 2 || nextNode->r > (LINES - 2)
		|| nextNode->c < 1 || nextNode->c > (COLS-2)) {

		hitsWall = true;
	}

	return hitsWall;
}

bool biteSelf(Piece *head) {
	bool biteSelf = false;
	Piece *nextNode;
	int r, c;
	nextNode = head;

	while (nextNode->next != NULL) {
		nextNode = nextNode->next;
	}

	r = nextNode->r;
	c = nextNode->c;

	nextNode = head;
	while(nextNode != NULL) {
		if(nextNode->c == c && nextNode->r == r && nextNode->next != NULL
			&& nextNode->next->next != NULL) {
			biteSelf = true;
		}
		nextNode = nextNode->next;
	}

	return biteSelf;
}

Piece getItemAtHead(Piece *head) {
	Piece p;

	p.r = head->r;
	p.c = head->c;

	return p;
}
