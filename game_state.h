//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "move.h"
#include "board_constants.h"
#include <list>

enum nCastleingRight{
    whiteKingside,
    whiteQueenSide,
    blackKingside,
    blackQueenside,
};

class GameState {
public:
    GameState();
    bool getWhiteToMove() const;
    void setWhiteToMove(bool);
    void resetCastlingRights();
    void setCastlingRight(nCastleingRight index);
    bool getCastlingRight(nCastleingRight index) const;
    U64 getEnPassantSquare() const;
    void setEnPassantSquare(int square);
    void resetEnPassantSquare();
    void setHalfMoveClock(int);
    void setMoveNum(int);
    void passTurn();
    void addMoveToHistory(Move);
    Move getLastMove();
    void deleteLastMove();


private:
    bool castlingRights[4]{};
    int halfmoveClock{};
    int moveNum{};
    bool KingInCheck{};
    bool whiteToMove{};
    U64 enPassantSquare{};

    std::list<Move> gameHistory;
};


