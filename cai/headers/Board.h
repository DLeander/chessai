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
    // Setters
    void setScore(bool team, int score);
    void updateKingPos(Coordinate newPos, int side);

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
};