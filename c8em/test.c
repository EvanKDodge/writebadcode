#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	//printf("%d\n", atoi(argv[1]) % 100);
	//printf("%d\n", atoi(argv[1])  % 10);
	int i = atoi(argv[1]);
	printf("%d\n", (i - (i % 100)) / 100);
	printf("%d\n", ((i % 100) - (i % 10)) / 10);
	printf("%d\n", i % 10);
}
