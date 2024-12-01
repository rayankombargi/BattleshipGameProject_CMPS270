# The Four Musketeers: Battleship Game

This repository contains the source code for a console-based Battleship game developed as part of our project titled "The Four Musketeers". The game is played on a 10x10 grid where the goal is to sink all of the opponent's ships by guessing their locations. Special moves, including radar sweeps, smoke screens, artillery, and torpedoes, add an extra layer of strategy to the gameplay.

## Group Members
- Carl El Helayel
- Rayyan Kombargi
- Ahmad Chreif
- Housni Antar


## High-Level Description

The Battleship game consists of two players (one human, one bot) who take turns guessing coordinates on a 10x10 grid to hit and sink their opponent's ships. The grid is dynamically updated to show hits and misses, and the game ends when all ships of one player are sunk. The bot utilizes a state-based logic to strategically select targets, adapting to the results of previous moves.

### Special Moves:
- **Radar Sweep:** Reveals if a ship is in a 2x2 area.
- **Smoke Screen:** Hides the user's ships from enemy radar sweeps.
- **Artillery:** Strikes a 2x2 area on the grid.
- **Torpedo:** Targets an entire row or column.

## How to Run the Game
To run Phase 1 of the program, simply execute Phase1.c  
The instructions for running Phase 2 specify that `MainGame.c` (located in the Phase2 folder) should be used instead of `Phase1.c`.







