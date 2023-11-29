#pragma once
#include "board_constants.h"
#include "MoveGenerator.h"
#include "Position.h"
#include "BitBoard.h"



class Perft{
public:
    Perft(MoveGenerator*,Position*,BitBoard*);
    MoveGenerator* moveGenerator;
    Position* position;
    BitBoard* board;

    U64 countNodes(int depth);
    void perft(int maxDepth);

};
