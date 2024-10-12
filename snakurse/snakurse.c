#include <curses.h>
#include <unistd.h>

typedef struct Player {
	int r;
	int c;
	int rvel;
	int cvel;
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
		mvaddch(user.r, user.c, '@');
		switch(c)
		{
			case 'w':
				player_dir_change(&user, -1, 0);
				break;
			case 's':
				player_dir_change(&user, 1, 0);
				break;
			case 'a':
				player_dir_change(&user, 0, -1);
				break;
			case 'd':
				player_dir_change(&user, 0, 1);
				break;
		};
		player_update(&user);
		usleep(150000);
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
	p.cvel = 0;

	return p;
}
