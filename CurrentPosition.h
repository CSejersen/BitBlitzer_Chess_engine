//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "Move.h"
#include "move_new.h"
#include "board_constants.h"
#include <list>
#include <iostream>
#include <vector>
#include "BitBoard.h"

enum nCastleingRight{
    whiteKingSide = 1,
    whiteQueenSide,
    blackKingSide,
    blackQueenSide,
};

struct Position{
    int castlingRights;
    bool whiteToMove;
    U64 enPassantSquare;
    bool whiteInCheck;
    bool blackInCheck;
    int previousMove;
    U64 boardPosition[14];
};

class CurrentPosition {
public:
    explicit CurrentPosition(BitBoard* board);
    BitBoard* _board;
    //Move related
    bool getWhiteToMove() const;
    void addPositionToHistory(int move);
    Position getPrevPosition();
    void revertToPreviousPosition();
    void deleteLastPositionFromHistory();
    void passTurn();
    void makeMove(int&);
    void undoMove();

    // for FEN parsing
    void setHalfMoveClock(int);
    void setMoveNum(int);
    void setWhiteToMove(bool);

    // Castling
    void setCastlingRight(int enumRight);
    void removeCastlingRight(int enumRight);
    bool getCastlingRight(int enumRight) const;
    void resetCastlingRights();

    // En-Passant
    U64 getEnPassantSquare() const;
    void setEnPassantSquare(int square);
    void resetEnPassantSquare();

    // Debug
    void printCastlingRights();


private:

    // game state attributes
    bool whiteToMove;
    int halfmoveClock;
    int moveNum;
    U64 enPassantSquare;
    int castlingRights;
    int previousMove;
    bool blackInCheck;
    bool whiteInCheck;

    std::vector<Position> gameHistory;

    void handleCaptureFlag(int targetSquare);

    void handleEnPassantFlag(uint8_t targetSquare);

    void handleCastlesQueenSide();

    void handleCastlesKingSide();

};


