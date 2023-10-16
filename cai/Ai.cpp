#include "headers/Ai.h"
#include "headers/Board.h"
#include "headers/Piece.h"
#include "headers/PieceSquareTables.h"
#include <iostream>
#include <vector>
#include <limits>
#include <tuple>
#include <cmath>
#include <algorithm> 

int counter;

bool compareMoves(Piece** board, int row, int col, const Coordinate move1, const Coordinate move2) {
    // Implement your comparison logic here
    // Compare move1 and move2 based on the given board, row, and col.
    int diff1 = board[move1.y][move1.x].getValue() - board[row][col].getValue();
    int diff2 = board[move2.y][move2.x].getValue() - board[row][col].getValue();

    return diff1 < diff2;
}

std::vector<Coordinate> orderMoves(Piece** board, int row, int col, std::vector<Coordinate> moves) {
    // Sort the moves in order of difference in value between captured pieces to reduce alpha beta pruning.
    std::sort(moves.begin(), moves.end(), [board, row, col](const Coordinate& move1, const Coordinate& move2) {
        return compareMoves(board, row, col, move2, move1); // Descending order.
    });

    return moves;
}

int getSideHeuristic(Board* chessboard, Piece** board, int currentSide){
    int totMaterial = 0;
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            int type = board[row][col].getType();
            int side = board[row][col].getSide();
            if (type != -1 && side == currentSide){
                int squareTableVal;
                board[row][col].getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
                std::vector<Coordinate> moves = board[row][col].getMoves();

                // TODO!
                // "Mobility"-heuristic (The number of available moves for the piece.)

                switch (type){
                    // Scale up all pieces by 1000 to make sure that piece table is not dominant.
                    case 1:
                    // Pawn
                        if (currentSide == 0){
                            squareTableVal = BlackPawnSquareTable[row][col];
                        }
                        else{
                            squareTableVal = WhitePawnSquareTable[row][col];
                        }
                        totMaterial = totMaterial + board[row][col].getValue()*1000 + squareTableVal;
                        break;
                    case 2:
                    // Bishop
                        if (currentSide == 0){
                            squareTableVal = BlackBishopSquareTable[row][col];
                        }
                        else{
                            squareTableVal = WhiteBishopSquareTable[row][col];
                        }
                        totMaterial = totMaterial + board[row][col].getValue()*1000 + squareTableVal;
                        break;
                    case 3:
                    // Knight
                        if (currentSide == 0){
                            squareTableVal = BlackKnightSquareTable[row][col];
                        }
                        else{
                            squareTableVal = WhiteKnightSquareTable[row][col];
                        }
                        totMaterial = totMaterial + board[row][col].getValue()*100 + squareTableVal;
                        break;
                    case 4:
                    // Rook
                        if (currentSide == 0){
                            squareTableVal = BlackRookSquareTable[row][col];
                        }
                        else{
                            squareTableVal = WhiteRookSquareTable[row][col];
                        }
                        totMaterial = totMaterial + board[row][col].getValue()*100 + squareTableVal;
                        break;
                    case 5:
                    // Queen
                        if (currentSide == 0){
                            squareTableVal = BlackQueenSquareTable[row][col];
                        }
                        else{
                            squareTableVal = WhiteQueenSquareTable[row][col];
                        }
                        totMaterial = totMaterial + board[row][col].getValue()*100 + squareTableVal;
                        break;
                    case 6:
                    // King
                        // Make king desirable (scaled by 10000)
                        if (currentSide == 0){
                            squareTableVal = BlackKingSquareTable[row][col];
                        }
                        else{
                            squareTableVal = WhiteKingSquareTable[row][col];
                        }
                        totMaterial = totMaterial + board[row][col].getValue()*1000 + squareTableVal;
                        break;
                }
            }
        }
    }
    return totMaterial;
}

// Heuristic function: Material  + Piece Square Table value (See headers/PieceSquareTables.h)
int evaluateHeuristic(Board* chessboard, Piece** board, int maxSide, int currentSide) {
    // Initialize player and opponent's total material values
    int player_material = getSideHeuristic(chessboard, board, maxSide);
    int opponent_material = getSideHeuristic(chessboard, board, !maxSide);

    int material_advantage = player_material + opponent_material;
    // The commented out code below seems to perform worse compared to the code above. Save both if something else comes up.

    // int material_advantage = 0;
    // if (currentSide == maxSide){
    //     // Maximize side wants this as high as possible.
    //     material_advantage = player_material - opponent_material;
    // }
    // else{
    //     // Minimize side wants this as low as possible.
    //     material_advantage = opponent_material - player_material;
    // }
    return material_advantage;
}

// Search here that evaluates heuristic. In findBestMove this will replace the bestMove variable but only if its the maximize side.
std::tuple <Coordinate, Coordinate, int> Search(Board* chessboard, Piece** board, int depth, bool currentSide, bool maxSide, int alpha, int beta){
    counter++;
    if (depth == 0){
        // If leaf, return the heuristic.
        int heuristic = evaluateHeuristic(chessboard, board, maxSide, currentSide);
        return {{-1, -1}, {-1, -1}, heuristic};
    }

    if (currentSide == maxSide){
        // Initialize the best move as the worst possible move for the maximizing side.
        // +1 to the final argument to prevent integer overflow
        std::tuple <Coordinate,Coordinate, int> bestMove = {{-1, -1}, {-1, -1}, std::numeric_limits<int>::min()+1};
        for (int row = 0; row < 8; row++){
            for (int col = 0; col < 8; col++){
                // Go through each piece and look through all moves to find optimal one.
                Piece piece = board[row][col];
                if (piece.getSide() == currentSide){
                    // Generate all possible moves for current piece.
                    piece.getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
                    std::vector<Coordinate> moves = piece.getMoves();
                    moves = orderMoves(board, row, col, moves);
                    Coordinate moveFrom= {col, row};
                    for (Coordinate move : moves){
                        Piece selectedPiece = piece;
                        Piece capturedPiece = board[move.y][move.x];
                        Move* pos_move = new Move(row, col, move.y, move.x);

                        // There should be some logic here abuot promotion of pawns, currently it will always be promoted to a Queen.
                        pos_move->applyMove(chessboard, 5, currentSide);
                        std::tuple<Coordinate, Coordinate, int> eval = Search(chessboard, board, depth-1, !currentSide, maxSide, alpha, beta);
                        pos_move->undoMove(chessboard, selectedPiece, capturedPiece);
                        delete pos_move;
                        
                        int evalHeuristic = std::get<2>(eval);
                        if (evalHeuristic >= alpha){
                            alpha = evalHeuristic;
                            std::get<0>(bestMove) = moveFrom; // Piece starting position.
                            std::get<1>(bestMove) = move;     // Piece end position.
                            std::get<2>(bestMove) = alpha;    // Heuristic of that move.
                        }
                        if (alpha >=beta){
                            break;
                        }
                    }
                }
            }
        }
        return bestMove;
    }
    else{
        // Initialize the best move as the worst possible move for the minimizing side.
        // -1 to the final argument to prevent integer overflow
        std::tuple <Coordinate,Coordinate, int> bestMove = {{-1, -1}, {-1, -1}, std::numeric_limits<int>::max()-1};
        for (int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                Piece piece = board[row][col];
                if (piece.getSide() == currentSide){
                    piece.getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
                    std::vector<Coordinate> moves = piece.getMoves();
                    for (Coordinate move : moves){
                        Piece selectedPiece = piece;
                        Piece capturedPiece = board[move.y][move.x];
                        Coordinate moveFrom= {col, row};
                        Move* pos_move = new Move(row, col, move.y, move.x);

                        // There should be some logic here abuot promotion of pawns, currently it will always be promoted to a Queen.
                        pos_move->applyMove(chessboard, 5, currentSide);
                        std::tuple<Coordinate, Coordinate, int> eval = Search(chessboard, board, depth-1, !currentSide, maxSide, alpha, beta);
                        pos_move->undoMove(chessboard, selectedPiece, capturedPiece);
                        delete pos_move;

                        // To minimize, negate the heuristic of the move.
                        int evalHeuristic = -std::get<2>(eval);
                        if (evalHeuristic < beta){
                            beta = evalHeuristic;
                            std::get<0>(bestMove) = moveFrom; // Piece starting position.
                            std::get<1>(bestMove) = move;     // Piece end position.
                            std::get<2>(bestMove) = beta;     // Heuristic of that move.
                        }
                        if (alpha >= beta){
                            break;
                        }
                    }
                }
            }
        }
        return bestMove;
    }
}

// (Piece start, Piece end, heuristic) is returned.
std::tuple <Coordinate, Coordinate, int> findBestMove(Board* chessboard, Piece** board, int depth, bool maxSide){
    // Counter is the number of nodes (Incremented each time a recursive call is made.)
    counter = 0;
    // Initialize alpha and beta.
    int minInt = std::numeric_limits<int>::min();
    int maxInt = std::numeric_limits<int>::max();
    // Look for best move.
    auto bestMove = Search(chessboard, chessboard->getBoard(), depth, maxSide, maxSide, minInt, maxInt);
    printf("Number of Nodes created: %d\n", counter);
    return bestMove;
}