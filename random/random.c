#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_coord(int high, int low)  // time and stdlib
{
    int num;

    //srand(time(NULL));
    
    num = rand() % 256;

    return num;
}

int main(void) {
	int rnum;

	srand(time(NULL));

	while(rnum != 0) {
		rnum = generate_coord(255,0);
		printf("%d\n", rnum);
	}
}
