#pragma once
#include "BitBoard.h"
#include "AttackTables.h"
#include "bit_manipulation.h"
#include "move.h"
#include <vector>

class MoveGenerator {

public:
    MoveGenerator(BitBoard* board, AttackTables* attackTables);
    BitBoard* _board;
    AttackTables* _atkTables;

    void generateMoves(std::vector<int> &moves, bool whiteToMove, int castlingRights, const U64 &enPassantSquare,
                       bool inCheck);
    std::vector<int> pseudoLegal;
    std::vector<int> pseudoLegalCapture;

    // Debug functions
    void printLegalMoves();

private:
    // generate pseudo-legal moves

    void generateKnightMovesWhite(bool whiteToMove);
    void generateBishopMoves(bool whiteToMove);
    void generateRookMoves(bool whiteToMove);
    void generateKingMoves(bool whiteToMove);
    void generateQueenMoves(bool whiteToMove);
    void generatePawnAdvances(bool whiteToMove);
    void generatePawnCaptures(bool whiteToMove);
    void generateEnPassant(bool whiteToMove, const U64 &enPassantSquare);
    void generateCastlesWhite(int castlingRights, bool inCheck);
    void generateCastlesBlack(int castlingRights, bool inCheck);
    void generatePawnAdvancesWhite();
    void generatePawnAdvancesBlack();
    bool decodeCastlingRight(int castlingRights, int right);

    // utility
    bool isCapture(int targetSquare, bool whiteToMove) const;
    bool whiteKingInCheck() const;
    bool blackKingInCheck() const;

};


