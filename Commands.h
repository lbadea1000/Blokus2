//
// Created by Lucian Badea on 01.07.2024.
//

#pragma once

using namespace std;

class Command {
public:
    static const int DEFAULT = 0;
    static const int GEN_MOVE = 1;
    static const int PLAY = 2;
    static const int QUIT = 3;
    static const int SET_GAME = 4;
    static const int CLEAR_BOARD = 5;
    static const int CPU_TIME = 6;
    static const int UNKNOWN = 7;
    int type = DEFAULT;
    string firstCommand = "0";
    int player;
    string move;

    void computeCommand();
};
