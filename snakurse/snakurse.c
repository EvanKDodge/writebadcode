#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SEGMENTS 1600

typedef struct Piece {
	int r;
	int c;
	char face;
} Piece;

typedef struct Player {
	Piece body[MAX_SEGMENTS];
	int rvel;
	int cvel;
	int alive;
	int tail;
	Piece food;
} Player;

int screen_init();
Player player_init();
void player_dir_change(Player*, int, int);
void player_update(Player*);
void draw(Player*);
void player_input(Player*, char);
int generate_coord(int);

int main(void)
{
	char c;
	Player user;
	Piece food;


	screen_init();
	user = player_init();

	while((c = getch()) != 'q')
	{
		erase(); //clear the screen

		player_input(&user, c);
		if(user.alive)
		{
			player_update(&user);
			usleep(100000);
		}

		draw(&user); //draw what needs drawing
		refresh();   //refresh the screen
	}

	endwin();

	return 0;
}

int screen_init()
{
	initscr(); //init curses
	cbreak(); //no buffering...make characters immediately available
	noecho(); //don't show characters the user has typed
	nodelay(stdscr, TRUE); //make getch() a non-blocking call
	curs_set(0); //make cursor invisible (for now anyway)

	return 0;
}

void player_dir_change(Player *p, int rvel, int cvel)
{
	p->rvel = rvel;
	p->cvel = cvel;
}

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

			    p->food.r = generate_coord(LINES - 1);
				p->food.c = generate_coord(COLS - 1);
			}

			// if the snake hits a wall, it dies
			if(p->body[i].r < 0 || p->body[i].r > (LINES - 1) || 
				p->body[i].c < 0 || p->body[i].c > (COLS - 1))
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

void draw(Player *p)
{
	int i;

	if(p->alive)
	{
		//draw the snake
		for(i = 0;p->body[i].r > 0 && p->body[i].c > 0;i++)
		{
			mvaddch(p->body[i].r, p->body[i].c, p->body[i].face);
		}

		//draw the food
		mvaddch(p->food.r, p->food.c, p->food.face);
	}
	else
	{
		mvaddstr(LINES / 2, COLS / 2 - 4, "Game Over");
		mvaddstr(LINES / 2 + 2, COLS / 2 - 9, "Press 'r' to reset");
	}
}

void player_input(Player *p, char c)
{
	if(p->alive)
	{
		switch(c)
		{
			case 'w':
				if(p->rvel != 1)
					player_dir_change(p, -1, 0);
				break;
			case 's':
				if(p->rvel != -1)
					player_dir_change(p, 1, 0);
				break;
			case 'a':
				if(p->cvel != 1)
					player_dir_change(p, 0, -1);
				break;
			case 'd':
				if(p->cvel != -1)
					player_dir_change(p, 0, 1);
				break;
		};
	}
	else
	{
		if(c == 'r')
		{
			*p = player_init();
		}
	}
}

Player player_init()
{
	Player p;
	int i;

	p.body[0].r = LINES / 2;
	p.body[0].c = COLS / 2;
	p.body[0].face = '@';
	p.rvel = 0;
	p.cvel = 1;
	p.alive = 1;
	
	p.food.r = generate_coord(LINES);
	p.food.c = generate_coord(COLS);
	p.food.face = '$';

	p.tail = 0;
	for(i = 1;i < MAX_SEGMENTS;i++)
	{
		// marking segments that are not in use
		p.body[i].r = -1;
		p.body[i].c = -1;
	}

	return p;
}

int generate_coord(int range)
{
    int num;

    srand(time(NULL));

    num = rand() % (range + 1) + 1;

    return num;
}
