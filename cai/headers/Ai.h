#pragma once  // Include guards to prevent multiple inclusion

// #include "Move.h"
#include "Piece.h"
#include "Move.h"

std::tuple <Coordinate,Coordinate> findBestMove(Board* chessboard, Piece** board, int depth, bool side);

