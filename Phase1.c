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

typedef struct {
    char name[20];
    int size;
    int hits;
    int sunk;
    int coordinates[5][2]; 
} Ship;

typedef struct {
    char display;   
    int hasShip;   
    int isHit;      
    int hasSmoke;   
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



int main() {
    srand(time(NULL)); // to randomize dr zalgout said we can do it like dr zalghout said 

    Player player1, player2;
    int trackingDifficulty;

    trackingDifficulty = getTrackingDifficulty();

    initializePlayer(&player1);
    clearScreen();

    initializePlayer(&player2);
    clearScreen();

    printf("\n%s, place your ships.\n", player1.name);
    placeShips(&player1);
    clearScreen();

    printf("\n%s, place your ships.\n", player2.name);
    placeShips(&player2);
    clearScreen();

    Player *currentPlayer, *opponent;
    if (rand() % 2 == 0) {
        currentPlayer = &player1;
        opponent = &player2;
    } else {
        currentPlayer = &player2;
        opponent = &player1;
    }
    printf("%s will go first.\n", currentPlayer->name);

    gameLoop(currentPlayer, opponent, trackingDifficulty);

    return 0;
}

void initializeGrid(GridCell grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].display = '~';
            grid[i][j].hasShip = 0;
            grid[i][j].isHit = 0;
            grid[i][j].hasSmoke = 0;
        }
    }
}

void displayGrid(GridCell grid[GRID_SIZE][GRID_SIZE], int trackingDifficulty) {
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%2d", i + 1);
        for (int j = 0; j < GRID_SIZE; j++) {
            if (trackingDifficulty == HARD && grid[i][j].isHit == 0) {
                printf(" ~");
            } else {
                printf(" %c", grid[i][j].display);
            }
        }
        printf("\n");
    }
}

int getTrackingDifficulty() {
    int choice;
    printf("Select tracking difficulty level:\n");
    printf("1. Easy (tracks hits and misses)\n");
    printf("2. Hard (tracks hits only)\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &choice);
    while (choice != 1 && choice != 2) {
        printf("Invalid choice. Please enter 1 or 2: ");
        scanf("%d", &choice);
    }
    clearInputBuffer();
    return (choice == 1) ? EASY : HARD;
}

void initializePlayer(Player *player) { // got help from my friend (Fadel) on this due to some errors 
    printf("Enter player name: ");
    fgets(player->name, sizeof(player->name), stdin);
    player->name[strcspn(player->name, "\n")] = '\0'; 
    initializeGrid(player->grid);
    player->radarCount = MAX_RADAR_SWEEPS;
    player->smokeCount = 0;
    player->artilleryAvailable = 0;
    player->torpedoAvailable = 0;
    player->shipsSunk = 0;

    char *shipNames[NUM_SHIPS] = {"Carrier", "Battleship", "Destroyer", "Submarine"};
    int shipSizes[NUM_SHIPS] = {5, 4, 3, 2};

    for (int i = 0; i < NUM_SHIPS; i++) {
        strncpy(player->ships[i].name, shipNames[i], sizeof(player->ships[i].name));
        player->ships[i].size = shipSizes[i];
        player->ships[i].hits = 0;
        player->ships[i].sunk = 0;
        memset(player->ships[i].coordinates, 0, sizeof(player->ships[i].coordinates));
    }
}