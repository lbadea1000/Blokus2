//
// Created by Lucian Badea on 01.07.2024.
//

#pragma once
#include "GlobalVars.h"
#include "Cell.h"
#include "Piece.h"
#include "Move.h"
#include <vector>
#include <set>
using namespace std;

class Board {
public:
    static const int FREE_CELL = 3;

    char mat[TABLE_HEIGHT][TABLE_WIDTH];
    /// 0 = black, 1 = white
    int hand[2];
    Cell startCorner[2];
    set<Cell> Corners[2];
    set<Cell> DeadCells[2];

    Board();
    void init(string firstCommand);
    bool isInBoard(Cell c);
    bool checkNeighbors(Cell c, int player);
    bool checkCorners(Cell c, int player);
    bool checkLegal(Move m);
    void execMove(Move m);
    void print();
    void printANSI();
    void printFinalScores();
    bool eval(int player);
};
