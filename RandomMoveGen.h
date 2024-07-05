//
// Created by Lucian Badea on 03.07.2024.
//

#pragma once
#include "Move.h"
#include "Board.h"
#include "PieceSet.h"

class RandomMoveGen {
public:
    vector<Move> allPossibleMoves;

    void genAllMoves(PieceSet& pieceSet, Board& board, int player);

    Move genRandMove();
    void print();
};

class Node {
public:
    Board board;
    RandomMoveGen gen;
    int wins;
    int total;
    int prevWins;
    int prevTotal;
    int player;
    int noPass;
    int height;
    Node* parent;
    vector<pair<Node, Move>> children;
    Node();
    Node(Node* n, Board b, int height, int player, int noPass);
    double formula();

private:
    const double ROOT2 = 1.414213;
    const double INF = 1e9;
};


class MonteCarloTree {
public:
    static const int NO_ITERATIONS = 100;
    const double INF = 1e9;
    static const int HEIGHTMAX = 50;
    Node root;
    int count[HEIGHTMAX];

    MonteCarloTree(Board b, int player);
    Node* findLeaf();
    void expand(PieceSet& PieceSet, Node* node);
    Node* playTillTheEnd(PieceSet& PieceSet, Node* node);
    void backpropag(Node* node);
    Move genMove(PieceSet& PieceSet);
    void print();
};
