#include <iostream>
#include "headers/Piece.h"
#include "headers/Move.h"
#include "headers/Board.h"

void GameLoop(Board* chessboard, int level){
    // The infinite game loop until game is quit (By pressing q?).
    while (true){
        chessboard->printBoard();
        int x_start, y_start, x_end, y_end;
        std::cout << "Select piece (y x): ";
        std::cin >> y_start >> x_start;
        
        chessboard->showAvailableMoves(y_start, x_start);

        std::cout << "Select destination (y x): ";
        std::cin >> y_end >> x_end;

        Piece** board = chessboard->getBoard();
        // board[y_start][x_start].getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
        Move* move = new Move(y_start, x_start, y_end, x_end);

        // If illegal move, redo it.
        if (!move->applyMove(chessboard, 0)){
            printf("olabero\n");
            delete move;
            continue;
        }
        delete move;
    }

    // return;
}

int main() {
    Board* board = new Board();
    GameLoop(board, 0);
    // board->showAvailableMoves(4, 6);
    // board->printBoard();

    // Move* move1 = new Move(0, 1, 4, 0);
    // Move* move2 = new Move(0, 2, 4, 1);
    // Move* move3 = new Move(0, 3, 4, 2);
    // Move* move4 = new Move(0, 5, 4, 3);
    // Move* move5 = new Move(0, 6, 4, 4);
    // Move* move6 = new Move(6, 7, 5, 1);
   
    // move1->applyMove(board, 0);
    // move2->applyMove(board, 0);
    // move3->applyMove(board, 0);
    // move4->applyMove(board, 0);
    // move5->applyMove(board, 0);
    // move6->applyMove(board, 0);
    // move4->applyMove(board, 5);
    // move4->applyMove(board);
    // move5->applyMove(board);

    // Move* move7 = new Move(0, 7, 0, 4);
    // move7->applyMove(board, 0);

    // board->showAvailableMoves(0, 7);
    // board->showAvailableMoves(0, 0);

    // board->printBoard();

    // Move* move3 = new Move(3, 2, 2, 3);

    // move3->applyMove(board, 0);
    // board->printBoard();
    // printf("THE TYPE: %d\n", board->getBoard()[5][4].getType());
    
    // Move* move2 = new Move(5-1, 5-1, 2-1, 6-1);
    // move2->applyMove(board);
    // board->printBoard();

    delete board;
    return 0;
}