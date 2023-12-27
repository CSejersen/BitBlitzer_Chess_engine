#pragma once
#include "move.h"
#include "board_constants.h"
#include <string>
#include <iostream>
#include "bit_manipulation.h"

class BitBoard{
public:
    explicit BitBoard();

    U64 pieceBB[14]; // Bitboards for all piece sets (1 for white pawns, 1 for black pawns and so on...)
    void placePiece(int piece, int square); // Places piece on the given square
    void removePiece(int piece, int square); // Places piece on a given square
    U64 getPieceSet(int pieceType) const; // returns the bitboard for the requested piece
    U64 getAllPieces() const;
    void clearBoard(); // clears entire board

    // Board indexing util functions
    static int coordinateToIndex(std::string coordinate); // conversion function, example A1 --> 0 or B1 --> 1
    static std::string indexToCoordinate(int); // conversion function, example 0 --> a1 or 1 --> b1

    // Debug
    void printBB() const ; // prints a nice view of the given bitboard.
private:
    void printBB_helper(const U64 &bb, char piece);
};
