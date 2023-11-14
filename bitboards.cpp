#include "bitboards.h"

// constants for using ranks and files with bitboards
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



// Bitboards constructor
BitBoard::BitBoard(){
    pieceBB[nWhite] =           0b0000000000000000000000000000000000000000000000001111111111111111;
    pieceBB[nBlack] =           0b1111111111111111000000000000000000000000000000000000000000000000;

    pieceBB[nWhitePawn] =       0b0000000000000000000000000000000000000000000000001111111100000000;
    pieceBB[nWhiteRook] =       0b0000000000000000000000000000000000000000000000000000000010000001;
    pieceBB[nWhiteKnight] =     0b0000000000000000000000000000000000000000000000000000000001000010;
    pieceBB[nWhiteBishop] =     0b0000000000000000000000000000000000000000000000000000000000100100;
    pieceBB[nWhiteQueen] =      0b0000000000000000000000000000000000000000000000000000000000001000;
    pieceBB[nWhiteKing] =       0b0000000000000000000000000000000000000000000000000000000000010000;

    pieceBB[nBlackPawn] =       0b0000000011111111000000000000000000000000000000000000000000000000;
    pieceBB[nBlackRook] =       0b1000000100000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackKnight] =     0b0100001000000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackBishop] =     0b0010010000000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackQueen] =      0b0000100000000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackKing] =       0b0001000000000000000000000000000000000000000000000000000000000000;
}

// returns the bitboard for the requested piece
U64 BitBoard::getPieceSet(enumPieceBB pieceType) const {
    return pieceBB[pieceType];
}

// places piece on a given square of associated Bitboard
void BitBoard::placePiece(enumPieceBB pieceType, enumSquareBB square){
    U64 placementMask = 1ULL << square;
    pieceBB[pieceType] |= placementMask;
}

// Functions to set and get bits on Bitboards.
void BitBoard::set_bit(U64 &bb, int i){
    (bb) |= (1ULL << i);
}
U64 BitBoard::get_bit(U64 &bb, int i) {
    return (bb) & (1ULL << i);
}
void BitBoard::clear_bit(U64 &bb, int i) {
    (bb) &= ~(1ULL << i);
}
int BitBoard::get_LSB(U64 bb) {
    return (__builtin_ctzll(bb));
}

// prints a nice view of bitboard for a given piece.
void BitBoard::printBB(const U64& bb) {
    U64 shiftMe = 1ULL;
    for(int file = 7; file >= 0; file--){
        for(int i = 0; i < 8; i++){
            if((shiftMe << ((file * 8) + i)) & bb){
                std::cout << "x ";
            }
            else{
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void BitBoard::loadAttackTables() {
    // Leaping pieces (king, nights, pawn) are implemented as simple look-up tables
    // sliding pieces will require another solution as they can be blocked.

    // Knights
    U64 knight = 0ULL;
    for (int i = 0; i <= 63; i++) {
        set_bit(knight, i);
        this->knightAttacks[i] = (((knight >> 6) | (knight << 10)) & ~FILE_GH) |
                                 (((knight >> 10) | (knight << 6)) & ~FILE_AB) |
                                 (((knight >> 15) | (knight << 17)) & ~FILE_H) |
                                 (((knight >> 17) | (knight << 15)) & ~FILE_A);
        clear_bit(knight, i);
    }
    // Kings
    U64 king = 0LL;
    for (int i = 0; i <= 63; i++) {
        set_bit(king, i);
        this->kingAttacks[i] = (((king << 9) | (king << 1) | (king >> 7)) & ~FILE_H) |
                               ((king << 8) | (king >> 8)) |
                               (((king << 7) | (king >> 1) | (king >> 9)) & ~FILE_A);
        clear_bit(king, i);
    }
    // Pawns
    U64 pawn = 0ULL;
    for(int i = 0; i <= 63; i++){
        set_bit(pawn,i);
        this->pawnAttacks[i] = ((pawn << 7 ) & ~FILE_A) | ((pawn << 9) & ~FILE_H);
        clear_bit(pawn,i);
    }
}


