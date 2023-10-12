#include "headers/Ai.h"
#include "../headers/Board.h"
#include "../headers/Piece.h"
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>

// Return the material value for the current side (Material value is the current heuristic)
int evaluateHeuristic(Board* chessboard, int side){
    if (side == 0){
        return chessboard->getMaterial(side);
    }
    return chessboard->getMaterial(side);
}

std::tuple<Coordinate, int> minimax(Board* chessboard, Piece** board, Coordinate position, int depth, bool side){
    // Get all the current moves for the current piece.
    board[position.y][position.x].getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
    
    if (depth == 0 || board[position.y][position.x].getMoves().size() == 0){
        int val = evaluateHeuristic(chessboard, side);
        std::tuple <Coordinate,int> result(position, val);
        return result;
    }
    
    // If side is black (Ai side)
    if (side == 0){
        int maxEval = evaluateHeuristic(chessboard, side);
        result = std::make_tuple(position, maxEval); 
        for (Coordinate move : board[position.y][position.x].getMoves()){

            // Save old pieces and apply the move.
            Piece selectedPiece = board[position.y][position.x];
            Piece capturedPiece = board[move.y][move.x];
            Move* move = new Move(position.y, position.x, move.y, move.x);
            move->applyMove(chessboard, 0, side)

            std::tuple eval = minimax(chessboard, board, move, depth-1, 1);
            maxEval = std::max(maxEval, std::get<1>(eval));
            if (maxEval == std::get<1>(eval)){
                std::get<0>(result) = std::get<0>(eval);
                std::get<1>(result) = std::get<1>(eval);
            }

            // Undo the move since checking has been done.
            move->undoMove(chessboard, selectedPiece, capturedPiece);
            // Unallocate the move from the heap.
            delete move;
            

        }
        return result;
    }
    else{
        int minEval = evaluateHeuristic(chessboard, side);
        result = std::make_tuple(position, minEval); 
        for (Coordinate move : board[position.y][position.x].getMoves()){
            // Set piece pos, make sure to also replace taken piece if any... (Maybe 3 tuple with taken piece as third argument?)
            std::tuple eval = minimax(chessboard, board, move, depth-1, 0);
            minEval = std::min(minEval, std::get<1>(eval));
            if (minEval == std::get<1>(eval)){
                std::get<0>(result) = std::get<0>(eval);
                std::get<1>(result) = std::get<1>(eval);
            }
            // Reset piece position
            // Reset old loc
        }
        return result;
    }
}

// Find the best possible move for side using minimax and alpha-beta pruning.
std::tuple <Coordinate,Coordinate> findBestMove(Board* chessboard, Piece** board, int depth, bool side){
    printf("hej\n");
    Piece** board = chessboard->getBoard;
    
    // Init tuple which will be reset with new moves.
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            
            if (board[i][j].getSide() == side){
                Coordinate position;
                position.y = i;
                position.x = j;
                std::tuple <Coordinate, int> move = minimax(chessboard, chessboard->getBoard, position, depth, side);
            }
            

            // Select move with highest heuristic.

        }
    }

    return move;
}