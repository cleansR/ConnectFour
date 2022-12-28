#include <iostream>
#include <stdio.h>
#include <cmath>
#include <numeric>
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
#define p(x) cout << x << '\n'


/**
 * @brief Class which performs AI computations for Connect Four
 * 
 */
class ConnectFourAI
{
    private:
        const static int BOARD_WIDTH = 7;
        const static int BOARD_HEIGHT = 6;

        const static int MAXIMUM_DEPTH = 8;
        const static ll FULL_BOARD = 141845657554976;
        const static ll FULL_BOARD_MASK = 562949953421311;

        const static int WORST_CASE = -100;
        const static int BEST_CASE = 100;

        const static int FOUR_IN_A_ROW_VALUE = 500;
        const static int THREE_IN_A_ROW_VALUE = 10;

        unordered_map<ll, unordered_map<ll, ll>> exploredMax[MAXIMUM_DEPTH];
        unordered_map<ll, unordered_map<ll, ll>> exploredMin[MAXIMUM_DEPTH];

        int countSetBits(ll n)
        {
            int res = 0;
            for(ll i = 0; i < 64; i++){
                if((1LL << i) & n) res++;
            }
            return res;
        }

        bool checkWin(ll board1, ll board2)
        {
            ll cboard = -1;
            for(int i = 0; i < 2; i++){
                if(i==0) cboard = board1;
                else cboard = board2;

                //Horizontal
                ll mask1 = cboard << 7LL; ll mask2 = cboard << 14LL; ll mask3 = cboard << 21LL;
                if(cboard & mask1 & mask2 & mask3) return true;

                //Vertical 4
                mask1 = cboard << 1LL;  mask2 = cboard << 2LL; mask3 = cboard << 3LL;
                if(cboard & mask1 & mask2 & mask3) return true;

                //Diagonal / 4
                mask1 = cboard << 8LL; mask2 = cboard << 16LL; mask3 = cboard << 24LL;
                if(cboard & mask1 & mask2 & mask3) return true;

                //Diagonal \ 4
                mask1 = cboard << 6LL; mask2 = cboard << 12LL; mask3 = cboard << 18LL;
                if(cboard & mask1 & mask2 & mask3) return true;
            }
            return false;
        }

        bool fourInARow(ll board)
        {
            ll mask1 = board << 7LL; ll mask2 = board << 14LL; ll mask3 = board << 21LL;
            if(board & mask1 & mask2 & mask3) return true;

            mask1 = board << 1LL; mask2 = board << 2LL; mask3 = board << 3LL;
            if(board & mask1 & mask2 & mask3) return true;

            mask1 = board << 8LL; mask2 = board << 16LL; mask3 = board << 24LL;
            if(board & mask1 & mask2 & mask3) return true;
    
            mask1 = board << 6LL; mask2 = board << 12LL; mask3 = board << 18LL;
            if(board & mask1 & mask2 & mask3) return true;

            return false;
        }

        ll threeInARow(ll board)
        {
            ll count = 0;

            ll mask1 = board << 7LL; ll mask2 = board << 14LL;
            ll finalMask = (board & mask1 & mask2);
            ll leftCheckMask = (finalMask >> 21LL); 
            ll rightCheckMask = (finalMask << 7LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            mask1 = board << 1LL; mask2 = board << 2LL; 
            finalMask = (board & mask1 & mask2);
            leftCheckMask = (finalMask >> 3LL); 
            rightCheckMask = (finalMask << 1LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            mask1 = board << 8LL; mask2 = board << 16LL; 
            finalMask = (board & mask1 & mask2);
            leftCheckMask = (finalMask >> 24LL); 
            rightCheckMask = (finalMask << 8LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            mask1 = board << 6LL; mask2 = board << 12LL; 
            finalMask = (board & mask1 & mask2);
            leftCheckMask = (finalMask >> 18LL); 
            rightCheckMask = (finalMask << 6LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));
 
            return count;
        }

        ll twoInARow(ll board)
        {
            ll count = 0;

            ll mask1 = board << 7LL; 
            ll finalMask = (board & mask1);
            ll leftCheckMask = (finalMask >> 14LL); 
            ll rightCheckMask = (finalMask << 7LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            mask1 = board << 1LL; 
            finalMask = (board & mask1);
            leftCheckMask = (finalMask >> 2LL); 
            rightCheckMask = (finalMask << 1LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            mask1 = board << 8LL; 
            finalMask = (board & mask1);
            leftCheckMask = (finalMask >> 16LL); 
            rightCheckMask = (finalMask << 8LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            mask1 = board << 6LL;  
            finalMask = (board & mask1);
            leftCheckMask = (finalMask >> 12LL); 
            rightCheckMask = (finalMask << 6LL); 
            rightCheckMask &= FULL_BOARD_MASK;
            count += (countSetBits((~board) & leftCheckMask) + countSetBits((~board) & rightCheckMask));

            return count;
        }

        int heruistic(ll opBoard, ll aiBoard, bool maxPlayer, ll depth)
        {
            int value = 0;

            if(fourInARow(aiBoard)) return FOUR_IN_A_ROW_VALUE * (depth+1);
            if(fourInARow(opBoard)) return -1 * FOUR_IN_A_ROW_VALUE* (8 - depth);

            if(threeInARow(aiBoard)) value += THREE_IN_A_ROW_VALUE * threeInARow(aiBoard);
            if(threeInARow(opBoard)) value -= (THREE_IN_A_ROW_VALUE+1) * threeInARow(opBoard);

            if(twoInARow(aiBoard)) value += twoInARow(aiBoard);
            if(twoInARow(opBoard)) value -= twoInARow(opBoard);

            return value;
        }

    public:
        /**
         * @brief MiniMax algorithm which determines the best column to play
         * 
         * @param playerBoard the player's board
         * @param aiBoard the AI's board
         * @param currentDepth the current depth of the recursion tree
         * @param maxDepth the maximum depth of the recursion tree
         * @param maxTurn whether the turn is the maximizer or minimizer
         * @param alpha alpha value of alpha-beta pruning
         * @param beta beta value of alpha-beta pruning
         * @return int the calculated value of the current position for the AI
         */
        int miniMax(ll playerBoard, ll aiBoard, int currentDepth, int maxDepth, bool maxTurn, int alpha, int beta)
        {
            ll bothBoard = playerBoard | aiBoard; 
            if(currentDepth == maxDepth || (FULL_BOARD == (FULL_BOARD & bothBoard)) || checkWin(playerBoard, aiBoard)){
                int result = heruistic(playerBoard, aiBoard, maxTurn, currentDepth);
                return result;
            }
            else if(currentDepth!=0 && maxTurn && exploredMax[currentDepth].count(playerBoard) && exploredMax[currentDepth][playerBoard].count(aiBoard)){
                return exploredMax[currentDepth][playerBoard][aiBoard];
            }
            else if(currentDepth!=0 && !maxTurn && exploredMin[currentDepth].count(playerBoard) && exploredMin[currentDepth][playerBoard].count(aiBoard)){
                return exploredMin[currentDepth][playerBoard][aiBoard];
            }
            else{
                if(maxTurn){
                    int bestCost = INT_MIN;
                    int chosenCol = -1;
                    for(int i = 0; i < BOARD_WIDTH; i++){
                        if( ((1LL << (7LL*i+5)) & bothBoard)== 0){
                            ll addPieceMask = (1LL << ( ((ll) BOARD_WIDTH) *i));
                            while(addPieceMask & bothBoard) addPieceMask <<= 1LL;

                            ll newBoard = aiBoard | addPieceMask;
                            int thisCost = miniMax(playerBoard, newBoard, currentDepth+1, maxDepth, !maxTurn, alpha, beta);
                            if(thisCost > bestCost){
                                bestCost = thisCost;
                                chosenCol = i;
                            }
                            alpha = max(alpha, thisCost);
                            if(beta <= alpha) break;

                            if(currentDepth==0) cout << i+1 << " : " << thisCost << '\n';
                        }
                    }
                    if(currentDepth!=0){
                        exploredMax[currentDepth][playerBoard][aiBoard] = bestCost;
                        return bestCost;
                    }
                    else{
                        return chosenCol;
                    }
                }
                else{
                    int worstCost = INT_MAX;
                    for(int i = 0; i < BOARD_WIDTH; i++){
                        if( ((1LL << (7LL*i+5)) & bothBoard)==0 ){
                            ll addPieceMask = (1LL << ( ((ll) BOARD_WIDTH) *i));
                            while(addPieceMask & bothBoard) addPieceMask <<= 1LL;

                            ll newBoard = playerBoard | addPieceMask;
                            int thisCost = miniMax(newBoard, aiBoard, currentDepth+1, maxDepth, !maxTurn, alpha, beta);
                            if(thisCost < worstCost){
                                worstCost = thisCost;
                            }

                            beta = min(beta, thisCost);
                            if(beta <= alpha) break;
                        }
                        
                    }
                    exploredMin[currentDepth][playerBoard][aiBoard] = worstCost;
                    return worstCost;
                }
            }
        }

};

