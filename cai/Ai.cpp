#include "headers/Ai.h"
#include "headers/Board.h"
#include "headers/Piece.h"
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>
#include <cmath>

// Return the material difference for the current side (Material value is the current heuristic)
int evaluateHeuristic(Board* chessboard, int side, Coordinate position) {
    // Get material and scale up by a factor of 10.
    int whiteMaterial = 10*chessboard->getMaterial(0);
    int blackMaterial = 10*chessboard->getMaterial(1);

    // Moves close to the middle is more valuable than those at the corner.
    int middleScore = round((abs(position.x - 4) + abs(position.x - 3))/2);
    // printf("Middle score: %d", middleScore);

    // Pieces with alot of moves are generally better.
    // TODO.

    if (side == 1) {
        // It's White's turn, so return the difference in material.
        return whiteMaterial - blackMaterial;
    } else {
        // It's Black's turn, so return the difference in material from Black's perspective.
        return blackMaterial - whiteMaterial;
    }
}

std::tuple<Coordinate, int> minimax(Board* chessboard, Piece** board, Coordinate position, int depth, bool side, bool maximizeSide, int alpha, int beta) {
    // Get all the current moves for the current piece.
    board[position.y][position.x].getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
    
    if (depth == 0 || board[position.y][position.x].getMoves().size() == 0) {
        int val = evaluateHeuristic(chessboard, side, position);
        std::tuple<Coordinate, int> result(position, val);
        return result;
    }
    
    std::tuple<Coordinate, int> result;
    
    if (side == maximizeSide) { // Maximizing player
        int maxEval = evaluateHeuristic(chessboard, side, position);
        result = std::make_tuple(position, maxEval); 

        for (Coordinate move : board[position.y][position.x].getMoves()) {
            Piece selectedPiece = board[position.y][position.x];
            Piece capturedPiece = board[move.y][move.x];
            Move* pos_move = new Move(position.y, position.x, move.y, move.x);
            pos_move->applyMove(chessboard, 0, side);
            
            std::tuple<Coordinate, int> eval = minimax(chessboard, board, move, depth - 1, !side, maximizeSide, alpha, beta);
            pos_move->undoMove(chessboard, selectedPiece, capturedPiece);
            delete pos_move;

            if (maxEval <= std::get<1>(eval)) {
                printf("MOVE X: %d Y: %d with Heuristic: %d\n", move.x, move.y, maxEval);
                maxEval = std::get<1>(eval);
                std::get<0>(result) = move;
                std::get<1>(result) = maxEval;
            }
            
            alpha = std::max(alpha, maxEval);
            
            // Prune the subtree if beta is less than or equal to alpha
            // if (beta <= alpha) {
            //     printf("MAX PRUNE!\n");
            //     break;
            // }
        }
        
        return result;
    } 
    else { // Minimizing player
        int minEval = evaluateHeuristic(chessboard, side, position);
        result = std::make_tuple(position, minEval); 
        
        for (Coordinate move : board[position.y][position.x].getMoves()) {
            // Your move exploration logic here...
            
            Piece selectedPiece = board[position.y][position.x];
            Piece capturedPiece = board[move.y][move.x];
            Move* pos_move = new Move(position.y, position.x, move.y, move.x);
            pos_move->applyMove(chessboard, 0, side);
            
            std::tuple<Coordinate, int> eval = minimax(chessboard, board, move, depth - 1, !side, maximizeSide, alpha, beta);

            pos_move->undoMove(chessboard, selectedPiece, capturedPiece);
            delete pos_move;

            if (minEval >= std::get<1>(eval)) {
                minEval = std::get<1>(eval);
                std::get<0>(result) = move;
                std::get<1>(result) = minEval;
                // printf("Best move heuristic: %d\n", std::get<1>(pos_move));
            }
            
            beta = std::min(beta, minEval);
            
            // Prune the subtree if beta is less than or equal to alpha
            // if (beta <= alpha) {
            //     printf("MIN PRUNE!\n");
            //     break;
            // }
        }
        
        return result;
    }
}

// Find the best possible move for side using minimax and alpha-beta pruning.
std::tuple <Coordinate,Coordinate> findBestMove(Board* chessboard, Piece** board, int depth, bool side){
    std::tuple <Coordinate, Coordinate> move;
    int minInt = std::numeric_limits<int>::min();
    int maxInt = std::numeric_limits<int>::max();

    int bestMoveHeuristic = minInt;
    // Init tuple which will be reset with new moves.
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            
            if (board[i][j].getSide() == side){
                Coordinate position;
                position.y = i;
                position.x = j;
                std::tuple <Coordinate, int> pos_move = minimax(chessboard, board, position, depth, side, side, minInt, maxInt);

                if (std::get<1>(pos_move) > bestMoveHeuristic){
                    std::get<0>(move) = position;
                    std::get<1>(move) = std::get<0>(pos_move);
                    bestMoveHeuristic = std::get<1>(pos_move);
                }
            }
            

            // Select move with highest heuristic.

        }
    }

    return move;
}