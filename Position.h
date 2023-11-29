//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "move.h"
#include "board_constants.h"
#include <iostream>
#include <vector>
#include "BitBoard.h"
#include "AttackTables.h"


struct State{
    int castlingRights;
    bool whiteToMove;
    U64 enPassantSquare = 0;
    bool whiteInCheck;
    bool blackInCheck;
    int previousMove;
    int capturedPiece;
};

class Position {
public:
    explicit Position(BitBoard *board, AttackTables *atkTables);
    BitBoard* _board;
    AttackTables* _atkTables;
    //Move related
    bool getWhiteToMove() const;
    void addStateToHistory(int move);
    State getPrevPosition();
    void deleteLastPositionFromHistory();
    void passTurn();
    bool makeMove(int&);
    void undoMove();
    void undoIllegal(int start, int target, int piece, bool enPassant);

    // for FEN parsing
    void setHalfMoveClock(int);
    void setMoveNum(int);
    void setWhiteToMove(bool);

    // Castling
    void setCastlingRight(int enumRight);
    void removeCastlingRight(int enumRight);
    bool getCastlingRight(int enumRight) const;
    int getCastlingRighs();
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
    int capturedPiece;
    bool blackInCheck;
    bool whiteInCheck;

    std::vector<State> gameHistory;

    void handleCaptureFlag(int targetSquare);

    void handleEnPassantFlag(uint8_t targetSquare);

    void handleCastlesQueenSide();

    void handleCastlesKingSide();

};


