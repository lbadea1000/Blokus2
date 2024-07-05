//
// Created by Lucian Badea on 02.07.2024.
//

#include "Cell.h"
#include "GlobalVars.h"

Cell::Cell() {
    x = y = -1;
}

Cell::Cell(int x, int y) {
    this->x = x;
    this->y = y;
    toChar();
}

bool Cell::operator==(Cell other) {
    return (x == other.x && y == other.y);
}
bool Cell::operator<(const Cell& other) const {
    return x < other.x || (x == other.x && y < other.y);
}

void Cell::toChar() {
    lin = col = "";
    int cpy = TABLE_HEIGHT - x;
    if(cpy >= 10) lin += (char)(cpy / 10 + '0');
    lin += (char)(cpy % 10 + '0');
    col += (char)(y + 'a');
}
void Cell::toInt() {
    x = lin[0] -  '0';
    if(lin.size() == 2) x = x * 10 + lin[1] - '0';
    y = col[0] - 'a';
    x = TABLE_HEIGHT - x;
}