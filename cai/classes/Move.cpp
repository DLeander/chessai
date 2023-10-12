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

// Undo a move.
void Move::undoMove(Board* chessboard, Piece selectedPiece, Piece capturedPiece){
    // Get the board.
    Piece** board = chessboard->getBoard();
    // Since move is reversed, start and end locations are switched.
    int y_start = this->yend;
    int x_start = this->xend;
    int y_end   = this->ystart;
    int x_end   = this->xstart;
    
    // Check if en passant move: Pawn moved away from column to empty an tile.
    if (selectedPiece.getType() == 1 && x_start-x_end != 0 && capturedPiece.getType() == -1){
        if (selectedPiece.getSide() == 0){
            // Set empty tile where pawned moved.
            board[y_start][x_start].UpdatePiece();
            // Set pawn to starting location.
            board[y_end][x_end].UpdatePiece(selectedPiece.getType(), selectedPiece.getSide(), selectedPiece.getValue(), y_end, x_end, selectedPiece.getMoves(), selectedPiece.getTotMoves()-1);
            // Set captured pawn to starting location.
            // Setting total moves to 0 here doesn't matter, since its only used to check castle moves. (And this is a pawn)
            board[y_start-1][x_start].UpdatePiece(1, 1, 1, y_start-1, x_start, capturedPiece.getMoves(), 0);
            chessboard->setMaterial(1, -1);
        }
        else{
            // Set empty tile where pawned moved.
            board[y_start][x_start].UpdatePiece();
            // Set pawn to starting location.
            board[y_end][x_end].UpdatePiece(selectedPiece.getType(), selectedPiece.getSide(), selectedPiece.getValue(), y_end, x_end, selectedPiece.getMoves(), selectedPiece.getTotMoves() - 1);
            // Set captured pawn to starting location.
            board[y_start+1][x_start].UpdatePiece(1, 0, 1, y_start+1, x_start, capturedPiece.getMoves(), 0);
            chessboard->setMaterial(0, -1);
            // board[y_start+1][x_start].UpdatePiece(capturedPiece.getType(), capturedPiece.getSide(), capturedPiece.getValue(), y_start+1, x_start, capturedPiece.getMoves(), capturedPiece.getTotMoves() - 1);
        }
        return;
    }
    
    // Check if castle move. (If the captured piece is a king, and the selected piece is a rook, and they are on the same side, then it is a castle move.)
    if ((capturedPiece.getType() == 6 && selectedPiece.getType() == 4) && (capturedPiece.getSide() == selectedPiece.getSide())){
        // Check if right or left side castle.
        // Right side. (x and y reversed, therefore x_end is starting position of rook.)
        if (x_end == 7){
            // Remove old pieces.
            board[y_end][x_end-1].UpdatePiece();
            board[y_end][x_end-2].UpdatePiece();
            // Rook
            board[y_end][x_end].UpdatePiece(selectedPiece.getType(), selectedPiece.getSide(), selectedPiece.getValue(), y_end, x_end, selectedPiece.getMoves(), selectedPiece.getTotMoves() - 1);
            // King
            board[y_end][x_end-3].UpdatePiece(capturedPiece.getType(), capturedPiece.getSide(), capturedPiece.getValue(), y_end, x_end-3, capturedPiece.getMoves(), capturedPiece.getTotMoves() - 1);
            // Update kingpos
            Coordinate pos;
            pos.x = x_end-3;
            pos.y = y_end;
            chessboard->updateKingPos(pos, selectedPiece.getSide());
        }
        // Left side.
        else if (x_end == 0){
            // Remove old pieces.
            board[y_end][x_end+3].UpdatePiece();
            board[y_end][x_end+2].UpdatePiece();
            // Rook
            board[y_end][x_end].UpdatePiece(selectedPiece.getType(), selectedPiece.getSide(), selectedPiece.getValue(), y_end, x_end, selectedPiece.getMoves(), selectedPiece.getTotMoves() - 1);
            // King
            board[y_end][x_end+4].UpdatePiece(capturedPiece.getType(), capturedPiece.getSide(), capturedPiece.getValue(), y_end, x_end+4, capturedPiece.getMoves(), capturedPiece.getTotMoves() - 1);
            // Update kingpos.
            Coordinate pos;
            pos.x = x_end+4;
            pos.y = y_end;
            chessboard->updateKingPos(pos, selectedPiece.getSide());
        }
        return;
    }

    // If not castle or en passant move: Undo normal move. Transformation moves here are fine, since the value of the piece before the transformation occurs is saved, not the transformed piece.
    // Given selectedPiece and capturedPiece, reverse the move.
    if (capturedPiece.getType() == -1){
        board[y_end][x_end].UpdatePiece(selectedPiece.getType(), selectedPiece.getSide(), selectedPiece.getValue(), y_end, x_end, selectedPiece.getMoves(), selectedPiece.getTotMoves() - 1);
        board[y_start][x_start].UpdatePiece();
    }
    else{
        board[y_end][x_end].UpdatePiece(selectedPiece.getType(), selectedPiece.getSide(), selectedPiece.getValue(), y_end, x_end, selectedPiece.getMoves(), selectedPiece.getTotMoves() - 1);
        board[y_start][x_start].UpdatePiece(capturedPiece.getType(), capturedPiece.getSide(), capturedPiece.getValue(), y_start, x_start, capturedPiece.getMoves(), capturedPiece.getTotMoves() - 1);
        // Negate the value to undo the removal of material.
        chessboard->setMaterial(capturedPiece.getSide(), -capturedPiece.getValue());
    }

    // If the king moved, reset kingpos.
    if (selectedPiece.getType() == 6){
        Coordinate pos;
        pos.x = x_end;
        pos.y = y_end;
        chessboard->updateKingPos(pos, selectedPiece.getSide());
    }
}

// Applies the move on the board.
bool Move::applyMove(Board* chessboard, int transformTo, int currentSide){
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

    // If piece not on the current side. Return false.
    if (side != currentSide){
        return false;
    }
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
        Piece pieceRemove = board[start_row][end_col];
        chessboard->setMaterial(pieceRemove.getSide(), pieceRemove.getValue());
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
        chessboard->setMaterial(end_piece.getSide(), score);

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
    if (board[endy][endx].getType() == 6 && board[starty][startx].getType() == 4 && board[endy][endx].getTotMoves() == 0 && board[starty][startx].getTotMoves() == 0){
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