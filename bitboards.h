#pragma once
#include "move.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <list>

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
enum castleingRight{
    whiteKingside,
    whiteQueenSide,
    blackKingside,
    blackQueenside,
};
enum castleingSide{
    kingSide,
    queenSide,
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
    int coordinateToIndex(std::string coordinate);
    std::list<Move> gameHistory;
    std::list<Move> pseudoLegal;
    std::list<Move> captures;
    std::string indexToCoordinate(int);

    // Move generation
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateQueenMoves();
    void generatePawnAdvances();
    void generatePawnCaptures();
    void generateKingMoves();
    void generateCastles();
    void generateEnPassant();

    //search
    void undoMove();


    // Position loading
    void clearBoard();
    void loadStartingPosition();
    void loadFenPosition(const std::string& fenString);

    // attack-map getters
    U64 getAttacks(bool) const; // returns all possible attacks for color to move.
    U64 getRookAttacks(int square) const;
    U64 getBishopAttacks(int square) const;
    U64 getKnightAttacks(int square) const;
    U64 getPawnAttacks(int square) const;
    U64 getKingAttacks(int square) const;

    // Bit manipulation functions
    static void setBit(U64&, int);
    static U64 getBit(U64&, int);
    static void clearBit(U64&, int i);
    static int getLSB(const U64&);
    static int countBits(U64);

    // debug functions
    static void printBB(const U64& bb) ;
    void placePiece(int piece, int square);
    void makeMove(const Move&);

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

    // Game state
    U64 enPassantSquares{};
    int halfmoveClock{};
    int moveNum{};
    bool whiteToMove{};
    bool castleingRights[4]{};
    bool KingInCheck{};

    //for calculating all attack-tables at compile-time.
    void loadAttackTables();
    U64 generateBlockers(int,int,U64) const;
    void generateBishopAttackMasks();
    void generateRookAttackMasks();
    U64 rookAttacksOnTheFly(int square, U64 blockers);
    U64 bishopAttacksOnTheFly(int square, U64 blockers);

    // FEN parsing utility functions
    int fenParsePieces(std::string& fen); // returns index number reached in fen-string
    int fenParseTurn(int index, std::string& fen); // returns index number reached in fen-string
    int fenParseCastleingRights(int index, std::string& fen); // returns index number reached in fen-string
    int fenParseEnPassant(int index, std::string& fen); // returns index number reached in fen-string
    int fenParseHalfmove(int index, std::string& fen); // returns index number reached in fen-string
    void fenParseMoveNum(int index, std::string& fen); // returns index number reached in fen-string

    // Move generation
    bool isCapture(int targetSquare) const;
    void removeCastlingRight(castleingSide);

};
