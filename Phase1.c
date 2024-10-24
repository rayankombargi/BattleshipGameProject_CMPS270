#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define GRID_SIZE 10
#define NUM_SHIPS 4

#define EASY 0
#define HARD 1

#define MAX_RADAR_SWEEPS 3

// Structure Definitions

typedef struct {
    char name[20];
    int size;
    int hits;
    int sunk;
    int coordinates[5][2];  // Maximum size is 5 (for Carrier)
} Ship;

typedef struct {
    char display;    // What is shown on the grid ('~', '*', 'o')
    int hasShip;     // 1 if there's a ship, 0 otherwise
    int isHit;       // 1 if the cell has been targeted, 0 otherwise
    int hasSmoke;    // 1 if the cell is under smoke screen, 0 otherwise
} GridCell;

typedef struct {
    char name[50];
    GridCell grid[GRID_SIZE][GRID_SIZE];
    Ship ships[NUM_SHIPS];
    int radarCount;
    int smokeCount;
    int artilleryAvailable;
    int torpedoAvailable;
    int shipsSunk;
} Player;


void initializeGrid(GridCell grid[GRID_SIZE][GRID_SIZE]);
void displayGrid(GridCell grid[GRID_SIZE][GRID_SIZE], int trackingDifficulty);
int getTrackingDifficulty();
void initializePlayer(Player *player);
void placeShips(Player *player);
int validateAndPlaceShip(GridCell grid[GRID_SIZE][GRID_SIZE], Ship *ship, int row, int col, char *orientation);
void performMove(Player *attacker, Player *defender, int trackingDifficulty);
void showMoveOptions(Player *player);
void performFire(Player *attacker, Player *defender, char *coord);
void performRadar(Player *attacker, Player *defender, char *coord);
void performSmoke(Player *player, char *coord);
void performArtillery(Player *attacker, Player *defender, char *coord);
void performTorpedo(Player *attacker, Player *defender, char *input);
void updateShipStatus(Player *player, int row, int col);
void unlockSpecialMoves(Player *player);
int checkWinCondition(Player *player);
void gameLoop(Player *player1, Player *player2, int trackingDifficulty);
void clearScreen();
void clearInputBuffer();
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
                Battle_Floor_General[Row_Index][Column_Index] = 'X';
            }
        }
        else if (Battle_Floor_General[Row_Index][Column_Index] == 'h')
        {
            printf("You already shot there\n");
        }
        else
        {
            printf("Bravo Darabta\n");
            Battle_Floor_General[Row_Index][Column_Index] = 'h';
        }
    }
    else
    {
        printf("Choose a number from a-j and number 1-10\n");
    }
}


int main() {
    srand(time(NULL));

    Player player1, player2;
    int trackingDifficulty;

    printf("Welcome to Advanced Battleship!\n");

    trackingDifficulty = getTrackingDifficulty();

    initializePlayer(&player1);
    clearScreen();

    initializePlayer(&player2);
    clearScreen();

    // Each player places their ships
    printf("\n%s, place your ships.\n", player1.name);
    placeShips(&player1);
    clearScreen();

    printf("\n%s, place your ships.\n", player2.name);
    placeShips(&player2);
    clearScreen();

    // Randomly choose the first player
    Player *currentPlayer, *opponent;
    if (rand() % 2 == 0) {
        currentPlayer = &player1;
        opponent = &player2;
    } else {
        currentPlayer = &player2;
        opponent = &player1;
    }
    printf("%s will go first.\n", currentPlayer->name);

    // Start the game loop
    gameLoop(currentPlayer, opponent, trackingDifficulty);

    return 0;
}
