#pragma once  // Include guards to prevent multiple inclusion

// Includes
#include <vector>
#include <string>

// Structs
// Coordinate is used to store the x and y coordinate for a piece's move
struct Coordinate{
  int x;
  int y;
  bool enPassant;
};

class Piece {
public:
    Piece();  // Constructor
    // ~Piece(); // Destructor

    // Member functions
    void UpdatePiece(int type, int side, int value, int ypos, int xpos, std::vector<Coordinate> moves, int totMoves);
    void UpdatePos(int xpos, int ypos);
    void UpdatePiece();
    bool isPiece();
    void printPieceInfo();
    void printPiece();
    void getAvailableMoves(Piece** board, Coordinate wKingPos, Coordinate bKing);

    // Pretty print
    void printType();

    // Getters
    int getType();
    int getSide();
    int getValue();
    int getXpos();
    int getYpos();
    std::vector<Coordinate> getMoves();
    std::string getLine1();
    std::string getLine2();
    std::string getLine3();
    std::string getLine4();
    std::string getLine5();
    std::string getLine6();
    int getTotMoves();

    // Setters
    void setType(int type);
    void increaseTotMoves();

private:
    // The side the piece belongs to (Black/White - 0/1).
    int side;
    // The type of piece (See printPiece() for the current piece types.)
    int type;
    // the value of the piece (1, 3, 5, 8).
    int value;
    // The x position on the board.
    int xpos;
    // The y position on the board.
    int ypos;

    // The number of moves done by the piece.
    int totMoves;

    std::string line1;
    std::string line2;
    std::string line3;
    std::string line4;
    std::string line5;
    std::string line6;

    // The current tiles a piece can move to.
    std::vector<Coordinate> moves;

    // Find the current moves for a piece.
    void pawnMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos);
    void bishopMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos);
    void knightMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos);
    void rookMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos);
    void queenMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos);
    void kingMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos);

    // Examine functions.
    bool isCheckedTiles(Piece** board, Coordinate possible_location, Coordinate wKingPos, Coordinate bKingPos);

    // Pretty prints for different pieces.
    void printEmptyTile();
    void printPawn();
    void printBishop();
    void printKnight();
    void printRook();
    void printQueen();
    void printKing();
};