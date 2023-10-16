#include <iostream>
#include <tuple>
#include <cassert>
#include <limits>
#include <unistd.h>
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

// Test to make sure that undoing of moves work (Used in the alpha-beta algorithm.)
void test_undo_moves(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves = board[3][2].getMoves();
    emptyBoard(board);

    // Test take piece and undo move.
    board[3][2].UpdatePiece(2, 1, 3, 3, 2, moves, 1);
    board[0][5].UpdatePiece(2, 0, 3, 0, 5, moves, 1);
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
    board[3][4].UpdatePiece(1, 0, 1, 3, 4, moves, 1);
    board[3][3].UpdatePiece(1, 1, 1, 3, 3, moves, 1);
    selectedPiece = board[3][3];
    capturedPiece = board[2][4];
    move = new Move(3, 3, 2, 4);
    move->applyMove(chessboard, 1, 1);
    assert(board[2][4].getType() == 1 && board[3][4].getType() == -1);
    move->undoMove(chessboard, selectedPiece, capturedPiece);
    assert(board[3][4].getType() == 1 && board[3][3].getType() == 1);
    delete move;
    delete chessboard;

    printf("Test 'Undo Moves' was successful.\n");
}
// Check if the Ai captures a piece.
void ai_test_capture_piece(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);

    board[3][2].UpdatePiece(2, 1, 3, 3, 2, moves, 1);
    board[0][5].UpdatePiece(2, 0, 3, 0, 5, moves, 1);
    board[0][0].UpdatePiece(3, 1, 3, 0, 0, moves, 1);
    // Get the best move.
    std::tuple <Coordinate,Coordinate,int> selected_move = findBestMove(chessboard, board, 3, 0);
    // Create the move 
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 0);
    assert(board[3][2].getType() == 2 && board[3][2].getSide() == 0);

    delete move;
    delete chessboard;
    printf("Test 'Capture Piece' was successful.\n");
}

// Make sure that the Ai captures the best piece. (A bishop can choose to capture a rook or a bishop, rook should be taken.)
void ai_test_capture_best_piece(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);
    // White pieces.
    board[3][2].UpdatePiece(2, 1, 3, 3, 2, moves, 1); // Bishop
    // Black pieces.
    board[0][5].UpdatePiece(2, 0, 3, 0, 5, moves, 1); // Bishop
    board[5][4].UpdatePiece(4, 0, 5, 5, 4, moves, 1); // Rook

    std::tuple<Coordinate,Coordinate, int> selected_move = findBestMove(chessboard, board, 3, 1);
    // Create the move 
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 1);
    assert(board[5][4].getType() == 2 && board[5][4].getSide() == 1);

    delete move;
    delete chessboard;

    printf("Test 'Capture best piece' was successful.\n");
}

// See if the Ai makes a move even though no heuristic gain is made.
void ai_test_no_heuristic_gain(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);

    board[0][0].UpdatePiece(1, 0, 1, 6, 4, moves, 1);
    board[0][1].UpdatePiece(1, 0, 1, 6, 4, moves, 1);
    board[1][0].UpdatePiece(1, 0, 1, 6, 4, moves, 1);

    board[6][2].UpdatePiece(1, 1, 1, 6, 2, moves, 1);

    board[1][4].UpdatePiece(1, 0, 1, 1, 4, moves, 1);
    board[6][4].UpdatePiece(1, 1, 1, 6, 4, moves, 1);

    std::tuple<Coordinate,Coordinate, int> selected_move = findBestMove(chessboard, board, 3, 1);
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 1);

    assert(board[5][2].getType() == 1 || board[4][2].getType() == 1 || board[5][4].getType() == 1 || board[4][4].getType() == 1);

    delete move;
    delete chessboard;
    printf("Test 'No Heuristic Gain' was successful.\n");
}

// See if the AI checks the king in the constructed board.
void ai_test_checks_king(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);
    board[0][4].UpdatePiece(6, 0, 9, 0, 4, moves, 1);
    board[0][7].UpdatePiece(4, 0, 5, 0, 7, moves, 1);
    board[6][6].UpdatePiece(4, 0, 5, 6, 6, moves, 1);

    board[7][4].UpdatePiece(6, 1, 9, 7, 4, moves, 1);
    // Check in this scenario is only found when the depth is set to 5. (AI is bad at endgame.)
    std::tuple<Coordinate,Coordinate, int> selected_move = findBestMove(chessboard, board, 5, 0);
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 0);
    assert(board[7][7].getType() == 4);
    delete move;
    delete chessboard;
    printf("Test 'Checks King' was successful.\n");
}

// Evaluate on a board mid game to see the number of nodes created (Used to check difference with alpha-beta pruning and ordering the moves vector.)
void ai_test_number_of_nodes(){
    Board* chessboard = new Board();
    Piece** board = chessboard->getBoard();
    std::vector<Coordinate> moves;
    emptyBoard(board);

    // Create midgame board.
    board[0][5].UpdatePiece(4, 0, 5, 0, 5, moves, 1);
    board[0][6].UpdatePiece(6, 0, 9, 0, 6, moves, 1); // King

    board[1][2].UpdatePiece(5, 0, 8, 1, 2, moves, 1);

    board[2][0].UpdatePiece(1, 0, 1, 2, 0, moves, 1);
    board[2][5].UpdatePiece(1, 0, 1, 2, 5, moves, 1);
    board[2][7].UpdatePiece(1, 0, 1, 2, 7, moves, 1);

    board[3][1].UpdatePiece(1, 0, 1, 3, 1, moves, 1);
    board[3][2].UpdatePiece(1, 0, 1, 3, 2, moves, 1);
    board[3][4].UpdatePiece(2, 0, 3, 3, 4, moves, 1);
    board[3][5].UpdatePiece(5, 1, 8, 3, 5, moves, 1);
    board[3][6].UpdatePiece(1, 0, 1, 3, 6, moves, 1);

    board[4][5].UpdatePiece(1, 0, 1, 4, 5, moves, 1);

    board[5][2].UpdatePiece(1, 1, 1, 5, 2, moves, 1);
    board[5][5].UpdatePiece(2, 1, 3, 5, 5, moves, 1);
    board[5][7].UpdatePiece(1, 1, 1, 5, 7, moves, 1);

    board[6][0].UpdatePiece(1, 1, 1, 6, 0, moves, 1);
    board[6][1].UpdatePiece(1, 1, 1, 6, 1, moves, 1);
    board[6][3].UpdatePiece(4, 0, 5, 6, 3, moves, 1);
    board[6][5].UpdatePiece(1, 1, 1, 6, 5, moves, 1);
    board[6][6].UpdatePiece(1, 1, 1, 6, 6, moves, 1);

    board[7][0].UpdatePiece(4, 1, 5, 7, 0, moves, 1);
    board[7][4].UpdatePiece(4, 1, 5, 7, 4, moves, 1);
    board[7][6].UpdatePiece(6, 1, 9, 7, 6, moves, 1); // King

    Coordinate bkpos = {0,6};
    Coordinate wkpos = {7,6};
    chessboard->updateKingPos(bkpos, 0);
    chessboard->updateKingPos(wkpos, 1);

    std::tuple<Coordinate,Coordinate, int> selected_move = findBestMove(chessboard, board, 4, 1);
    Move* move = new Move(std::get<0>(selected_move).y, std::get<0>(selected_move).x, std::get<1>(selected_move).y, std::get<1>(selected_move).x);
    move->applyMove(chessboard, 0, 1);
    delete move;
    delete chessboard;
    printf("Test 'Test Number of Nodes' was successful.\n");
}

void test_ai_vs_ai(){
    Board* chessboard = new Board();
    // The infinite game loop until checkmate is reached.
    int side = 1;
    Piece** board = chessboard->getBoard();
    int depth = 3;

    int x_start, y_start, x_end, y_end;
    int promotion = 0;

    std::cin.clear();
    while (true){
        sleep(1);
        chessboard->printBoard();

        // White plays.
        promotion = 0;
        x_start = 0;
        y_start = 0;
        x_end = 0;
        y_end = 0;

        if (side == 1){
            std::tuple <Coordinate, Coordinate, int> result = findBestMove(chessboard, board, depth, side);

            if (std::get<2>(result) == std::numeric_limits<int>::min()+1){
                std::cout<< u8"\033[2J\033[1;1H";
                printf("Checkmate!\n");
                return;
            }

            y_start = std::get<0>(result).y;
            x_start = std::get<0>(result).x;
            y_end = std::get<1>(result).y;
            x_end = std::get<1>(result).x;

            // Currently AI only promotes to Queen.
            promotion = 5;
        }
        else{
            std::tuple <Coordinate, Coordinate, int> result = findBestMove(chessboard, board, depth, side);

            if (std::get<2>(result) == std::numeric_limits<int>::min()+1){
                std::cout<< u8"\033[2J\033[1;1H";
                printf("Checkmate!\n");
                return;
            }

            y_start = std::get<0>(result).y;
            x_start = std::get<0>(result).x;
            y_end = std::get<1>(result).y;
            x_end = std::get<1>(result).x;

            // Currently AI only promotes to Queen.
            promotion = 5;
        }
        Move* move = new Move(y_start, x_start, y_end, x_end);

        // If illegal move, redo it.
        if (!move->applyMove(chessboard, promotion, side)){
            delete move;
            continue;
        }
        delete move;

        // Switch the side.
        if (side == 1){
            side = 0;
        }
        else{
            side = 1;
        }
    }
}

int main(int argc, char* argv[]) {
    // Run all tests.
    if (argc == 1){
        printf("-------------------------------\n");
        test_undo_moves();
        ai_test_capture_piece();
        ai_test_capture_best_piece();
        ai_test_no_heuristic_gain();
        ai_test_checks_king();
        ai_test_number_of_nodes();
        printf("-------------------------------\n");
    }
    else{
        test_ai_vs_ai();
    }
    return 0;
}