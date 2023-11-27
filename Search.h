#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "CurrentPosition.h"
#include "board_constants.h"

class Search {
public:
    BitBoard* _board;
    MoveGenerator* _generator;
    AttackTables* _atkTables;
    CurrentPosition* _state;

    Search(BitBoard* board, MoveGenerator* generator, AttackTables* atkTables, CurrentPosition* state);

private:
};
