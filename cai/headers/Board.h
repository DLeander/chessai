#pragma once  // Include guards to prevent multiple inclusion

// #include "Move.h"
#include "Piece.h"

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
    std::string getAlignedPieceLine(int row, int col, int line, int cellWidth);

    // Getters
    Coordinate getbKingPos();
    Coordinate getwKingPos();
    int getMaterial(int side);

    // Setters
    void setScore(bool side, int score);
    void updateKingPos(Coordinate newPos, int side);
    void setMaterial(int side, int value);

private:
    // The chessboard.
    Piece** board;

    // The current score of the white side.
    int whiteScore;
    // The current score of the black side.
    int blackScore;

    bool bCheck;
    bool wCheck;
    Coordinate bKingPos;
    Coordinate wKingPos;
    
    int whiteMaterial;
    int blackMaterial;
};