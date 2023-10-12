#pragma once  // Include guards to prevent multiple inclusion

// Includes
#include <vector>
#include "../headers/Piece.h"

class Board; // Forward declaration
class Piece; // Forward declaration

class Move {
public:
    Move(int ystart, int xstart, int yend, int xend);  // Constructor
    // Move(); // Destructor

    // Member functions
    bool applyMove(Board* chessboard, int transformTo, int currentSide);
    void undoMove(Board* chessboard, Piece selectedPiece, Piece capturedPiece);
    bool incorrectSideMove(Piece start_piece, bool side);
    bool isCastleMove(Piece** board, Piece piece, int starty, int startx, int endy, int endx);
    bool isEnPassantMove(Piece** board, Piece piece, int starty, int startx, int endy, int endx);
    bool isValidMove(std::vector<Coordinate> moves, int end_row, int end_col);
    // Getters

    // Setters
    
    
private:
    // The start x coordinate for the move.
    int xstart;
    // The start y coordinate for the move.
    int ystart;
    // The end x coordinate for the move.
    int xend;
    // The end y coordinate for the move.
    int yend;
};