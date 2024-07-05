//
// Created by Lucian Badea on 02.07.2024.
//

#pragma once
#include <string>
using namespace std;

class Cell {
public:
    string lin, col; /// lin = digit, col = letter
    int x, y;
    Cell();
    Cell(int x, int y);
    bool operator==(Cell other);
    bool operator<(const Cell& other) const;
    void toInt();
    void toChar();
};