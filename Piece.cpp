//
// Created by Lucian Badea on 01.07.2024.
//

#include "Piece.h"
#include "GlobalVars.h"
#include <iostream>

Piece::Piece(Shape shape) {
    pieceShape = shape;
    height = 0;
    while(height < MAX_PIECE_SIZE && pieceShape.bitmap[height][0])
        height++;
    width = 0;
    while(width < MAX_PIECE_SIZE && pieceShape.bitmap[0][width])
        width++;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            pieceShape.bitmap[i][j] = (pieceShape.bitmap[i][j] == '*' ? 1 :
                                       (pieceShape.bitmap[i][j] == '.' ? 0 :
                                        pieceShape.bitmap[i][j]));
        }
    }
    sz = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            sz += pieceShape.bitmap[i][j];
        }
    }
}

/* 90 clockwise rotation */
void Piece::rotate() {
    Shape newShape;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            newShape.bitmap[j][height - i - 1] = pieceShape.bitmap[i][j];
    pieceShape = newShape;
    swap(height, width);
}

void Piece::flip() {
    Shape newShape;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            newShape.bitmap[height - i - 1][j] = pieceShape.bitmap[i][j];
    pieceShape = newShape;
}

void Piece::printShape() {
//    cout << "height = " << height << '\n';
//    cout << "width = " << width << '\n';
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            cerr << (pieceShape.bitmap[i][j] == 1 ? '#' : ' ');
        }
        cerr << '\n';
    }
    cerr << "\n";
}

bool Piece::compare(Piece other) {
    if(width != other.width)
        return false;
    if(height != other.height)
        return false;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++)
            if(pieceShape.bitmap[i][j] != other.pieceShape.bitmap[i][j])
                return false;
    }
    return true;
}