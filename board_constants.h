#pragma once
#include <cstdint>

typedef uint64_t U64;

// Enumeration for squares A1 = 0, H8 = 63
enum enumSquare
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
    nWhitePawn,
    nWhiteKnight,
    nWhiteBishop,
    nWhiteRook,
    nWhiteQueen,
    nWhiteKing,
    nBlack,
    nBlackPawn,
    nBlackKnight,
    nBlackBishop,
    nBlackRook,
    nBlackQueen,
    nBlackKing,
};

// rook relevant blocker bits
extern const int rookRelevantBits[64];
// bishop relevant blocker bits
extern const int bishopRelevantBits[64];
// constants for using ranks and files with bitboards
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
