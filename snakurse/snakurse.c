#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "snakurse.h"

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
			usleep(125000);
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

void draw(Player *p)
{
	int i, j;

	if(p->alive)
	{
		//draw the playfield
		for(i = 0;i <= LINES - 1;i++)
		{
			for(j = 0;j <= COLS - 1;j++)
			{
				if(i == 1 || i == LINES - 1)
					mvaddch(i, j, '-');
				else if(j == 0 || j == COLS - 1)
					mvaddch(i, j, '|');
			}
		}
		mvaddstr(0, 2, "Score:");
		mvprintw(0, 9, "%d", p->tail);

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

int generate_coord(int high, int low)  // time and stdlib
{
    int num;

    srand(time(NULL));

    num = rand() % (high - low + 1) + low;

    return num;
}
