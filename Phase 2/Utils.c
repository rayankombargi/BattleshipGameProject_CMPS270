#include "Battleship.h"

void initializeGrid(GridCell grid[GRID_SIZE][GRID_SIZE]){
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].display = '~';
            grid[i][j].hasShip = 0;
            grid[i][j].isHit = 0;
            grid[i][j].hasSmoke = 0;
        }
    }
}

void displayGrid(GridCell grid[GRID_SIZE][GRID_SIZE], int trackingDifficulty){
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%2d", i + 1);
        for (int j = 0; j < GRID_SIZE; j++) {
            if (trackingDifficulty == HARD && grid[i][j].isHit == 0){
                printf(" ~");
            }
            else{
                printf(" %c", grid[i][j].display);
            }
        }
        printf("\n");
    }
    usleep(1000000);
}

int getTrackingDifficulty(){
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

void initializePlayerAttributes(Player *player){
    player->radarCount = MAX_RADAR_SWEEPS;
    player->smokeCount = 0;
    player->artilleryAvailable = 0;
    player->torpedoAvailable = 0;
    player->shipsSunk = 0;
}

void initializePlayer(Player *player) { 
    do {
        printf("Enter player name: ");
        fgets(player->name, sizeof(player->name), stdin);
        player->name[strcspn(player->name, "\n")] = '\0'; 
        if (!strcmp(player->name, "Bot")) {
            printf("Player's name cannot be 'Bot', try again.\n");
        }
    }
    while (strcmp(player->name, "Bot") == 0);

    initializeGrid(player->grid);
    initializePlayerAttributes(player);

    const char *shipNames[NUM_SHIPS] = {"Carrier", "Battleship", "Destroyer", "Submarine"}; // make const to avoid modifying
    const int shipSizes[NUM_SHIPS] = {5, 4, 3, 2}; // similarly, make const to avoid modifying

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
    initializePlayerAttributes(bot);

    const char *shipNames[NUM_SHIPS] = {"Carrier", "Battleship", "Destroyer", "Submarine"};
    const int shipSizes[NUM_SHIPS] = {5, 4, 3, 2};

    // delegate the below into a function
    for (int i = 0; i < NUM_SHIPS; i++) {
        strncpy(bot->ships[i].name, shipNames[i], sizeof(bot->ships[i].name));
        bot->ships[i].size = shipSizes[i];
        bot->ships[i].hits = 0;
        bot->ships[i].sunk = 0;
        memset(bot->ships[i].coordinates, 0, sizeof(bot->ships[i].coordinates));
    }
}


void placeShips(Player *player){
    char coord[10]; 
    char orientation[15]; 

    for (int i = 0; i < NUM_SHIPS; i++){

        int validPlacement = 0;
        while (validPlacement==0){

            printf("%s, place your %s (size %d).\n", player->name, player->ships[i].name, player->ships[i].size);
            usleep(1000000);
            printf("Enter starting coordinate (e.g., B3): ");
            fgets(coord, sizeof(coord), stdin);
            coord[strcspn(coord, "\n")] = '\0';

            printf("Enter orientation (horizontal/vertical): ");
            fgets(orientation, sizeof(orientation), stdin);
            orientation[strcspn(orientation, "\n")] = '\0';

            int row, col;
            if (sscanf(coord, "%c%d", &coord[0], &row) != 2){
                printf("Invalid coordinate format. Try again.\n");
                continue;
            }
            row -= 1; // convert to a 0-based index (between 0-9)
            col = toupper(coord[0]) - 'A'; // convert 'A'-'J' to 0-9

            validPlacement = validateAndPlaceShip(player->grid, &player->ships[i], row, col, orientation);
            if (!validPlacement){
                printf("Invalid placement. Try again.\n");
            }
            else{
                printf("Ship placed successfully.\n");
                usleep(1500000);
            }
        }
    }
}


void BotPlaceShips(Player *bot){
    srand(time(NULL)); // seed the random number generator

    char orientation[15];

    for (int i = 0; i < NUM_SHIPS; i++){
        int validPlacement = 0;

        while (validPlacement==0){

            int row, col;
            row = rand() % 10; // random row 0-9
            col = rand() % 10; // random column 0-9

            if ((rand() % 2) == 0){
                strcpy(orientation, "horizontal");
            }
            else{
                strcpy(orientation, "vertical");
            }

            validPlacement = validateAndPlaceShip(bot->grid, &bot->ships[i], row, col, orientation);
            if (validPlacement){
                char letter = (char)(64 + col+1); // ASCII conversion of the value of the col integer into its corresponding character
                printf("Bot's %s placed successfully.\t%c%d\t%s\n", bot->ships[i].name, letter, row+1, orientation);  // Bot's placed ships are visible for debugging purposes
                usleep(2000000);
            }
        }
    }
}


int validateAndPlaceShip(GridCell grid[GRID_SIZE][GRID_SIZE], Ship *ship, int row, int col, char *orientation){
    int dr = 0, dc = 0; // delta row / delta column

    if (strcmp(orientation, "horizontal") == 0){
        dc = 1;
    }
    else if (strcmp(orientation, "vertical") == 0){
        dr = 1;
    }
    else{
        return 0;
    }

    // Check for any out of bounds problems
    if (row < 0 || col < 0 || row >= GRID_SIZE || col >= GRID_SIZE || row + dr * (ship->size - 1) >= GRID_SIZE || col + dc * (ship->size - 1) >= GRID_SIZE){
        return 0;
    }

    // Check if ship overlaps with an existing ship
    for (int i = 0; i < ship->size; i++){
        if (grid[row + dr * i][col + dc * i].hasShip){
            return 0;
        }
    }

    // Otherwise placement will be successful
    for (int i = 0; i < ship->size; i++){
        grid[row + dr * i][col + dc * i].hasShip = 1;
        ship->coordinates[i][0] = row + dr * i;
        ship->coordinates[i][1] = col + dc * i;
    }
    return 1;
}

void updateShipStatus(Player *player, int row, int col, Player *attacker){
    for (int i = 0; i < NUM_SHIPS; i++){

        Ship *ship = &player->ships[i];
        for (int j = 0; j < ship->size; j++){

            if (ship->coordinates[j][0] == row && ship->coordinates[j][1] == col){
                ship->hits++;
                if (ship->hits == ship->size && !ship->sunk){
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

void unlockSpecialMoves(Player *defender, Player *attacker){
    attacker->smokeCount++;

    if (defender->shipsSunk == 1){
        attacker->artilleryAvailable = 1;
        printf("%s has unlocked Artillery for the next turn!\n", attacker->name);
        
    }
    if (defender->shipsSunk == 3){
        attacker->torpedoAvailable = 1;
        printf("%s has unlocked Torpedo for the next turn!\n", attacker->name);
    }
    usleep(4000000);
}


int checkWinCondition(Player *player){
    for (int i = 0; i < NUM_SHIPS; i++){
        if (!player->ships[i].sunk){
            return 0;
        }
    }
    return 1;
}

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


void gameLoop(Player *currentPlayer, Player *opponent, int trackingDifficulty){
    while (1){
        if (!strncmp(opponent->name, "Bot", 3)){
            performMove(currentPlayer, opponent, trackingDifficulty);
            usleep(3000);
        }
        else {
            BotPerformMove(currentPlayer, opponent, trackingDifficulty);
            usleep(3000);
        }
        if (checkWinCondition(opponent)){
            printf("\n%s wins the game!\n", currentPlayer->name);
            break;
        }

        Player *temp = currentPlayer;
        currentPlayer = opponent;
        opponent = temp;
        clearScreen();
    }
}



void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF){}
}