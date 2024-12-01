This is a dynamic implementation of the classic Battleship game, featuring a 10x10 grid, special moves like Radar 
Sweeps, Artillery Strikes, Torpedoes, and Smoke Screens. The game includes both a Bot version and a 2-player version,
with two difficulty levels: Easy and Hard. The objective is to sink your opponent’s ships before they sink yours.

Features  
Game Modes:  
Single-player: Play against an AI bot with two difficulty levels (Easy and Hard)  
Two-player: Play with a friend on the same machine, alternating turns  
  
Gameplay Mechanics:  
Radar Sweep: Locate enemy ships within a certain radius  
Artillery Strike: Launch a powerful attack on the enemy grid  
Torpedoes: Precision strikes to target specific coordinates  
Smoke Screen: Conceal your ships from enemy radar for a turn  
  
Difficulty Levels:  
Easy: The bot has a simpler AI strategy, allowing for a more relaxed game, also trackes hits and misses  
Hard: The bot uses advanced tactics, providing a more challenging experience, only trackes hits  
  
Interactive 10x10 Grid:   
Players place their ships on a grid, and the game alternates between attacking and defending  
  
How to Play  
1-Player Mode (Against Bot)   
  
Choose the difficulty level (Easy or Hard)  
Place your ships on the 10x10 grid  
Take turns with the bot to guess the location of the opponent's ships  
Use special moves like Radar Sweeps, Artillery Strikes, Torpedoes, and Smoke Screens  
The first player to sink all of the opponent’s ships wins  
  
2-Player Mode (Local Multiplayer)  
  
Each player takes turns placing their ships on the 10x10 grid  
Players then alternate turns guessing the opponent’s ship locations  
Special moves can be used during gameplay to gain an advantage  
The first player to sink all of the opponent’s ships wins  
  
Game Rules  
  
Players take turns guessing the coordinates of the opponent's ships  
Ships are placed horizontally or vertically on the grid  
Special moves are limited per game and provide strategic advantages  
The game ends when one player has sunk all the opponent’s ships  
  
Game Structure:  
  
The grid starts out as an empty (~) 10x10 grid  
The gird tracks the hits (*) and misses (o)  depending on the level selected by the user  
Each player has 4 types of ships of different sizes (5,4,3,2).   
Ships also have orientation (vertical or horizontal)  
In the game the players turn alternate after each move (valid or invalid)  
Game ends when all the ships are sunk  
  
Strategies used:  
  
1. Offensive Strategy:  
  
Random firing occurs to be able to identify the location of the 
placed ships 
After targeting a ship and hitting it we fire shots adjacent to it 
(not diagonally)
Artillery  targets a 2x2 grid which helps improve the probability 
of a hit 
Torpedo target a whole row or column which is useful for when you 
want to target a big ship

2. Defensive Strategy:

Place you ships away from each other to avoid being hit by the  
Artillery
Avoid placing the ships on the same row or column to avoid the 
Torpedo
Try using smoke screens to hinder the radar sweeps that the 
attacker might use
     
3. Bot Strategies:

Bot starts out with random moves and they also avoid pre targeted 
areas
Radars are done when there are no recent hits and they focus on 
unexplored areas on the grid
Once a hit is identified the bot tries to figure out the 
orientation of the ship and fires accordingly 
