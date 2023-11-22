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
    std::list<Move> pseudoLegal;
    std::list<Move> captures;

private:
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateKingMoves();
    void generateQueenMoves();
    void generatePawnAdvances();
    void generatePawnCaptures();
    void generateEnPassant();
    void generateCastlesWhite();
    void generateCastlesBlack();
    bool isCapture(int targetSquare) const;
    void generatePawnAdvancesWhite();
    void generatePawnAdvancesBlack();
};


