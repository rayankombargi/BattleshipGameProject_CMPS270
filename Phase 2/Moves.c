#include "Battleship.h"

void showMoveOptions(Player *player){
    printf("Available moves:\n");
    printf("- Fire [coordinate]\n");
    if (player->radarCount > 0){
        printf("- Radar [coordinate] (Remaining: %d)\n", player->radarCount);
    }
    if (player->smokeCount > 0){
        printf("- Smoke [coordinate] (Available: %d)\n", player->smokeCount);
    }
    if (player->artilleryAvailable){
        printf("- Artillery [coordinate] (Available)\n");
    }
    if (player->torpedoAvailable){
        printf("- Torpedo [row/column] (Available)\n");
    }
}

void performMove(Player *attacker, Player *defender, int trackingDifficulty){
    char input[50];
    printf("\n%s's turn.\n", attacker->name);
    displayGrid(defender->grid, trackingDifficulty);
    showMoveOptions(attacker);

    printf("Enter your move: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    Move(attacker, defender, input);
}


// Maps each command to an enum
enum Command getCommandType(const char *input){
    if (strncmp(input, "Fire", 4) == 0) return FIRE;
    if (strncmp(input, "Radar", 5) == 0) return RADAR;
    if (strncmp(input, "Smoke", 5) == 0) return SMOKE;
    if (strncmp(input, "Artillery", 9) == 0) return ARTILLERY;
    if (strncmp(input, "Torpedo", 7) == 0) return TORPEDO;
    return INVALID;
}


void Move(Player *attacker, Player *defender, char *input) {
    enum Command command = getCommandType(input); // Get the command type from input

    switch (command) {
        case FIRE: {
            char coord[10];
            if (sscanf(input, "Fire %s", coord) == 1) {
                performFire(attacker, defender, coord);
            } else {
                printf("Invalid command format. You lose your turn.\n");
            }
            break;
        }

        case RADAR: {
            if (attacker->radarCount > 0) {
                char coord[10];
                if (sscanf(input, "Radar %s", coord) == 1) {
                    performRadar(attacker, defender, coord);
                } else {
                    printf("Invalid command format. You lose your turn.\n");
                }
            } else {
                printf("No radar sweeps remaining. You lose your turn.\n");
            }
            break;
        }

        case SMOKE: {
            if (attacker->smokeCount > 0) {
                char coord[10];
                if (sscanf(input, "Smoke %s", coord) == 1) {
                    performSmoke(attacker, coord);
                } else {
                    printf("Invalid command format. You lose your turn.\n");
                }
            } else {
                printf("No smoke screens available. You lose your turn.\n");
            }
            break;
        }

        case ARTILLERY: {
            if (attacker->artilleryAvailable) {
                char coord[10];
                if (sscanf(input, "Artillery %s", coord) == 1) {
                    performArtillery(attacker, defender, coord);
                    attacker->artilleryAvailable = 0; // Can use only once
                } else {
                    printf("Invalid command format. You lose your turn.\n");
                }
            } else {
                printf("Artillery not available. You lose your turn.\n");
            }
            break;
        }

        case TORPEDO: {
            if (attacker->torpedoAvailable) {
                char param[10];
                if (sscanf(input, "Torpedo %s", param) == 1) {
                    performTorpedo(attacker, defender, param);
                    attacker->torpedoAvailable = 0; // Can use only once
                } else {
                    printf("Invalid command format. You lose your turn.\n");
                }
            } else {
                printf("Torpedo not available. You lose your turn.\n");
            }
            break;
        }

        case INVALID:
        default: {
            printf("Invalid command. You lose your turn.\n");
            break;
        }
    }
}



void BotPerformMove(Player *attacker, Player *defender, int trackingDifficulty) {
    int row, col;
    char input[50];

    printf("\n%s's turn.\n", attacker->name);
    displayGrid(defender->grid, trackingDifficulty);
    showMoveOptions(attacker);


    if (findNextTarget(&row, &col)) {
        if (attacker->torpedoAvailable){
            snprintf(input, sizeof(input), "Torpedo %c%d", 'A' + col, row + 1);
        }
        else if (attacker->artilleryAvailable){
            snprintf(input, sizeof(input), "Artillery %c%d", 'A' + col, row + 1);
        }
        else{
            snprintf(input, sizeof(input), "Fire %c%d", 'A' + col, row + 1);
        }
    } 
    else {
        chooseRandomTarget(&row, &col);
        if (attacker->radarCount>0){
            snprintf(input, sizeof(input), "Radar %c%d", 'A' + col, row + 1);

        }
        else{
            snprintf(input, sizeof(input), "Fire %c%d", 'A' + col, row + 1);
        }
    }

    printf("Bot chooses: %s\n", input);
    usleep(3000000);

    // Execute the move
    if (strncmp(input, "Fire", 4) == 0) {
        char coord[10];
        sscanf(input, "Fire %s", coord);
        performFire(attacker, defender, coord);
        updateBotState(defender, row, col, defender->grid[row][col].hasShip);
    } 
    else if (strncmp(input, "Radar", 5) == 0 && attacker->radarCount > 0) {
        char coord[10];
        sscanf(input, "Radar %s", coord);
        performRadar(attacker, defender, coord);
        attacker->radarCount--;
    } 
    else if (strncmp(input, "Smoke", 5) == 0 && attacker->smokeCount > 0) {
        char coord[10];
        sscanf(input, "Smoke %s", coord);
        performSmoke(attacker, coord);
        attacker->smokeCount--;
    } 
    else if (strncmp(input, "Artillery", 9) == 0 && attacker->artilleryAvailable) {
        char coord[10];
        sscanf(input, "Artillery %s", coord);
        performArtillery(attacker, defender, coord);
        attacker->artilleryAvailable = 0;
    } 
    else if (strncmp(input, "Torpedo", 7) == 0 && attacker->torpedoAvailable) {
        char param[10];
        sscanf(input, "Torpedo %s", param);
        performTorpedo(attacker, defender, param);
        attacker->torpedoAvailable = 0;
    } else {
        printf("Invalid move or unavailable command. Bot loses its turn.\n");
    }
}


int validateAndParseCoord(const char *coord, int *row, int *col) {
    char columnLetter;

    if (sscanf(coord, "%c%d", &columnLetter, row) != 2) 
        return 0;

    // Convert to 0-based indices
    *row -= 1;
    *col = toupper(columnLetter) - 'A';

    // Check if within valid range
    if (*row < 0 || *row >= GRID_SIZE || *col < 0 || *col >= GRID_SIZE) 
        return 0;

    // Validation successful
    return 1;
}


void performFire(Player *attacker, Player *defender, char *coord){
    int row, col;

    if (validateAndParseCoord(coord, &row, &col) == 0){
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }
    
    GridCell *cell = &defender->grid[row][col];

    if (cell->isHit){
        printf("You already targeted this location. You lose your turn.\n");
        return;
    }

    cell->isHit = 1;

    if (cell->hasShip){
        cell->display = '*';
        printf("Hit!\n");
        updateShipStatus(defender, row, col, attacker);
    }
    else{
        cell->display = 'o';
        printf("Miss.\n");
    }
    usleep(3000000);
}


void performRadar(Player *attacker, Player *defender, char *coord){
    int row, col;

    if (validateAndParseCoord(coord, &row, &col) == 0){
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }

    int found = 0;
    for (int i = row; i <= row + 1; i++){
        for (int j = col; j <= col + 1; j++){
            if (defender->grid[i][j].hasSmoke)
                continue;
            if (defender->grid[i][j].hasShip){
                found = 1;
                break;
            }
        }
        if (found)
            break;
    }

    if (found){
        printf("Enemy ships found.\n");
    }
    else{
        printf("No enemy ships found.\n");
    }
    attacker->radarCount--;
    usleep(3000000);
}



void performSmoke(Player *player, char *coord){
    int row, col;

    if (validateAndParseCoord(coord, &row, &col) == 0){
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }

    for (int i = row; i <= row + 1; i++){ // smoke a 2x2 area
        for (int j = col; j <= col + 1; j++){ 
            player->grid[i][j].hasSmoke = 1;
        }
    }
    player->smokeCount--;
    clearScreen();
    printf("Smoke screen deployed.\n");
    usleep(3000000);
}


void performArtillery(Player *attacker, Player *defender, char *coord){
    int row, col;

    if (validateAndParseCoord(coord, &row, &col) == 0){
        printf("Invalid coordinate format. You lose your turn.\n");
        return;
    }
    
    int hit = 0;
    for (int i = row; i <= row + 1; i++){
        for (int j = col; j <= col + 1; j++){
            GridCell *cell = &defender->grid[i][j];
            if (!cell->isHit){
                cell->isHit = 1;
                if (cell->hasShip){
                    cell->display = '*';
                    updateShipStatus(defender, i, j, attacker);
                    hit = 1;
                }
                else{
                    cell->display = 'o';
                }
            }
        }
    }

    if (hit){
        printf("Artillery strike hit enemy ships!\n");
    }
    else{
        printf("Artillery strike missed.\n");
    }
    usleep(3000000);
}

void performTorpedo(Player *attacker, Player *defender, char *input){
    char rc = toupper(input[0]);
    int index;
    int hit = 0;

    if (isalpha(rc)){
        index = rc - 'A';
        if (index < 0 || index >= GRID_SIZE){
            printf("Invalid column. You lose your turn.\n");
            return;
        }
        for (int i = 0; i < GRID_SIZE; i++){
            GridCell *cell = &defender->grid[i][index];
            if (!cell->isHit){
                cell->isHit = 1;
                if (cell->hasShip){
                    cell->display = '*';
                    updateShipStatus(defender, i, index, attacker);
                    hit = 1;
                }
                else{
                    cell->display = 'o';
                }
            }
        }
    }
    else{
        index = atoi(input) - 1;
        if (index < 0 || index >= GRID_SIZE){
            printf("Invalid row. You lose your turn.\n");
            return;
        }

        for (int j = 0; j < GRID_SIZE; j++){

            GridCell *cell = &defender->grid[index][j];
            if (!cell->isHit){
                cell->isHit = 1;
                if (cell->hasShip){
                    cell->display = '*';
                    updateShipStatus(defender, index, j, attacker);
                    hit = 1;
                }
                else{
                    cell->display = 'o';
                }
            }
        }
    }

    if (hit){
        printf("Torpedo hit enemy ships!\n");
    }
    else{
        printf("Torpedo missed.\n");
    }
    usleep(3000000);
}


void initializeBotState() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            botState.hits[i][j] = -1; // set to unexplored
            botState.pendingHits[i][j] = 0;
        }
    }
}

void updateBotState(Player *defender, int row, int col, int hit) {
    botState.hits[row][col] = hit;
    if (hit) {
        botState.pendingHits[row][col] = 1;
    }
}

int findNextTarget(int *row, int *col) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (botState.pendingHits[i][j]) {
                if (i > 0 && botState.hits[i - 1][j] == -1) { // potetial target above current cell
                    *row = i - 1; *col = j; 
                    return 1; 
                }
                if (i < GRID_SIZE - 1 && botState.hits[i + 1][j] == -1) { // potetial target below current cell
                    *row = i + 1; *col = j; 
                    return 1; 
                }
                if (j > 0 && botState.hits[i][j - 1] == -1) { // potetial target left current cell
                    *row = i; *col = j - 1; 
                    return 1; 
                }
                if (j < GRID_SIZE - 1 && botState.hits[i][j + 1] == -1) { 
                    *row = i; *col = j + 1; 
                    return 1; 
                }
            }
        }
    }
    return 0;
}

void chooseRandomTarget(int *row, int *col) {
    do {
        *row = rand() % GRID_SIZE;
        *col = rand() % GRID_SIZE;
    } while (botState.hits[*row][*col] != -1); // -1 if cell is unexplored, otherwise it was either a hit or miss
}