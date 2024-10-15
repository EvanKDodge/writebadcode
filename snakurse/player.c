#include <curses.h>
#include "snakurse.h"

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

                p->food.r = generate_coord(LINES - 2, 2);
                p->food.c = generate_coord(COLS - 2, 1);

                p->score++;
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

    p.food.r = generate_coord(LINES - 2, 2);
    p.food.c = generate_coord(COLS - 2, 1);
    p.food.face = '$';

    p.score = 0;

    p.tail = 0;

    return p;
}
