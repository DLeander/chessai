#include <iostream>
#include<tuple>
#include "headers/Piece.h"
#include "headers/Move.h"
#include "headers/Board.h"
#include "headers/Ai.h"

// Returns true if side has no possible moves.
bool isCheckmate(Piece** board, int side, Coordinate wk, Coordinate bk){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board[i][j].getSide() == side){
                board[i][j].getAvailableMoves(board, wk, bk);
                // printf("ola?\n");
                std::vector<Coordinate> moves = board[i][j].getMoves();
                if (moves.size() > 0){
                    return false;
                }
            }
        }
    }
    return true;
}

void GameLoop(Board* chessboard, int level){
    // The infinite game loop until checkmate is reached.
    int side = 1;
    Piece** board = chessboard->getBoard();

    while (true){
        // if (!isCheckmate(board, side, chessboard->getwKingPos(), chessboard->getbKingPos())){
        //     std::cout<< u8"\033[2J\033[1;1H";
        //     printf("\nCheckmate. Side %d won.\n", side);
        //     break;
        // }
        chessboard->printBoard();

        // White plays.
        int x_start, y_start, x_end, y_end;
        int promotion = 0;
        if (side == 1){
            printf("Playing as white.\n");
            std::cout << "Select piece (y x): ";
            std::cin >> y_start >> x_start;
            
            chessboard->showAvailableMoves(y_start, x_start);

            std::cout << "Select destination (y x): ";
            std::cin >> y_end >> x_end;

            // If pawn is moving to top/bottom of board, ask what to promote it to.
            if (board[y_start][x_start].getType() == 1 && (y_end == 0 || y_end == 7)){
                std::cout << "Promote Pawn (2,3,4,5): ";
                std::cin >> promotion;
            }
        }
        else{
            printf("Playing as black.\n");
            std::tuple <Coordinate, Coordinate, int> result = findBestMove(chessboard, board, 3, 0);
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

int main() {
    Board* board = new Board();
    GameLoop(board, 0);
    delete board;
    return 0;
}