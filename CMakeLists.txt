cmake_minimum_required(VERSION 3.26)
project(Blokus)

set(CMAKE_CXX_STANDARD 14)

add_executable(Blokus
        main.cpp
        Commands.h
        Commands.cpp
        Clock.cpp
        Clock.h
        Board.cpp
        Board.h
        GlobalVars.h
        Piece.cpp
        Piece.h
        AllShapes.h
        PieceSet.cpp
        PieceSet.h
        Cell.cpp
        Cell.h
        Move.cpp
        Move.h
        RandomMoveGen.cpp
        RandomMoveGen.h
        Random.h)
