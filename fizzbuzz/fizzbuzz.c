#include <stdio.h>
#include <string.h>

#define MAX_FB 110

void checkfactor(int num, int factor, char *s, char *out)
{
	if(num % factor == 0)
	{
		strcat(out, s);
	}
}

int main (void)
{
	int i;
	char output[9];

	for(i = 1;i <= MAX_FB;i++)
	{
		strcpy(output, "");

		checkfactor(i, 3, "Fizz", output);
		checkfactor(i, 5, "Buzz", output);

		if(strcmp(output, "") == 0)
		{
			printf("%d\n", i);
		}
		else
		{
			printf("%s\n", output);
		}
	}
}
