// Includes
#include <iostream>
#include <vector>
// #include <string>
#include "../headers/Piece.h"

// Constructor (Empty tile on chessboard)
Piece::Piece() {
    this->type        = -1;
    this->side        = -1;
    this->value       = -1;
    this->xpos        = -1;
    this->ypos        = -1;
    this->totMoves    = 0;
    printEmptyTile();
}

// Update tiles on chessboard to piece.
void Piece::UpdatePiece(int type, int side, int value, int ypos, int xpos, std::vector<Coordinate> moves, int totMoves) {
    this->type  = type;
    this->side  = side;
    this->value = value;
    this->xpos  = xpos;
    this->ypos  = ypos;
    this->moves = moves;
    this->totMoves = totMoves;
}

// Overloaded UpdatePiece method. Used for empty tiles.
void Piece::UpdatePiece() {
    this->type        = -1;
    this->side        = -1;
    this->value       = -1;
    this->xpos        = -1;
    this->ypos        = -1;
    this->totMoves    = 0;
    printEmptyTile();
}

void Piece::UpdatePos(int xpos, int ypos){
    this->xpos = xpos;
    this->ypos = ypos;
}

// True if piece, false if not piece.
bool Piece::isPiece(){
    return(this->type >= 0);
}

// Print piece information (For debugging).
void Piece::printPieceInfo() {
    printf("-----------------------------\n");
    switch (this->type){
        case 1:
            printf("Type: Pawn\nSide: %d\nValue: %d\nXpos: %d\nYpos: %d\n", this->side, this->value, this->xpos, this->ypos);
            break;
        case 2:
            printf("Type: Bishop\nSide: %d\nValue: %d\nXpos: %d\nYpos: %d\n", this->side, this->value, this->xpos, this->ypos);
        case 3:
            printf("Type: Knight\nSide: %d\nValue: %d\nXpos: %d\nYpos: %d\n", this->side, this->value, this->xpos, this->ypos);
            break;
        case 4:
            printf("Type: Rook\nSide: %d\nValue: %d\nXpos: %d\nYpos: %d\n", this->side, this->value, this->xpos, this->ypos);
            break;
        case 5:
            printf("Type: Queen\nSide: %d\nValue: %d\nXpos: %d\nYpos: %d\n", this->side, this->value, this->xpos, this->ypos);
            break;
        case 6:
            printf("Type: King\nSide: %d\nValue: %d\nXpos: %d\nYpos: %d\n", this->side, this->value, this->xpos, this->ypos);
            break;
    }
    printf("-----------------------------\n");
}

// Print the current piece
void Piece::printPiece(){
    if (this->side){
        printf(" W%d ", this->type);
    }
    else{
        printf(" B%d ", this->type);
    }
}

// Get the current moves for a specific piece.
void Piece::getAvailableMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    // Clear the current moves, since last turns move could have affected it.
    this->moves.clear();
    switch (this->type){
        case 1:
            pawnMoves(board, wKingPos, bKingPos);
            break;
        case 2:
            bishopMoves(board, wKingPos, bKingPos);
            break;
        case 3:
            knightMoves(board, wKingPos, bKingPos);
            break;
        case 4:
            rookMoves(board, wKingPos, bKingPos);
            break;
        case 5:
            queenMoves(board, wKingPos, bKingPos);
            break;
        case 6:
            kingMoves(board, wKingPos, bKingPos);
            break;
    }
}

// Helper functions for showAvailableMoves that prints the path for the current piece.
void Piece::pawnMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    // The current x and y coordinate.
    int x = this->xpos;
    int y = this->ypos;

    // If white side use, move upwards.
    if (this->side == 1){
        y--;
        // If outside board, do nothing and return.
        if (y < 0){
            return;
        }

        // A pawn can move to at most 3 positions at any time.
        for (int col = x-1; col < x+2; col++){
            // If move is outside the board, continue.
            if (col < 0 || col > 7){
                continue;
            }
            // Look for en passant move.
            if (col != x && !(board[y][col].isPiece()) && board[y+1][col].getType() == 1 && board[y+1][col].getSide() != this->side){
                Coordinate cord;
                cord.x = col;
                cord.y = y;
                if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                    // Piece can perform en passant
                    this->moves.push_back(cord);
                }
                continue;
            }
            // If there is an enemy piece to the diagnonal right/left, then add it as a move (Pawns captures sideways).
            if ( (col != x) && (board[y][col].isPiece()) && (board[y][col].getSide() != this->side) ){
                Coordinate cord;
                cord.x = col;
                cord.y = y;
                if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                    this->moves.push_back(cord);
                }
                continue;
            }
            else if (col == x){
                // Add the move if no piece is blocking.
                if (board[y][x].isPiece()){
                    continue;
                }
                else{
                    // If piece in starting position, push that move aswell.
                    if (y+1 == 6){
                        if (!(board[y-1][x].isPiece())){
                            Coordinate cordEx;
                            cordEx.x = col;
                            cordEx.y = y-1;
                            if (!isCheckedTiles(board, cordEx, wKingPos, bKingPos)){
                                this->moves.push_back(cordEx);
                            }
                        }
                    }
                    // Push move.
                    Coordinate cord;
                    cord.x = x;
                    cord.y = y;
                    if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                        this->moves.push_back(cord);
                    }
                }
            }

        }
    }
    // Else move downwards.
    else if (side == 0){
        y++;
        if (y > 7){
            return;
        }

        // A pawn can move to at most 3 positions at any time.
        for (int col = x-1; col < x+2; col++){
            // If move is outside the board, continue.
            if (col < 0 || col > 7){
                continue;
            }

            // Look for en passant move.
            if (col != x && !(board[y][col].isPiece()) && board[y-1][col].getType() == 1 && board[y-1][col].getSide() != this->side){
                Coordinate cord;
                cord.x = col;
                cord.y = y;
                if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                    // Piece can perform en passant
                    this->moves.push_back(cord);
                }
                continue;
            }

            // If there is an enemy piece to the diagnonal right/left, then add it as a move (Pawns captures sideways).
            if ( (col != x) && (board[y][col].isPiece()) && (board[y][col].getSide() != this->side) ){
                Coordinate cord;
                cord.x = col;
                cord.y = y;
                if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                    this->moves.push_back(cord);
                }
                continue;
            }
            else if (col == x){
                // Add the move if no piece is blocking.
                if (board[y][x].isPiece()){
                    continue;
                }
                else{
                    // If piece in starting position, push that move aswell.
                    if (y-1 == 1){
                        if (!(board[y+1][x].isPiece())){
                            Coordinate cordEx;
                            cordEx.x = col;
                            cordEx.y = y+1;
                            if (!isCheckedTiles(board, cordEx, wKingPos, bKingPos)){
                                this->moves.push_back(cordEx);
                            }
                        }
                    }
                    // Push move.
                    Coordinate cord;
                    cord.x = x;
                    cord.y = y;
                    if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                        this->moves.push_back(cord);
                    }
                }
            }

        }
    }
}

void Piece::bishopMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    // A Bishop can move to any tile on either diagonal.

    int x = this->xpos;
    int y = this->ypos;

    // At most 4 possible diagonals.
    // Upper Left.
    for (int ul = 1; ul <= x; ul++){
        // Move outside board, no more moves in the diagonal.
        if (y - ul < 0){
            break;
        }
        int x_ = x-ul;
        int y_ = y-ul;

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
    // Upper right.
    for (int ur = 1; ur <= 7-x; ur++){
        // Move outside board, no more moves in the diagonal.
        if (y - ur < 0){
            break;
        }
        int x_ = x+ur;
        int y_ = y-ur;

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
    // Lower left.
    for (int ll = 1; ll <= x; ll++){
        // Move outside board, no more moves in the diagonal.
        if (y + ll > 7){
            break;
        }
        int x_ = x-ll;
        int y_ = y+ll;

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
        
    }
    // Lower right.
    for (int lr = 1; lr <= 7-x; lr++){
        // Move outside board, no more moves in the diagonal.
        if (y + lr > 7){
            break;
        }
        int x_ = x+lr;
        int y_ = y+lr;

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
}

void Piece::knightMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    int x = this->xpos;
    int y = this->ypos;

    // Vector for all possible moves for the knight.
    std::vector<Coordinate> posMoves;

    // At most 8 possible moves. A Knight can move over tiles with pieces on them.
    Coordinate cord1;
    Coordinate cord2;
    Coordinate cord3;
    Coordinate cord4;
    Coordinate cord5;
    Coordinate cord6;
    Coordinate cord7;
    Coordinate cord8;

    // The possible moves downwards on the board.
    cord1.x = x + 1;
    cord1.y = y + 2;
    cord2.x = x + 2;
    cord2.y = y + 1;
    cord3.x = x - 1;
    cord3.y = y + 2;
    cord4.x = x - 2;
    cord4.y = y + 1;

    // The possible moves upwards on the board.
    cord5.x = x + 1;
    cord5.y = y - 2;
    cord6.x = x + 2;
    cord6.y = y - 1;
    cord7.x = x - 1;
    cord7.y = y - 2;
    cord8.x = x - 2;
    cord8.y = y - 1;

    // Add the moves to the posMove vector.
    posMoves.push_back(cord1);
    posMoves.push_back(cord2);
    posMoves.push_back(cord3);
    posMoves.push_back(cord4);
    posMoves.push_back(cord5);
    posMoves.push_back(cord6);
    posMoves.push_back(cord7);
    posMoves.push_back(cord8);

    // Go through all moves and add those that do not break the bounds or have a piece of the same side on the tile.
    for (Coordinate posMove : posMoves){
        // printf("MOVE X: %d\nMOVE Y: %d\n", posMove.x, posMove.y);
        if (((posMove.x <= 7 && posMove.x >= 0) && (posMove.y <= 7 && posMove.y >= 0))){
            if (board[posMove.y][posMove.x].getSide() != -1){
                if ((board[posMove.y][posMove.x].getSide() != this->side)){
                    if (!isCheckedTiles(board, posMove, wKingPos, bKingPos)){
                        this->moves.push_back(posMove);
                    }
                }
            }
            else{
                if (!isCheckedTiles(board, posMove, wKingPos, bKingPos)){
                    this->moves.push_back(posMove);
                }
            }
            
        }
    }
}

void Piece::rookMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    // Print path of a Rook.
    // A Rook moves orthogonally.

    int x = this->xpos;
    int y = this->ypos;


    // Check for castle move to the left.
    if (this->totMoves == 0 && !board[y][x-1].isPiece() && !board[y][x-2].isPiece() && board[y][x-3].getType() == 6 && board[y][x-3].getTotMoves() == 0){
        printf("hej\n");
        Coordinate pos1;
        Coordinate pos2;
        pos1.x = x-1;
        pos1.y = this->side*7;
        pos2.x = x-2;
        pos2.y = this->side*7;

        Coordinate cord;
        cord.x = x;
        cord.y = y;
        if (!isCheckedTiles(board, cord, wKingPos, bKingPos) && !isCheckedTiles(board, cord, pos1, pos1) && !isCheckedTiles(board, cord, pos2, pos2)){
            if (this->side == 1){
                printf("olabero\n");
                this->moves.push_back(wKingPos);
            }
            else if (this->side == 0){
                this->moves.push_back(bKingPos);
            }
            
        }
    }
    // Check for castle move to the right.
    if (this->totMoves == 0 && !board[y][x+1].isPiece() && !board[y][x+2].isPiece() && !board[y][x+3].isPiece() && board[y][x+4].getType() == 6 && board[y][x+4].getTotMoves() == 0){
        Coordinate pos1;
        Coordinate pos2;
        Coordinate pos3;
        pos1.x = x+1;
        pos1.y = this->side*7;
        pos2.x = x+2;
        pos2.y = this->side*7;
        pos3.x = x+3;
        pos3.y = this->side*7;

        Coordinate cord;
        cord.x = x;
        cord.y = y;
        if (!isCheckedTiles(board, cord, wKingPos, bKingPos) && !isCheckedTiles(board, cord, pos1, pos1) && !isCheckedTiles(board, cord, pos2, pos2) && !isCheckedTiles(board, cord, pos3, pos3)){
            if (this->side == 1){
                this->moves.push_back(wKingPos);
            }
            else if (this->side == 0){
                this->moves.push_back(bKingPos);
            }
            
        }
    }

    // Up
    for (int u = 1; u <= y; u++){
        int y_ = y - u;
        int x_ = x;

        if (y_ < 0){
            break;
        }

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
    // Down
    for (int d = 1; d < 8 - y; d++){
        int y_ = y + d;
        int x_ = x;

        if (y_ > 7){
            break;
        }

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
    // Left
    for (int l = 1; l <= x; l++){
        int y_ = y;
        int x_ = x - l;

        if (x_ < 0){
            break;
        }

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
    // Right
    for (int l = 1; l < 8-x; l++){
        int y_ = y;
        int x_ = x + l;

        if (x_ > 7){
            break;
        }

        // If piece is blocked, dont add the move and remaining ones.
        if (board[y_][x_].isPiece() && board[y_][x_].getSide() == this->side){
            break;
        }

        Coordinate cord;
        cord.x = x_;
        cord.y = y_;

        if (board[y_][x_].getSide() == -1){
            // Add the move.
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            continue;
        }
        // If enemy piece, add it and dont add remaining ones.
        else if (board[y_][x_].getSide() != this->side){
            if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                this->moves.push_back(cord);
            }
            break;
        }
    }
    
}

void Piece::queenMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    // Print path of the Queen.
    
    // Call pathfinder for Bishop and Rook (The Queen moves both diagonally and orthogonally)
    bishopMoves(board, wKingPos, bKingPos);
    rookMoves(board, wKingPos, bKingPos);
}

void Piece::kingMoves(Piece** board, Coordinate wKingPos, Coordinate bKingPos){
    // Print path of the King.

    // The King can move one tile in any direction.

    int x = this->xpos;
    int y = this->ypos;

    // Go through a 3x3 grid and find available tiles.
    for (int row = y-1; row < y+2; row++){
        // If outside of board, continue.
        if (row < 0 || row > 7){
            continue;
        }
        
        // Go trough each column, check if tile available.
        for (int col = x-1; col < x+2; col++){
            // If outside of board, break.
            if (col < 0 || col > 7){
                break;;
            }

            Coordinate cord;
            cord.x = col;
            cord.y = row;
            if (board[row][col].getSide() == -1){
                // Add the move.
                if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                    this->moves.push_back(cord);
                }
            }
            // If enemy piece, add it.
            else if (board[row][col].getSide() != this->side){
                if (!isCheckedTiles(board, cord, wKingPos, bKingPos)){
                    this->moves.push_back(cord);
                }
            }
        }
    }
}

// Return true if a possible move for a piece creates a check, else false.
bool Piece::isCheckedTiles(Piece** board, Coordinate possible_location, Coordinate wKingPos, Coordinate bKingPos){
    // Get king location.
    int row = possible_location.y;
    int col = possible_location.x;

    // Store old piece information, apply the move to see if king is checked later redo the move.
    int oldrow = this->ypos;
    int oldcol = this->xpos;
    int oldside = this->side;
    int oldtype = this->type;
    int oldvalue = this->value;
    int oldtotMoves = this->totMoves;
    std::vector<Coordinate> oldmoves = this->moves;

    int storedValue = board[row][col].getValue();
    int storedSide = board[row][col].getSide();
    int storedType = board[row][col].getType();
    int storedtotMoves = board[row][col].getTotMoves();
    std::vector<Coordinate> storedMoves = board[row][col].getMoves();

    board[oldrow][oldcol].UpdatePiece();
    board[row][col].UpdatePiece(oldtype, oldside, oldvalue, row, col, oldmoves, oldtotMoves);

    // If the king is the one that moves, update king pos.
    if (this->type == 6 && side == 0){
        bKingPos = possible_location;
    }
    else if (this->type == 6 && side == 1){
        wKingPos = possible_location;
    }

    int kingRow;
    int kingCol;
    if (this->side == 0){
        kingRow = bKingPos.y;
        kingCol = bKingPos.x;
    }
    else{
        kingRow = wKingPos.y;
        kingCol = wKingPos.x;
    }

    // Examine possible pawn checks (With bishop and queen at the same time).
    if (this->side == 0){
        if (kingRow+1 <= 7){
            if (kingCol+1 <= 7){
                if (board[kingRow+1][kingCol+1].getType() == 1 && board[kingRow+1][kingCol+1].getSide() != this->side){
                    board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
                    board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
                    printf("PAWN1\n");
                    return true;
                }
            }
            if (col-1 >= 0){
                if (board[kingRow+1][kingCol-1].getType() == 1 && board[kingRow+1][kingCol-1].getSide() != this->side){
                    board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
                    board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
                    printf("PAWN2\n");
                    return true;
                }
            }
        }
    }
    else{
        if (kingRow-1 >= 0){
            if (kingCol+1 <= 7){
                if (board[kingRow-1][kingCol+1].getType() == 1 && board[kingRow-1][kingCol+1].getSide() != this->side){
                    board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
                    board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
                    printf("PAWN3\n");
                    return true;
                }
            }
            if (col-1 >= 0){
                if (board[kingRow-1][kingCol-1].getType() == 1 && board[kingRow-1][kingCol-1].getSide() != this->side){
                    board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
                    board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
                    printf("PAWN4\n");
                    return true;
                }
            }
        }
    }

    // Examine diagonals (Bishop and queen).
    // Upper right
    for (int ur = 1; ur <= 7-kingCol; ur++){
        // Move outside board, no more moves in the diagonal. Shouldn't be possible given loop condition, but just in case...
        if (kingRow - ur < 0){
            break;
        }
        // If a piece on the diagonal is a piece on the same side as the king, then its safe, stop looking.
        if ((board[kingRow-ur][kingCol+ur].getSide() == this->side ) || (board[kingRow-ur][kingCol+ur].getType() == 1 || board[kingRow-ur][kingCol+ur].getType() == 3) || board[kingRow-ur][kingCol+ur].getType() == 4 || board[kingRow-ur][kingCol+ur].getType() == 6){
            break;
        }
        // If piece is queen or bishop on the diagonal, return true (checked tile.)
        if ((board[kingRow-ur][kingCol+ur].getType() == 2 && board[kingRow-ur][kingCol+ur].getSide() != this->side) || (board[kingRow-ur][kingCol+ur].getType() == 5 && board[kingRow-ur][kingCol+ur].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("TYPE: %d\nX:%d\nY:%d\n", board[kingRow-ur][kingCol+ur].getType(), kingCol+ur, kingRow-ur);
            printf("MOVING TO X:%d and Y:%d\n", col, row);
            printf("BISHOP1\n");
            return true;
        }
    }

    // Upper left
    for (int ul = 1; ul <= kingCol; ul++){
        // Move outside board, no more moves in the diagonal.
        if (kingRow - ul < 0){
            break;
        }
        // If a piece on the diagonal is a piece on the same side as the king, then its safe, stop looking.
        if ((board[kingRow-ul][kingCol-ul].getSide() == this->side) || (board[kingRow-ul][kingCol-ul].getType() == 1 || board[kingRow-ul][kingCol-ul].getType() == 3 || board[kingRow-ul][kingCol-ul].getType() == 4 || board[kingRow-ul][kingCol-ul].getType() == 6)){
            break;
        }
        // If piece is queen or bishop on the diagonal, return true (checked tile.)
        if ((board[kingRow-ul][kingCol-ul].getType() == 2 && board[kingRow-ul][kingCol-ul].getSide() != this->side) || (board[kingRow-ul][kingCol-ul].getType() == 5 && board[kingRow-ul][kingCol-ul].getSide() != this->side && board[kingRow-ul][kingCol-ul].getType() != -1)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            // printf("TYPE: %d\nX:%d\nY:%d\n", board[kingRow-ul][kingCol-ul].getType(), kingCol-ul, kingRow-ul);
            // printf("MOVING TO X:%d and Y:%d\n", col, row);
            printf("BISHOP2\n");
            return true;
        }
    }

    // // Lower right
    for (int lr = 1; lr <= 7-kingCol; lr++){
        // Move outside board, no more moves in the diagonal.
        if (kingRow + lr > 7){
            break;
        }
        // If a piece on the diagonal is a piece on the same side as the king, then its safe, stop looking.
        if ((board[kingRow+lr][kingCol+lr].getSide() == this->side) || (board[kingRow+lr][kingCol+lr].getType() == 1 || board[kingRow+lr][kingCol+lr].getType() == 3 || board[kingRow+lr][kingCol+lr].getType() == 4 || board[kingRow+lr][kingCol+lr].getType() == 6)){
            break;
        }
        // If piece is queen or bishop on the diagonal, return true (checked tile.)
        if ((board[kingRow+lr][kingCol+lr].getType() == 2 && board[kingRow+lr][kingCol+lr].getSide() != this->side) || (board[kingRow+lr][kingCol+lr].getType() == 5 && board[kingRow+lr][kingCol+lr].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("BISHOP3\n");
            return true;
        }
    }

    // Lower left
    for (int ll = 1; ll <= kingCol; ll++){
        // Move outside board, no more moves in the diagonal.
        if (kingRow + ll > 7){
            break;
        }
        // If a piece on the diagonal is a piece on the same side as the king, then its safe, stop looking.
        if ((board[kingRow+ll][kingCol-ll].getSide() == this->side) || (board[kingRow+ll][kingCol-ll].getType() == 1 || board[kingRow+ll][kingCol-ll].getType() == 3 || board[kingRow+ll][kingCol-ll].getType() == 4 || board[kingRow+ll][kingCol-ll].getType() == 6)){
            break;
        }
        // If piece is queen or bishop on the diagonal, return true (checked tile.)
        if ((board[kingRow+ll][kingCol-ll].getType() == 2 && board[kingRow+ll][kingCol-ll].getSide() != this->side) || (board[kingRow+ll][kingCol-ll].getType() == 5 && board[kingRow+ll][kingCol-ll].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("BISHOP4\n");
            return true;
        }
    }

    // Examine orthogonal
    // Up
    for (int u = kingRow-1; u >= 0; u--){
        if ((board[u][kingCol].getSide() == this->side) || (board[u][kingCol].getType() == 1 || board[u][kingCol].getType() == 2 || board[u][kingCol].getType() == 3 || board[u][kingCol].getType() == 6)){
            break;
        }
        // If queen or rook, then checked tile.
        if ((board[u][kingCol].getType() == 4 && board[u][kingCol].getSide() != this->side) || (board[u][kingCol].getType() == 5 && board[u][kingCol].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("ROOK1\n");
            return true;
        }
    }
    // Down
    for (int d = kingRow+1; d < 8; d++){
        if ((board[d][kingCol].getSide() == this->side) || (board[d][kingCol].getType() == 1 || board[d][kingCol].getType() == 2 || board[d][kingCol].getType() == 3 || board[d][kingCol].getType() == 6)){
            break;
        }
        // If queen or rook, then checked tile.
        if ((board[d][kingCol].getType() == 4 && board[d][kingCol].getSide() != this->side) || (board[d][kingCol].getType() == 5 && board[d][kingCol].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("ROOK2\n");
            return true;
        }
    }
    // Right
    for (int r = kingCol+1; r < 8; r++){
        if ((board[kingRow][r].getSide() == this->side) || (board[kingRow][r].getType() == 1 || board[kingRow][r].getType() == 2 || board[kingRow][r].getType() == 3 || board[kingRow][r].getType() == 6)){
            break;
        }
        // If queen or rook, then checked tile.
        if ((board[kingRow][r].getType() == 4 && board[kingRow][r].getSide() != this->side) || (board[kingRow][r].getType() == 5 && board[kingRow][r].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("ROOK3\n");
            return true;
        }
    }
    // Left
    for (int l = kingCol-1; l >= 0; l--){
        if ((board[kingRow][l].getSide() == this->side) || (board[kingRow][l].getType() == 1 && board[kingRow][l].getType() == 2 || board[kingRow][l].getType() == 3 || board[kingRow][l].getType() == 6)){
            break;
        }
        // If queen or rook, then checked tile.
        if ((board[kingRow][l].getType() == 4 && board[kingRow][l].getSide() != this->side) || (board[kingRow][l].getType() == 5 && board[kingRow][l].getSide() != this->side)){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("ROOK4\n");
            return true;
        }
    }

    // Examine knights.
    if (kingRow - 2 >= 0 && kingCol + 1 < 8){
        if (board[kingRow - 2][kingCol + 1].getType() == 3 && board[kingRow - 2][kingCol + 1].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT1\n");
            return true;
        }
    }
    if (kingRow - 1 >= 0 && kingCol + 2 < 8){
        if (board[kingRow - 1][kingCol + 2].getType() == 3 && board[kingRow - 1][kingCol + 2].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT2\n");
            return true;
        }
    }
    // 
    if (kingRow - 2 >= 0 && kingCol - 1 >= 0){
        if (board[kingRow - 2][kingCol - 1].getType() == 3 && board[kingRow - 2][kingCol - 1].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT3\n");
            return true;
        }
    }
    if (kingRow - 1 >= 0 && kingCol - 2 >= 0){
        if (board[kingRow - 1][kingCol - 2].getType() == 3 && board[kingRow - 1][kingCol - 2].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT4\n");
            return true;
        }
    }
    // 
    if (kingRow + 1 < 8 && kingCol - 2 >= 0){
        if (board[kingRow + 1][kingCol - 2].getType() == 3 && board[kingRow + 1][kingCol - 2].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT5\n");
            return true;
        }
    }
    if (kingRow + 2 < 8 && kingCol - 1 >= 0){
        if (board[kingRow + 2][kingCol - 1].getType() == 3 && board[kingRow + 2][kingCol - 1].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT6\n");
            return true;
        }
    }
    // 
    if (kingRow + 2 < 8 && kingCol + 1 < 8){
        if (board[kingRow + 2][kingCol + 1].getType() == 3 && board[kingRow + 2][kingCol + 1].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT7\n");
            return true;
        }
    }
    if (kingRow + 1 < 8 && kingCol + 2 < 8){
        if (board[kingRow + 1][kingCol + 2].getType() == 3 && board[kingRow + 1][kingCol + 2].getSide() != this->side){
            board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
            board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
            printf("KNIGHT8\n");
            return true;
        }
    }

    // Examine king (All tiles around current tile)
    for (int i = kingRow-1; i < kingRow+2; i++){
        if (i < 0 || i > 7){
            continue;
        }
        for (int j = kingCol-1; j < kingCol+2; j++){
            if (j < 0 || j > 7){
                continue;
            }
            // If king around in 3x3, then checked tiles.
            if (board[i][j].getType() == 6 && board[i][j].getSide() != this->side){
                board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
                board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
                printf("KING.\n");
                return true;
            }
        }
    }

    board[oldrow][oldcol].UpdatePiece(oldtype, oldside, oldvalue, oldrow, oldcol, oldmoves, oldtotMoves);
    board[row][col].UpdatePiece(storedType, storedSide, storedValue, row, col, storedMoves, storedtotMoves);
    return false;
}

// Get the current piece to print.
void Piece::printType(){
    switch (this->type){
        case -1:
            printEmptyTile();
            break;
        case 1:
            printPawn();
            break;
        case 2:
            printBishop();
            break;
        case 3:
            printKnight();
            break;
        case 4:
            printRook();
            break;
        case 5:
            printQueen();
            break;
        case 6:
            printKing();
            break;
    }
}

// Pretty prints for tiles with/without pieces.
void Piece::printEmptyTile(){
    this->line1 = "_ _ _ _ _ _ _";
    this->line2 = "|     	   |";
    this->line3 = "|           |";
    this->line4 = "|           |";
    this->line5 = "|           |";
    this->line6 = "|_ _ _ _ _ _|";
}
void Piece::printPawn(){
    if (this->side == 0){
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|     	  |')";
        this->line3 = R"('|     o     |')";
        this->line4 = R"('|    ( )    |')";
        this->line5 = R"('|   (___)   |')";
        this->line6 = R"('|_ _ _ _ _ _|')";
    }
    else{
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|     	  |')";
        this->line3 = R"('|     o     |')";
        this->line4 = R"('|    (*)    |')";
        this->line5 = R"('|   (***)   |')";
        this->line6 = R"('|_ _ _ _ _ _|')";
    }
}
void Piece::printBishop(){
    if (this->side == 0){
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|     o	  |')";
        this->line3 = R"('|    (\)    |')";
        this->line4 = R"('|    ( )    |')";
        this->line5 = R"('|    ( )    |')";
        this->line6 = R"('|_ _(_ _)_ _|')";
    }
    else{
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|     o	  |')";
        this->line3 = R"('|    (\)    |')";
        this->line4 = R"('|    (*)    |')";
        this->line5 = R"('|    (*)    |')";
        this->line6 = R"('|_ _(***)_ _|')";
    }
}
void Piece::printKnight(){
    if (this->side == 0){
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|    |\_    |')";
        this->line3 = R"('|   /  .\_  |')";
        this->line4 = R"('|  (    __) |')";
        this->line5 = R"('|  (    )   |')";
        this->line6 = R"('|_/_ _ _\_ _|')";
    }
    else{
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|    |\_    |')";
        this->line3 = R"('|   /**.\_  |')";
        this->line4 = R"('|  (****__) |')";
        this->line5 = R"('|  (****)   |')";
        this->line6 = R"('|_/* * *\_ _|')";
    }
}
void Piece::printRook(){
    if (this->side == 0){
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|   |_|_|   |')";
        this->line3 = R"('|    | |    |')";
        this->line4 = R"('|   /   \   |')";
        this->line5 = R"('|   |   |   |')";
        this->line6 = R"('|_ _|_ _|_ _|')";
    }
    else{
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|   |_|_|   |')";
        this->line3 = R"('|    |*|    |')";
        this->line4 = R"('|   /***\   |')";
        this->line5 = R"('|   |***|   |')";
        this->line6 = R"('|_ _|***|_ _|')";
    }
}
// 11
void Piece::printQueen(){
    if (this->side == 0){
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|    \0/    |')";
        this->line3 = R"('|    { }    |')";
        this->line4 = R"('|    / \    |')";
        this->line5 = R"('|    \ /    |')";
        this->line6 = R"('|_ _(_ _)_ _|')";
    }
    else{
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|    \0/    |')";
        this->line3 = R"('|    {*}    |')";
        this->line4 = R"('|    /*\    |')";
        this->line5 = R"('|    \*/    |')";
        this->line6 = R"('|_ _(***)_ _|')";
    }
}
void Piece::printKing(){
    if (this->side == 0){
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|    \+/    |')";
        this->line3 = R"('|    / \    |')";
        this->line4 = R"('|    \ /    |')";
        this->line5 = R"('|    ) (    |')";
        this->line6 = R"('|_ _(_ _)_ _|')";
    }
    else{
        this->line1 = R"('_ _ _ _ _ _ _')";
        this->line2 = R"('|    \+/    |')";
        this->line3 = R"('|    /*\    |')";
        this->line4 = R"('|    \*/    |')";
        this->line5 = R"('|    )*(    |')";
        this->line6 = R"('|_ _(***)_ _|')";
    }
}

std::string Piece::getLine1(){
    return this->line1;
}
std::string Piece::getLine2(){
    return this->line2;
}
std::string Piece::getLine3(){
    return this->line3;
}
std::string Piece::getLine4(){
    return this->line4;
}
std::string Piece::getLine5(){
    return this->line5;
}
std::string Piece::getLine6(){
    return this->line6;
}
int Piece::getType(){
    return this->type;
}
int Piece::getSide(){
    return this->side;
}
int Piece::getValue(){
    return this->value;
}
int Piece::getXpos(){
    return this->xpos;
}
int Piece::getYpos(){
    return this->ypos;
}
int Piece::getTotMoves(){
    return this->totMoves;
}
std::vector<Coordinate> Piece::getMoves(){
    return this->moves;
}
void Piece::setType(int type){
    this->type = type;
}
void Piece::increaseTotMoves(){
    this->totMoves++;
}