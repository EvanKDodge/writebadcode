#include <stdio.h>

char rot13b(char c, char base)
{
	return (c - base + 13) % 26 + base;
}

char rot13(char c)
{
	// upper case
	if(c >= 'A' && c <= 'Z') 
	{
		return rot13b(c, 'A');
	}
	// lower case
	else if (c >= 'a' && c <= 'z')
	{
		return rot13b(c, 'a');
	}
	else
	{
		return c;
	}
}

int main(void)
{
	int c;

	while((c = getchar()) != EOF)
	{
		putchar(rot13(c));
	}
}
