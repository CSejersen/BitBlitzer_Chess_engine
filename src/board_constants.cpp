#include "board_constants.h"

// Files and ranks described by hexadecimal numbers.
const U64 FILE_A = 0x8080808080808080ULL;
const U64 FILE_B = 0x4040404040404040ULL;
const U64 FILE_C = 0x2020202020202020ULL;
const U64 FILE_D = 0x1010101010101010ULL;
const U64 FILE_E = 0x0808080808080808ULL;
const U64 FILE_F = 0x0404040404040404ULL;
const U64 FILE_G = 0x0202020202020202ULL;
const U64 FILE_H = 0x0101010101010101ULL;
const U64 FILE_AB = FILE_A | FILE_B;
const U64 FILE_GH = FILE_G | FILE_H;

const U64 RANK_1 = 0xffULL;
const U64 RANK_2 = 0xff00ULL;
const U64 RANK_3 = 0xff0000ULL;
const U64 RANK_4 = 0xff000000ULL;
const U64 RANK_5 = 0xff00000000ULL;
const U64 RANK_6 = 0xff0000000000ULL;
const U64 RANK_7 = 0xff000000000000ULL;
const U64 RANK_8 = 0xff00000000000000ULL;

// rook relevant blocker bits
const int rookRelevantBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

// bishop relevant blocker bits
const int bishopRelevantBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

