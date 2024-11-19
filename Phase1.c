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

typedef struct
{
    char name[20];
    int size;
    int hits;
    int sunk;
    int coordinates[5][2]; // for Carrier size 5
} Ship;

typedef struct
{
    char display; // What is shown on the grid ('~', '*', 'o')
    int hasShip;  // 1 if there's a ship, 0 else
    int isHit;    // 1 if the cell has been targeted, 0 else
    int hasSmoke; // 1 if the cell is under smoke screen, 0 else
} GridCell;

typedef struct
{
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
int main()
{
    srand(time(NULL));

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
    if (rand() % 2 == 0)
    {
        currentPlayer = &player1;
        opponent = &player2;
    }
    else
    {
        currentPlayer = &player2;
        opponent = &player1;
    }
    printf("%s will go first.\n", currentPlayer->name);
    gameLoop(currentPlayer, opponent, trackingDifficulty);

    return 0;
}

void initializeGrid(GridCell grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j].display = '~';
            grid[i][j].hasShip = 0;
            grid[i][j].isHit = 0;
            grid[i][j].hasSmoke = 0;
        }
    }
}

void displayGrid(GridCell grid[GRID_SIZE][GRID_SIZE], int trackingDifficulty)
{
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf("%2d", i + 1);
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (trackingDifficulty == HARD && grid[i][j].isHit == 0)
            {
                printf(" ~");
            }
            else
            {
                printf(" %c", grid[i][j].display);
            }
        }
        printf("\n");
    }
}

int getTrackingDifficulty()
{
    int choice;
    printf("Select tracking difficulty level:\n");
    printf("1. Easy (tracks hits and misses)\n");
    printf("2. Hard (tracks hits only)\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &choice);
    while (choice != 1 && choice != 2)
    {
        printf("Invalid choice. Please enter 1 or 2: ");
        scanf("%d", &choice);
    }
    clearInputBuffer();
    return (choice == 1) ? EASY : HARD;
}

void initializePlayer(Player *player)
{
    printf("Enter player name: ");
    fgets(player->name, sizeof(player->name), stdin);
    player->name[strcspn(player->name, "\n")] = '\0';
    initializeGrid(player->grid);
    player->radarCount = MAX_RADAR_SWEEPS;
    player->smokeCount = 0;
    player->artilleryAvailable = 0;
    player->torpedoAvailable = 0;
    player->shipsSunk = 0;

    // Initialize ships
    char *shipNames[NUM_SHIPS] = {"Carrier", "Battleship", "Destroyer", "Submarine"};
    int shipSizes[NUM_SHIPS] = {5, 4, 3, 2};

    for (int i = 0; i < NUM_SHIPS; i++)
    {
        strncpy(player->ships[i].name, shipNames[i], sizeof(player->ships[i].name));
        player->ships[i].size = shipSizes[i];
        player->ships[i].hits = 0;
        player->ships[i].sunk = 0;
        memset(player->ships[i].coordinates, 0, sizeof(player->ships[i].coordinates));
    }
}

void placeShips(Player *player)
{
    char coord[10];
    char orientation[15];

    for (int i = 0; i < NUM_SHIPS; i++)
    {
        int validPlacement = 0;
        while (!validPlacement)
        {
            printf("%s, place your %s (size %d).\n", player->name, player->ships[i].name, player->ships[i].size);
            printf("Enter starting coordinate (e.g., B3): ");
            fgets(coord, sizeof(coord), stdin);
            coord[strcspn(coord, "\n")] = '\0';

            printf("Enter orientation (horizontal/vertical): ");
            fgets(orientation, sizeof(orientation), stdin);
            orientation[strcspn(orientation, "\n")] = '\0';
            int row, col;
            if (sscanf(coord, "%c%d", &coord[0], &row) != 2)
            {
                printf("Invalid coordinate format. Try again.\n");
                continue;
            }
            row -= 1;
            col = toupper(coord[0]) - 'A';

            validPlacement = validateAndPlaceShip(player->grid, &player->ships[i], row, col, orientation);
            if (!validPlacement)
            {
                printf("Invalid placement. Try again.\n");
            }
            else
            {
                printf("Ship placed successfully.\n");
            }
        }
    }
}

int validateAndPlaceShip(GridCell grid[GRID_SIZE][GRID_SIZE], Ship *ship, int row, int col, char *orientation)
{
    int dr = 0, dc = 0;
    if (strcmp(orientation, "horizontal") == 0)
    {
        dc = 1;
    }
    else if (strcmp(orientation, "vertical") == 0)
    {
        dr = 1;
    }
    else
    {
        return 0;
    }

    if (row < 0 || col < 0 || row >= GRID_SIZE || col >= GRID_SIZE ||
        row + dr * (ship->size - 1) >= GRID_SIZE || col + dc * (ship->size - 1) >= GRID_SIZE)
    {
        return 0;
    }

    for (int i = 0; i < ship->size; i++)
    {
        if (grid[row + dr * i][col + dc * i].hasShip)
        {
            return 0;
        }
    }
    for (int i = 0; i < ship->size; i++)
    {
        grid[row + dr * i][col + dc * i].hasShip = 1;
        ship->coordinates[i][0] = row + dr * i;
        ship->coordinates[i][1] = col + dc * i;
    }
    return 1;
}

void performMove(Player *attacker, Player *defender, int trackingDifficulty)
{
    char input[50];
    printf("\n%s's turn.\n", attacker->name);
    displayGrid(defender->grid, trackingDifficulty);
    showMoveOptions(attacker);

    printf("Enter your move: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strncmp(input, "Fire", 4) == 0)
    {
        char coord[10];
        if (sscanf(input, "Fire %s", coord) == 1)
        {
            performFire(attacker, defender, coord);
        }
        else
        {
            printf("Invalid command format. You lose your turn.\n");
        }
    }
    else if (strncmp(input, "Radar", 5) == 0)
    {
        if (attacker->radarCount > 0)
        {
            char coord[10];
            if (sscanf(input, "Radar %s", coord) == 1)
            {
                performRadar(attacker, defender, coord);
            }
            else
            {
                printf("Invalid command format. You lose your turn.\n");
            }
        }
        else
        {
            printf("No radar sweeps remaining. You lose your turn.\n");
        }
    }
    else if (strncmp(input, "Smoke", 5) == 0)
    {
        if (attacker->smokeCount > 0)
        {
            char coord[10];
            if (sscanf(input, "Smoke %s", coord) == 1)
            {
                performSmoke(attacker, coord);
            }
            else
            {
                printf("Invalid command format. You lose your turn.\n");
            }
        }
        else
        {
            printf("No smoke screens available. You lose your turn.\n");
        }
    }
    else if (strncmp(input, "Artillery", 9) == 0)
    {
        if (attacker->artilleryAvailable)
        {
            char coord[10];
            if (sscanf(input, "Artillery %s", coord) == 1)
            {
                performArtillery(attacker, defender, coord);
                attacker->artilleryAvailable = 0; // Can use only once
            }
            else
            {
                printf("Invalid command format. You lose your turn.\n");
            }
        }
        else
        {
            printf("Artillery not available. You lose your turn.\n");
        }
    }
    else if (strncmp(input, "Torpedo", 7) == 0)
    {
        if (attacker->torpedoAvailable)
        {
            char param[10];
            if (sscanf(input, "Torpedo %s", param) == 1)
            {
                performTorpedo(attacker, defender, param);
                attacker->torpedoAvailable = 0; // Can use only once
            }
            else
            {
                printf("Invalid command format. You lose your turn.\n");
            }
        }
        else
        {
            printf("Torpedo not available. You lose your turn.\n");
        }
    }
    else
    {
        printf("Invalid command. You lose your turn.\n");
    }
}

void showMoveOptions(Player *player)
{
    printf("Available moves:\n");
    printf("- Fire [coordinate]\n");
    if (player->radarCount > 0)
        printf("- Radar [coordinate] (Remaining: %d)\n", player->radarCount);
    if (player->smokeCount > 0)
        printf("- Smoke [coordinate] (Available: %d)\n", player->smokeCount);
    if (player->artilleryAvailable)
        printf("- Artillery [coordinate] (Available)\n");
    if (player->torpedoAvailable)
        printf("- Torpedo [row/column] (Available)\n");
}

void performFire(Player *attacker, Player *defender, char *coord)
{
    int row, col;
    if (sscanf(coord, "%c%d", &coord[0], &row) != 2)
    {
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }
    row -= 1;
    col = toupper(coord[0]) - 'A';

    if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
    {
        printf("Invalid coordinates. You lose your turn.\n");
        return;
    }

    GridCell *cell = &defender->grid[row][col];

    if (cell->isHit)
    {
        printf("You already targeted this location. You lose your turn.\n");
        return;
    }

    cell->isHit = 1;

    if (cell->hasShip)
    {
        cell->display = '*';
        printf("Hit!\n");
        updateShipStatus(defender, row, col);
    }
    else
    {
        cell->display = 'o';
        printf("Miss.\n");
    }
}

void performRadar(Player *attacker, Player *defender, char *coord)
{
    int row, col;
    if (sscanf(coord, "%c%d", &coord[0], &row) != 2)
    {
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }
    row -= 1;
    col = toupper(coord[0]) - 'A';

    if (row < 0 || row >= GRID_SIZE - 1 || col < 0 || col >= GRID_SIZE - 1)
    {
        printf("Invalid coordinates. You lose your turn.\n");
        return;
    }

    int found = 0;
    for (int i = row; i <= row + 1; i++)
    {
        for (int j = col; j <= col + 1; j++)
        {
            if (defender->grid[i][j].hasSmoke)
                continue;
            if (defender->grid[i][j].hasShip)
            {
                found = 1;
                break;
            }
        }
        if (found)
            break;
    }

    if (found)
    {
        printf("Enemy ships found.\n");
    }
    else
    {
        printf("No enemy ships found.\n");
    }
    attacker->radarCount--;
}

void performSmoke(Player *player, char *coord)
{
    int row, col;
    if (sscanf(coord, "%c%d", &coord[0], &row) != 2)
    {
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }
    row -= 1;
    col = toupper(coord[0]) - 'A';

    if (row < 0 || row >= GRID_SIZE - 1 || col < 0 || col >= GRID_SIZE - 1)
    {
        printf("Invalid coordinates. You lose your turn.\n");
        return;
    }

    for (int i = row; i <= row + 1; i++)
    {
        for (int j = col; j <= col + 1; j++)
        {
            player->grid[i][j].hasSmoke = 1;
        }
    }
    player->smokeCount--;
    clearScreen();
    printf("Smoke screen deployed.\n");
}

void performArtillery(Player *attacker, Player *defender, char *coord)
{
    int row, col;
    if (sscanf(coord, "%c%d", &coord[0], &row) != 2)
    {
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }
    row -= 1;
    col = toupper(coord[0]) - 'A';

    if (row < 0 || row >= GRID_SIZE - 1 || col < 0 || col >= GRID_SIZE - 1)
    {
        printf("Invalid coordinates. You lose your turn.\n");
        return;
    }

    int hit = 0;
    for (int i = row; i <= row + 1; i++)
    {
        for (int j = col; j <= col + 1; j++)
        {
            GridCell *cell = &defender->grid[i][j];
            if (!cell->isHit)
            {
                cell->isHit = 1;
                if (cell->hasShip)
                {
                    cell->display = '*';
                    updateShipStatus(defender, i, j);
                    hit = 1;
                }
                else
                {
                    cell->display = 'o';
                }
            }
        }
    }

    if (hit)
    {
        printf("Artillery strike hit enemy ships!\n");
    }
    else
    {
        printf("Artillery strike missed.\n");
    }
}

void performTorpedo(Player *attacker, Player *defender, char *input)
{
    char rc = toupper(input[0]);
    int index;
    int hit = 0;

    if (isalpha(rc))
    {
        index = rc - 'A';
        if (index < 0 || index >= GRID_SIZE)
        {
            printf("Invalid column. You lose your turn.\n");
            return;
        }
        for (int i = 0; i < GRID_SIZE; i++)
        {
            GridCell *cell = &defender->grid[i][index];
            if (!cell->isHit)
            {
                cell->isHit = 1;
                if (cell->hasShip)
                {
                    cell->display = '*';
                    updateShipStatus(defender, i, index);
                    hit = 1;
                }
                else
                {
                    cell->display = 'o';
                }
            }
        }
    }
    else
    {
        index = atoi(input) - 1;
        if (index < 0 || index >= GRID_SIZE)
        {
            printf("Invalid row. You lose your turn.\n");
            return;
        }
        for (int j = 0; j < GRID_SIZE; j++)
        {
            GridCell *cell = &defender->grid[index][j];
            if (!cell->isHit)
            {
                cell->isHit = 1;
                if (cell->hasShip)
                {
                    cell->display = '*';
                    updateShipStatus(defender, index, j);
                    hit = 1;
                }
                else
                {
                    cell->display = 'o';
                }
            }
        }
    }

    if (hit)
    {
        printf("Torpedo hit enemy ships!\n");
    }
    else
    {
        printf("Torpedo missed.\n");
    }
}

void updateShipStatus(Player *player, int row, int col)
{
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        Ship *ship = &player->ships[i];
        for (int j = 0; j < ship->size; j++)
        {
            if (ship->coordinates[j][0] == row && ship->coordinates[j][1] == col)
            {
                ship->hits++;
                if (ship->hits == ship->size && !ship->sunk)
                {
                    ship->sunk = 1;
                    player->shipsSunk++;
                    printf("You sunk %s's %s!\n", player->name, ship->name);
                    unlockSpecialMoves(player);
                }
                return;
            }
        }
    }
}

void unlockSpecialMoves(Player *player)
{
    player->smokeCount++;

    if (player->shipsSunk == 1)
    {
        player->artilleryAvailable = 1;
        printf("%s has unlocked Artillery for the next turn!\n", player->name);
    }
    if (player->shipsSunk == 3)
    {
        player->torpedoAvailable = 1;
        printf("%s has unlocked Torpedo for the next turn!\n", player->name);
    }
}

int checkWinCondition(Player *player)
{
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        if (!player->ships[i].sunk)
        {
            return 0;
        }
    }
    return 1;
}

void gameLoop(Player *currentPlayer, Player *opponent, int trackingDifficulty)
{
    while (1)
    {
        performMove(currentPlayer, opponent, trackingDifficulty);
        if (checkWinCondition(opponent))
        {
            printf("\n%s wins the game!\n", currentPlayer->name);
            break;
        }
        Player *temp = currentPlayer;
        currentPlayer = opponent;
        opponent = temp;
    }
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
    // to fix my branch
}