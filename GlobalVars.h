//
// Created by Lucian Badea on 01.07.2024.
//

#pragma once

const int TABLE_WIDTH = 14;
const int TABLE_HEIGHT = 14;
const int MAX_PIECE_SIZE = 5;
const int NO_PIECES = 21;
const int DIRS = 4;
const int ORIENTATIONS = 2;
const int NLDIR[4] = {-1, 0, 1, 0}; // neighbor line/column directions
const int NCDIR[4] = {0, 1, 0, -1};
const int CLDIR[4] = {-1, -1, 1, 1}; // corner line/column directions
const int CCDIR[4] = {-1, 1, 1, -1};

