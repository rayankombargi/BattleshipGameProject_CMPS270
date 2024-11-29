#include <stdio.h>
#include <unistd.h>
#include <windows.h>
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

typedef struct {
    int hitFound;
    enum decision {FireSearch, RadarSearch} dec;
    char Target[2];
    char current[2];
    enum direction {Up, Down, Left, Right} dir;
    int row, col, i, j;
} Memory;


void initializeGrid(GridCell grid[GRID_SIZE][GRID_SIZE]);
void displayGrid(GridCell grid[GRID_SIZE][GRID_SIZE], int trackingDifficulty);
int getTrackingDifficulty();
void initializePlayer(Player *player);
void initializeBot(Player *bot);
void initializeMemory(Memory *mem);
void placeShips(Player *player);
void BotPlaceShips(Player *bot);
int validateAndPlaceShip(GridCell grid[GRID_SIZE][GRID_SIZE], Ship *ship, int row, int col, char *orientation);
void Move(Player *attacker, Player *defender, char * input, Memory *mem);
void performMove(Player *attacker, Player *defender, Memory *mem, int trackingDifficulty);
void BotPerformMove(Player *attacker, Player *defender, Memory *mem, int trackingDifficulty);
void BotDecision(Player *attacker, Player *defender, Memory *mem);
void showMoveOptions(Player *player);
void SearchFire(Player *attacker, Player *defender, char *coord, Memory *mem);
void SearchRadar(Player *attacker, Player *defender, char *coord, Memory *mem);
void performFire(Player *attacker, Player *defender, char *coord, Memory *mem);
void performRadar(Player *attacker, Player *defender, char *coord, Memory *mem);
void performSmoke(Player *player, char *coord);
void performArtillery(Player *attacker, Player *defender, char *coord, Memory *mem);
void performTorpedo(Player *attacker, Player *defender, char *input, Memory *mem);
void updateShipStatus(Player *player, int row, int col, Player *attacker);
void unlockSpecialMoves(Player *defender, Player *attacker);
int checkWinCondition(Player *player);
void gameLoop(Player *player1, Player *player2, Memory *mem, int trackingDifficulty);
void clearScreen();
void clearInputBuffer();



int main() {
    srand(time(NULL)); // to randomize dr zalgout said we can do it like dr zalghout said 

    Player player, bot;
    
    Memory m;
    initializeMemory(&m);

    int trackingDifficulty;

    trackingDifficulty = getTrackingDifficulty();

    initializePlayer(&player);
    clearScreen();

    initializeBot(&bot);

    printf("\n%s, place your ships.\n", player.name);
    placeShips(&player);                 // Skipped the process of the player to put his/her Ships
    clearScreen();

    BotPlaceShips(&bot);
    Sleep(1000);
    clearScreen();

    Player *currentPlayer, *opponent;

    int FirstPlayer = rand() % 2;
    if (FirstPlayer == 0) {
        currentPlayer = &player;
        opponent = &bot;
    } else if (FirstPlayer == 1) {
        currentPlayer = &bot;
        opponent = &player;
    }
    printf("%s will go first.\n", currentPlayer->name);

    Memory *mem = &m;
    gameLoop(currentPlayer, opponent, mem, trackingDifficulty);     // the gameLoop method is under development

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
    do {
        printf("Enter player name: ");
        fgets(player->name, sizeof(player->name), stdin);
        player->name[strcspn(player->name, "\n")] = '\0'; 
        if (!strcmp(player->name, "Bot")) {
            printf("Player's name cannot be 'Bot', try again.\n");
        }
    }
    while (!strncmp(player->name, "Bot", 3));

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

void initializeBot(Player *bot) {
    strncpy(bot->name, "Bot", 3);
    initializeGrid(bot->grid);
    bot->radarCount = MAX_RADAR_SWEEPS;
    bot->smokeCount = 0;
    bot->artilleryAvailable = 0;
    bot->torpedoAvailable = 0;
    bot->shipsSunk = 0;

    char *shipNames[NUM_SHIPS] = {"Carrier", "Battleship", "Destroyer", "Submarine"};
    int shipSizes[NUM_SHIPS] = {5, 4, 3, 2};

    for (int i = 0; i < NUM_SHIPS; i++) {
        strncpy(bot->ships[i].name, shipNames[i], sizeof(bot->ships[i].name));
        bot->ships[i].size = shipSizes[i];
        bot->ships[i].hits = 0;
        bot->ships[i].sunk = 0;
        memset(bot->ships[i].coordinates, 0, sizeof(bot->ships[i].coordinates));
    }
}

void initializeMemory(Memory *mem) {
    mem->hitFound = 0;
    mem->dec = FireSearch;
    strcpy(mem->Target,"  ");       strcpy(mem->current,"  ");
    mem->dir = Down;
    mem->row = 0;   mem->col = 0;
    mem->i = 0; mem->j = 0;
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

void BotPlaceShips(Player *bot)
{
    srand(time(NULL));
    char orientation[15];

    for (int i = 0; i < NUM_SHIPS; i++)
    {
        int validPlacement = 0;
        while (!validPlacement)
        {
            int row, col;
            row = rand() % 10;
            col = rand() % 10;

            if ((rand() % 2) == 0)
                strcpy(orientation, "horizontal");
            else
                strcpy(orientation, "vertical");

            validPlacement = validateAndPlaceShip(bot->grid, &bot->ships[i], row, col, orientation);
            if (validPlacement)
            {
                char letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
                printf("Bot's %s placed successfully.\t%c%d\t%s\n", bot->ships[i].name, letter, row+1, orientation);  // Bot's placed ships are visible for debugging purposes
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

void Move(Player *attacker, Player *defender,char * input, Memory *mem) {
    if (strncmp(input, "Fire", 4) == 0)
    {
        char coord[10];
        if (sscanf(input, "Fire %s", coord) == 1)
        {
            performFire(attacker, defender, coord, mem);
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
                performRadar(attacker, defender, coord, mem);
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
                performArtillery(attacker, defender, coord, mem);
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
                performTorpedo(attacker, defender, param, mem);
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
void performMove(Player *attacker, Player *defender, Memory *mem, int trackingDifficulty)
{
    char input[50];
    printf("\n%s's turn.\n", attacker->name);
    displayGrid(defender->grid, trackingDifficulty);
    showMoveOptions(attacker);

    printf("Enter your move: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    Move(attacker, defender, input, mem);
}

void BotPerformMove(Player *attacker, Player *defender, Memory *mem, int trackingDifficulty) {
    printf("\n%s's turn.\n", attacker->name);
    displayGrid(defender->grid, trackingDifficulty);
    showMoveOptions(attacker);

    printf("Enter your move: ");

    Sleep(2000);

    BotDecision(attacker, defender, mem);

    // Move(attacker, defender, input);
}

void BotDecision(Player *attacker, Player *defender, Memory *mem) {
    srand(time(NULL));

    enum command {Fire, Radar, Smoke, Artillery, Torpedo} mve;
    if (mem->hitFound == 0) {

        if (attacker->radarCount > 0) {
            int action = rand() % 4;
            if (action == 0) {
                mve = Radar;
            } else if (action == 1 || action == 2 || action == 3){
                mve = Fire;
            }
        }
        else {
            mve = Fire;
        }

        int row = 0; int col = 0;
        do {
            row = rand() % 10;
            col = rand() % 10;
        } while(defender->grid[row][col].isHit);

        char letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character

        char coord[2];  sprintf(coord, "%c%d", letter, row+1);
        

        // char coord[2];  sprintf(coord, "%c%d", 'D', 5); // for testing
        if (mve == Radar) {
            printf("Radar %s\n", coord);
            performRadar(attacker, defender, coord, mem);
        } else if (mve == Fire) {
            printf("Fire %s\n", coord);
            performFire(attacker, defender, coord, mem);
        }
    } else {
        if (mem->dec == RadarSearch) {
            if (attacker->artilleryAvailable) {
                performArtillery(attacker, defender, mem->Target, mem);
            } //else 
                //SearchRadar(attacker, defender, mem->Target, mem);
        } else if (mem->dec == FireSearch) {
            SearchFire(attacker, defender, mem->Target, mem);
        }
            
    }

}
void SearchFire(Player *attacker, Player *defender, char *coord, Memory *mem) {

    char localCoord[2];
    char letter;
    int row = coord[1]-1; int col = coord[0]-64-1;

    if (mem->i < 5) {
        mem->i = mem->i + 1;
        if (mem->dir == Down) {
            if (row + mem->i >= 10) {
                mem->i = 1;
                mem->dir = Up;
                row = row - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else {
                row = row + mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }
        if (mem->dir == Up) {
            if (row - mem->i >= 10) {
                mem->i = 1;
                mem->dir = Down;
                row = row + mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else {
                row = row - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }
        if (mem->dir == Right) {
            if (col + mem->i >= 10) {
                mem->i = 1;
                mem->dir = Left;
                col = col - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else {
                col = col + mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }
        if (mem->dir == Left) {
            if (col - mem->i >= 10) {
                mem->i = 1;
                mem->dir = Right;
                col = col + mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else {
                col = col - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }
    } else {
        mem->i = 1;
        if (mem->dir == Down) {
            if (row + mem->i < 10) {
                mem->dir = Up;
                row = row - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else if (col + mem->i < 10) {
                mem->dir = Right;
                col = col + mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else {
                mem->dir = Left;
                col = col - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }
        if (mem->dir == Up) {
            if (col + mem->i < 10) {
                mem->dir = Right;
                col = col + mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
            else {
                mem->dir = Left;
                col = col - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }
        if (mem->dir == Right) {
            if (col - mem->i > 0) {
                mem->dir = Left;
                col = col - mem->i;
                letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
            }
        }

    }

    sprintf(localCoord, "%c%d", letter, mem->row + 1);
    printf("Fire %s\n", localCoord);
    performFire(attacker, defender, localCoord, mem);
}

void SearchRadar(Player *attacker, Player *defender, char *coord, Memory *mem) {
    char localCoord[2];
    int *row, *col;

    row = &mem->row;
    col = &mem->col;

    // Update the coordinates for each turn

    char letter = (char)(64 + mem->col+1); // ASCII conversion of the value of the col integer into its corresponding character  

    sprintf(localCoord, "%c%d", letter, row);
    performFire(attacker, defender, localCoord, mem);
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



void performFire(Player *attacker, Player *defender, char *coord, Memory *mem)
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
        if (strcmp(attacker->name, "Bot") == 0) {
            if (mem->dec == FireSearch && mem->hitFound == 1) {
                if (mem->i > 0)
                    mem->i == 0;
                if (mem->j > 0)
                    mem->j == 0;
                if (mem->dir == Down)
                    mem->dir = Up;
                if (mem->dir == Up)
                    mem->dir = Right;
                if (mem->dir == Right)
                    mem->dir = Left;
                if (mem->dir == Left) {
                    mem->hitFound = 0;
                }
            }
        }
        return;
    }

    cell->isHit = 1;

    if (cell->hasShip)
    {
        cell->display = '*';
        printf("Hit!\n");
        updateShipStatus(defender, row, col, attacker);
        if (strcmp(attacker->name, "Bot") == 0)
        {
            if (mem->hitFound == 0) {
                mem->hitFound = 1;
                strncpy(mem->Target, coord, sizeof(coord));
                mem->dec = FireSearch;
            } 
        }
    }
    else
    {
        cell->display = 'o';
        printf("Miss.\n");
        if (strcmp(attacker->name, "Bot") == 0) {
            if (mem->dec == FireSearch && mem->hitFound == 1) {
                if (mem->i > 0)
                    mem->i == 0;
                if (mem->j > 0)
                    mem->j == 0;
                if (mem->dir == Down)
                    mem->dir = Up;
                if (mem->dir == Up)
                    mem->dir = Right;
                if (mem->dir == Right)
                    mem->dir = Left;
                if (mem->dir == Left) {
                    mem->hitFound = 0;
                }
            }
        }
    }
}

void performRadar(Player *attacker, Player *defender, char *coord, Memory *mem)
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
        if (strcmp(attacker->name, "bot") == 0)
        {
            if (mem->hitFound == 0) {
                mem->hitFound = 1;
                strncpy(mem->Target, coord, sizeof(coord));
                mem->dec = RadarSearch;
            }
        }
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

void performArtillery(Player *attacker, Player *defender, char *coord, Memory *mem)
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
                    updateShipStatus(defender, i, j, attacker);
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
        if (strcmp(attacker->name, "Bot") == 0){
            if (mem->hitFound == 1) {
                mem->hitFound = 0;
            }
        }
    }
    else
    {
        printf("Artillery strike missed.\n");
    }
}

void performTorpedo(Player *attacker, Player *defender, char *input, Memory *mem)
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
                    updateShipStatus(defender, i, index, attacker);
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
                    updateShipStatus(defender, index, j, attacker);
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

void updateShipStatus(Player *player, int row, int col, Player *attacker)
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
                    unlockSpecialMoves(player, attacker);
                }
                return;
            }
        }
    }
}

void unlockSpecialMoves(Player *defender, Player *attacker)
{
    attacker->smokeCount++;

    if (defender->shipsSunk == 1)
    {
        attacker->artilleryAvailable = 1;
        printf("%s has unlocked Artillery for the next turn!\n", attacker->name);
    }
    if (defender->shipsSunk == 3)
    {
        attacker->torpedoAvailable = 1;
        printf("%s has unlocked Torpedo for the next turn!\n", attacker->name);
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

void gameLoop(Player *currentPlayer, Player *opponent, Memory *mem, int trackingDifficulty)
{
    while (1)
    {
        if (!strncmp(opponent->name, "Bot", 3))
        {
            performMove(currentPlayer, opponent, mem, trackingDifficulty);
            Sleep(3000);
        }
        else {
            BotPerformMove(currentPlayer, opponent, mem, trackingDifficulty);
            Sleep(3000);
        }
        if (checkWinCondition(opponent))
        {
            printf("\n%s wins the game!\n", currentPlayer->name);
            break;
        }

        Player *temp = currentPlayer;
        currentPlayer = opponent;
        opponent = temp;
        clearScreen();
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
}

// new line to show in main 
