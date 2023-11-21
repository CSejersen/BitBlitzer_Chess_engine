//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "move.h"
#include "board_constants.h"
#include <list>

enum nCastleingRight{
    whiteKingSide,
    whiteQueenSide,
    blackKingSide,
    blackQueenSide,
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
    bool whiteKingMoved();
    bool blackKingMoved();
    bool blackKingsRookMoved();
    bool blackQueensRookMoved();
    bool whiteKingsRookMoved();
    bool whiteQueensRookMoved();
    void updateCastlingRights();

    bool castlingRights[4]{};

private:
    int halfmoveClock{};
    int moveNum{};
    bool KingInCheck{};
    bool whiteToMove{};
    U64 enPassantSquare{};


    std::list<Move> gameHistory;
};


