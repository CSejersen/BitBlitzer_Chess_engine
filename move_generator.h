//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "bitboards.h"

enum color{
    white,
    black,
};

class move_generator {

public:
    move_generator(&BitBoard);

    BitBoard* board;

    void generateMoves();
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateKingMoves();
    void generateQueenMoves();
    void generatePawnAdvancesWhite();
    void generatePawnAdvancesBlack();
    void generatePawnCaptures();
    void generateEnPassant();
    void generateCastles();
private:
    bool isCapture(int targetSquare) const;
    std::list<Move> pseudoLegal;
    std::list<Move> captures;
};


