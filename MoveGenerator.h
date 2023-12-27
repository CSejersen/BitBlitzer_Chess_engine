#pragma once
#include "BitBoard.h"
#include "AttackTables.h"
#include "bit_manipulation.h"
#include "move.h"
#include <vector>

class MoveGenerator {

public:
    MoveGenerator(BitBoard* board, AttackTables* attackTables);


    // Generate all pseudo-legal moves
    void generateMoves(std::vector<int> &moves, bool whiteToMove, int castlingRights, const U64 &enPassantSquare, bool inCheck);
    bool generateCaptures(std::vector<int> &captures, bool whiteToMove, int castlingRights, const U64 &enPassantSquare);
    // Generate all moves the evade the check
    void generateEvasionMoves(std::vector<int>& moves, bool whiteToMove, const U64& enPassantSquare);

    // move-lists
    std::vector<int> pseudoLegal;
    std::vector<int> pseudoLegalCapture;

    // Debug functions
    void printLegalMoves();

private:

    BitBoard* _board;
    AttackTables* _atkTables;

    // generate pseudo-legal moves
    void generateKnightMoves(bool whiteToMove);
    void generateBishopMoves(bool whiteToMove);
    void generateRookMoves(bool whiteToMove);
    void generateKingMoves(bool whiteToMove);
    void generateQueenMoves(bool whiteToMove);
    void generatePawnAdvances(bool whiteToMove);
    void generateCastlesWhite(int castlingRights, bool inCheck);
    void generateCastlesBlack(int castlingRights, bool inCheck);
    void generatePawnAdvancesWhite();
    void generatePawnAdvancesBlack();

    // generate captures
    void generatePawnCaptures(bool whiteToMove);
    void generateEnPassant(bool whiteToMove, const U64 &enPassantSquare);
    void generateKnightCaptures(bool whiteToMove);
    void generateBishopCaptures(bool whiteToMove);
    void generateRookCaptures(bool whiteToMove);
    void generateQueenCaptures(bool whiteToMove);
    void generateKingCaptures(bool whiteToMove);

    // utility
    bool isCapture(int targetSquare, bool whiteToMove) const;
    bool decodeCastlingRight(int castlingRights, int right);

};


