#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN (1 << 28) / 8

int main(void) {
	double *d;
	int i;

	if((d = (double *)malloc(1 << 28)) == NULL) {
		printf("Memory allocation error\n");
		exit(1);
	}

	for(i = 0;i < MAX_LEN;i++) {
		d[i] = 1.23456789;
		printf("%d\t%f\n", i, d[i]);
	}

	free(d);
}
