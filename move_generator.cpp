//
// Created by Christian Sejersen on 21/11/2023.
//

#include "move_generator.h"
MoveGenerator::MoveGenerator(BitBoard *board, GameState *state, AttackTables *attackTables) {
    _board = board;
    _state = state;
    _atkTables = attackTables;
}
void MoveGenerator::generateMoves() {
    generateKnightMovesWhite();
    generateBishopMoves();
    generateRookMoves();
    generateKingMoves();
    generateQueenMoves();
    generatePawnAdvances();
    generatePawnCaptures();
    generateEnPassant();
    if(_state->getWhiteToMove()){
        generateCastlesWhite();
    }
    else
        generateCastlesBlack();

    pruneIllegalMoves();
}
void MoveGenerator::generateKnightMovesWhite() {
    U64 knights;
    U64 attacks;
    if(_state->getWhiteToMove())
        knights = _board->getPieceSet(nWhiteKnight);
    else
        knights = _board->getPieceSet(nBlackKnight);

    while(knights){
        U16 startSquare = getLSB(knights);
        clearBit(knights,startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getKnightAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getKnightAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while(attacks){
            U16 targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            U16 flags = 0;

            if(isCapture(targetSquare)) {
                flags = nCapture;
                pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);
            }
            else
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
        }
    }
}
void MoveGenerator::generateBishopMoves() {
    U64 bishops = 0ULL;
    U64 attacks;
    if(_state->getWhiteToMove())
        bishops = _board->getPieceSet(nWhiteBishop);
    else
        bishops = _board->getPieceSet(nBlackBishop);

    while(bishops){
        int startSquare = getLSB(bishops);
        clearBit(bishops,startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        U16 flags = 0;
        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);

            }
            else{
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }


        }

    }
}

void MoveGenerator::generateRookMoves() {
    U64 rooks = 0ULL;
    U64 attacks;
    if(_state->getWhiteToMove())
        rooks = _board->getPieceSet(nWhiteRook);
    else
        rooks = _board->getPieceSet(nBlackRook);

    while(rooks){
        int startSquare = getLSB(rooks);
        clearBit(rooks, startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nBlack);
        U16 flags = 0;
        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);

            }
            else{
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }


        }

    }

}

void MoveGenerator::generateKingMoves() {
    U64 king = 0ULL;
    U64 attacks;
    // getting king of player to move
    if (_state->getWhiteToMove())
        king = _board->getPieceSet(nWhiteKing);
    else
        king = _board->getPieceSet(nBlackKing);

    U16 startSquare = getLSB(king);
    if(_state->getWhiteToMove())
        attacks = _atkTables->getKingAttacks(startSquare) & ~_board->getPieceSet(nWhite);
    else
        attacks = _atkTables->getKingAttacks(startSquare) & ~_board->getPieceSet(nBlack);

    U16 flags = 0;

    while (attacks) {
        int targetSquare = getLSB(attacks);
        clearBit(attacks, targetSquare);

        if (isCapture(targetSquare)) {
            flags = nCapture;
            pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);
        }
        else {
            pseudoLegal.emplace_back(startSquare, targetSquare, flags);
        }
    }
}

void MoveGenerator::generateQueenMoves() {
    U64 queens = 0ULL;
    U64 straightAttacks;
    U64 diagonalAttacks;
    if(_state->getWhiteToMove())
        queens = _board->getPieceSet(nWhiteQueen);
    else
        queens = _board->getPieceSet(nBlackQueen);

    while(queens){
        int startSquare = getLSB(queens);
        clearBit(queens, startSquare);
        if(_state->getWhiteToMove()){
            straightAttacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nWhite);
            diagonalAttacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        }
        else{
            straightAttacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nBlack);
            diagonalAttacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nBlack);
        }

        U64 attacks = straightAttacks | diagonalAttacks;
        U16 flags = 0;
        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);

            }
            else{
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }
        }
    }
}

void MoveGenerator::generatePawnAdvancesWhite() {
    U64 move = 0ULL;
    U16 flags = 0;
    U16 targetSquare;
    U64 pawns = _board->getPieceSet(nWhitePawn);

    while(pawns){
        int startSquare = getLSB(pawns);
        U64 startSquareMask = 1ULL << startSquare;
        clearBit(pawns,startSquare);

        // adding single advances
        move = (startSquareMask << 8) & ~(_board->getPieceSet(nBlack) | _board->getPieceSet(nWhite));
        if(move){
            targetSquare = getLSB(move);
            flags = nQuietMoves;
            pseudoLegal.emplace_back(startSquare,targetSquare,flags);
        }
        else
            continue;

        // getting double pawn advances
        if(startSquareMask & RANK_2){
            move = startSquareMask << 16 & ~(_board->getPieceSet(nBlack) | _board->getPieceSet(nWhite));
            if(move){
                targetSquare = getLSB(move);
                flags = nDoublePawnPush;
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }
        }
    }
}

void MoveGenerator::generatePawnAdvancesBlack() {
    U64 move = 0ULL;
    U16 flags = 0;
    U16 targetSquare;
    U64 pawns = _board->getPieceSet(nBlackPawn);

    while(pawns){
        int startSquare = getLSB(pawns);
        U64 startSquareMask = 1ULL << startSquare;
        clearBit(pawns,startSquare);

        // adding single advances
        move = (startSquareMask >> 8) & ~(_board->getPieceSet(nBlack) | _board->getPieceSet(nWhite));
        if(move){
            targetSquare = getLSB(move);
            flags = nQuietMoves;
            pseudoLegal.emplace_back(startSquare,targetSquare,flags);
        }
        else
            continue;

        // getting double pawn advances for white
        if(startSquareMask & RANK_7){
            move = startSquareMask >> 16 & ~(_board->getPieceSet(nBlack) | _board->getPieceSet(nWhite));
            if(move){
                targetSquare = getLSB(move);
                flags = nDoublePawnPush;
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }
        }

    }

}

void MoveGenerator::generatePawnAdvances() {
    if(_state->getWhiteToMove())
        generatePawnAdvancesWhite();
    else
        generatePawnAdvancesBlack();
}

void MoveGenerator::generatePawnCaptures() {
    U64 pawns = 0ULL;
    U64 attacks;
    U16 flags = 0;
    if(_state->getWhiteToMove())
        pawns = _board->getPieceSet(nWhitePawn);
    else
        pawns = _board->getPieceSet(nBlackPawn);

    while(pawns){
        int startSquare = getLSB(pawns);
        clearBit(pawns,startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getPawnAttacksBlack(startSquare) & ~_board->getPieceSet(nBlack);

        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);
            }
        }
    }
}

void MoveGenerator::generateEnPassant(){
    U64 pawns = 0ULL;
    U16 flags = 0;
    U64 attacks;
    if(_state->getWhiteToMove())
        pawns = _board->getPieceSet(nWhitePawn);
    else
        pawns = _board->getPieceSet(nBlackPawn);

    while(pawns){
        int startSquare = getLSB(pawns);
        clearBit(pawns,startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nBlack);

        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(1ULL << targetSquare & _state->getEnPassantSquare()){
                flags = nEnPassantCapture;
                pseudoLegalCapture.emplace_back(startSquare, targetSquare, flags);
            }
        }
    }
}

void MoveGenerator::generateCastlesWhite() {

    U64 kingSideSquares = 1ULL << G1 | 1ULL << F1;
    U64 queenSideSquares = 1ULL << C1 | 1ULL << D1;
    U64 queenSideBlockSquares = 1ULL << C1 | 1ULL << D1 | 1ULL << B1;

    if(_state->getCastlingRight(whiteKingSide)){
        // checking if f1 and g1 are emptu
        if((kingSideSquares & (_board->getPieceSet(nWhite) | _board->getPieceSet(nBlack)))== 0){
            // checking if f1 or g1 is attacked
            U64 blackAttack = _atkTables->getAttacksBlack();
            if(blackAttack & kingSideSquares){
                std::cout << "attacked" << std::endl;
            }
            else{
                pseudoLegal.emplace_back(E1,G1,nKingCastle);
            }
        }
    }
    if(_state->getCastlingRight(whiteQueenSide)){
        // checking if b1, c1 and d1 are empty
        if((queenSideBlockSquares & (_board->getPieceSet(nWhite) | _board->getPieceSet(nBlack))) == 0){
            // checking if c1 or d1 is attacked
            U64 blackAttack = _atkTables->getAttacksBlack();
            if(blackAttack & queenSideSquares){
                std::cout << "attacked" << std::endl;
            }
            else{
                pseudoLegal.emplace_back(E1,C1,nQueenCastle);
            }
        }
    }
}

void MoveGenerator::generateCastlesBlack(){
    U64 kingSideSquares = 1ULL << G8 | 1ULL << F8;
    U64 queenSideSquares = 1ULL << C8 | 1ULL << D8;
    U64 queenSideBlockSquares = 1ULL << C3 | 1ULL << D8 | 1ULL << B8;

    if(_state->getCastlingRight(blackKingSide)){
        // checking if f1 and g1 are emptu
        if((kingSideSquares & _board->getPieceSet(nBlack) | _board->getPieceSet(nWhite)) == 0){
            // checking if f1 or g1 is attacked
            U64 whiteAttack = _atkTables->getAttacksWhite();
            if(whiteAttack & kingSideSquares){
                std::cout << "attacked" << std::endl;
            }
            else{
                pseudoLegal.emplace_back(E8,G8,nKingCastle);
            }
        }
    }
    if(_state->getCastlingRight(blackQueenSide)){
        // checking if b8, c8 and d8 are emptu
        if((queenSideBlockSquares & _board->getPieceSet(nBlack) & _board->getPieceSet(nWhite)) == 0){
            // checking if c8 or d8 is attacked
            U64 whiteAttack = _atkTables->getAttacksWhite();
            if(whiteAttack & queenSideSquares){
                std::cout << "attacked" << std::endl;
            }
            else{
                pseudoLegal.emplace_back(E8,C8,nQueenCastle);
            }
        }
    }
}

bool MoveGenerator::isCapture(int targetSquare) const{
    enumPieceBB captureColor = nWhite;
    if(_state->getWhiteToMove())
        captureColor = nBlack;

    if((1ULL << targetSquare) & _board->getPieceSet(captureColor))
        return true;
    else
        return false;
}

bool MoveGenerator::whiteKingInCheck() {
    return(_atkTables->getAttacksBlack() & _board->getPieceSet(nWhiteKing));
}

bool MoveGenerator::blackKingInCheck() {
    return(_atkTables->getAttacksWhite() & _board->getPieceSet(nBlackKing));
}

void MoveGenerator::pruneIllegalMoves() {
    // non captures
    for(Move& move:pseudoLegal){
        // for every move
        if(_state->getWhiteToMove()){
            _board->makeMove(move);
            if(!whiteKingInCheck()){
                legalMoves.emplace_back(move);
            }
        }
        else{
            _board->makeMove(move);
            if(!blackKingInCheck()){
                legalMoves.emplace_back(move);
            }
        }
        _board->undoMove();
    }

    // Captures
    for(Move& move:pseudoLegalCapture){
        // for every move
        if(_state->getWhiteToMove()){
            _board->makeMove(move);
            if(!whiteKingInCheck()){
                legalCaptures.emplace_back(move);
            }
        }
        else{
            _board->makeMove(move);
            if(!blackKingInCheck()){
                legalCaptures.emplace_back(move);
            }
        }
        _board->undoMove();
    }
}

void MoveGenerator::printLegalMoves(){
    std::cout << "Quiet moves: " << std::endl;
    for (Move &move: legalMoves) {
        std::cout << _board->indexToCoordinate(move.getStartSquare()) << "-" << _board->indexToCoordinate(move.getTargetSquare())
                  << std::endl;
    }

    std::cout << "Captures: " << std::endl;
    for (Move &move: legalCaptures) {
        std::cout << _board->indexToCoordinate(move.getStartSquare()) << "-"
                  << _board->indexToCoordinate(move.getTargetSquare()) << std::endl;
    }
}