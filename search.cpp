#include "search.h"

Search::Search(BitBoard* board, MoveGenerator *generator, AttackTables *atkTables,
               GameState *state) {
    _board = board;
    _generator = generator;
    _atkTables = atkTables;
    _state = state;
}

bool Search::whiteKingInCheck() {
    return(_atkTables->getAttacksBlack() & _board->getPieceSet(nWhiteKing));
}

bool Search::blackKingInCheck() {
    return(_atkTables->getAttacksWhite() & _board->getPieceSet(nBlackKing));
}
