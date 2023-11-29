#pragma once
#include "move.h"
#include "board_constants.h"
#include <string>
#include <iostream>
#include "bit_manipulation.h"

class BitBoard{
public:
    explicit BitBoard();

    U64 pieceBB[14];
    void placePiece(int piece, int square);
    void removePiece(int piece, int square);
    U64 getPieceSet(enumPieceBB) const;
    U64 getAllPieces() const;
    void clearBoard();

    // Board indexing util functions
    static int coordinateToIndex(std::string coordinate);
    static std::string indexToCoordinate(int);

    // debug functions
    static void printBB(const U64& bb) ;
private:
};
