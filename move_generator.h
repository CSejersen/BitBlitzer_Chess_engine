#pragma once
#include "bitboards.h"
#include "game_state.h"
#include "attack_tables.h"
#include "bit_manipulation.h"
#include "move.h"


class MoveGenerator {

public:
    MoveGenerator(BitBoard* board, GameState* state, AttackTables* attackTables);
    BitBoard* _board;
    GameState* _state;
    AttackTables* _atkTables;

    void generateMoves();
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateKingMoves();
    void generateQueenMoves();
    void generatePawnAdvancesWhite();
    void generatePawnAdvancesBlack();
    void generatePawnCaptures();
    void generateEnPassant();
    void generateCastles();
    std::list<Move> pseudoLegal;
    std::list<Move> captures;

private:
    bool isCapture(int targetSquare) const;
};


