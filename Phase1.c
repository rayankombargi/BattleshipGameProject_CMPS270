#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//dear rayyan check this now please 

#define N 10

int Ship_Placmentt(char grid[N][N], int row, int column, int length, char orientation[12])
{
    if (orientation == "horizontal")
    {
        if (column + length > N)
            return 0;//overflow condition
        for (int j = column; j < column + length; j++)
        {
            if (grid[row][j] != '~')// checking if smth already exists 
                return 0; 
        }
    }
    else if (orientation == "vertical")
    { 
        if (row + length > N)
            return 0; //overflow condition
        for (int i = row; i < row + length; i++)
        {
            if (grid[i][column] != '~')// checking if smth already exists 
                return 0; 
        }
    }
    return 1; 
}

void Guess_Maker(char Battle_Floor_General[N][N], char Difficulty[5]) // no need to do difficulty manager if we have here
{

    char col;
    int row;

    printf("Enter your guess: ");
    scanf(" %c%d", &col, &row);

    int Column_Index = col - 'A'; // hon we just check the index using ascii code w the one under we do -1 to get index 0
    int Row_Index = row - 1;

    if (Row_Index >= 0 && Row_Index < N && Column_Index >= 0 && Column_Index < N)
    {
        if (Battle_Floor_General[Row_Index][Column_Index] == '~')
        {
            printf("LOSER you missed :) \n");
            if (Difficulty == "easy")
            {
                Battle_Floor_General[Row_Index][Column_Index] = 'o';
            }
        }
        else if (Battle_Floor_General[Row_Index][Column_Index] == '*')
        {
            printf("You already shot there\n");
        }
        else
        {
            printf("Bravo Darabta\n");
            Battle_Floor_General[Row_Index][Column_Index] = '*';
        }
    }
    else
    {
        printf("Choose a number from a-j and number 1-10\n");
    }
}

int main()
{
    char Battle_Floor_1[N][N];
    char Battle_Floor_2[N][N];
    char Difficulty[5];
    char Player_1_Name[50];
    char Player_2_Name[50];
    char Ship_Orientation[13];
    int Random_Varible = (rand() % 2 + 1);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Battle_Floor_1[i][j] = '~';
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
            printf("%c ", Battle_Floor_1[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Battle_Floor_2[i][j] = '~';
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
            printf("%c ", Battle_Floor_2[i][j]);
        }
        printf("\n");
    }

    printf("Choose the difficulty (Easy/Hard): ");
    scanf("%4s", &Difficulty); // In the easy mode, the grid will track both the hits and the misses,
    //                              whereas in the hard mode, the grid will only track hits
    printf("\n");

    printf("Player 1 please enter you name: ");
    scanf("%49s", Player_1_Name);
    printf("\n");

    printf("Player 2 please enter you name: ");
    scanf("%49s", Player_2_Name);
    printf("\n");
    if (Random_Varible == 1)
    {
        printf("Player 1 starts! \n");
    }
    else
        printf("Player 2 starts!\n");

    return 0;

<<<<<<< Updated upstream
    
=======
    printf("enter the boat orientation for the Carrier (5 cells); ");
    scanf(""); // continue for the rest 
>>>>>>> Stashed changes
}
