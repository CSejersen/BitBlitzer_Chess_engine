#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "GameState.h"
#include "board_constants.h"

class Search {
public:
    BitBoard* _board;
    MoveGenerator* _generator;
    AttackTables* _atkTables;
    GameState* _state;

    Search(BitBoard* board, MoveGenerator* generator, AttackTables* atkTables, GameState* state);

private:
};
