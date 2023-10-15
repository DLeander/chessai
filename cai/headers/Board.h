#pragma once  // Include guards to prevent multiple inclusion

// #include "Move.h"
#include "Piece.h"

struct CheckInfo {
    bool isCheck;  // true if the king is in check, false otherwise.
    int x;  // row of the piece causing the check, -1 if not check.
    int y;  // column of the piece causing the check, -1 if not check.
};

class Piece;

class Board {
public:
    Board();  // Constructor
    ~Board(); // Destructor

    // Member functions
    Piece** getBoard();
    void populateBoard();
    void printBoard();
    void showAvailableMoves(int x, int y);

    // Getters
    Coordinate getbKingPos();
    Coordinate getwKingPos();
    // Unused
    int getMaterial(int side);
    // Unused
    bool isCheckedSide(bool side);

    // Setters
    void setScore(bool side, int score);
    void updateKingPos(Coordinate newPos, int side);
    // Unused
    void setMaterial(int side, int value);
    void changeNumChecks(int i, int side);

private:
    // The chessboard.
    Piece** board;

    // The current score of the white side.
    int whiteScore;
    // The current score of the black side.
    int blackScore;

    // If this value is larger than 0, then there is a check.
    int blackNumChecks;
    int whiteNumChecks;

    // The current coordinate of the king for each side.
    Coordinate bKingPos;
    Coordinate wKingPos;
    
    // Material for the current side (Unused)
    int whiteMaterial;
    int blackMaterial;
};