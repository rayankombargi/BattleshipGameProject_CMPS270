#include <stdio.h>
#include <stdlib.h>

#define N 10

void Difficulty_Managment(char Difficlty[5]){



}

void Starting_Player(){


    
}

int main()
{
    char Battle_Floor[N][N];
    char Difficulty[5];
    char Player_1_Name[50];
    char Player_2_Name[50];


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Battle_Floor[i][j] = '~';
        }
    }

    printf("   ");
    for (char Top_Letter_Indexing = 'A'; Top_Letter_Indexing < 'A' + N; Top_Letter_Indexing++)
    {
        printf("%c ", Top_Letter_Indexing);
    }
    printf("\n");

    for (int i = 0; i < N; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < N; j++)
        {
            printf("%c ", Battle_Floor[i][j]);
        }
        printf("\n");
    }

    printf("Choose the difficulty (Easy/Hard): ");
    scanf("%4s", &Difficulty);//In the easy mode, the grid will track both the hits and the misses,
//                              whereas in the hard mode, the grid will only track hits
    printf("\n");


    printf("Player 1 please enter you name: ");
    scanf("%49s",Player_1_Name );
    printf("\n");

    printf("Player 2 please enter you name: ");
    scanf("%49s",Player_2_Name );
    printf("\n");    

    return 0;
}
