#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define N 10

// Player Profile
typedef struct player {
    char Name[50];
    char **grid;
} Player;

// Function prototypes
void StartTurn(Player *Current, Player *Other); // Starts the match with the first player
void MovePicker(Player *Playing, Player *Other); // Function supposed to let the Player choose a move (not developped yet)
void Fire_Move(char **Player_Grid, char **Opponent_Grid, char Difficulty[5]); // Added Player's grid and opponent's grid
int Ship_Placment(char **grid, int row, int column, int length, char orientation[12]);
bool PlayerLost(Player *player); // Checks if a player has lost all its ships

// Written functions
int main()
{
    char Difficulty[5];
    Player Player1;
    Player Player2;

    srand(time(0));
    int Random_Varible = (rand() % 3);

    char **Battle_Floor = (char **)malloc(sizeof(char*) * N);
    for (int i = 0; i < N; i++)
    {
        Battle_Floor[i] = (char *)malloc(sizeof(char) * N);
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
    scanf("%4s", &Difficulty); // In the easy mode, the grid will track both the hits and the misses,
    //                              whereas in the hard mode, the grid will only track hits
    printf("\n");

    printf("Player 1 please enter you name: ");
    scanf("%49s", Player1.Name);
    Player1.grid = Battle_Floor;
    printf("\n");

    printf("Player 2 please enter you name: ");
    scanf("%49s", Player2.Name);
    Player2.grid = Battle_Floor;
    printf("\n");

    Player *CurrentPlayer;
    Player *OtherPlayer;
    if (Random_Varible == 1)
    {
        printf("Player 1 starts! \n");
        CurrentPlayer = &Player1;
        OtherPlayer = &Player2;
    }
    else {
        printf("Player 2 starts!\n");
        CurrentPlayer = &Player2;
        OtherPlayer = &Player1;
    }

    StartTurn(CurrentPlayer,OtherPlayer);

    return 0;
}

void StartTurn(Player *Current, Player *Other) {

    do {
        MovePicker(Current, Other);
        Player *temp = Current;
        Current = Other;
        Other = temp;

    } while (true);
}
void MovePicker(Player *Playing, Player *Other)
{
    
}

void Fire_Move(char **Player_Grid, char **Opponent__Grid, char Difficulty[5]) // no need to do difficulty manager if we have here 
{
    char col;
    int row;

    printf("Enter your guess: ");
    scanf(" %c%d", &col, &row);

    int Column_Index = col - 'A'; // hon we just check the index using ascii code w the one under we do -1 to get index 0
    int Row_Index = row - 1;

    if (Row_Index >= 0 && Row_Index < N && Column_Index >= 0 && Column_Index < N)
    {
        if (Opponent__Grid[Row_Index][Column_Index] == '~')
        {
            printf("LOSER you missed :) \n");
            if (Difficulty == "easy")
            {
                Player_Grid[Row_Index][Column_Index] = 'X';
            }
        }
        else if (Opponent__Grid[Row_Index][Column_Index] == 'h')
        {
            printf("You already shot there\n");
        }
        else
        {
            printf("Bravo Darabta\n");
            Opponent__Grid[Row_Index][Column_Index] = 'h';
        }
    }
    else
    {
        printf("Choose a number from a-j and number 1-10\n");
    }
}

int Ship_Placment(char **grid, int row, int column, int length, char orientation[12])
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
        for (int j = column; j < column + length; j++)
        {
            grid[row][j] = '-'; // Marking fragment of the ship
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
        for (int i = column; i < column + length; i++)
        {
            grid[row][i] = '|'; // Marking fragment of the ship
        }
    }
    return 1;
}


bool PlayerLost(Player *player) {
    char **grid = player->grid;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) 
            if (grid[i][j] == '-' || grid[i][j] == '|')
                return 0;
    }
    return 1;
}