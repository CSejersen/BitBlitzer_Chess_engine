//
// Created by Christian Sejersen on 21/11/2023.
//

#include "MoveGenerator.h"
MoveGenerator::MoveGenerator(BitBoard *board, GameState *state, AttackTables *attackTables) {
    _board = board;
    _state = state;
    _atkTables = attackTables;
}
void MoveGenerator::generateMoves(std::vector<int>& moves) {
    pseudoLegal.clear();
    pseudoLegalCapture.clear();
    _legalMoves.clear();
    _legalCaptures.clear();

    generatePawnAdvances();
    generatePawnCaptures();
    generateKnightMovesWhite();
    generateBishopMoves();
    generateRookMoves();
    generateKingMoves();
    generateQueenMoves();
    generateEnPassant();
    if(_state->getWhiteToMove()){
        generateCastlesWhite();
    }
    else
        generateCastlesBlack();
    pruneIllegalMoves();


    moves.reserve( _legalMoves.size() + _legalCaptures.size() ); // preallocate memory
    moves.insert( moves.end(), _legalCaptures.begin(), _legalCaptures.end() );
    moves.insert( moves.end(), _legalMoves.begin(), _legalMoves.end() );
}
void MoveGenerator::generateKnightMovesWhite() {
    U64 knights;
    U64 attacks;
    knights = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhiteKnight) : _board->getPieceSet(nBlackKnight);

    while (knights) {
        int startSquare = getLSB(knights);
        clearBit(knights, startSquare);

        if (_state->getWhiteToMove())
            attacks = _atkTables->getKnightAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getKnightAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while (attacks) {
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            if (isCapture(targetSquare)) {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteKnight, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackKnight, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            } else {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteKnight, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackKnight, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                }
            }
        }
    }
}

void MoveGenerator::generateBishopMoves() {
    U64 bishops = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhiteBishop) : _board->getPieceSet(nBlackBishop);
    U64 attacks;

    while(bishops){
        int startSquare = getLSB(bishops);
        clearBit(bishops,startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while(attacks) {
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            if (isCapture(targetSquare)) {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteBishop, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackBishop, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            } else {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteBishop, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackBishop, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                }
            }
        }
    }
}

void MoveGenerator::generateRookMoves() {
    U64 rooks = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhiteRook) : _board->getPieceSet(nBlackRook);
    U64 attacks;

    while (rooks) {
        int startSquare = getLSB(rooks);
        clearBit(rooks, startSquare);
        if (_state->getWhiteToMove())
            attacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while (attacks) {
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            if (isCapture(targetSquare)) {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteRook, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
                else {
                    int move = encodeMove(startSquare, targetSquare, nBlackRook, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }

            } else {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteRook, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                }
                else {
                    int move = encodeMove(startSquare, targetSquare, nBlackRook, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                }
            }
        }
    }
}

void MoveGenerator::generateKingMoves() {
    U64 king = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhiteKing) : _board->getPieceSet(nBlackKing);
    U64 attacks;

    U16 startSquare = getLSB(king);
    if (_state->getWhiteToMove())
        attacks = _atkTables->getKingAttacks(startSquare) & ~_board->getPieceSet(nWhite);
    else
        attacks = _atkTables->getKingAttacks(startSquare) & ~_board->getPieceSet(nBlack);

    while (attacks) {
        int targetSquare = getLSB(attacks);
        clearBit(attacks, targetSquare);

        // adding capture flag
        if (isCapture(targetSquare)) {
            if (_state->getWhiteToMove()) {
                int move = encodeMove(startSquare, targetSquare, nWhiteKing, 0, 1, 0, 0, 0);
                pseudoLegalCapture.push_back(move);
            } else {
                int move = encodeMove(startSquare, targetSquare, nBlackKing, 0, 1, 0, 0, 0);
                pseudoLegalCapture.push_back(move);
            }
        }
        else {
            if (_state->getWhiteToMove()) {
                int move = encodeMove(startSquare, targetSquare, nWhiteKing, 0, 0, 0, 0, 0);
                pseudoLegal.push_back(move);
            } else {
                int move = encodeMove(startSquare, targetSquare, nBlackKing, 0, 0, 0, 0, 0);
                pseudoLegal.push_back(move);
            }
        }
    }
}

void MoveGenerator::generateQueenMoves() {
    U64 queens = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhiteQueen) : _board->getPieceSet(nBlackQueen);
    U64 straightAttacks;
    U64 diagonalAttacks;

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

        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            // adding capture flag
            if (isCapture(targetSquare)) {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteQueen, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackQueen, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            }
            else {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteQueen, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackQueen, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                }
            }
        }
    }
}

void MoveGenerator::generatePawnAdvancesWhite() {
    U64 pawns = _board->getPieceSet(nWhitePawn);
    while(pawns){
        int startSquare = getLSB(pawns);
        bool blocked = (1ULL << (startSquare + 8) & ~_board->getAllPieces()) == 0;
        clearBit(pawns,startSquare);

        // adding single advances
        if(!blocked){
            int targetSquare = startSquare + 8;
            int move = encodeMove(startSquare,targetSquare,nWhitePawn,0,0,0,0,0);
            pseudoLegal.push_back(move);
        }
        else
            continue;

        // getting double pawn advances
        if(startSquare <= 15){
            blocked = (1ULL << (startSquare + 16) & ~_board->getAllPieces()) == 0;
            if(!blocked){
                int targetSquare = startSquare + 16;
                int move = encodeMove(startSquare,targetSquare,nWhitePawn,0,0,1,0,0);
                pseudoLegal.push_back(move);
            }
        }
    }
}

void MoveGenerator::generatePawnAdvancesBlack() {
    U64 pawns = _board->getPieceSet(nBlackPawn);
    while(pawns){
        int startSquare = getLSB(pawns);
        bool blocked = (1ULL << (startSquare - 8) & ~_board->getAllPieces()) == 0;
        clearBit(pawns,startSquare);

        // adding single advances
        if(!blocked){
            int targetSquare = startSquare - 8;
            int move = encodeMove(startSquare,targetSquare,nBlackPawn,0,0,0,0,0);
            pseudoLegal.push_back(move);
        }
        else
            continue;

        // getting double pawn advances
        if(startSquare >= 48){
            blocked = (1ULL << (startSquare - 16) & ~_board->getAllPieces()) == 0;
            if(!blocked){
                int targetSquare = startSquare - 16;
                int move = encodeMove(startSquare,targetSquare,nBlackPawn,0,0,1,0,0);
                pseudoLegal.push_back(move);
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
    U64 pawns = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhitePawn) : _board->getPieceSet(nBlackPawn);
    U64 moves;

    while(pawns){
        int startSquare = getLSB(pawns);
        clearBit(pawns,startSquare);

        if(_state->getWhiteToMove()){
            moves = (_atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nWhite)) & _board->getPieceSet(nBlack);
        }
        else{
            moves = (_atkTables->getPawnAttacksBlack(startSquare) & ~_board->getPieceSet(nBlack)) & _board->getPieceSet(nWhite);
        }

        while(moves){
            int targetSquare = getLSB(moves);
            clearBit(moves,targetSquare);
            if (isCapture(targetSquare)) {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhitePawn, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackPawn, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            }
            else {
                if (_state->getWhiteToMove()) {
                    int move = encodeMove(startSquare, targetSquare, nWhitePawn, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackPawn, 0, 0, 0, 0, 0);
                    pseudoLegal.push_back(move);
                }
            }
        }
    }
}

void MoveGenerator::generateEnPassant(){
    U64 pawns = (_state->getWhiteToMove()) ? _board->getPieceSet(nWhitePawn) : _board->getPieceSet(nBlackPawn);
    U64 attacks;

    while(pawns){
        int startSquare = getLSB(pawns);
        clearBit(pawns,startSquare);
        if(_state->getWhiteToMove())
            attacks = _atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nWhite) & _state->getEnPassantSquare();
        else
            attacks = _atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nBlack) & _state->getEnPassantSquare();

        if(attacks){
            int targetSquare = getLSB(attacks);
            if(_state->getWhiteToMove()){
                int move = encodeMove(startSquare,targetSquare,nWhitePawn,0,0,0,1,0);
            }
            else{
                int move = encodeMove(startSquare,targetSquare,nBlackPawn,0,0,0,1,0);
            }
        }
    }
}

void MoveGenerator::generateCastlesWhite() {

    U64 kingSideSquares = 1ULL << G1 | 1ULL << F1;
    U64 queenSideSquares = 1ULL << C1 | 1ULL << D1;
    U64 queenSideBlockSquares = 1ULL << C1 | 1ULL << D1 | 1ULL << B1;
    U64 blackAttack = _atkTables->getAttacksBlack();

    if(_state->getCastlingRight(whiteKingSide)){
        // checking if f1 and g1 are empty
        if((kingSideSquares & _board->getAllPieces()) == 0){
            // checking if f1 or g1 is attacked
            if(!(blackAttack & kingSideSquares)){
                int move = encodeMove(E1,G1,nWhiteKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
            }
        }
    }
    if(_state->getCastlingRight(whiteQueenSide)){
        // checking if b1, c1 and d1 are empty
        if((queenSideBlockSquares & _board->getAllPieces()) == 0){
            // checking if c1 or d1 is attacked
            if(!(blackAttack & queenSideSquares)){
                int move = encodeMove(E1,C1,nWhiteKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
            }
        }
    }
}

void MoveGenerator::generateCastlesBlack(){
    U64 kingSideSquares = 1ULL << G8 | 1ULL << F8;
    U64 queenSideSquares = 1ULL << C8 | 1ULL << D8;
    U64 queenSideBlockSquares = 1ULL << C3 | 1ULL << D8 | 1ULL << B8;
    U64 whiteAttack = _atkTables->getAttacksWhite();

    if(_state->getCastlingRight(blackKingSide)){
        // checking if f1 and g1 are emptu
        if((kingSideSquares & _board->getAllPieces()) == 0){
            // checking if f1 or g1 is attacked
            if((whiteAttack & kingSideSquares)){
                int move = encodeMove(E8,G8,nBlackKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
            }
        }
    }
    if(_state->getCastlingRight(blackQueenSide)){
        // checking if b8, c8 and d8 are emptu
        if((queenSideBlockSquares & _board->getAllPieces()) == 0){
            // checking if c8 or d8 is attacked
            if(!(whiteAttack & queenSideSquares)){
                int move = encodeMove(E8,C3,nBlackKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
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

bool MoveGenerator::whiteKingInCheck() const {
    return(_atkTables->getAttacksBlack() & _board->getPieceSet(nWhiteKing));
}

bool MoveGenerator::blackKingInCheck() const {
    return(_atkTables->getAttacksWhite() & _board->getPieceSet(nBlackKing));
}

void MoveGenerator::pruneIllegalMoves() {
    // non captures
    for(int& move:pseudoLegal){
        // for every move
        if(_state->getWhiteToMove()){
            _board->makeMove(move);
            if(!whiteKingInCheck()){
                _legalMoves.emplace_back(move);
            }
        }
        else{
            _board->makeMove(move);
            if(!blackKingInCheck()){
                _legalMoves.emplace_back(move);
            }
        }
        _board->undoMove();
    }

    // Captures
    for(int& move:pseudoLegalCapture){
        // for every move
        if(_state->getWhiteToMove()){
            _board->makeMove(move);
            if(!whiteKingInCheck()){
                _legalCaptures.emplace_back(move);
            }
        }
        else{
            _board->makeMove(move);
            if(!blackKingInCheck()){
                _legalCaptures.emplace_back(move);
            }
        }
        _board->undoMove();
    }
}

void MoveGenerator::printLegalMoves(){
    std::cout << "Quiet moves: " << std::endl;
    for (int &move: _legalMoves) {
        std::cout << _board->indexToCoordinate(getMoveStart(move)) << "-" << _board->indexToCoordinate(getMoveTarget(move)) << std::endl;
    }

    std::cout << "Captures: " << std::endl;
    for (int &move: _legalCaptures) {
        std::cout << _board->indexToCoordinate(getMoveStart(move)) << "-"
                  << _board->indexToCoordinate(getMoveTarget(move)) << std::endl;
    }
}

U64 MoveGenerator::countNodes(int depth) {
    U64 nodes = 0;
    std::vector<int> moves;
    if (depth == 0)
        return 1ULL;
    generateMoves(moves);

    for(int move : moves){
        _board->makeMove(move);
        nodes += countNodes(depth - 1);
        _board->undoMove();
    }
    return nodes;
}

void MoveGenerator::perft(int depth){
    std::vector<int> moves;
    generateMoves(moves);
    U64 nodes = 0;
    U64 totalNodes = 0;

    for(int& move : moves){
        _board->makeMove(move);
        nodes = countNodes(depth - 1);
        totalNodes += nodes;
        std::cout << _board->indexToCoordinate(getMoveStart(move)) << "-" << _board->indexToCoordinate(getMoveStart(move)) << ": " << nodes << " nodes" << std::endl;
        _board->undoMove();
    }
    std::cout << "\ntotal nodes: " << totalNodes << std::endl;
}


void MoveGenerator::generateMoves() {
    pseudoLegal.clear();
    pseudoLegalCapture.clear();
    _legalMoves.clear();
    _legalCaptures.clear();

    generateKnightMovesWhite();
    generateBishopMoves();
    generateRookMoves();
    generateKingMoves();
    generateQueenMoves();
    generatePawnAdvances();
    generatePawnCaptures();
    generateEnPassant();
    if (_state->getWhiteToMove()) {
        generateCastlesWhite();
    } else
        generateCastlesBlack();
    pruneIllegalMoves();

}

