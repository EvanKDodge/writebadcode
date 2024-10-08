#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FB 100
#define MAX_LEN 20

typedef struct Entry
{
	int num;		// 4 bytes
	char word[28];	// 28 bytes
} Entry;			// 32 total...I think

void checkfactor(int num, int factor, char *s, char *out)
{
	if(num % factor == 0)
	{
		strcat(out, s);
	}
}

int read_data(Entry *entry_ptr)
{
    FILE *ptr;
    int i, linesread;

    ptr = fopen("fb.txt", "r");

    if(ptr == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    i = 0;
    while(fscanf(ptr, "%d%s", &entry_ptr[i].num, entry_ptr[i].word) != EOF)
    {
        i++;
    }

    linesread = i;

    fclose(ptr);

	return linesread;
}

int main (void)
{
	int i, j;
	char *output;
	Entry *entries;
	int num_entries;

	entries = calloc(MAX_LEN, 32);
	num_entries = read_data(entries);
	output = malloc(num_entries * 4 + 1);

	for(i = 1;i <= MAX_FB;i++)
	{
		strcpy(output, "");

		for(j=0;j < num_entries;j++)
		{
			checkfactor(i, entries[j].num, entries[j].word, output);
		}

		if(strcmp(output, "") == 0)
		{
			printf("%d\n", i);
		}
		else
		{
			printf("%s\n", output);
		}
	}

	free(output);
	free(entries);
}
