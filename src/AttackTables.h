//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "BitBoard.h"
#include "bit_manipulation.h"
#include "board_constants.h"
#include "magics.h"

class AttackTables {
public:
  explicit AttackTables(BitBoard *board);
  BitBoard *_board;
  // attack-map getters
  U64 getAttacksWhite() const; // returns all possible attacks for white.
  U64 getAttacksBlack() const; // returns all possible attacks for black.
  U64 getPawnAttacksWhite(int square) const;
  U64 getPawnAttacksBlack(int square) const;
  U64 getKnightAttacks(int square) const;
  U64 getBishopAttacks(int square) const;
  U64 getRookAttacks(int square) const;
  U64 getQueenAttacks(int square) const;
  U64 getKingAttacks(int square) const;

  bool squareAttackedBy(int square,
                        bool white); // bool white: True = attacked by white,
                                     // False = attacked by black

private:
  // initialization of attack tables.
  // Init is handled by constructor.
  void loadAttackTables();
  U64 generateBlockers(int, int, U64) const;
  void generateBishopAttackMasks();
  void generateRookAttackMasks();
  U64 rookAttacksOnTheFly(int square, U64 blockers);
  U64 bishopAttacksOnTheFly(int square, U64 blockers);

  // Arrays of all possible attacks (initialized by constructor)
  // rook and bishop 2nd index stores blocker patterns using magic number
  // indexing.
  U64 knightAttacks[64]{};
  U64 bishopAttackMask[64]{};
  U64 rookAttackMask[64]{};
  U64 bishopAttacks[64][512]{};
  U64 rookAttacks[64][4096]{};
  U64 kingAttacks[64]{};
  U64 pawnAttacks[64][2]{}; // last index for color
};
