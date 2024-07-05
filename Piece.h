//
// Created by Lucian Badea on 01.07.2024.
//
#pragma once


#include <string>
#include "GlobalVars.h"

using namespace std;

class Shape {
public:
    char bitmap[MAX_PIECE_SIZE + 1][MAX_PIECE_SIZE + 1];
};

class Piece {
public:
    Shape pieceShape;
    int index;
    int height;
    int width;
    int sz;

    Piece() {};
    Piece(Shape shape);
    void rotate();
    void flip();
    void printShape();
    bool compare(Piece other);
};


