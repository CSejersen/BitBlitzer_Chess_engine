#pragma once
#include "move.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <list>
#include "move_generator.h"
#include "board_constants.h"



class BitBoard{
public:
    BitBoard();
    void clearBoard();
    void loadStartingPosition();
    void makeMove(const Move&);
    void undoMove();
    void placePiece(int piece, int square);
    U64 getPieceSet(enumPieceBB) const;

    // Board indexing util functions
    int coordinateToIndex(std::string coordinate);
    std::string indexToCoordinate(int);

    // attack-map getters
    U64 getAttacks(bool) const; // returns all possible attacks for color to move.
    U64 getRookAttacks(int square) const;
    U64 getBishopAttacks(int square) const;
    U64 getKnightAttacks(int square) const;
    U64 getPawnAttacks(int square) const;
    U64 getKingAttacks(int square) const;

    // debug functions
    static void printBB(const U64& bb) ;

private:
    // bitboards containing current position
    U64 pieceBB[14];
    // Arrays of all possible attacks (initialized by BitBoard constructor)
    // rook and bishop 2nd index stores blocker patterns using magic number indexing.
    U64 knightAttacks[64]{};
    U64 bishopAttackMask[64]{};
    U64 rookAttackMask[64]{};
    U64 bishopAttacks[64][512]{};
    U64 rookAttacks[64][4096]{};
    U64 kingAttacks[64]{};
    U64 pawnAttacks[64][2]{}; // last pawn index is for color
};
