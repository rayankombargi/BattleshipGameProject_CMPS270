#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10

void Player_Turn(char opponent_grid[N][N], const char* player_name, char difficulty[5]) {
    char guess[3]; // e.g., "B3"
    int row, column;

    printf("%s, it's your turn! Enter your guess (e.g., B3): ", player_name);
    scanf("%s", guess);

    // Convert guess to grid indices
    column = guess[0] - 'A'; // Column index
    row = atoi(guess + 1) - 1; // Row index (convert from string to int)

    if (row >= 0 && row < N && column >= 0 && column < N) {
        if (opponent_grid[row][column] == '~') {
            printf("Miss!\n");
            if (strcmp(difficulty, "Easy") == 0) {
                opponent_grid[row][column] = 'o'; // Mark miss
            }
        } else if (opponent_grid[row][column] == '*') {
            printf("You already shot there!\n");
        } else {
            printf("Hit!\n");
            opponent_grid[row][column] = '*'; // Mark hit
        }
    } else {
        printf("Invalid coordinates! You lose your turn.\n");
    }
}

void Clear_Board(char Battle_Floor[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Battle_Floor[i][j] = '~'; // Resetting to water
        }
    }
}

int Can_Place_Ship(char grid[N][N], int row, int column, int length, const char* orientation) {
    if (strcmp(orientation, "horizontal") == 0) {
        if (column + length > N) return 0; // Overflow condition
        for (int j = column; j < column + length; j++) {
            if (grid[row][j] != '~') return 0; // Checking if something already exists
        }
    } else if (strcmp(orientation, "vertical") == 0) {
        if (row + length > N) return 0; // Overflow condition
        for (int i = row; i < row + length; i++) {
            if (grid[i][column] != '~') return 0; // Checking if something already exists
        }
    }
    return 1; // Can place ship
}

void Place_Ship(char grid[N][N], int row, int column, int length, const char* orientation, char ship_symbol) {
    if (strcmp(orientation, "horizontal") == 0) {
        for (int j = column; j < column + length; j++) {
            grid[row][j] = ship_symbol;
        }
    } else if (strcmp(orientation, "vertical") == 0) {
        for (int i = row; i < row + length; i++) {
            grid[i][column] = ship_symbol;
        }
    }
}

void Player_Ship_Placement(char grid[N][N], const char* player_name) {
    int ship_lengths[] = {5, 4, 3, 2}; // Lengths of ships
    const char* ship_names[] = {"Carrier", "Battleship", "Destroyer", "Submarine"};
    char orientation[13];
    char location[3]; // E.g., B3
    int row, column;

    for (int i = 0; i < 4; i++) {
        while (1) {
            printf("%s, enter the location and orientation for the %s (%d cells): ", player_name, ship_names[i], ship_lengths[i]);
            scanf("%s %s", location, orientation);

            column = location[0] - 'A'; 
            row = atoi(location + 1) - 1; 
            if (Can_Place_Ship(grid, row, column, ship_lengths[i], orientation)) {
                Place_Ship(grid, row, column, ship_lengths[i], orientation, ship_names[i][0]); // Use first letter of ship name
                break; 
            } else {
                printf("Invalid placement. Please try again.\n");
            }
        }
    }
}

void Guess_Maker(char Battle_Floor_General[N][N], char Difficulty[5])
{

    char col;
    int row;

    printf("Enter your guess: ");
    scanf(" %c%d", &col, &row);

    int Column_Index = col - 'A'; // hon we just check the index using ascii code w the one under we do -1 to get index 0
    int Row_Index = row - 1;

    if (Row_Index >= 0 && Row_Index < N && Column_Index >= 0 && Column_Index < N)
    {
        if (Battle_Floor_General[Row_Index][Column_Index] == '~')
        {
            printf("LOSER you missed :) \n");
            if (Difficulty == "easy")
            {
                Battle_Floor_General[Row_Index][Column_Index] = 'o';
            }
        }
        else if (Battle_Floor_General[Row_Index][Column_Index] == '*')
        {
            printf("You already shot there\n");
        }
        else
        {
            printf("Bravo Darabta\n");
            Battle_Floor_General[Row_Index][Column_Index] = '*';
        }
    }
    else
    {
        printf("Choose a number from a-j and number 1-10\n");
    }
}

void Initialize_Grid(char grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = '~';
        }
    }

    printf("   ");
    for (char col = 'A'; col < 'A' + N; col++) {
        printf("%c ", col);
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < N; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char Battle_Floor_1[N][N], Battle_Floor_2[N][N];
    char Difficulty[5];
    char Player_1_Name[50], Player_2_Name[50];

    srand(time(NULL));

    Initialize_Grid(Battle_Floor_1);
    Initialize_Grid(Battle_Floor_2);

    printf("Choose the difficulty (Easy/Hard): ");
    scanf("%4s", Difficulty);
    printf("\n");

    printf("Player 1, please enter your name: ");
    scanf("%49s", Player_1_Name);
    printf("\n");

    printf("Player 2, please enter your name: ");
    scanf("%49s", Player_2_Name);
    printf("\n");

    int Random_Variable = rand() % 2;
    if (Random_Variable == 0) {
        printf("%s starts first!\n", Player_1_Name);
    } else {
        printf("%s starts first!\n", Player_2_Name);
    }

    Player_Ship_Placement(Battle_Floor_1, Player_1_Name);
    system("clear"); 
    Player_Ship_Placement(Battle_Floor_2, Player_2_Name);

    // Gameplay loop
    while (1) {
        Display_Grid(Battle_Floor_2);
        Player_Turn(Battle_Floor_2, Player_1_Name, Difficulty);

        Display_Grid(Battle_Floor_1);
        Player_Turn(Battle_Floor_1, Player_2_Name, Difficulty);
    }

    return 0;
}


