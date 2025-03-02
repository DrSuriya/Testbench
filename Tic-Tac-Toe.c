#include <stdio.h>	
#include <stdlib.h>	
#include <math.h>	
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


void toLowerCase(char *str)
{
	int length = strlen(str);
	for (int i = 0; i < length; i++)
	{
		str[i] = tolower(str[i]);
	}
}

bool wincondition(char *board, char ox)
{
	int winconditions[8][3] = {
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},
		{0, 4, 8},
		{2, 4, 6}
	};

	for (int i = 0; i < 8; i++)
	{
		int a = winconditions[i][0];
		int b = winconditions[i][1];
		int c = winconditions[i][2];
		if (ox == board[a] && ox == board[b] && ox == board[c])
		{
			return true;
		}
	}
	return false;
}

void playtictactoe()
{

		printf("The following is the starting board\n");
		char arr[9] = "         ";
		int arrSize = 9;
		printf("%c|%c|%c\n-----\n%c|%c|%c\n-----\n%c|%c|%c\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6],
			arr[7], arr[8]);

		for (int i = 0; i < arrSize; i++)
		{
			printf("Round %d\n", i + 1);

			char ox;
			if (i % 2 == 0)
			{
				ox = 'O';
			}
			else if (i % 2 == 1)
			{
				ox = 'X';
			}
			int input;

			printf("Where do you want to put your %c:\n", ox);
			scanf("%d", &input);
			if (arr[input - 1] == ' ')
			{
				printf("Updating Grid \n");
				arr[input - 1] = ox;
				printf("%c|%c|%c\n-----\n%c|%c|%c\n-----\n%c|%c|%c\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5],
					arr[6], arr[7], arr[8]);
			}
			else
			{
				printf("Invalid Input, retry\n");
				i--;
			}

			if (wincondition(arr, ox))
			{
				printf("Congrats the %c player has won in round %d\n", ox, i + 1);
				break;
			}
			else if (i + 1 == 9)
			{
				printf("Unfortunately it was a draw, better luck next time\n");
				break;
			}
		}

		printf("Do you want to play again:\n");
		char replay[10];
		scanf("%s", replay);
		toLowerCase(replay);
		if (strcmp(replay, "yes") == 0)
		{
			playtictactoe();
		}
		else if (strcmp(replay, "no") == 0)
		{
			printf("Too bad\n");
		}
		else
		{
			printf("Invalid Input, Program is closing\n");
		}

}

int main()
{
	playtictactoe();
	return 0;
}
