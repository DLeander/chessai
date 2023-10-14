#include "headers/Ai.h"
#include "headers/Board.h"
#include "headers/Piece.h"
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>
#include <cmath>

int counter;

int getTotalMaterial(Piece** board){
    int totMaterial = 0;
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            if (board[row][col].getType() != -1){
                totMaterial = totMaterial + board[row][col].getValue();
            }
        }
    }
    printf("TOTMATERIAL: %d\n", totMaterial);
    return totMaterial;
}

// Return the material difference for the current side (Material value is the current heuristic)
int evaluateHeuristic(Board* chessboard, int maxSide) {
    int materialScore = getTotalMaterial(chessboard->getBoard());
    // printf("MATERIAL SCORE: %d\n", materialScore);
    return materialScore;
}

// Search here that evaluates heuristic. In findBestMove this will replace the bestMove variable but only if its the maximize side.
std::tuple <Coordinate, Coordinate, int> Search(Board* chessboard, Piece** board, int depth, bool currentSide, bool maxSide, int alpha, int beta){
    counter++;
    if (depth == 0){
        int heuristic = evaluateHeuristic(chessboard, maxSide);
        return {{-1, -1}, {-1, -1}, heuristic};
    }

    std::tuple <Coordinate,Coordinate, int> bestMove = {{-1, -1}, {-1, -1}, 0};

    if (currentSide == maxSide){
        int maxEval = std::numeric_limits<int>::min();
        // std::tuple <Coordinate,Coordinate, int> bestMove = {{-1, -1}, {-1, -1}, std::numeric_limits<int>::min()};
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
                        if (evalHeuristic > maxEval){
                            // printf("\nMAXEVALBEFORE: %d\n", maxEval);
                            maxEval = evalHeuristic;
                            // printf("MAXEVALAFTER: %d\n", maxEval);
                            std::get<0>(bestMove) = moveFrom; // Piece starting position.
                            std::get<1>(bestMove) = move; // Piece end position.
                            std::get<2>(bestMove) = maxEval; // Heuristic of that move.
                        }
                        // if (alpha >=beta){
                        //     break;
                        // }
                    }
                }
            }
        }
        return bestMove;
    }
    else{
        int minEval = std::numeric_limits<int>::max();
        // std::tuple <Coordinate,Coordinate, int> bestMove = {{-1, -1}, {-1, -1}, minEval};
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
                            // printf("MIN!!!!EVAL: %d\n", minEval);
                            std::get<0>(bestMove) = moveFrom; // Piece starting position.
                            std::get<1>(bestMove) = move; // Piece end position.
                            std::get<2>(bestMove) = minEval; // Heuristic of that move.
                        }
                        // if (alpha >= beta){
                        //     break;
                        // }
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
    auto bestMove = Search(chessboard, chessboard->getBoard(), depth, maxSide, maxSide, minInt, maxInt);
    printf("COUNTER: %d\n", counter);
    return bestMove;
}