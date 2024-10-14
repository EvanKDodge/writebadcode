#include <curses.h>
#include <unistd.h>

typedef struct Segment {
	int r;
	int c;
} Segment;

typedef struct Player {
	Segment body[1];
	int rvel;
	int cvel;
	int alive;
} Player;

int screen_init();
Player player_init();
void player_dir_change(Player*, int, int);
void player_update(Player*);
void draw(Player*);
void player_input(Player*, char);

int main(void)
{
	char c;
	Player user;

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
	if(p->body[0].r < 0 || p->body[0].r > (LINES - 1) || p->body[0].c < 0 || p->body[0].c > (COLS - 1))
	{
		p->alive = 0;
	}

	p->body[0].r += p->rvel;
	p->body[0].c += p->cvel;
}

void draw(Player *p)
{
	if(p->alive)
	{
		mvaddch(p->body[0].r, p->body[0].c, '@');
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

	p.body[0].r = LINES / 2;
	p.body[0].c = COLS / 2;
	p.rvel = 0;
	p.cvel = 1;
	p.alive = 1;

	return p;
}
