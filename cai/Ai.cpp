#include "headers/Ai.h"
#include "headers/Board.h"
#include "headers/Piece.h"
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>
#include <cmath>

int counter;

// Return the material difference for the current side (Material value is the current heuristic)
int evaluateHeuristic(Board* chessboard, int maxSide) {
    int maxMaterial = 10*chessboard->getMaterial(maxSide);
    int minMaterial = 10*chessboard->getMaterial(1 - maxSide); // Assuming the other side is the opponent
    
    // Evaluate based on material difference
    int materialScore = maxMaterial - minMaterial;
    
    // You can add more evaluation terms here, such as piece activity, pawn structure, king safety, etc.
    
    // Combine the evaluation terms
    int totalScore = materialScore;
    
    return totalScore;
}

// Search here that evaluates heuristic. In findBestMove this will replace the bestMove variable but only if its the maximize side.
std::tuple <Coordinate, Coordinate, int> Search(Board* chessboard, Piece** board, int depth, bool currentSide, bool maxSide, int alpha, int beta){

    if (depth == 0){
        int heuristic = evaluateHeuristic(chessboard, maxSide);
        return {{-1, -1}, {-1, -1}, heuristic};
    }

    std::tuple <Coordinate,Coordinate, int> bestMove = {{-1, -1}, {-1, -1}, 0};

    if (currentSide == maxSide){
        int maxEval = std::numeric_limits<int>::min();
        for (int row = 0; row < 8; row++){
            for (int col = 0; col < 8; col++){
                // Go through each piece and look through all moves to find optimal one.
                Piece piece = board[row][col];
                if (piece.getSide() == currentSide){
                    // Generate all possible moves for current piece.
                    piece.getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
                    for (Coordinate move : piece.getMoves()){
                        Piece selectedPiece = piece;
                        Piece capturedPiece = board[move.y][move.x];
                        Coordinate moveFrom= {col, row};
                        Move* pos_move = new Move(row, col, move.y, move.x);
                        pos_move->applyMove(chessboard, 0, currentSide);

                        std::tuple<Coordinate, Coordinate, int> eval = Search(chessboard, board, depth-1, !currentSide, maxSide, alpha, beta);
                        pos_move->undoMove(chessboard, selectedPiece, capturedPiece);
                        delete pos_move;
                        
                        int evalHeuristic = std::get<2>(eval);
                        if (evalHeuristic >= maxEval){
                            maxEval = evalHeuristic;
                            std::get<0>(bestMove) = moveFrom; // Piece starting position.
                            std::get<1>(bestMove) = move; // Piece end position.
                            std::get<2>(bestMove) = maxEval; // Heuristic of that move.
                        }
                        counter = counter + 1;
                    }
                }
            }
        }
        return bestMove;
    }
    else{
        int minEval = std::numeric_limits<int>::max();
        for (int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                Piece piece = board[row][col];
                if (piece.getSide() == currentSide){
                    piece.getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
                    for (Coordinate move : piece.getMoves()){
                        Piece selectedPiece = piece;
                        Piece capturedPiece = board[move.y][move.x];
                        Coordinate moveFrom= {col, row};
                        Move* pos_move = new Move(row, col, move.y, move.x);
                        pos_move->applyMove(chessboard, 0, currentSide);

                        std::tuple<Coordinate, Coordinate, int> eval = Search(chessboard, board, depth-1, !currentSide, maxSide, alpha, beta);
                        pos_move->undoMove(chessboard, selectedPiece, capturedPiece);
                        delete pos_move;

                        // To minimize, negate the heuristic of the move.
                        int evalHeuristic = -std::get<2>(eval);
                        if (evalHeuristic < minEval){
                            minEval = evalHeuristic;
                            std::get<0>(bestMove) = moveFrom; // Piece starting position.
                            std::get<1>(bestMove) = move; // Piece end position.
                            std::get<2>(bestMove) = minEval; // Heuristic of that move.
                        }
                        counter = counter + 1;
                    }
                }
            }
        }
        return bestMove;
    }
}

// Piece start, Piece end, heuristic.
std::tuple <Coordinate, Coordinate, int> findBestMove(Board* chessboard, Piece** board, int depth, bool maxSide){
    counter = 0;
    int minInt = std::numeric_limits<int>::min();
    int maxInt = std::numeric_limits<int>::max();
    // printf("COUNTER: %d\n", counter);
    return Search(chessboard, chessboard->getBoard(), depth, maxSide, maxSide, minInt, maxInt);
}