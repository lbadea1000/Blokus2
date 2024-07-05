//
// Created by Lucian Badea on 01.07.2024.
//
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Commands.h"
#include "Clock.h"
#include "PieceSet.h"
#include "Board.h"
#include "Move.h"
#include "RandomMoveGen.h"

using namespace std;



int main() {
    cerr << "SEED = " << chrono::steady_clock::now().time_since_epoch().count() << '\n';
    PieceSet allPieces;
    allPieces.generate();
//    allPieces.print();
    Command comm;
    Clock clock;
    Board board;
    RandomMoveGen gen;

    while(comm.type != comm.QUIT && comm.type != comm.UNKNOWN) {
        comm.computeCommand();
//        if(comm.firstCommand != "0") cout << "first = " << comm.firstCommand << '\n';
        Move move;
        string message;
        switch (comm.type) {
            case Command::CPU_TIME:
                if(!clock.isOn) {
                    clock.startClock();
                    cout << "= 0.000" << "\n\n";
                    cout.flush();
                } else {
                    cout << "= " << clock.stopClock() << "\n\n";
                    cout.flush();
                }
                break;
            case Command::GEN_MOVE:
                /*
                 * genMove()
                 */

                gen.genAllMoves(allPieces, board, comm.player);
//                gen.print();
                if(gen.allPossibleMoves.empty()) {
                    cout << "= pass" << "\n\n";
                    cout.flush();
                } else {
//                gen.print();
                    MonteCarloTree tree(board, comm.player);
                    move = tree.genMove(allPieces);
//                    tree.print();
//                    cerr << "BAAAAAAAA" << '\n';
//                    move.print();
//                    cerr << "BAAAAAAAA" << '\n';
                    board.execMove(move);
                    board.printANSI();
                    //move.print();
                    message = move.translateMove();
//                board.print();

                    cout << "= " << message << "\n\n";
                    cout.flush();
                }
                break;
            case Command::PLAY:
                /*
                 * makeMove()
                 */
                move.interpretMove(comm.player, comm.move, allPieces);
//                move.print();
                board.execMove(move);
                board.printANSI();
//                board.print();

                cout << "= " << "\n\n";
                cout.flush();
                break;
            default:
                cout << "= " << "\n\n";
                cout.flush();
                break;
        }
    }

    board.printFinalScores();

    return 0;
}