#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "Position.h"
#include "board_constants.h"
#include <bit>
#include <bitset>

class Search {
public:

    Search(BitBoard* board, MoveGenerator* generator, AttackTables* atkTables, Position* state);
    int evaluate();
    int negaMax(int depth);
    int searchPos(int depth);
    int search(int depth);

    BitBoard* board;
    MoveGenerator* moveGen;
    AttackTables* atkTables;
    Position* position;

private:
    const int pieceValue[14] = {
            0,
            100,
            300,
            330,
            500,
            900,
            10000,
            0,
            -100,
            -300,
            -330,
            -500,
            -900,
            -10000
    };


};
