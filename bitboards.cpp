#include "bitboards.h"
#include "magics.h"

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

// rook rellevant occupancy bits
int rookRellevantBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

// bishop rellevant occupancy bits
int bishopRellevantBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

// Bitboards constructor
BitBoard::BitBoard(){
    // generate attack masks for magic bitboards, results used by loadAttackTables().
    generateBishopAttackMasks();
    generateRookAttackMasks();

    // Calculate all attack-tables
    loadAttackTables();

    pieceBB[nWhite] =           0b0000000000000000000000000000000000000000000000001111111111111111;
    pieceBB[nBlack] =           0b1111111111111111000000000000000000000000000000000000000000000000;

    pieceBB[nWhitePawn] =       0b0000000000000000000000000000000000000000000000001111111100000000;
    pieceBB[nWhiteKnight] =     0b0000000000000000000000000000000000000000000000000000000001000010;
    pieceBB[nWhiteBishop] =     0b0000000000000000000000000000000000000000000000000000000000100100;
    pieceBB[nWhiteRook] =       0b0000000000000000000000000000000000000000000000000000000010000001;
    pieceBB[nWhiteQueen] =      0b0000000000000000000000000000000000000000000000000000000000001000;
    pieceBB[nWhiteKing] =       0b0000000000000000000000000000000000000000000000000000000000010000;

    pieceBB[nBlackPawn] =       0b0000000011111111000000000000000000000000000000000000000000000000;
    pieceBB[nBlackKnight] =     0b0100001000000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackBishop] =     0b0010010000000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackRook] =       0b1000000100000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackQueen] =      0b0000100000000000000000000000000000000000000000000000000000000000;
    pieceBB[nBlackKing] =       0b0001000000000000000000000000000000000000000000000000000000000000;
}

// returns the bitboard for the requested piece
U64 BitBoard::getPieceSet(enumPieceBB pieceType) const {
    return pieceBB[pieceType];
}

// places piece on a given square of associated Bitboard
void BitBoard::placePiece(int pieceType, int square){
    U64 placementMask = 1ULL << square;
    pieceBB[pieceType] |= placementMask;
    if (pieceType < 8) {
        pieceBB[nWhite] |= placementMask;
    }
    else{
        pieceBB[nBlack] |= placementMask;
    }

}

// Functions to set and get bits on Bitboards.
void BitBoard::setBit(U64 &bb, int i){
    (bb) |= (1ULL << i);
}
U64 BitBoard::getBit(U64 &bb, int i) {
    return (bb) & (1ULL << i);
}
void BitBoard::clearBit(U64 &bb, int i) {
    (bb) &= ~(1ULL << i);
}
int BitBoard::getLSB(const U64& bb) {
    return (__builtin_ctzll(bb));
}
int BitBoard::countBits(U64 bb) {
    // bit count
    int count = 0;
    // pop bits until bitboard is empty
    while (bb) {
        // increment count
        count++;
        // consecutively reset least significant 1st bit
        bb &= bb - 1;

    }
    return count;
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

void BitBoard::generateBishopAttackMasks() {
    for (int sq = A1; sq <= H8; sq++) {
        // making sure we start at 0
        this->bishopAttackMask[sq] = 0ULL;

        int file = sq % 8; // file ranging from 0-7
        int rank = sq / 8; // rank ranging from 0-7

        for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++) {
            this->bishopAttackMask[sq] |= (1ULL << ((r * 8) + f));
        }
        for (int r = rank + 1, f = file - 1; r <= 6 && f > 0; r++, f--) {
            this->bishopAttackMask[sq] |= 1ULL << ((r * 8) + f);
        }
        for (int r = rank - 1, f = file + 1; r > 0 && f <= 6; r--, f++) {
            this->bishopAttackMask[sq] |= 1ULL << ((r * 8) + f);
        }
        for (int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--) {
            this->bishopAttackMask[sq] |= 1ULL << ((r * 8) + f);
       }
    }
}

U64 BitBoard::bishopAttacksOnTheFly(int sq, U64 blockers) {
    U64 attacks = 0ULL;

    int file = sq % 8; // file ranging from 0-7
    int rank = sq / 8; // rank ranging from 0-7

    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << ((r * 8) + f));
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= 1ULL << ((r * 8) + f);
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= 1ULL << ((r * 8) + f);
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= 1ULL << ((r * 8) + f);
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    return attacks;
}

void BitBoard::generateRookAttackMasks() {
    for (int sq = A1; sq <= H8; sq++) {
        // making sure we start at 0
        this->rookAttackMask[sq] = 0ULL;

        int file = sq % 8; // file ranging from 0-7
        int rank = sq / 8; // rank ranging from 0-7

        //generating attack mask
        for (int r = rank + 1; r <= 6; r++) {
            this->rookAttackMask[sq] |= (1ULL << ((r * 8) + file));
        }
        for (int f = file - 1; f > 0; f--) {
            this->rookAttackMask[sq] |= 1ULL << ((rank * 8) + f);
        }
        for (int f = file + 1; f <= 6; f++) {
            this->rookAttackMask[sq] |= 1ULL << ((rank * 8) + f);
        }
        for (int r = rank - 1; r > 0; r--) {
            this->rookAttackMask[sq] |= 1ULL << ((r * 8) + file);
        }
    }
}

U64  BitBoard::rookAttacksOnTheFly(int sq, U64 blockers) {
    U64  attacks = 0ULL;

    int file = sq % 8; // file ranging from 0-7
    int rank = sq / 8; // rank ranging from 0-7

    //generating attack mask
    // North movement
    for (int r = rank + 1; r <= 7; r++) {
        attacks |= (1ULL << ((r * 8) + file));
        if((blockers & (1ULL << ((r*8) + file)))) break;
    }
    // East movement
    for (int f = file - 1; f >= 0; f--) {
        attacks |= 1ULL << ((rank * 8) + f);
        if((blockers & (1ULL << ((rank*8) + f)))) break;
    }
    // West movement
    for (int f = file + 1; f <= 7; f++) {
        attacks |= 1ULL << ((rank * 8) + f);
        if((blockers & (1ULL << ((rank *8 ) + f)))) break;
    }
    // South movement
    for (int r = rank - 1; r >= 0; r--) {
        attacks |= 1ULL << ((r * 8) + file);
        if((blockers & (1ULL << ((r*8) + file)))) break;
    }

    return attacks;
}

U64 BitBoard::generateBlockers(int patternIndex, int bitsInMask, U64 mask) {
    U64 blockers = 0ULL;
    U64 attackMask = mask;
    for (int i = 0; i < bitsInMask; i++) {
        U64 square = getLSB(attackMask);
        clearBit(attackMask, square);
        if (patternIndex & (1 << i)) {
            blockers |= 1ULL << square;
        }
    }
    return blockers;
}

void BitBoard::clearBoard(){
    for(U64& pieceSet : pieceBB){
        pieceSet = 0ULL;
    }
}
void BitBoard::loadPosition(const std::string& fen) {
    clearBoard();
}

U64 BitBoard::getAttacks(bool whiteToPlay, U64 blockers) {
    U64 attacks = 0ULL;
    if(whiteToPlay){
        U64 pawns = getPieceSet(nWhitePawn);
        U64 knights = getPieceSet(nWhiteKnight);
        U64 bishops = getPieceSet(nWhiteBishop);
        U64 rooks = getPieceSet(nWhiteRook);
        U64 queens = getPieceSet(nWhiteQueen);
        U64 king = getPieceSet(nWhiteKing);
        while(pawns){
            int square = getLSB(pawns);
            attacks |= pawnAttacks[square][white];
            clearBit(pawns,square);
        }
        while(knights){
            int square = getLSB( knights);
            attacks |= knightAttacks[square];
            clearBit(knights,square);
        }
        while(bishops){
            int square = getLSB( bishops);
            attacks |= getBishopAttacks(square,blockers);
            clearBit(bishops,square);
        }
        while(rooks){
            int square = getLSB(rooks);
            attacks |= getRookAttacks(square, blockers);
            clearBit(rooks,square);
        }
        while(queens){
            int square = getLSB(queens);
            attacks |= (getRookAttacks(square,blockers) | getBishopAttacks(square,blockers));
            clearBit(queens,square);
        }
        while(king){
            int square = getLSB(king);
            attacks |= kingAttacks[square];
            clearBit(king,square);
        }
        }
    else {
        U64 pawns = getPieceSet(nBlackPawn);
        U64 knights = getPieceSet(nBlackKnight);
        U64 bishops = getPieceSet(nBlackBishop);
        U64 rooks = getPieceSet(nBlackRook);
        U64 queens = getPieceSet(nBlackQueen);
        U64 king = getPieceSet(nBlackKing);
        while (pawns) {
            int square = getLSB(pawns);
            attacks |= pawnAttacks[square][black];
            clearBit(pawns, square);
        }
        while (knights) {
            int square = getLSB(knights);
            attacks |= knightAttacks[square];
            clearBit(knights, square);
        }
        while (bishops) {
            int square = getLSB(bishops);
            attacks |= getBishopAttacks(square, blockers);
            clearBit(bishops, square);
        }
        while (rooks) {
            int square = getLSB(rooks);
            attacks |= getRookAttacks(square, blockers);
            clearBit(rooks, square);
        }
        while (queens) {
            int square = getLSB(queens);
            attacks |= (getRookAttacks(square, blockers) | getBishopAttacks(square, blockers));
            clearBit(queens, square);
        }
        while (king) {
            int square = getLSB(king);
            attacks |= kingAttacks[square];
            clearBit(king, square);
        }
    }
    if (whiteToPlay){
        return (attacks & ~getPieceSet(nWhite));
    }
    else{
        return (attacks & ~getPieceSet(nBlack));
    }
}
void BitBoard::loadAttackTables() {
    // Leaping pieces (king, nights, pawns) are implemented using simple 1D-array look-up tables.
    // Sliding pieces will require another solution as they can be blocked.

    // Generating lookup table for knights
    U64 knight = 0ULL;
    for (int i = 0; i <= 63; i++) {
        setBit(knight, i);
        this->knightAttacks[i] = (((knight >> 6) | (knight << 10)) & ~FILE_GH) |
                                 (((knight >> 10) | (knight << 6)) & ~FILE_AB) |
                                 (((knight >> 15) | (knight << 17)) & ~FILE_H) |
                                 (((knight >> 17) | (knight << 15)) & ~FILE_A);
        clearBit(knight, i);
    }
    // Kings
    U64 king = 0LL;
    for (int i = 0; i <= 63; i++) {
        setBit(king, i);
        this->kingAttacks[i] = (((king << 9) | (king << 1) | (king >> 7)) & ~FILE_H) |
                               ((king << 8) | (king >> 8)) |
                               (((king << 7) | (king >> 1) | (king >> 9)) & ~FILE_A);
        clearBit(king, i);
    }
    // Pawns
    U64 pawn = 0ULL;
    for(int color = white; color <= black; color++){
        for(int i = A1; i <= H8; i++){
            if(color == white){
                setBit(pawn,i);
                this->pawnAttacks[i][color] = ((pawn << 7 ) & ~FILE_A) | ((pawn << 9) & ~FILE_H);
                clearBit(pawn,i);
            }
            else{
                setBit(pawn,i);
                this->pawnAttacks[i][color] = ((pawn >> 7 ) & ~FILE_H) | ((pawn >> 9) & ~FILE_A);
                clearBit(pawn,i);

            }
        }
    }
    // implement sliding pieces (bishops, rooks, queens) with 2D-array lookup tables,
    // every square can be looked up for all possible blocker patterns.

    // Rooks
    // looping over all 64 squares
    for(int i = A1; i <= H8; i++){
        //getting relevant attack mask
        U64 mask = rookAttackMask[i];
        // counting number of attacked squares
        int bitCount = countBits(mask);
        // calculating number of possible blocker patterns
        int blockerVariations = 1 << bitCount;

        // looping over all possible blocker variations. think of index as a binary mask.
        for (int index = 0; index < blockerVariations; index++){

            U64 blockers = generateBlockers(index, bitCount,mask);
            U64 magicIndex = (blockers * rookMagics[i]) >> (64 - rookRellevantBits[i]);
            rookAttacks[i][magicIndex] = rookAttacksOnTheFly(i,blockers);
        }
    }
    // Bishops

    for(int i = A1; i <=H8; i++){
        U64 mask = bishopAttackMask[i];
        int bitCount = countBits(mask);
        int blockerVariations = 1 << bitCount;

        for (int index = 0; index < blockerVariations; index++){
            U64 blockers = generateBlockers(index,bitCount,mask);
            U64 magicIndex = (blockers * bishopMagics[i]) >> (64 - bishopRellevantBits[i]);
            bishopAttacks[i][magicIndex] = bishopAttacksOnTheFly(i,blockers);
        }
    }
}

U64 BitBoard::getRookAttacks(int square, U64 position) {
    U64 blockers = position & rookAttackMask[square];
    U64 magicIndex = (blockers * rookMagics[square]) >> (64 - rookRellevantBits[square]);

    return rookAttacks[square][magicIndex];
}

U64 BitBoard::getBishopAttacks(int square, U64 position) {
    U64 blockers = position & bishopAttackMask[square];
    U64 magicIndex = (blockers * bishopMagics[square]) >> (64 - bishopRellevantBits[square]);

    return bishopAttacks[square][magicIndex];
}


