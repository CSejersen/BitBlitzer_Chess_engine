#pragma once
#include "move.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <string>

typedef uint64_t U64;

// Enumeration for squares A1 = 0, H8 = 63
enum enumSquareBB
{
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
};

// Enumeration for all 14 piece sets
enum enumPieceBB
{
    nWhite,
    nBlack,
    nWhitePawn,
    nWhiteKnight,
    nWhiteBishop,
    nWhiteRook,
    nWhiteQueen,
    nWhiteKing,
    nBlackPawn,
    nBlackKnight,
    nBlackBishop,
    nBlackRook,
    nBlackQueen,
    nBlackKing,
};

enum color{
    white,
    black,
};

// Files and ranks described by hexadecimal numbers. (Values assigned in bitboards.cpp)
extern const U64 FILE_A;
extern const U64 FILE_B;
extern const U64 FILE_C;
extern const U64 FILE_D;
extern const U64 FILE_E;
extern const U64 FILE_F;
extern const U64 FILE_G;
extern const U64 FILE_H;
extern const U64 FILE_AB;
extern const U64 FILE_GH;

extern const U64 RANK_1;
extern const U64 RANK_2;
extern const U64 RANK_3;
extern const U64 RANK_4;
extern const U64 RANK_5;
extern const U64 RANK_6;
extern const U64 RANK_7;
extern const U64 RANK_8;

// Movement functions using bit-shifts maybe useful later...
//inline U64 south(U64 bb){return bb >> 8;}
//inline U64 north(U64 bb){return bb << 8;}
//inline U64 east(U64 bb){return (bb & ~FILE_H) >> 1;}
//inline U64 west(U64 bb){return (bb & ~FILE_A) << 1;}
//inline U64 northEast(U64 bb){return (bb & ~FILE_H) << 7;}
//inline U64 northWest(U64 bb){return (bb & ~FILE_A) << 9;}
//inline U64 southEast(U64 bb){return (bb & ~FILE_H) >> 9;}
//inline U64 southWest(U64 bb){return (bb & ~FILE_A) >> 7;}

class BitBoard{
public:
    BitBoard();

    // Util functions
    U64 getPieceSet(enumPieceBB) const;
    U64 getRookAttacks(int square, U64 position);
    U64 getBishopAttacks(int square, U64 position);
    U64 generateBlockers(int,int,U64);
    U64 getAttacks(bool WhiteToPlay, U64 blockers);
    void loadPosition(const std::string&);
    void clearBoard();

    // Static bit manipulation functions
    static void setBit(U64&, int);
    static U64 getBit(U64&, int);
    static void clearBit(U64&, int i);
    static int getLSB(const U64&);
    static int countBits(U64);

    // debug functions
    static void printBB(const U64& bb) ;
    void placePiece(int piece, int square);

    // attack look-up tables (loaded by loadAttackTables())
    U64 knightAttacks[64];
    U64 kingAttacks[64];
    // last pawn index is for color
    U64 pawnAttacks[64][2];

    // attack masks to use with magic bitboards


private:
    // bitboards with current game-state
    U64 pieceBB[14];
    // All possible rook and bishop moves stored by blocker patterns using magicNumber indexing
    U64 bishopAttackMask[64];
    U64 rookAttackMask[64];
    U64 bishopAttacks[64][512];
    U64 rookAttacks[64][4096];

    std::vector<Move> pseudoLegalMoves;


    // utility bitboards
    const U64 emptyBB = 0ULL;
    const U64 fullyOccupiedBB = 0xffffffffffffffffULL;

    //for calculating all attack-tables when class is created.
    void loadAttackTables();
    // generates attack masks for magic bitboard implementation
    void generateBishopAttackMasks();
    void generateRookAttackMasks();
    // generate moves for magic bitboards
    U64 rookAttacksOnTheFly(int square, U64 blockers);
    U64 bishopAttacksOnTheFly(int square, U64 blockers);

    bool isKingInCheck();






};