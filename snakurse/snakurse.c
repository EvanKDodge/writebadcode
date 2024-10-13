#include <curses.h>
#include <unistd.h>

typedef struct Player {
	int r;
	int c;
	int rvel;
	int cvel;
	int alive;
} Player;

int screen_init();
Player player_init();
void player_dir_change(Player*, int, int);
void player_update(Player*);

int main(void)
{
	char c;
	Player user;

	screen_init();
	user = player_init();

	while((c = getch()) != 'q')
	{
		erase();
		if(user.alive)
		{
			mvaddch(user.r, user.c, '@');
			if(user.r < 0 || user.r > (LINES - 1) || user.c < 0 || user.c > (COLS - 1))
			{
				user.alive = 0;
			}
			switch(c)
			{
				case 'w':
					if(user.rvel != 1)
						player_dir_change(&user, -1, 0);
					break;
				case 's':
					if(user.rvel != -1)
						player_dir_change(&user, 1, 0);
					break;
				case 'a':
					if(user.cvel != 1)
						player_dir_change(&user, 0, -1);
					break;
				case 'd':
					if(user.cvel != -1)
						player_dir_change(&user, 0, 1);
					break;
			};
			player_update(&user);
			usleep(100000);
		}
		else
		{
			mvaddstr(LINES / 2, COLS / 2 - 4, "Game Over");
			mvaddstr(LINES / 2 + 2, COLS / 2 - 9, "Press 'r' to reset");
			if(c == 'r')
			{
				user = player_init();
			}
		}
		refresh();
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
	p->r += p->rvel;
	p->c += p->cvel;
}

Player player_init()
{
	Player p;

	p.r = LINES / 2;
	p.c = LINES / 2;
	p.rvel = 0;
	p.cvel = 1;
	p.alive = 1;

	return p;
}
