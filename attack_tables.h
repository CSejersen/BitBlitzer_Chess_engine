//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "bitboards.h"
#include "magics.h"
#include "game_state.h"
#include "bit_manipulation.h"
#include "board_constants.h"

class AttackTables {
public:
    AttackTables(BitBoard* board, GameState* state);
    BitBoard* _board;
    GameState* _state;
    // attack-map getters
    U64 getAttacksCurrentTurn() const;
    U64 getAttacksWhite() const; // returns all possible attacks for color to move.
    U64 getAttacksBlack() const; // returns all possible attacks for color to move.
    U64 getRookAttacks(int square) const;
    U64 getBishopAttacks(int square) const;
    U64 getKnightAttacks(int square) const;
    U64 getPawnAttacks(int square) const;
    U64 getKingAttacks(int square) const;

private:
    void loadAttackTables();
    U64 generateBlockers(int,int,U64) const;
    void generateBishopAttackMasks();
    void generateRookAttackMasks();
    U64 rookAttacksOnTheFly(int square, U64 blockers);
    U64 bishopAttacksOnTheFly(int square, U64 blockers);


    // Arrays of all possible attacks (initialized by constructor);
    // rook and bishop 2nd index stores blocker patterns using magic number indexing.
    U64 knightAttacks[64]{};
    U64 bishopAttackMask[64]{};
    U64 rookAttackMask[64]{};
    U64 bishopAttacks[64][512]{};
    U64 rookAttacks[64][4096]{};
    U64 kingAttacks[64]{};
    U64 pawnAttacks[64][2]{}; // last pawn index is for color
};


