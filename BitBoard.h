#pragma once
#include "Move.h"
#include "move_new.h"
#include "board_constants.h"
#include "GameState.h"
#include <string>
#include <iostream>
#include "bit_manipulation.h"

class BitBoard{
public:
    explicit BitBoard(GameState*);
    GameState* _state;
    void makeMove(int&);
    void undoMove();
    void placePiece(int piece, int square);
    U64 getPieceSet(enumPieceBB) const;
    U64 getAllPieces() const;
    void clearBoard();

    // Board indexing util functions
    int coordinateToIndex(std::string coordinate);
    std::string indexToCoordinate(int);

    // debug functions
    static void printBB(const U64& bb) ;

    U64 pieceBB[14];
private:
    // bitboards containing current position

    // flag handlers for makeMove()
    void handleEnPassantFlag(int&);
    void handleCaptureFlag(int&);
    void handleCastlesKingSide();
    void handleCastlesQueenSide();
};
