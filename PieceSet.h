//
// Created by Lucian Badea on 02.07.2024.
//

#pragma once

#include <vector>
#include "Piece.h"

class PieceSet {
public:
    vector<Piece> PossibleForms;
    void generate();
    void print();
private:
    bool compare(Piece a, Piece b);
    void pushPiece(Piece p, int index);
};
