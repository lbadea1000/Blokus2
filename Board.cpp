//
// Created by Lucian Badea on 01.07.2024.
//

#include "Board.h"
#include "Move.h"
#include "Cell.h"
#include "Piece.h"
#include "GlobalVars.h"
#include <iostream>

// (x, y) -> (c.x, c.y)
// => (a, b) -> (c.x - x + a, c.y - y + b)

Board::Board() {
    for(int i = 0; i < TABLE_HEIGHT; i++)
        for(int j = 0; j < TABLE_WIDTH; j++)
            mat[i][j] = FREE_CELL;
    hand[0] = (1 << NO_PIECES) - 1;
    hand[1] = (1 << NO_PIECES) - 1;
    startCorner[0].x = 4, startCorner[0].y = 4;
    startCorner[1].x = 9, startCorner[1].y = 9;
    Corners[0].clear(), Corners[1].clear();
    Corners[0].insert(startCorner[0]);
    Corners[1].insert(startCorner[1]);
    DeadCells[0].clear(), DeadCells[1].clear();
}


bool Board::checkLegal(Move m) {
    // is it in my hand?
    if(!(hand[m.player] & (1 << m.p.index)))
        return false;
    // is it on board?
    if(m.c.x - m.x < 0 ||
            m.c.y - m.y < 0 ||
            m.c.x - m.x + m.p.height - 1 >= TABLE_HEIGHT ||
            m.c.y - m.y + m.p.width - 1 >= TABLE_WIDTH) {
//        cout << "\t\t\tnot on board" << '\n';
        return false;
    }
    // are the cells not occupied?
    for(int i = 0; i < m.p.height; i++) {
        for(int j = 0; j < m.p.width; j++) {
            if(m.p.pieceShape.bitmap[i][j] == 1 && mat[m.c.x - m.x + i][m.c.y - m.y + j] != FREE_CELL) {
//                cout << "\t\t\toccupied cells";
                return false;
            }
        }
    }
    // is the piece touching at least one of my corners?
    // is the piece not having any 'dead' cells?
    bool ok = false;
    if(hand[m.player] == (1 << NO_PIECES) - 1)
        ok = true;
    for(int i = 0; i < m.p.height; i++) {
        for(int j = 0; j < m.p.width; j++) {
            if(m.p.pieceShape.bitmap[i][j] == 1) {
                Cell pCell(m.c.x - m.x + i, m.c.y - m.y + j);
                if(!checkNeighbors(pCell, m.player)) {
//                    cout << "\t\t\tneighbors" << '\n';
                    return false;
                }
                if(checkCorners(pCell, m.player))
                    ok = true;
            }
        }
    }
//    if(!ok)
////        cout << "\t\t\tcorners" << '\n';
    return ok;
}

bool Board::isInBoard(Cell c) {
    return (c.x >= 0 && c.x < TABLE_HEIGHT && c.y >= 0 && c.y < TABLE_WIDTH);
}

bool Board::checkNeighbors(Cell c, int player) {
    for(int i = 0; i < DIRS; i++) {
        if(isInBoard(Cell(c.x + NLDIR[i], c.y + NCDIR[i])) &&
           mat[c.x + NLDIR[i]][c.y + NCDIR[i]] == player)
            return false;
    }
    return true;
}
bool Board::checkCorners(Cell c, int player) {
    for(int i = 0; i < DIRS; i++) {
        if(isInBoard(Cell(c.x + CLDIR[i], c.y + CCDIR[i])) &&
        mat[c.x + CLDIR[i]][c.y + CCDIR[i]] == player)
            return true;
    }
    return false;
}

void Board::execMove(Move m) {
    for(int i = 0; i < m.p.height; i++) {
        for(int j = 0; j < m.p.width; j++) {
            if(m.p.pieceShape.bitmap[i][j] == 1)
                mat[m.c.x - m.x + i][m.c.y - m.y + j] = m.player;
        }
    }
    hand[m.player] ^= (1 << m.p.index);
    for(int i = 0; i < m.p.height; i++) {
        for(int j = 0; j < m.p.width; j++) {
            if(m.p.pieceShape.bitmap[i][j] == 1) {
                const Cell aux(m.c.x - m.x + i, m.c.y - m.y + j);
                Corners[m.player].erase(aux);
                Corners[1 - m.player].erase(aux);
                for(int d = 0; d < DIRS; d++) {
                    int lin = m.c.x - m.x + i + NLDIR[d];
                    int col = m.c.y - m.y + j + NCDIR[d];
                    const Cell neigh(lin, col);
                    if(isInBoard(neigh)) {
                        if (mat[lin][col] == FREE_CELL)
                            DeadCells[m.player].insert(neigh);
                        Corners[m.player].erase(neigh);
                    }
                }
            }
        }
    }
    for(int i = 0; i < m.p.height; i++) {
        for(int j = 0; j < m.p.width; j++) {
            if(m.p.pieceShape.bitmap[i][j] == 1) {
                for(int d = 0; d < DIRS; d++) {
                    int lin = m.c.x - m.x + i + CLDIR[d];
                    int col = m.c.y - m.y + j + CCDIR[d];
                    const Cell corner(lin, col);
                    if(isInBoard(corner)) {
                        if (mat[lin][col] == FREE_CELL &&
                            DeadCells[m.player].find(corner) == DeadCells[m.player].end())
                            Corners[m.player].insert(corner);
                    }
                }
            }
        }
    }
//    for(int i = 0; i < m.p.height; i++) {
//        for(int j = 0; j < m.p.width; j++) {
//            if(m.p.pieceShape.bitmap[i][j] == 1) {
//                for(int d = 0; d < DIRS; d++) {
//                    int lin = m.c.x - m.x + i + CLDIR[d];
//                    int col = m.c.y - m.y + j + CCDIR[d];
//                    const Cell aux(i, j);
//                    if (isInBoard(aux) && mat[lin][col] == m.player) {
//                        Corners[m.player].erase(aux);
//                    }
//                    const Cell aux2(lin, col);
//                    if (mat[lin][col] == FREE_CELL) {
//                        Corners[m.player].insert(aux);
//                    }
//                    Corners[1 - m.player].erase(aux);
//                }
//                const Cell me(i, j);
//                Corners[1 - m.player].erase(me);
//                for(int d = 0; d < DIRS; d++) {
//                    int lin = m.c.x - m.x + i + NLDIR[d];
//                    int col = m.c.y - m.y + j + NCDIR[d];
//                    const Cell aux(lin, col);
//                    if(isInBoard(aux)) {
//                        if (mat[lin][col] == FREE_CELL) {
//                            Corners[m.player].erase(aux);
//                        }
//                    }
//                }
//            }
//        }
//    }
}

void Board::print() {
//    cerr << "Black corners: " << '\n';
//    for(auto x : Corners[0]) {
//        cerr << x.x << ' ' << x.y << '\n';
//    }
//    cerr << "White corners: " << '\n';
//    for(auto x : Corners[1]) {
//        cerr << x.x << ' ' << x.y << '\n';
//    }
    for(int i = 0; i < TABLE_HEIGHT; i++) {
        for (int j = 0; j < TABLE_WIDTH; j++)
            cerr << (int)mat[i][j];
        cerr << '\n';
    }
    cerr << '\n';
//    cerr << "Black remaining pieces : \n";
//    for(int i = 0; i < NO_PIECES; i++)
//        cerr << ((hand[0] & (1 << i)) == (1 << i)) << ' ';
//    cerr << '\n';
//    cerr << "White remaining pieces : \n";
//    for(int i = 0; i < NO_PIECES; i++)
//        cerr << ((hand[1] & (1 << i)) == (1 << i)) << ' ';
//    cerr << '\n';
}

void Board::printANSI() {
//    for(int i = 0; i < TABLE_HEIGHT; i++) {
//        fprintf(stderr, "%2d ", TABLE_HEIGHT - i);
//        for(int j = 0; j < TABLE_WIDTH; j++) {
//            const Cell aux(i, j);
//            if(Corners[0].find(aux) != Corners[0].end()) {
//                fprintf(stderr, "\e[105m  \e[49m\e[39m");
//            } else {
//                fprintf(stderr, "  ");
//            }
//        }
//        fprintf(stderr, "\n");
//    }
//    fprintf(stderr, "   ");
//    for (int j = 0; j < TABLE_WIDTH; j++) {
//        fprintf(stderr, "%c%c", j + 'a', j + 'a');
//    }
//    fprintf(stderr, "\n");
//
//    for(int i = 0; i < TABLE_HEIGHT; i++) {
//        fprintf(stderr, "%2d ", TABLE_HEIGHT - i);
//        for(int j = 0; j < TABLE_WIDTH; j++) {
//            const Cell aux(i, j);
//            if(Corners[1].find(aux) != Corners[1].end()) {
//                fprintf(stderr, "\e[43m  \e[49m\e[39m");
//            } else {
//                fprintf(stderr, "  ");
//            }
//        }
//        fprintf(stderr, "\n");
//    }
//    fprintf(stderr, "   ");
//    for (int j = 0; j < TABLE_WIDTH; j++) {
//        fprintf(stderr, "%c%c", j + 'a', j + 'a');
//    }
//    fprintf(stderr, "\n");

    for(int i = 0; i < TABLE_HEIGHT; i++) {
        fprintf(stderr, "%2d ", TABLE_HEIGHT - i);
        for(int j = 0; j < TABLE_WIDTH; j++) {
            if(mat[i][j] != FREE_CELL) {
                int p = mat[i][j];
                if(p == 0) {
                    fprintf(stderr, "\e[105m  \e[49m\e[39m");
                } else {
                    fprintf(stderr, "\e[43m  \e[49m\e[39m");
                }
            } else {
                fprintf(stderr, "  ");
            }
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "   ");
    for (int j = 0; j < TABLE_WIDTH; j++) {
        fprintf(stderr, "%c%c", j + 'a', j + 'a');
    }
    fprintf(stderr, "\n");
}

void Board::printFinalScores() {
    int black = 0, white = 0;
    for(int i = 0; i < TABLE_HEIGHT; i++) {
        for(int j = 0; j < TABLE_WIDTH; j++) {
            if(mat[i][j] == 0)
                black++;
            if(mat[i][j] == 1)
                white++;
        }
    }
    fprintf(stderr, "Final score: %d %d diff %d\n", black, white, black - white);
}

bool Board::eval(int player) {
    int me = 0, opp = 0;
    for(int i = 0; i < TABLE_HEIGHT; i++) {
        for(int j = 0; j < TABLE_WIDTH; j++) {
            if(mat[i][j] == player)
                me++;
            else if(mat[i][j] == 1 - player)
                opp++;
        }
    }
    return (me > opp);
}