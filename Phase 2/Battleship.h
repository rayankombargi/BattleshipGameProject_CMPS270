#ifndef BATTLESHIP_H

#define BATTLESHIP_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define GRID_SIZE 10
#define NUM_SHIPS 4
#define EASY 0
#define HARD 1
#define MAX_RADAR_SWEEPS 3
#define MAX_RADAR_HITS 10 //random number

enum Command {FIRE, RADAR, SMOKE, ARTILLERY, TORPEDO,INVALID};

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

typedef struct{
    int row;
    int col;
}RadarHit;

typedef struct {
    char name[50];
    GridCell grid[GRID_SIZE][GRID_SIZE];
    Ship ships[NUM_SHIPS];
    int radarCount;
    int smokeCount;
    int artilleryAvailable;
    int torpedoAvailable;
    int shipsSunk;
    int RadarHitsCount;
    RadarHit RadarHistory[MAX_RADAR_HITS];
} Player;

typedef struct {
    int hits[GRID_SIZE][GRID_SIZE]; // 1: hit, 0: miss, -1: unexplored
    int pendingHits[GRID_SIZE][GRID_SIZE]; // 1: needs follow-up, 0: no follow-up needed
} BotState;

BotState botState;


void initializeGrid(GridCell grid[GRID_SIZE][GRID_SIZE]);
void displayGrid(GridCell grid[GRID_SIZE][GRID_SIZE], int trackingDifficulty);
int getTrackingDifficulty();
void initializePlayerAttributes(Player *player);
void initializePlayer(Player *player);
void initializeBot(Player *bot);
void placeShips(Player *player);
void BotPlaceShips(Player *bot);
int validateAndPlaceShip(GridCell grid[GRID_SIZE][GRID_SIZE], Ship *ship, int row, int col, char *orientation);
int checkWinCondition(Player *player);
void gameLoop(Player *player1, Player *player2, int trackingDifficulty);
void clearScreen();
void clearInputBuffer();

enum Command getCommandType(const char *input);
void Move(Player *attacker, Player *defender, char * input);
void performMove(Player *attacker, Player *defender, int trackingDifficulty);
void BotPerformMove(Player *attacker, Player *defender, int trackingDifficulty);
int isRadarScanned(Player *attacker, int row, int col);
void showMoveOptions(Player *player);
int validateAndParseCoord(const char *coord, int *row, int *col);
void performFire(Player *attacker, Player *defender, char *coord);
void performRadar(Player *attacker, Player *defender, char *coord);
void performSmoke(Player *player, char *coord);
void performArtillery(Player *attacker, Player *defender, char *coord);
void performTorpedo(Player *attacker, Player *defender, char *input);
void botUseRadar(Player *bot, int row, int col);
void updateShipStatus(Player *player, int row, int col, Player *attacker);
void unlockSpecialMoves(Player *defender, Player *attacker);





void initializeBotState();
void updateBotState(Player *defender, int row, int col, int hit);
int findNextTarget(int *row, int *col);
void chooseRandomTarget(int *row, int *col);
void initializeBot(Player *bot);

#endif