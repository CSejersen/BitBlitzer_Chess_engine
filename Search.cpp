#include "Search.h"

Search::Search(BitBoard* board, MoveGenerator *generator, AttackTables *atkTables,
               GameState *state) {
    _board = board;
    _generator = generator;
    _atkTables = atkTables;
    _state = state;
}

