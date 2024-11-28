Till now we have the grid and the numeration of A,B,....J and 1,2,3,...10
we also have a a line that takes the name and difficulty from the users

we added a battlefeild maker that tracks hits and misses and gives each player equal probability to start

Oct 22 2024
Replaced the Battle Floor double arrays into double pointers character arrays. Added a function prototype for the Guess_Maker method.

Added Carl's Ship_Placement method, included its prototype. Implemented a for loop that allows marking the fragments of the ship in each marked Coordinate.

Oct 23 2024
Fixed the Random variable. Added three new functions StartTurn, MovePicker and PlayerLost. Added Player struct which contains Name and grid.



Nov 20 2024
Added a new file Phase2.c where I replaced player2 with bot. The bot can now randomly place its own ships.

Attempted to make the bot input move commands. Still broken. Bug: Bot and Player names are switched up during the game.

Nov 28 2024
Fixed the issue where the bot's turn uses player's name and vice-versa.
Fixed the incorrect placement of Bot's ships. 
Corrected the functionality of UnlockSpecialMoves.
