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
    int score;
} Player;

int screen_init();
Player player_init();
void player_dir_change(Player*, int, int);
void player_update(Player*);
void draw(Player*);
void player_input(Player*, char);
int generate_coord(int, int);
