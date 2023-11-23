//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "move.h"
#include "board_constants.h"
#include <list>
#include <iostream>

enum nCastleingRight{
    whiteKingSide,
    whiteQueenSide,
    blackKingSide,
    blackQueenSide,
};

class GameState {
public:
    GameState();
    //Move related
    bool getWhiteToMove() const;
    void addMoveToHistory(Move);
    Move getLastMove();
    void deleteLastMoveFromHistory();
    void passTurn();

    // for FEN parsing
    void setHalfMoveClock(int);
    void setMoveNum(int);
    void setWhiteToMove(bool);

    // Castling
    void setCastlingRight(int index);
    bool getCastlingRight(int index) const;
    void resetCastlingRights();
    bool whiteKingsRookCaptured() const;
    bool whiteQueensRookCaptured() const;
    bool blackKingsRookCaptured() const;
    bool blackQueensRookCaptured() const;
    bool whiteKingMoved() const;
    bool blackKingMoved() const;
    bool blackKingsRookMoved() const;
    bool blackQueensRookMoved() const;
    bool whiteKingsRookMoved() const;
    bool whiteQueensRookMoved() const;
    void updateCastlingRights();

    // En-Passant
    U64 getEnPassantSquare() const;
    void setEnPassantSquare(int square);
    void resetEnPassantSquare();

    // Debug
    void printCastlingRights();


private:

    // game state attributes
    bool whiteToMove{};
    int halfmoveClock{};
    int moveNum{};
    U64 enPassantSquare{};
    bool castlingRights[4]{};
    std::list<Move> gameHistory;
    bool whiteInCheck;
    bool blackInCheck;
};


