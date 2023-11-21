//
// Created by Christian Sejersen on 21/11/2023.
//

#ifndef BBCHESS_ENGINE_MOVEGENERATOR_H
#define BBCHESS_ENGINE_MOVEGENERATOR_H


#include "bitboards.h"

class MoveGenerator : public BitBoard {

public:
// Move generation
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
};


#endif //BBCHESS_ENGINE_MOVEGENERATOR_H
