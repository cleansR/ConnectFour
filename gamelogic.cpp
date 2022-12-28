#include <iostream>
#include <stdio.h>
#include <cmath>
#include <numeric>
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
#define p(x) cout << x << '\n'

class GameLogic
{
    private:
        const int BOARD_WIDTH = 7;
        const int BOARD_HEIGHT = 6;
        const ll FULL_BOARD = 141845657554976;
        
        ll board1;
        ll board2;
        vector<vector<int>> mainBoard;
        
        bool firstPlayerTurn = true;
        bool finished = false;
    
    public:
        /**
         * @brief Construct a new Game Logic object
         * 
         */
        GameLogic()
        {
            vector<vector<int>> v(6, vector<int>(7, 0));
            mainBoard = v;
            board1 = 0;
            board2 = 0;
        }

        /**
         * @brief Get the Player1 Board object
         * 
         * @return ll Player1's board
         */
        ll getPlayer1Board() { return board1; }

        /**
         * @brief Get the Player2 Board object
         * 
         * @return ll Player2's board
         */
        ll getPlayer2Board() { return board2; }
        
        /**
         * @brief Get the Main Board object
         * 
         * @return vector<vector<int>> the main board
         */
        vector<vector<int>> getMainBoard() { return mainBoard; }

        /**
         * @brief updates the state of the game if it is finished
         * 
         * @return true if it is finished
         * @return false if it is not finished
         */
        bool done() { 
            finished = checkForWin(); return finished; 
        }

        /**
         * @brief returns true/false depending on whether it is Player1's turn
         * 
         * @return true if it is Player1's turn
         * @return false if is not
         */
        bool player1Turn() { return firstPlayerTurn; }

        /**
         * @brief swaps the current turn
         * 
         */
        void swapTurns() { firstPlayerTurn = !firstPlayerTurn; }

        /**
         * @brief resets the board and other variables
         * 
         */
        void reset(){
            for(int i = 0; i < 6; i++){
                for(int j = 0; j < 7; j++){
                    mainBoard[i][j] = 0;
                }
            }
            board1 = 0;
            board2 = 0;
            firstPlayerTurn = true;
        }
        
        /**
         * @brief prints out the game board in a readable form 
         * 
         */
        void printBoard()
        {
            char result[6][7];
            ll mask = 1;
            for(int i = 0; i < 7; i++){
                for(int j = 0; j < 6; j++){
                    if(mask & board1) result[5-j][i] = 'X';
                    else if(mask & board2) result[5-j][i] = 'O';
                    else result[5-j][i] = '_'; 
                    mask <<= 1LL;
                }
                mask <<= 1LL;
            }
        
            cout << '\n';
            for(int i = 0; i < 6; i++){
                cout << "|";
                for(int j = 0; j < 7; j++){
                    cout << result[i][j] << "|";
                }
                cout << '\n';
            }
            cout << " ";
            for(int i = 0; i < 7; i++) cout << i+1 << " ";
            cout << '\n' << '\n';
        }
        
        /**
         * @brief checks if a specified column has room for another piece
         * 
         * @param col the specified column
         * @return true if the column has room for another piece
         * @return false if the column is full
         */
        bool validDrop(int col)
        {
            ll mask = 63LL << (col * 7LL); 
            ll result = mask & (board1 | board2);
            return mask != result;
        }
        
        /**
         * @brief drops a piece in the specified column of the board
         * 
         * @param col the column to drop a piece in
         */
        void dropPiece(int col)
        {
            ll occupiedBoard = board1 | board2;
            ll addPieceMask = 1LL << (col*7); int currentHeight = 5;
            while(addPieceMask & occupiedBoard){
              addPieceMask <<= 1LL; currentHeight--;
            }
        
            if(firstPlayerTurn){
                board1 |= addPieceMask;
                mainBoard[currentHeight][col] = 1;
            }
            else{
                board2 |= addPieceMask;
                mainBoard[currentHeight][col] = 2;
            } 
        
            occupiedBoard |= addPieceMask;
        }
        
        /**
         * @brief checks if either player has won
         * 
         * @return true if either player has won
         * @return false if neither player has won
         */
        bool checkForWin()
        {
            ll cboard = (firstPlayerTurn ? board1 : board2);
        
            ll mask1 = cboard << 7LL; ll mask2 = cboard << 14LL; ll mask3 = cboard << 21LL;
            if(cboard & mask1 & mask2 & mask3) return true;
            
            mask1 = cboard << 1LL; mask2 = cboard << 2LL; mask3 = cboard << 3LL;
            if(cboard & mask1 & mask2 & mask3) return true;
        
            mask1 = cboard << 8LL; mask2 = cboard << 16LL; mask3 = cboard << 24LL;
            if(cboard & mask1 & mask2 & mask3) return true;
        
            mask1 = cboard << 6LL; mask2 = cboard << 12LL; mask3 = cboard << 18LL;
            if(cboard & mask1 & mask2 & mask3) return true;
        
            return false;
        }
        
        /**
         * @brief checks if the board is full
         * 
         * @return true if the board is filled
         * @return false if the board isn't filled
         */
        bool boardFilled()
        {
           return (FULL_BOARD == (FULL_BOARD & (board1 | board2))); 
        }
        
    
};


