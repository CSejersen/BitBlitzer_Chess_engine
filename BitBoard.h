#pragma once
#include "Move.h"
#include "board_constants.h"
#include "GameState.h"
#include <string>
#include <iostream>
#include "bit_manipulation.h"

class BitBoard{
public:
    explicit BitBoard(GameState*);
    GameState* _state;
    void makeMove(Move&);
    void undoMove();
    void placePiece(int piece, int square);
    U64 getPieceSet(enumPieceBB) const;
    void clearBoard();

    // Board indexing util functions
    int coordinateToIndex(std::string coordinate);
    std::string indexToCoordinate(int);

    // debug functions
    static void printBB(const U64& bb) ;

private:
    // bitboards containing current position
    U64 pieceBB[14];

    // flag handlers for makeMove()
    void handleEnPassantFlag(Move&);
    void handleCaptureFlag(Move&);
    void handleCastlesKingSide();
    void handleCastlesQueenSide();
};
