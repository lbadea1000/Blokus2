//
// Created by Lucian Badea on 02.07.2024.
//

#include "Move.h"
#include <ctype.h>
#include <assert.h>
#include <vector>
#include "Piece.h"
#include "PieceSet.h"
#include "GlobalVars.h"
#include <iostream>
using namespace std;

Move::Move() {
    x = y = -1;
}

Move::Move(Cell c, Piece p, int x, int y, int player) {
    this->c = c;
    this->p = p;
    this->x = x;
    this->y = y;
    this->player = player;
}

void Move::interpretMove(int PlayerToMove, string s, PieceSet& pieceSet) {
    int ind = 0;
    vector<Cell> cells;
    Cell minCell(15, 15);
    Cell maxCell(0, 0);
    int PieceWidth, PieceHeight;

    while(ind < s.size()) {
        Cell aux;
        while(ind < s.size() && isalpha(s[ind])) {
            aux.col += s[ind];
            ind++;
        }
        while(ind < s.size() && isdigit(s[ind])) {
            aux.lin += s[ind];
            ind++;
        }
        ind++;
//        cerr << "lin = " << aux.lin << " col = " << aux.col << '\n';
        aux.toInt();
//        cerr << "x = " << aux.x << "y = " << aux.y << '\n';
        minCell.x = min(minCell.x, aux.x);
        minCell.y = min(minCell.y, aux.y);
        maxCell.x = max(maxCell.x, aux.x);
        maxCell.y = max(maxCell.y, aux.y);
        cells.push_back(aux);
    }
//    cerr << "minCell.x = " << minCell.x << " minCell.y = " << minCell.y << '\n';
//    cerr << "maxCell.x = " << maxCell.x << " maxCell.y = " << maxCell.y << '\n';

    PieceHeight = maxCell.x - minCell.x + 1;
    PieceWidth = maxCell.y - minCell.y + 1;
//    cerr << "PieceHeight = " << PieceHeight << ' ' << "PieceWidth = " << PieceWidth << '\n';
    Shape shape;
    for(int i = 0; i < MAX_PIECE_SIZE; i++)
        for(int j = 0; j < MAX_PIECE_SIZE; j++)
            shape.bitmap[i][j] = 0;
    for(int i = 0; i < PieceHeight; i++)
        for(int j = 0; j < PieceWidth; j++)
            shape.bitmap[i][j] = '.';
    for(auto cell : cells) {
        shape.bitmap[cell.x - minCell.x][cell.y - minCell.y] = '*';
    }
//    for(int i = 0; i < PieceHeight; i++) {
//        for(int j = 0; j < PieceWidth; j++) {
//            cout << shape.bitmap[i][j];
//        }
//        cout << '\n';
//    }
//    cerr << "Piece = " << '\n';
    Piece piece(shape);
    piece.index = -1;
    for(auto pc : pieceSet.PossibleForms) {
        if(piece.compare(pc)) {
            piece.index = pc.index;
            break;
        }
    }
//    cout << "Index = " << piece.index << '\n';
    p = piece;
    x = y = 0;
    c = minCell;
    player = PlayerToMove;
}

void Move::print() {
    cerr << "Cell = " << c.x << ' ' << c.y << '\n';
    cerr << "Pos = " << x << ' ' << y << '\n';
    cerr << "Player = " << player << '\n';
    cerr << "Piece = " << '\n';
    cerr << "Height = " << p.height << '\n';
    cerr << "Width = " << p.width << '\n';
    p.printShape();
    cerr << '\n';
}

string Move::translateMove() {
    vector<Cell> pieceCells;
    for(int i = 0; i < p.height; i++) {
        for(int j = 0; j < p.width; j++) {
            if(p.pieceShape.bitmap[i][j] == 1) {
                Cell aux(c.x - x + i, c.y - y + j);
                pieceCells.push_back(aux);
            }
        }
    }
    string message = "";
    for(int i = 0; i < pieceCells.size(); i++) {
//        cout << pieceCell.x << ' ' << pieceCell.y << ' ' << pieceCell.lin << ' ' << pieceCell.col << '\n';
        message += pieceCells[i].col;
        message += pieceCells[i].lin;
        if(i < pieceCells.size() - 1)
            message += ',';
    }
    return message;
}