#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 20
#define MAX_NUM 100

int generate_guess(int range)
{
	int num;

	srand(time(NULL));

	num = rand() % (range + 1) + 1;

	return num;
}

int main(void)
{
	char guess_str[MAX_LEN];
	int num, guess;
	int tries;
	char cont[1];

	printf("Would you like to guess what number I'm thinking of?\n");
	scanf("%s", cont);
	
	while(strcmp(cont, "y") == 0)
	{
		num = generate_guess(100);
		printf("I am thinking of a number from 1 to 100. Guess the number.\n");

		tries = 0;
		do
		{
			scanf("%s", guess_str);
			guess = atoi(guess_str);

			tries++;

			if (guess > MAX_NUM) {
				printf("Too high. Guess a number between 1 and %d\n", MAX_NUM);
			}
			else if (guess > num)
			{
				printf("Smaller\n");
			}
			else if (guess < num)
			{
				printf("Larger\n");
			}
			else {
				printf("You guessed the number in %d tries!\n", tries);
			}

		} while(guess !=num);

		printf("Would you like to play again?\n");
		scanf("%s", cont);
	}
}
