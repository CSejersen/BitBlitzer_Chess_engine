#pragma once
#include "bitboards.h"
#include "game_state.h"
#include "attack_tables.h"
#include "bit_manipulation.h"
#include "move.h"
#include <list>


class MoveGenerator {

public:
    MoveGenerator(BitBoard* board, GameState* state, AttackTables* attackTables);
    BitBoard* _board;
    GameState* _state;
    AttackTables* _atkTables;

    void generateMoves();
    void pruneIllegalMoves();
    bool whiteKingInCheck();
    bool blackKingInCheck();
    std::list<Move> legalMoves;
    std::list<Move> legalCaptures;

    std::list<Move> pseudoLegal;
    std::list<Move> pseudoLegalCapture;

private:
    // generate pseudo-legal moves
    void generateKnightMovesWhite();
    void generateBishopMoves();
    void generateRookMoves();
    void generateKingMoves();
    void generateQueenMoves();
    void generatePawnAdvances();
    void generatePawnCaptures();
    void generateEnPassant();
    void generateCastlesWhite();
    void generateCastlesBlack();
    void generatePawnAdvancesWhite();
    void generatePawnAdvancesBlack();

    // utility
    bool isCapture(int targetSquare) const;



};


