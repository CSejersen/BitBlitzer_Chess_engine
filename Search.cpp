#include "Search.h"

Search::Search(BitBoard* board, MoveGenerator *generator, AttackTables *atkTables,
               CurrentPosition *state) {
    _board = board;
    _generator = generator;
    _atkTables = atkTables;
    _state = state;
}

