#pragma once
#include "bitboards.h"
#include "move_generator.h"
#include "attack_tables.h"
#include "game_state.h"
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
