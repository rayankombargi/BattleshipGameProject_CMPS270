#include "Battleship.h"
#include "Utils.c"
#include "Moves.c"


int main() {
    srand(time(NULL)); // Randomize seed

    Player player, bot;
    int trackingDifficulty;

    // Select tracking difficulty
    trackingDifficulty = getTrackingDifficulty();

    // Initialize players
    initializePlayer(&player);
    clearScreen();
    initializeBot(&bot); // Uses the updated bot initialization

    // Ship placement
    printf("\n%s, place your ships.\n", player.name);
    usleep(1000000);
    placeShips(&player);                 
    clearScreen();
    BotPlaceShips(&bot);
    usleep(4000000);
    clearScreen();

    // Determine who goes first
    Player *currentPlayer, *opponent;
    int FirstPlayer = rand() % 2;
    if (FirstPlayer == 0) {
        currentPlayer = &player;
        opponent = &bot;
    } else {
        currentPlayer = &bot;
        opponent = &player;
    }

    printf("By random choice, %s will go first.\n", currentPlayer->name);
    usleep(2500000);
    clearScreen();

    // Game loop
    gameLoop(currentPlayer, opponent, trackingDifficulty);

    return 0;
}
