//
// Created by Lucian Badea on 02.07.2024.
//

#pragma once
#include "Cell.h"
#include "Piece.h"
#include "PieceSet.h"
using namespace std;

class Move {
public:
    Cell c;
    Piece p;
    int x;
    int y;
    int player;

    Move();
    Move(Cell c, Piece p, int x, int y, int player);

    void interpretMove(int PlayerToMove, string s, PieceSet& pieceSet);
    string translateMove();
    void print();
};
