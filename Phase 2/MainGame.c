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
    Sleep(1000);
    placeShips(&player);                 
    clearScreen();
    BotPlaceShips(&bot);
    Sleep(4000);
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
    Sleep(2500);
    clearScreen();

    // Game loop
    gameLoop(currentPlayer, opponent, trackingDifficulty);

    return 0;
}
