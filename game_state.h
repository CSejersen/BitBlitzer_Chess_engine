//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "bitboards.h"

enum nCastleingRight{
    whiteKingside,
    whiteQueenSide,
    blackKingside,
    blackQueenside,
};

class GameState {
public:
    bool getWhiteToMove();
    void setWhiteToMove(bool);
    void resetCastlingRights();
    void setCastlingRight(nCastleingRight enumIndex);
    void setEnPassantSquare(int square);
    void setHalfMoveClock(int);
    void setMoveNum(int);


private:
    bool castlingRights[4]{};
    int halfmoveClock{};
    int moveNum{};
    bool KingInCheck{};
    bool whiteToMove{};
    U64 enPassantSquare{};

    std::list<Move> gameHistory;
};


