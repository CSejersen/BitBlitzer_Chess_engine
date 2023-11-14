#pragma once
#include "move.h"
#include <cstdio>
#include <iostream>
#include <vector>

typedef uint64_t U64;

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

enum enumPieceBB
{
    nWhite,
    nBlack,
    nWhitePawn,
    nBlackPawn,
    nWhiteRook,
    nBlackRook,
    nWhiteKnight,
    nBlackKnight,
    nWhiteBishop,
    nBlackBishop,
    nWhiteQueen,
    nBlackQueen,
    nWhiteKing,
    nBlackKing,
};

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

inline U64 south(U64 bb){return bb >> 8;}
inline U64 north(U64 bb){return bb << 8;}
inline U64 east(U64 bb){return (bb & ~FILE_H) >> 1;}
inline U64 west(U64 bb){return (bb & ~FILE_A) << 1;}
inline U64 northEast(U64 bb){return (bb & ~FILE_H) << 7;}
inline U64 northWest(U64 bb){return (bb & ~FILE_A) << 9;}
inline U64 southEast(U64 bb){return (bb & ~FILE_H) >> 9;}
inline U64 southWest(U64 bb){return (bb & ~FILE_A) >> 7;}

class BitBoard{
public:
    BitBoard();
    U64 getPieceSet(enumPieceBB) const;
    void placePiece(enumPieceBB, enumSquareBB);
    void loadAttackTables();
    static void set_bit(U64&, int);
    static U64 get_bit(U64&, int) ;
    static void clear_bit(U64&, int i);
    static int get_LSB(U64) ;
    static void printBB(const U64& bb) ;

    U64 knightAttacks[64];
    U64 kingAttacks[64];
    U64 pawnAttacks[64];


private:
    U64 pieceBB[14];
    const U64 emptyBB = 0ULL;
    const U64 occupiedBB = 0xffffffffffffffffULL;



};