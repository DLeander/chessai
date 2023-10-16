#include <iostream>
#include <tuple>
#include <limits>
#include <unistd.h>
#include "headers/Piece.h"
#include "headers/Move.h"
#include "headers/Board.h"
#include "headers/Ai.h"

// Simple game loop to play the game.
void GameLoop(Board* chessboard, int level){
    // The infinite game loop until checkmate is reached.
    int side = 1;
    Piece** board = chessboard->getBoard();
    int depth;
    std::cout << "Choose the search depth (3 works just fine): ";
    std::cin >> depth;

    int x_start, y_start, x_end, y_end;
    int promotion = 0;

    std::cin.clear();
    while (true){
        chessboard->printBoard();

        promotion = 0;
        x_start = 0;
        y_start = 0;
        x_end = 0;
        y_end = 0;

        if (side == 1){
            printf("Playing as white.\n");
            std::cout << "Select piece (y x): ";
            // If non interger is entered.
            if (!(std::cin >> y_start >> x_start)) {
                // Input failed, clear error state and consume the bad input.
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            
            // If wrong move is entered.
            if (y_start > 7 || x_start > 7 || y_start < 0 || x_start < 0){
                continue;
            }

            // Show the available moves to the player.
            chessboard->showAvailableMoves(y_start, x_start);

            std::cout << "Select destination (y x): ";
            // If non interger is entered.
            if (!(std::cin >> y_end >> x_end)) {
                // Input failed, clear error state and consume the bad input.
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            // If wrong move is entered.
            if (y_end > 7 || x_end > 7 || y_end < 0 || x_end < 0){
                continue;
            }

            // If pawn is moving to top/bottom of board, ask what to promote it to.
            if (board[y_start][x_start].getType() == 1 && (y_end == 0 || y_end == 7)){
                std::cout << "Promote Pawn (2,3,4,5): ";
                std::cin >> promotion;
            }
        }
        else{
            printf("Playing as black.\n");
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

int main() {
    Board* board = new Board();
    GameLoop(board, 0);
    delete board;
    return 0;
}