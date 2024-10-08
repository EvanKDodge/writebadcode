#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 20

typedef struct Entry {
    int num;        // 4 bytes
    char word[28];  // 28 bytes
} Entry;            // 32 total...I think

int main(void)
{
	FILE *ptr;
	Entry *entry_ptr;
	int i, linesread;

	entry_ptr = calloc(MAX_LEN, 32);

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

	for(i = 0;i < linesread;i++)
	{
		printf("Factor: %d Word: %s\n", entry_ptr[i].num, entry_ptr[i].word);
	}

	free(entry_ptr);
	fclose(ptr);
}
