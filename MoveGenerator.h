#pragma once
#include "BitBoard.h"
#include "GameState.h"
#include "AttackTables.h"
#include "bit_manipulation.h"
#include "Move.h"
#include <list>

class MoveGenerator {

public:
    MoveGenerator(BitBoard* board, GameState* state, AttackTables* attackTables);
    BitBoard* _board;
    GameState* _state;
    AttackTables* _atkTables;

    void generateMoves();
    std::list<Move> legalMoves;
    std::list<Move> legalCaptures;

    // Debug functions
    void printLegalMoves();


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
    void pruneIllegalMoves();

    // utility
    bool isCapture(int targetSquare) const;
    bool whiteKingInCheck();
    bool blackKingInCheck();

    // Pseudo-legal moves
    std::list<Move> pseudoLegal;
    std::list<Move> pseudoLegalCapture;



};


