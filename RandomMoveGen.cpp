//
// Created by Lucian Badea on 03.07.2024.
//

#include "RandomMoveGen.h"
#include "Move.h"
#include "Board.h"
#include "PieceSet.h"
#include "GlobalVars.h"
#include "Random.h"
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;

void RandomMoveGen::genAllMoves(PieceSet &pieceSet, Board &board, int player) {
    allPossibleMoves.clear();
//    cerr << board.Corners[player].size() << '\n';
    for(auto corner : board.Corners[player]) {
//        cout << "corner = " << corner.x << ' ' << corner.y << '\n';
        for(auto piece : pieceSet.PossibleForms) {
//            cout << "piece = " << '\n';
//            piece.printShape();
            if(board.hand[player] & (1 << piece.index)) {
                for (int x = 0; x < piece.height; x++) {
                    for (int y = 0; y < piece.width; y++) {
                        if(piece.pieceShape.bitmap[x][y] == 1) {
                            Move m(corner, piece, x, y, player);
//                            m.print();
//                            board.print();
                            if (board.checkLegal(m)) {
//                                cout << "legal\n";
                                allPossibleMoves.push_back(m);
                            }
                        }
                    }
                }
            }
        }
    }
    random_device rd;
    mt19937 g(rd());
    // Shuffle the vector
    shuffle(allPossibleMoves.begin(), allPossibleMoves.end(), g);
}

Move RandomMoveGen::genRandMove() {
    return allPossibleMoves[randomNr(0, allPossibleMoves.size() - 1)];
}

void RandomMoveGen::print() {
    cerr << "Size = " << allPossibleMoves.size() << '\n';
    for(auto move : allPossibleMoves) {
        move.print();
    }
}

double Node::formula() {
    double a = 0, b = 0;
    if(total == 0)
        return INF;
    a = (double)wins / total;
    if(parent->total == 0)
        b = 0;
    else
        b = ROOT2 * sqrt(log(parent->total) / total);
    return a + b;
}

Node::Node() {
    children.clear();
}

Node::Node(Node *n, Board b, int height, int player, int noPass) {
    total = wins = 0;
    parent = n;
    board = b;
    this->height = height;
    this->player = player;
    this->noPass = noPass;
    children.clear();
}


MonteCarloTree::MonteCarloTree(Board b, int player) {
    root.parent = nullptr;
    root.board = b;
    root.player = player;
    root.total = root.wins = 0;
    root.height = 0;
    root.children.clear();
    for(int i = 0; i < HEIGHTMAX; i++)
        count[i] = 0;
    count[0]++;
}

Node* MonteCarloTree::findLeaf() {
    Node* leaf = &root;
    while(!leaf->children.empty()) {
        double maxx = -INF;
        Node* candidate;
        for(auto &x : leaf->children) {
            if(x.first.formula() > maxx) {
                maxx = x.first.formula();
                candidate = &x.first;
            }
        }
        leaf = candidate;
    }
    return leaf;
}

void MonteCarloTree::expand(PieceSet &PieceSet, Node *node) { /// node is a leaf
    if((*node).noPass == 2)
        return;
    (*node).gen.genAllMoves(PieceSet, (*node).board, (*node).player);
    if((*node).gen.allPossibleMoves.empty()) {
        count[(*node).height + 1]++;
        (*node).children.push_back({Node(node, (*node).board, (*node).height + 1, 1 - (*node).player, (*node).noPass + 1), Move()});
    } else {
        for (auto m: (*node).gen.allPossibleMoves) {
            Board b = (*node).board;
            b.execMove(m);
            count[(*node).height + 1]++;
            (*node).children.push_back({Node(node, b, (*node).height + 1, 1 - (*node).player, (*node).noPass), m});
        }
    }
}
Node* MonteCarloTree::playTillTheEnd(PieceSet& PieceSet, Node *node) { /// node is a leaf
    Node* ptr = node;
    while((*ptr).noPass < 2) {
        (*ptr).gen.genAllMoves(PieceSet, (*ptr).board, (*ptr).player);
        if((*ptr).gen.allPossibleMoves.empty()) {
            count[(*ptr).height + 1]++;
            (*ptr).children.push_back({Node(ptr, (*ptr).board, (*ptr).height + 1, 1 - (*ptr).player, (*ptr).noPass + 1), Move()});
        } else {
            Move m = (*ptr).gen.genRandMove();
            Board b = (*ptr).board;
            b.execMove(m);
            count[(*ptr).height + 1]++;
            (*ptr).children.push_back({Node(ptr, b, (*ptr).height + 1, 1 - (*ptr).player, (*ptr).noPass), m});
        }
        ptr = &((*ptr).children[0].first);
    }
    (*ptr).prevTotal = 0;
    (*ptr).prevWins = 0;
    (*ptr).total = 1;
    (*ptr).wins = (*ptr).board.eval((*ptr).player);
    return ptr;
}

void MonteCarloTree::backpropag(Node *node) {
    int leafPlayer = (*node).player;
    int hasWon = (*node).wins;
    while((*node).parent != nullptr) {
//        cerr << "TOTAL = " << (*node).total << " WINS = " << (*node).wins << " PLAYER = " << (*node).player << '\n';
//        (*node).board.printANSI();
//        cerr << "----------\n";
        (*(*node).parent).total++;
        if(hasWon == 1 && (*(*node).parent).player == leafPlayer)
            (*(*node).parent).wins++;
        node = (*node).parent;
    }
}
Move MonteCarloTree::genMove(PieceSet& PieceSet) {
    for(int iter = 0; iter < NO_ITERATIONS; iter++) {
        Node *leaf = findLeaf();
//        cerr << "ITER = " << iter << '\n';
//        (*leaf).board.printANSI();
//        cerr << "ITER = " << iter << '\n';
        if((*leaf).total == 0) {
            expand(PieceSet, leaf);
//            cerr << "SIZE = " << (*leaf).children.size() << '\n';
            Node *childToPlay = &(*leaf).children[randomNr(0, (*leaf).children.size() - 1)].first;
            Node *finalLeaf = playTillTheEnd(PieceSet, childToPlay);
            backpropag(finalLeaf);
        }
    }
//    for(int i = 0; i < 42; i++)
//        cerr << "H = " << i << " " << count[i] << '\n';
//    cerr << root.total << ' ' << root.wins << '\n';
    int maxx = -1;
    int maxWins = -1;
    int pieceSz = -1;
    Move retMove;
    for(auto x : root.children) {
        if(x.first.total > maxx) {
            maxx = x.first.total;
            maxWins = x.first.wins;
            pieceSz = x.second.p.sz;
            retMove = x.second;
        } else if(x.first.total == maxx && x.first.wins > maxWins) {
            maxWins = x.first.wins;
            pieceSz = x.second.p.sz;
            retMove = x.second;
        } else if(x.first.total == maxx && x.first.wins == maxWins && x.second.p.sz > pieceSz) {
            pieceSz = x.second.p.sz;
            retMove = x.second;
        }
    }
//    cerr << "TOTAL = " << maxx << '\n';
    return retMove;
}

void MonteCarloTree::print() {
    cerr << "ROOT = " << '\n';
    cerr << "WINS = " << root.wins << " TOTAL = " << root.total << '\n';
    root.board.print();
    cerr << "ROOT;\n";
    cerr << "ROOT SIZE = " << root.children.size() << '\n';
    for(int i = 0; i < min(5, (int)root.children.size()); i++) {
        cerr << "CHILDREN " << i << '\n';
        cerr << "WINS = " << root.children[i].first.wins << " TOTAL = " << root.children[i].first.total << '\n';
        root.children[i].second.print();
        cerr << "-------------\n";
    }
}