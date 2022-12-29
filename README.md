# ConnectFour
This AI uses the Minimax algorithm to play the game Connect Four. You can find it here without downloading the SFML library:  https://replit.com/@thecleanser/Connect-Four-AI

## What it Uses
This AI was created using C++, and the graphics and GUI were created using SFML (Simple and Fast Multimedia Library). 

## How the AI Works
The AI uses employs the Minimax algorithm which makes a decision on the optimal move based off a decision tree. 
The game state is evaluated using a custom heruistic function to give a score to the board based off how favorable it is for the AI.
To improve computational speed, several methods were used, including: a bitboard and bitwise operations in order to evaluate the board (which can be stored entirely within a 64-bit long long data type),
alpha-beta pruning (decreases the tree nodes visited by removing ones that will not affect the final decision), and maps to store game states which were already evaluated.
