#include "Battleship.h"
#include "Utils.c"
#include "Moves.c"

int main() {
    srand(time(NULL)); // to randomize dr zalgout said we can do it like dr zalghout said 

    Player player, bot;
    int trackingDifficulty;

    trackingDifficulty = getTrackingDifficulty();

    initializePlayer(&player);
    clearScreen();

    initializeBot(&bot);

    printf("\n%s, place your ships.\n", player.name);
    usleep(1000000);
    placeShips(&player);                 
    clearScreen();

    BotPlaceShips(&bot);
    usleep(4000000);
    clearScreen();


    Player *currentPlayer, *opponent;

    int FirstPlayer = rand() % 2;
    if (FirstPlayer == 0) {
        currentPlayer = &player;
        opponent = &bot;
    } 
    else if (FirstPlayer == 1) {
        currentPlayer = &bot;
        opponent = &player;
    }
    printf("By random choice, %s will go first.\n", currentPlayer->name);
    usleep(2500000);
    clearScreen();

    gameLoop(currentPlayer, opponent, trackingDifficulty);     // the gameLoop method is under development

    return 0;
}