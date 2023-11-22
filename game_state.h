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
    //Move related
    bool getWhiteToMove() const;
    void addMoveToHistory(Move);
    Move getLastMove();
    void deleteLastMove();
    void passTurn();

    // for FEN parsing
    void setHalfMoveClock(int);
    void setMoveNum(int);
    void setWhiteToMove(bool);

    // Castling
    void setCastlingRight(int index);
    bool getCastlingRight(int index) const;
    void resetCastlingRights();
    bool whiteKingsRookCaptured();
    bool whiteQueensRookCaptured();
    bool blackKingsRookCaptured();
    bool blackQueensRookCaptured();
    bool whiteKingMoved();
    bool blackKingMoved();
    bool blackKingsRookMoved();
    bool blackQueensRookMoved();
    bool whiteKingsRookMoved();
    bool whiteQueensRookMoved();
    void updateCastlingRights();

    // En-Passant
    U64 getEnPassantSquare() const;
    void setEnPassantSquare(int square);
    void resetEnPassantSquare();


private:

    // game state attributes
    bool whiteToMove{};
    bool KingInCheck{};
    int halfmoveClock{};
    int moveNum{};
    U64 enPassantSquare{};
    bool castlingRights[4]{};
    std::list<Move> gameHistory;
};


