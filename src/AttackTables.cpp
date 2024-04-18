//
// Created by Christian Sejersen on 21/11/2023.
//
#include "AttackTables.h"

AttackTables::AttackTables(BitBoard *board) {

  generateRookAttackMasks();
  generateBishopAttackMasks();
  loadAttackTables();
  _board = board;
}

void AttackTables::generateBishopAttackMasks() {
  for (int sq = A1; sq <= H8; sq++) {
    // making sure we start with empty bitboard.
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

// generate bishop attacks from a square and a bitboard with all pieces.
// used to initialize attack tables
U64 AttackTables::bishopAttacksOnTheFly(int sq, U64 blockers) {
  U64 attacks = 0ULL;

  int file = sq % 8; // file ranging from 0-7
  int rank = sq / 8; // rank ranging from 0-7

  for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
    attacks |= (1ULL << ((r * 8) + f));
    if (blockers & (1ULL << ((r * 8) + f)))
      break;
  }
  for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
    attacks |= 1ULL << ((r * 8) + f);
    if (blockers & (1ULL << ((r * 8) + f)))
      break;
  }
  for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
    attacks |= 1ULL << ((r * 8) + f);
    if (blockers & (1ULL << ((r * 8) + f)))
      break;
  }
  for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
    attacks |= 1ULL << ((r * 8) + f);
    if (blockers & (1ULL << ((r * 8) + f)))
      break;
  }
  return attacks;
}

void AttackTables::generateRookAttackMasks() {
  for (int sq = A1; sq <= H8; sq++) {
    // making sure we start at 0
    this->rookAttackMask[sq] = 0ULL;

    int file = sq % 8; // file ranging from 0-7
    int rank = sq / 8; // rank ranging from 0-7

    // generating attack mask
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

U64 AttackTables::rookAttacksOnTheFly(int sq, U64 blockers) {
  U64 attacks = 0ULL;

  int file = sq % 8; // file ranging from 0-7
  int rank = sq / 8; // rank ranging from 0-7

  // generating attack mask
  //  North movement
  for (int r = rank + 1; r <= 7; r++) {
    attacks |= (1ULL << ((r * 8) + file));
    if ((blockers & (1ULL << ((r * 8) + file))))
      break;
  }
  // East movement
  for (int f = file - 1; f >= 0; f--) {
    attacks |= 1ULL << ((rank * 8) + f);
    if ((blockers & (1ULL << ((rank * 8) + f))))
      break;
  }
  // West movement
  for (int f = file + 1; f <= 7; f++) {
    attacks |= 1ULL << ((rank * 8) + f);
    if ((blockers & (1ULL << ((rank * 8) + f))))
      break;
  }
  // South movement
  for (int r = rank - 1; r >= 0; r--) {
    attacks |= 1ULL << ((r * 8) + file);
    if ((blockers & (1ULL << ((r * 8) + file))))
      break;
  }

  return attacks;
}
U64 AttackTables::generateBlockers(int patternIndex, int bitsInMask,
                                   U64 mask) const {
  U64 blockers = 0ULL;
  U64 attackMask = mask;
  for (int i = 0; i < bitsInMask; i++) {
    int square = getLSB(attackMask);
    clearBit(attackMask, square);
    if (patternIndex & (1 << i)) {
      blockers |= 1ULL << square;
    }
  }
  return blockers;
}

void AttackTables::loadAttackTables() {
  // Leaping pieces (king, nights, pawns) are implemented using simple 1D-array
  // look-up tables. Sliding pieces use magic number indexing to look up blocker
  // patterns

  // Generating lookup table for knights
  U64 knight = 0ULL;
  for (int i = A1; i <= H8; i++) {
    // placing knight on current square
    setBit(knight, i);
    // generate attacks
    knightAttacks[i] = (((knight >> 6) | (knight << 10)) & ~FILE_GH) |
                       (((knight >> 10) | (knight << 6)) & ~FILE_AB) |
                       (((knight >> 15) | (knight << 17)) & ~FILE_H) |
                       (((knight >> 17) | (knight << 15)) & ~FILE_A);
    // removing knight from board
    clearBit(knight, i);
  }

  // Kings
  U64 king = 0LL;
  for (int i = A1; i <= H8; i++) {
    setBit(king, i);
    this->kingAttacks[i] =
        (((king << 9) | (king << 1) | (king >> 7)) & ~FILE_H) |
        ((king << 8) | (king >> 8)) |
        (((king << 7) | (king >> 1) | (king >> 9)) & ~FILE_A);
    clearBit(king, i);
  }

  // Pawns
  U64 pawn = 0ULL;
  for (int color = white; color <= black; color++) {
    for (int i = A1; i <= H8; i++) {
      if (color == white) {
        setBit(pawn, i);
        this->pawnAttacks[i][color] =
            ((pawn << 7) & ~FILE_A) | ((pawn << 9) & ~FILE_H);
        clearBit(pawn, i);
      } else {
        setBit(pawn, i);
        this->pawnAttacks[i][color] =
            ((pawn >> 7) & ~FILE_H) | ((pawn >> 9) & ~FILE_A);
        clearBit(pawn, i);
      }
    }
  }
  // implement sliding pieces (bishops, rooks, queens) with 2D-array lookup
  // tables, every square can be looked up for all possible blocker patterns.

  // Rooks
  for (int i = A1; i <= H8; i++) {
    // getting relevant attack mask
    U64 mask = rookAttackMask[i];
    // counting number of attacked squares
    int bitCount = countBits(mask);
    // calculating number of possible blocker patterns
    int blockerVariations = 1 << bitCount;

    // looping over all possible blocker variations. think of index as a binary
    // mask.
    for (int index = 0; index < blockerVariations; index++) {

      U64 blockers = generateBlockers(index, bitCount, mask);
      U64 magicIndex = (blockers * rookMagics[i]) >> (64 - rookRelevantBits[i]);
      rookAttacks[i][magicIndex] = rookAttacksOnTheFly(i, blockers);
    }
  }

  // Bishops
  for (int i = A1; i <= H8; i++) {
    U64 mask = bishopAttackMask[i];
    int bitCount = countBits(mask);
    int blockerVariations = 1 << bitCount;

    for (int index = 0; index < blockerVariations; index++) {
      U64 blockers = generateBlockers(index, bitCount, mask);
      U64 magicIndex =
          (blockers * bishopMagics[i]) >> (64 - bishopRelevantBits[i]);
      bishopAttacks[i][magicIndex] = bishopAttacksOnTheFly(i, blockers);
    }
  }
}
U64 AttackTables::getAttacksWhite() const {
  U64 attacks = 0ULL;
  U64 pawns = _board->getPieceSet(nWhitePawn);
  U64 knights = _board->getPieceSet(nWhiteKnight);
  U64 bishops = _board->getPieceSet(nWhiteBishop);
  U64 rooks = _board->getPieceSet(nWhiteRook);
  U64 queens = _board->getPieceSet(nWhiteQueen);
  U64 king = _board->getPieceSet(nWhiteKing);
  while (pawns) {
    int square = getLSB(pawns);
    attacks |= pawnAttacks[square][white];
    clearBit(pawns, square);
  }
  while (knights) {
    int square = getLSB(knights);
    attacks |= knightAttacks[square];
    clearBit(knights, square);
  }
  while (bishops) {
    int square = getLSB(bishops);
    attacks |= getBishopAttacks(square);
    clearBit(bishops, square);
  }
  while (rooks) {
    int square = getLSB(rooks);
    attacks |= getRookAttacks(square);
    clearBit(rooks, square);
  }
  while (queens) {
    int square = getLSB(queens);
    attacks |= (getRookAttacks(square) | getBishopAttacks(square));
    clearBit(queens, square);
  }
  while (king) {
    int square = getLSB(king);
    attacks |= kingAttacks[square];
    clearBit(king, square);
  }
  return (attacks & ~_board->getPieceSet(nWhite));
}

U64 AttackTables::getAttacksBlack() const {
  // get all black pieces
  U64 attacks = 0ULL;
  U64 pawns = _board->getPieceSet(nBlackPawn);
  U64 knights = _board->getPieceSet(nBlackKnight);
  U64 bishops = _board->getPieceSet(nBlackBishop);
  U64 rooks = _board->getPieceSet(nBlackRook);
  U64 queens = _board->getPieceSet(nBlackQueen);
  U64 king = _board->getPieceSet(nBlackKing);

  // generate attacks for every piece
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
    attacks |= getBishopAttacks(square);
    clearBit(bishops, square);
  }
  while (rooks) {
    int square = getLSB(rooks);
    attacks |= getRookAttacks(square);
    clearBit(rooks, square);
  }
  while (queens) {
    int square = getLSB(queens);
    attacks |= (getRookAttacks(square) | getBishopAttacks(square));
    clearBit(queens, square);
  }
  while (king) {
    int square = getLSB(king);
    attacks |= kingAttacks[square];
    clearBit(king, square);
  }
  // adding with complement of black pieces to remove attacks on own pieces.
  return (attacks & ~_board->getPieceSet(nBlack));
}

U64 AttackTables::getRookAttacks(int square) const {
  U64 blockers = _board->getAllPieces();
  blockers &= rookAttackMask[square];
  U64 magicIndex =
      (blockers * rookMagics[square]) >> (64 - rookRelevantBits[square]);
  return rookAttacks[square][magicIndex];
}

U64 AttackTables::getBishopAttacks(int square) const {
  U64 blockers = _board->getAllPieces();
  blockers &= bishopAttackMask[square];
  U64 magicIndex =
      (blockers * bishopMagics[square]) >> (64 - bishopRelevantBits[square]);

  return bishopAttacks[square][magicIndex];
}

U64 AttackTables::getQueenAttacks(int square) const {
  U64 attacks = 0;
  attacks |= getRookAttacks(square);
  attacks |= getBishopAttacks(square);
  return attacks;
}


U64 AttackTables::getKnightAttacks(int square) const {
  return knightAttacks[square];
}

U64 AttackTables::getPawnAttacksWhite(int square) const {
  return pawnAttacks[square][white];
}

U64 AttackTables::getPawnAttacksBlack(int square) const {
  return pawnAttacks[square][black];
}

U64 AttackTables::getKingAttacks(int square) const {
  return kingAttacks[square];
}

bool AttackTables::squareAttackedBy(int square, bool white) {
  //  attacked by white
  if (white) {
    // attacked by rook or queen
    if (getRookAttacks(square) &
        (_board->getPieceSet(nWhiteRook) | _board->getPieceSet(nWhiteQueen))) {
      return true;
    }
    // attacked by bishop or queen
    if (getBishopAttacks(square) & (_board->getPieceSet(nWhiteBishop) |
                                    _board->getPieceSet(nWhiteQueen))) {
      return true;
    }
    // attacked by knight
    if (getKnightAttacks(square) & _board->getPieceSet(nWhiteKnight)) {
      return true;
    }
    // ect...
    if (getPawnAttacksBlack(square) & _board->getPieceSet(nWhitePawn)) {
      return true;
    }
    return false;
  }
  //  attacked by black
  else {
    if (getRookAttacks(square) &
        (_board->getPieceSet(nBlackRook) | _board->getPieceSet(nBlackQueen))) {
      return true;
    }
    if (getBishopAttacks(square) & (_board->getPieceSet(nBlackBishop) |
                                    _board->getPieceSet(nBlackQueen))) {
      return true;
    }
    if (getKnightAttacks(square) & _board->getPieceSet(nBlackKnight)) {
      return true;
    }
    if (getPawnAttacksWhite(square) & _board->getPieceSet(nBlackPawn)) {
      return true;
    }
    return false;
  }

  return false;
}
