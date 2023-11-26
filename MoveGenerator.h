#pragma once
#include "BitBoard.h"
#include "GameState.h"
#include "AttackTables.h"
#include "bit_manipulation.h"
#include "Move.h"
#include "move_new.h"
#include <list>
#include <vector>

class MoveGenerator {

public:
    MoveGenerator(BitBoard* board, GameState* state, AttackTables* attackTables);
    BitBoard* _board;
    GameState* _state;
    AttackTables* _atkTables;

    void generateMoves(std::vector<int>&);
    void generateMoves();
    std::vector<int> _legalMoves;
    std::vector<int> _legalCaptures;

    // Debug functions
    void printLegalMoves();
    U64 countNodes(int depth);
    void perft(int maxDepth);


private:
    // generate pseudo-legal moves

    void generateKnightMovesWhite() ;
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
    bool whiteKingInCheck() const;
    bool blackKingInCheck() const;

    // Debug

    // Pseudo-legal moves
    std::vector<int> pseudoLegal;
    std::vector<int> pseudoLegalCapture;



};


