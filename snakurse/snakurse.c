#include <curses.h>
#include <unistd.h>

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

int main(void)
{
	int r, c, rvel, cvel;

	//curses initialization
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);

	//init ball
	r = LINES / 2;
	c = COLS / 2;
	rvel = 1;
	cvel = 1;

	while(!kbhit())
	{
		if(c >= COLS - 1 || c <= 0)
		{
			cvel = -cvel;
		}
		if(r >= LINES - 1 || r <= 0)
		{
			rvel = -rvel;
		}
		erase();
		mvaddch(r, c, 'O');
		c += cvel;
		r += rvel;
		refresh();
		usleep(30000);
	}

	endwin();
}
