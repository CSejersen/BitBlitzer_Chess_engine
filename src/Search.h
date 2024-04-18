#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "Position.h"
#include "board_constants.h"
#include <bit>
#include <bitset>
const int INF = 2147483647;
class Search {
public:
    Search(BitBoard* board, MoveGenerator* generator, AttackTables* atkTables, Position* position);
    BitBoard* board;
    MoveGenerator* moveGen;
    AttackTables* atkTables;
    Position* position;
    int evaluate();
    int search(int depth);


    // test
    void negamaxTest(int depth);

private:

    //  Negamax search with alpha beta pruning and quiescent search
    int quiesce(int alpha, int beta);
    int negamax(int alpha, int beta, int depth);

    // for eval
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
