// Includes
#include <iostream>
#include "../headers/Board.h"
#include "../headers/Piece.h"
#include <iomanip>

// Board Constructor.
Board::Board() {
    // The size of the chessboard.
    int rows = 8;
    int cols = 8;
    // Create the board.
    this->board = new Piece*[rows];
    for (int i = 0; i < rows; ++i) {
        this->board[i] = new Piece[cols];
    }

    // The scores of each team.
    this->whiteScore = 0;
    this->blackScore = 0;

    // Check is not possible from the start.
    this->bCheck = false;
    this->wCheck = false;

    bKingPos.x = 4;
    bKingPos.y = 0;
    wKingPos.x = 4;
    wKingPos.y = 7;

    // Populate the board.
    populateBoard();

}

Board::~Board(){
    // Deallocate the board
    for (int i = 0; i < 8; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

void Board::updateKingPos(Coordinate newPos, int side){
    if (side == 0){
        this->bKingPos = newPos;
    }
    else{
        this->wKingPos = newPos;
    }
}

void Board::setScore(bool team, int score){
    // If no piece is at tile, no score to set.
    if (score < 0){
        return;
    }

    // Set score to the correct team.
    if (team){
        this->whiteScore += score;
    }
    else{
        this->blackScore += score;
    }
}

Piece** Board::getBoard(){
    return this->board;
}
Coordinate Board::getwKingPos(){
    return this->wKingPos;
}
Coordinate Board::getbKingPos(){
    return this->bKingPos;
}

// Populate the board with starting pieces.
void Board::populateBoard(){
    // All pieces for the black side.
    int black_pieces[]        =    {4, 3, 2, 5, 6, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1};
    // Values of pieces on the black side.
    int black_piece_values[]  =    {5, 3, 3, 8, 9, 3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1};
    // All pieces for the white side.
    int white_pieces[]        =    {1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 5, 6, 2, 3, 4};
    // Values of pieces on the white side.
    int white_piece_values[]  =    {1, 1, 1, 1, 1, 1, 1, 1, 5, 3, 3, 8, 9, 3, 3, 5};

    // Populate black side.
    // Set the current piece information in each iteration:
    int current_piece = 0;
    for (int row = 0; row < 2; row++){
        for (int col = 0; col < 8; col++){
            std::vector<Coordinate> moves;
            this->board[row][col].UpdatePiece(black_pieces[current_piece], 0, black_piece_values[current_piece], row, col, moves, 0);
            this->board[row][col].printType();
            current_piece++;
        }
    }

    // Populate white side.
    // Set the current piece information in each iteration:
    current_piece = 0;
    for (int row = 6; row < 8; row++){
        for (int col = 0; col < 8; col++){
            std::vector<Coordinate> moves;
            this->board[row][col].UpdatePiece(white_pieces[current_piece], 1, white_piece_values[current_piece], row, col, moves, 0);
            this->board[row][col].printType();
            current_piece++;
        }
    }
}

// Show the current available moves for a piece.
void Board::showAvailableMoves(int y, int x){
    Piece piece = this->board[y][x];
    if (piece.isPiece()){
        piece.getAvailableMoves(this->board, this->wKingPos, this->bKingPos);
        std::vector<Coordinate> moves = piece.getMoves();
        for (int i = 0; i < moves.size(); i++){
            printf("Y: %d X: %d\n", moves[i].y, moves[i].x);
        }
    }
}

// Print the current board.
// void Board::printBoard() {
//     // Go through each tile, pieces are represented by their number, empty tiles are marked '-'.
//     for (int row = 0; row <= 8; row++){
//         if (row == 8){
//             printf("    a     b     c     d     e     f     g     h\n");
//         }
//         else{
//             for (int col = 0; col < 9; col++){
//                 if (col == 0){
//                     printf("%d ", row+1);
//                 }
//                 else{
//                     Piece piece = this->board[row][col-1];
//                     if (piece.isPiece()){
//                         printf("[");
//                         piece.printPiece();
//                         printf("]");
//                     }
//                     else{
//                         printf("[ -- ]");
//                     }
//                 }
//             }
//             if (row == 2){
//                 printf("   Black Score: %d", this->blackScore);
//             }
//             if (row == 5){
//                 printf("   White Score: %d", this->whiteScore);
//             }
//             printf("\n");
//         }

//     }
//     printf("\n");
// }
void Board::printBoard(){
    std::cout<< u8"\033[2J\033[1;1H";
    for (int row = 0; row < 8; row++){
        for (int lineNum = 1; lineNum < 7;  lineNum++){
            // if (lineNum == 4){ printf("%d", row+1); }
            for (int col = 0; col < 8; col++){
                std::string pieceLine;
                // printf("%d",lineNum);
                switch (lineNum){
                    case 1:
                        pieceLine = board[row][col].getLine1();
                        break;
                    case 2:
                        pieceLine = board[row][col].getLine2();
                        break;
                    case 3:
                        pieceLine = board[row][col].getLine3();
                        break;
                    case 4:
                        pieceLine = board[row][col].getLine4();
                        break;
                    case 5:
                        pieceLine = board[row][col].getLine5();
                        break;
                    case 6:
                        pieceLine = board[row][col].getLine6();
                        break;
                }
                printf("%s", pieceLine.c_str());
            }
            printf("\n");
        }
        // printf("\n");
    }
}