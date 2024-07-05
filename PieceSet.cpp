//
// Created by Lucian Badea on 02.07.2024.
//

#include "PieceSet.h"
#include "GlobalVars.h"
#include "AllShapes.h"
#include "Piece.h"
#include <iostream>



void PieceSet::pushPiece(Piece p, int index) {
    bool ok = true;
    for (auto x: PossibleForms) {
        if (p.compare(x))
            ok = false;
    }
    if (ok) {
        p.index = index;
        PossibleForms.push_back(p);
    }
}

void PieceSet::generate() {
    int ind = 0;
    for(int i = 0; i < NO_PIECES; i++) {
        Piece p(allShapes[i]);
        for(int o = 0; o < ORIENTATIONS; o++) {
            for(int d = 0; d < DIRS; d++) {
                pushPiece(p, ind);
                p.rotate();
            }
            p.flip();
        }
        ind++;
    }
}

void PieceSet::print() {
    cout << PossibleForms.size() << '\n';
    for(auto p : PossibleForms)
        p.printShape();
}