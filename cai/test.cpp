#include <iostream>
#include <tuple>
#include <cassert>
#include "headers/Piece.h"
#include "headers/Move.h"
#include "headers/Board.h"
#include "headers/Ai.h"

// Empty the board of all pieces.
void emptyBoard(Piece** board){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[i][j].UpdatePiece();
        }
    }
}

// void Piece::UpdatePiece(int type, int side, int value, int ypos, int xpos, std::vector<Coordinate> moves, int totMoves) {
void test_undo_moves(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves = board[3][2].getMoves();
    emptyBoard(board);

    // Test take piece and undo move.
    board[3][2].UpdatePiece(2, 1, 3, 3, 2, moves, 0);
    board[0][5].UpdatePiece(2, 0, 3, 0, 5, moves, 0);
    Piece selectedPiece = board[3][2];
    Piece capturedPiece = board[0][5];
    Move* move = new Move(3, 2, 0, 5);
    move->applyMove(chessboard, 0, 1);
    move->undoMove(chessboard, selectedPiece, capturedPiece);
    assert(board[3][2].getType() == 2 && board[0][5].getType() == 2);
    delete move;


    // Test undo non-capture move.
    selectedPiece = board[3][2];
    capturedPiece = board[1][4];
    move = new Move(3, 2, 1, 4);
    move->applyMove(chessboard, 0, 1);
    assert(board[3][2].getType() == -1 && board[1][4].getType() == 2);
    move->undoMove(chessboard, selectedPiece, capturedPiece);
    assert(board[3][2].getType() == 2 && board[1][4].getType() == -1);
    delete move;

    // Test castle undo.
    emptyBoard(board);
    // board[0][4].UpdatePiece(6, 0, 9, 0, 4, moves, 0);
    // board[0][7].UpdatePiece(4, 0, 5, 0, 7, moves, 0);
    board[0][4].UpdatePiece(6, 0, 9, 0, 4, moves, 0);
    board[0][7].UpdatePiece(4, 0, 5, 0, 7, moves, 0);
    selectedPiece = board[0][7];
    capturedPiece = board[0][4];
    move = new Move(0, 7, 0, 4);
    move->applyMove(chessboard, 0, 0);
    assert(board[0][5].getType() == 4 && board[0][6].getType() == 6);
    move->undoMove(chessboard, selectedPiece, capturedPiece);
    assert(board[0][7].getType() == 4 && board[0][4].getType() == 6);
    delete move;

    // Test en passant undo.
    emptyBoard(board);
    board[3][4].UpdatePiece(1, 0, 1, 3, 4, moves, 0);
    board[3][3].UpdatePiece(1, 1, 1, 3, 3, moves, 0);
    selectedPiece = board[3][3];
    capturedPiece = board[2][4];
    move = new Move(3, 3, 2, 4);
    move->applyMove(chessboard, 0, 1);
    assert(board[2][4].getType() == 1 && board[3][4].getType() == -1);
    move->undoMove(chessboard, selectedPiece, capturedPiece);
    assert(board[3][3].getType() == 1 && board[3][4].getType() == 1);
    delete move;
    delete board;

    printf("Test 'Undo Moves' was successful.\n");
}

void ai_test_capture_piece(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);

    board[3][2].UpdatePiece(2, 1, 3, 3, 2, moves, 0);
    board[0][5].UpdatePiece(2, 0, 3, 0, 5, moves, 0);
    board[0][0].UpdatePiece(3, 1, 3, 0, 0, moves, 0);
    // chessboard->printBoard();
    std::tuple <Coordinate,Coordinate,int> selected_move = findBestMove(chessboard, board, 3, 0);
    // Create the move 
    // printf("Move: y:%d x: %d\t y:%d x:%d\n\n", std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    printf("THE MOVE HEURISTIC: %d\n", std::get<2>(selected_move));
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    // chessboard->printBoard();
    move->applyMove(chessboard, 0, 0);
    chessboard->printBoard();
    assert(board[3][2].getType() == 2 && board[3][2].getSide() == 0);

    delete move;
    delete board;
    printf("Test 'Capture Piece' was successful.\n");
}

void ai_test_capture_best_piece(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);
    // White pieces.
    board[3][2].UpdatePiece(2, 1, 3, 3, 2, moves, 0); // Bishop
    // Black pieces.
    board[0][5].UpdatePiece(2, 0, 3, 0, 5, moves, 0); // Bishop
    board[5][4].UpdatePiece(4, 0, 5, 5, 4, moves, 0); // Rook

    std::tuple<Coordinate,Coordinate, int> selected_move = findBestMove(chessboard, board, 3, 1);
    // chessboard->printBoard();
    // Create the move 
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 1);
    printf("THE MOVE HEURISTIC: %d\n", std::get<2>(selected_move));
    // chessboard->printBoard();
    assert(board[5][4].getType() == 2 && board[5][4].getSide() == 1);

    delete move;
    delete board;

    printf("Test 'Capture best piece' was successful.\n");
}

void ai_test_no_heuristic_gain(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);

    // board[0][0].UpdatePiece(1, 0, 1, 6, 4, moves, 0);
    // board[0][1].UpdatePiece(1, 0, 1, 6, 4, moves, 0);
    // board[1][0].UpdatePiece(1, 0, 1, 6, 4, moves, 0);

    // board[6][2].UpdatePiece(1, 1, 1, 6, 2, moves, 0);

    board[1][4].UpdatePiece(1, 0, 1, 1, 4, moves, 0);
    board[6][4].UpdatePiece(1, 1, 1, 6, 4, moves, 0);
    // chessboard->printBoard();

    std::tuple<Coordinate,Coordinate, int> selected_move = findBestMove(chessboard, board, 3, 1);
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 1);
    printf("THE MOVE HEURISTIC: %d\n", std::get<2>(selected_move));

    // chessboard->printBoard();

    printf("Test 'No Heuristic Gain' was successful.\n");
}

void ai_test_negative_heuristic_gain(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    emptyBoard(board);

    printf("Test 'Test Negative Heuristic Gain' was successful.\n");
}

void ai_test_checks_king(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    emptyBoard(board);

    printf("Test 'Checks King' was successful.\n");
}

int main() {
    // Run all tests.
    printf("-------------------------------\n");
    test_undo_moves();
    ai_test_capture_piece();
    ai_test_capture_best_piece();
    ai_test_no_heuristic_gain();
    ai_test_negative_heuristic_gain();
    ai_test_checks_king();
    printf("-------------------------------\n");
    return 0;
}