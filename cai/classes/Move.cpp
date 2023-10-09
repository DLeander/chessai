// Includes
#include <iostream>
// #include <vector>
#include "../headers/Move.h"
#include "../headers/Board.h"
#include "../headers/Piece.h"

// Create the move.
Move::Move(int ystart, int xstart, int yend, int xend){
    this->xstart = xstart;
    this->xend = xend;
    this->ystart = ystart;
    this->yend = yend;
}

// Applies the move on the board.
bool Move::applyMove(Board* chessboard, int transformTo){
    Piece** board = chessboard->getBoard();

    // Translate strings to correct row and column
    int start_row = this->ystart;
    int start_col = this->xstart;
    int end_row = this->yend;
    int end_col = this->xend;

    // Get the piece thats to be moved.
    Piece piece = board[start_row][start_col];

    // Collect information about piece before it is removed from the board.
    int type = piece.getType();
    bool side = piece.getSide();
    int value = piece.getValue();
    int xpos = piece.getXpos();
    int ypos = piece.getYpos();
    int totMoves = piece.getTotMoves();
    piece.getAvailableMoves(board, chessboard->getwKingPos(), chessboard->getbKingPos());
    std::vector<Coordinate> moves = piece.getMoves();
    // Go through possible moves for the piece and make sure that the move to be made is there.
    if (!isValidMove(moves, end_row, end_col)){
        return false;
    }

    // If move is en passant
    if (type == 1 && isEnPassantMove(board, piece, start_row, start_col, end_row, end_col)){
        // Add one score, since its a pawn that gets captured.
        chessboard->setScore(side, 1);
        
        // Update old piece (Remove piece from that tile '-').
        board[start_row][start_col].UpdatePiece();

        // Place Piece on moved tile.
        board[end_row][end_col].UpdatePiece(type, side, value, end_row, end_col, moves, totMoves++);

        // Remove piece that gets captured due to en passant
        board[start_row][end_col].UpdatePiece();
    }
    else if (isCastleMove(board, piece, start_row, start_col, end_row, end_col)){
        Piece king = board[end_row][end_col];
        int ktype = king.getType();
        bool kside = king.getSide();
        int kvalue = king.getValue();
        int kxpos = king.getXpos();
        int kypos = king.getYpos();
        int ktotMoves = king.getTotMoves();
        std::vector<Coordinate> kmoves = king.getMoves();
        
        // Remove piece from last two positions.
        board[start_row][start_col].UpdatePiece();
        board[end_row][end_col].UpdatePiece();
        if (start_col == 7){
            board[end_row][end_col+1].UpdatePiece(type, side, value, end_row, end_col+1, moves, totMoves++);
            board[end_row][end_col+2].UpdatePiece(ktype, kside, kvalue, end_row, end_col+2, kmoves, ktotMoves++);
            Coordinate pos;
            pos.x = end_col+2;
            pos.y = end_row;
            chessboard->updateKingPos(pos, side);
        }
        else{
            board[end_row][end_col-1].UpdatePiece(type, side, value, end_row, end_col-1, moves, totMoves++);
            board[end_row][end_col-2].UpdatePiece(ktype, kside, kvalue, end_row, end_col-2, kmoves, ktotMoves++);
            Coordinate pos;
            pos.x = end_col-2;
            pos.y = end_row;
            chessboard->updateKingPos(pos, side);
        }
    }
    else{
        // If piece exists on moved tile, add score to the current team.
        // The piece at the spot to be moved (If there is one)
        Piece end_piece = board[end_row][end_col];
        int score = end_piece.getValue();
        chessboard->setScore(side, score);

        // If king, update king pos in chessboard. (This is needed to looks for checks.)
        if (piece.getType() == 6){
            Coordinate pos;
            pos.x = end_col;
            pos.y = end_row;
            chessboard->updateKingPos(pos, side);
        }

        // Update old piece (Remove piece from that tile '-').
        board[start_row][start_col].UpdatePiece();

        // Place Piece on moved tile.
        // If pawn at end of board, transform it into a transformTo piece.
        if (type == 1 && (end_row == 0 || end_row == 7) && (transformTo >= 2 && transformTo <= 5)){
            switch (transformTo){
                case 2:
                    board[end_row][end_col].UpdatePiece(transformTo, side, 3, end_row, end_col, moves, totMoves++);
                    break;
                case 3:
                    board[end_row][end_col].UpdatePiece(transformTo, side, 3, end_row, end_col, moves, totMoves++);
                    break;
                case 4:
                    board[end_row][end_col].UpdatePiece(transformTo, side, 5, end_row, end_col, moves, totMoves++);
                    break;
                case 5:
                    board[end_row][end_col].UpdatePiece(transformTo, side, 8, end_row, end_col, moves, totMoves++);
                    break;
            }
            
        }
        else{
            // Place Piece on moved tile.
            board[end_row][end_col].UpdatePiece(type, side, value, end_row, end_col, moves, totMoves++);
        }
    }
    return true;
}

// Make sure that move to be applied is indeed en passant.
bool Move::isEnPassantMove(Piece** board, Piece piece, int starty, int startx, int endy, int endx){
    if (piece.getSide() == 1){
        if (starty-1 >= 0 && !(board[starty-1][endx].isPiece()) && board[starty][endx].getType() == 1 && board[starty][endx].getSide() != piece.getSide()){
            return true;
        }
    }
    else{
        if (starty+1 < 8 && !(board[starty+1][endx].isPiece()) && board[starty][endx].getType() == 1 && board[starty][endx].getSide() != piece.getSide()){
            return true;
        }
    }

    return false;
}

// Neither the king nor the rook has previously moved.
// There are no pieces between the king and the rook.
// The king is not currently in check.
// The king does not pass through or finish on a square that is attacked by an enemy piece.
bool Move::isCastleMove(Piece** board, Piece piece, int starty, int startx, int endy, int endx){
    // If it is an available move and the current piece is a rook, with the destination being a tower, then
    // castle.
    if (board[endy][endx].getType() == 6 && board[starty][endy].getType() == 4){
        return true;
    }
    return false;
}

// Return true if move is valid, otherwise false.
bool Move::isValidMove(std::vector<Coordinate> moves, int end_row, int end_col){
    if (moves.size() == 0){
        return false;
    }
    bool valid = true;
    for (int i = 0; i < moves.size(); i++){
        if (moves[i].x == end_col && moves[i].y == end_row){
            valid = true;
            break;
        }
        // printf("MOVE X:%d Y:%d\nINPUT X:%d Y:%d\n", moves[i].x, moves[i].y, end_col, end_row);
        valid = false;
    }
    for (Coordinate move : moves){

    }
    return valid;
}

// Examine if the move causes a check for the opposite side.
bool isCheckmate(){
    return false;
}

bool Move::incorrectSideMove(Piece start_piece, bool side){
    return (start_piece.getSide() == side);
}