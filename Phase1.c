#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define N 10
#define totalShips 5
void displayGrid(char Battle_Floor[N][N]);
void displayStats(char Battle_Floor[N][N]);
// void ship_placement(char Battle_Floor[N][N], int row, int column, int shipLength, char orientation[12]);
void ship_placement(char Battle_Floor[N][N]);
void Guess_Maker(char Battle_Floor_General[N][N], char difficulty[5]);



int main(){
    char Battle_Floor_1[N][N];
    char Battle_Floor_2[N][N];
    char difficulty[5]; // store the difficulty level, i.e. "easy" or "hard"
    char player_1_name[50]; 
    char player_2_name[50];

    // Initilaize both grids to be "empty"
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            Battle_Floor_1[i][j] = '~';
            Battle_Floor_2[i][j] = '~'; 
        }
    }

    // displayGrid(Battle_Floor_1);
    // displayGrid(Battle_Floor_2);
    
    // Process difficulty and names as input
    // printf("Choose the difficulty (Easy/Hard): "); 
    // scanf("%4s", difficulty); 
    // printf("\n");

    // printf("Player 1 name: ");
    // scanf("%49s", player_1_name);
    // printf("\n");

    // printf("Player 2 name: ");
    // scanf("%49s", player_2_name);
    // printf("\n");

    // Set each players ships
    // Generate random choice for which player starts playing
    srand(time(0)); // ensures that rand() generates a new number after each run
    // int startChoice = rand()% 2;
    int startChoice = 0;

    if (startChoice == 0){ // start with player 1
        ship_placement(Battle_Floor_1);
        // ship_placement(Battle_Floor_2);
    }
    else{ // start with player 2
        ship_placement(Battle_Floor_2);
        ship_placement(Battle_Floor_1);
    }

    displayGrid(Battle_Floor_1);
    displayGrid(Battle_Floor_2);

    // // Main body of the game:
    // int player_1_ships = 5, player_2_ships = 5; // keep track of the number of ships remaining for each player
    
    // while (player_1_ships > 0 && player_2_ships > 0){
    //     break;
    // }
    
    // // We break out of the loop if one of the players loses, so one of the below must be true
    // if (player_1_ships == 0)
    //     printf("%s lost! %s wins", player_1_name, player_2_name);
    // else
    //     printf("%s lost! %s wins", player_2_name, player_1_name);

    return 0;
}


// This function displays the grid
void displayGrid(char Battle_Floor[N][N]){
    printf("   ");
    for (char Top_Letter_Indexing = 'A'; Top_Letter_Indexing < 'A' + N; Top_Letter_Indexing++){
        printf("%c ", Top_Letter_Indexing);
    }
    printf("\n");

    for (int i = 0; i < N; i++){
        printf("%2d ", i + 1);
        for (int j = 0; j < N; j++){
            printf("%c ", Battle_Floor[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    displayStats(Battle_Floor);
}

void displayStats(char Battle_Floor[N][N]){ // display the number of fire, torpedo...

}

void ship_placement(char Battle_Floor[N][N]){
    char *shipNames[] = {"Carrier (5 cells)", "Battleship (4 cells)", "Destroyer (3 cells)", "Submarine (2 cells)"};
    int currentShips = 5;
    while (currentShips > 1){
        int row;
        char col;
        char *orientation = malloc(10*sizeof(char));
        printf("Please enter the location of %s: ", shipNames[5-currentShips]);
        scanf(" %c%d %s",&col, &row, orientation);

        // Now we need to check if the input coordinates and orientation form a valid ship location
        if (strcmp(orientation, "horizontal") == 0){
            int bool = 1;
            for (int i = 0; i < currentShips; i++){
                if (Battle_Floor[row][(col-97)+i] == 'h'){ // h means this location is marked.
                    bool = 0;
                    break;
                }
            }
            if (bool == 0) // bool is 0 if we find an invalid location to insert the ships at
                continue;
            // Otherwise we continue normally and insert the ship at the desired location
            for (int i = 0; i < currentShips; i++){
                Battle_Floor[row][col-97+i] = 'h'; // populate the desired location with the ship of the correct size
            }
        }
        else if (strcmp(orientation, "veri") == 0){
            int bool = 1;
            for (int i = 0; i < currentShips; i++){
                if (Battle_Floor[row+i][(col-97)] == 'h'){ // h means this location is marked.
                    bool = 0;
                    break;
                }
            }
            if (bool == 0) // bool is 0 if we find an invalid location to insert the ships at
                continue;
            // Otherwise we continue normally and insert the ship at the desired location
            for (int i = 0; i < currentShips; i++){
                Battle_Floor[row+i][col-97] = 'h'; // populate the desired location with the ship of the correct size
            }
        }
        currentShips -= 1;
        free(orientation);
    }  
}


// void ship_placement(char Battle_Floor[N][N], int row, int column, int shipLength, char orientation[12]){
//     if (strcmp(orientation, "horizontal") == 0){
//         if (column + shipLength > N)
//             return 0; //overflow condition
//         for (int j = column; j < column + shipLength; j++){
//             if (Battle_Floor[row][j] != '~')// checking if smth already exists 
//                 return 0; 
//         }
//         for (int j = column; j < column + shipLength; j++){
//             Battle_Floor[row][j] = '-'; // Marking fragment of the ship
//         }
//     }

//     else if (strcmp(orientation, "vertical") == 0){ 
//         if (row + shipLength > N)
//             return 0; //overflow condition
//         for (int i = row; i < row + shipLength; i++){
//             if (Battle_Floor[i][column] != '~')// checking if smth already exists 
//                 return 0; 
//         }
//         for (int i = column; i < column + shipLength; i++){
//             Battle_Floor[row][i] = '|'; // Marking fragment of the ship
//         }
//     }
//     return 1;
// }



void Guess_Maker(char Battle_Floor_General[N][N], char difficulty[5]){ // no need to do difficulty manager if we have here 
    char col;
    int row;

    printf("Enter your guess: ");
    scanf(" %c%d", &col, &row);

    int col_idx = col - 'A', row_idx = row - 1;

    if (row_idx >= 0 && row_idx < N && col_idx >= 0 && col_idx < N){

        if (Battle_Floor_General[row_idx][col_idx] == '~'){
            printf("LOSER you missed :) \n");

            if (strcmp(difficulty, "easy") == 0) // Mark it only if on easy mode
                Battle_Floor_General[row_idx][col_idx] = 'X';
        }

        else if (Battle_Floor_General[row_idx][col_idx] == 'h'){
            printf("You already shot there\n");
        }

        else{
            printf("Bravo Darabta\n");
            Battle_Floor_General[row_idx][col_idx] = 'h';
        }
    }
    else{
        printf("Choose a number from a-j and number 1-10\n");
    }
}


