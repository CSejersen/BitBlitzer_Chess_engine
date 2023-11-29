//
// Created by Christian Sejersen on 21/11/2023.
//

#include "MoveGenerator.h"
MoveGenerator::MoveGenerator(BitBoard *board, AttackTables *attackTables) {
    _board = board;
    _atkTables = attackTables;
}
void MoveGenerator::generateMoves(std::vector<int> &moves, bool whiteToMove, int castlingRights, const U64 &enPassantSquare) {
    pseudoLegal.clear();
    pseudoLegalCapture.clear();

    generatePawnAdvances(whiteToMove);
    generatePawnCaptures(whiteToMove);
    generateKnightMovesWhite(whiteToMove);
    generateBishopMoves(whiteToMove);
    generateRookMoves(whiteToMove);
    generateKingMoves(whiteToMove);
    generateQueenMoves(whiteToMove);
    generateEnPassant(whiteToMove, enPassantSquare);
    if(whiteToMove){
        generateCastlesWhite(castlingRights);
    }
    else
        generateCastlesBlack(castlingRights);

    moves.reserve( pseudoLegal.size() + pseudoLegalCapture.size() ); // preallocate memory
    moves.insert( moves.end(), pseudoLegalCapture.begin(), pseudoLegalCapture.end() );
    moves.insert( moves.end(), pseudoLegal.begin(), pseudoLegal.end() );
}
void MoveGenerator::generateKnightMovesWhite(bool whiteToMove) {
    U64 knights;
    U64 attacks;
    knights = whiteToMove ? _board->getPieceSet(nWhiteKnight) : _board->getPieceSet(nBlackKnight);

    while (knights) {
        int startSquare = getLSB(knights);
        clearBit(knights, startSquare);

        if (whiteToMove)
            attacks = _atkTables->getKnightAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getKnightAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while (attacks) {
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            if (isCapture(targetSquare, whiteToMove)) {
                if (whiteToMove) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteKnight, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackKnight, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            } else {
                if (whiteToMove) {
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

void MoveGenerator::generateBishopMoves(bool whiteToMove) {
    U64 bishops = whiteToMove ? _board->getPieceSet(nWhiteBishop) : _board->getPieceSet(nBlackBishop);
    U64 attacks;

    while(bishops){
        int startSquare = getLSB(bishops);
        clearBit(bishops,startSquare);
        if(whiteToMove)
            attacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getBishopAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while(attacks) {
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            if (isCapture(targetSquare, whiteToMove)) {
                if (whiteToMove) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteBishop, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackBishop, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            } else {
                if (whiteToMove) {
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

void MoveGenerator::generateRookMoves(bool whiteToMove) {
    U64 rooks = whiteToMove ? _board->getPieceSet(nWhiteRook) : _board->getPieceSet(nBlackRook);
    U64 attacks;

    while (rooks) {
        int startSquare = getLSB(rooks);
        clearBit(rooks, startSquare);
        if (whiteToMove)
            attacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nWhite);
        else
            attacks = _atkTables->getRookAttacks(startSquare) & ~_board->getPieceSet(nBlack);

        while (attacks) {
            int targetSquare = getLSB(attacks);
            clearBit(attacks, targetSquare);

            if (isCapture(targetSquare, whiteToMove)) {
                if (whiteToMove) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteRook, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
                else {
                    int move = encodeMove(startSquare, targetSquare, nBlackRook, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }

            } else {
                if (whiteToMove) {
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

void MoveGenerator::generateKingMoves(bool whiteToMove) {
    U64 king = whiteToMove ? _board->getPieceSet(nWhiteKing) : _board->getPieceSet(nBlackKing);
    U64 attacks;

    U16 startSquare = getLSB(king);
    if (whiteToMove)
        attacks = _atkTables->getKingAttacks(startSquare) & ~_board->getPieceSet(nWhite);
    else
        attacks = _atkTables->getKingAttacks(startSquare) & ~_board->getPieceSet(nBlack);

    while (attacks) {
        int targetSquare = getLSB(attacks);
        clearBit(attacks, targetSquare);

        // adding capture flag
        if (isCapture(targetSquare, whiteToMove)) {
            if (whiteToMove) {
                int move = encodeMove(startSquare, targetSquare, nWhiteKing, 0, 1, 0, 0, 0);
                pseudoLegalCapture.push_back(move);
            } else {
                int move = encodeMove(startSquare, targetSquare, nBlackKing, 0, 1, 0, 0, 0);
                pseudoLegalCapture.push_back(move);
            }
        }
        else {
            if (whiteToMove) {
                int move = encodeMove(startSquare, targetSquare, nWhiteKing, 0, 0, 0, 0, 0);
                pseudoLegal.push_back(move);
            } else {
                int move = encodeMove(startSquare, targetSquare, nBlackKing, 0, 0, 0, 0, 0);
                pseudoLegal.push_back(move);
            }
        }
    }
}

void MoveGenerator::generateQueenMoves(bool whiteToMove) {
    U64 queens = whiteToMove ? _board->getPieceSet(nWhiteQueen) : _board->getPieceSet(nBlackQueen);
    U64 straightAttacks;
    U64 diagonalAttacks;

    while(queens){
        int startSquare = getLSB(queens);
        clearBit(queens, startSquare);
        if(whiteToMove){
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
            if (isCapture(targetSquare, whiteToMove)) {
                if (whiteToMove) {
                    int move = encodeMove(startSquare, targetSquare, nWhiteQueen, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                } else {
                    int move = encodeMove(startSquare, targetSquare, nBlackQueen, 0, 1, 0, 0, 0);
                    pseudoLegalCapture.push_back(move);
                }
            }
            else {
                if (whiteToMove) {
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
        bool blocked = ((1ULL << (startSquare + 8)) & ~_board->getAllPieces()) == 0;
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
            blocked = ((1ULL << (startSquare + 16)) & ~_board->getAllPieces())== 0;
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
        bool blocked = ((1ULL << (startSquare - 8)) & ~_board->getAllPieces()) == 0;
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
            blocked = ((1ULL << (startSquare - 16)) & ~_board->getAllPieces()) == 0;
            if(!blocked){
                int targetSquare = startSquare - 16;
                int move = encodeMove(startSquare,targetSquare,nBlackPawn,0,0,1,0,0);
                pseudoLegal.push_back(move);
            }
        }
    }
}

void MoveGenerator::generatePawnAdvances(bool whiteToMove) {
    if(whiteToMove)
        generatePawnAdvancesWhite();
    else
        generatePawnAdvancesBlack();
}

void MoveGenerator::generatePawnCaptures(bool whiteToMove) {
    U64 pawns = whiteToMove ? _board->getPieceSet(nWhitePawn) : _board->getPieceSet(nBlackPawn);
    U64 moves;
    U64 captures;

    while(pawns){
        int startSquare = getLSB(pawns);
        clearBit(pawns,startSquare);

        if(whiteToMove){
            moves = (_atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nWhite));
            captures = moves & _board->getPieceSet(nBlack);
        }
        else{
            moves = (_atkTables->getPawnAttacksBlack(startSquare) & ~_board->getPieceSet(nBlack));
            captures = moves & _board->getPieceSet(nWhite);
        }

        while(captures){
            int targetSquare = getLSB(captures);
            clearBit(captures,targetSquare);
            if (whiteToMove) {
                int move = encodeMove(startSquare, targetSquare, nWhitePawn, 0, 1, 0, 0, 0);
                pseudoLegalCapture.push_back(move);
            }
            else {
                int move = encodeMove(startSquare, targetSquare, nBlackPawn, 0, 1, 0, 0, 0);
                pseudoLegalCapture.push_back(move);
            }
        }
    }
}

void MoveGenerator::generateEnPassant(bool whiteToMove, const U64 &enPassantSquare) {

    // can be made faster by getting all pawn attacks then anding with en-Passant square and getting opposite color pawn attacks from resulting square.
    // ocerlap with white pawns are legal En-Passant moves

    U64 pawns = whiteToMove ? _board->getPieceSet(nWhitePawn) : _board->getPieceSet(nBlackPawn);
    U64 attacks;

    while(pawns){
        int startSquare = getLSB(pawns);
        clearBit(pawns,startSquare);
        if(whiteToMove){
            attacks = _atkTables->getPawnAttacksWhite(startSquare) & ~_board->getPieceSet(nWhite) & enPassantSquare;
        }
        else{
            attacks = _atkTables->getPawnAttacksBlack(startSquare) & ~_board->getPieceSet(nBlack) & enPassantSquare;
        }

        if(attacks){
            int targetSquare = getLSB(attacks);
            if(whiteToMove){
                int move = encodeMove(startSquare,targetSquare,nWhitePawn,0,0,0,1,0);
                pseudoLegalCapture.push_back(move);
            }
            else{
                int move = encodeMove(startSquare,targetSquare,nBlackPawn,0,0,0,1,0);
                pseudoLegalCapture.push_back(move);
            }
        }
    }
}

void MoveGenerator::generateCastlesWhite(int castlingRights) {

    U64 kingSideSquares = 1ULL << G1 | 1ULL << F1;
    U64 queenSideSquares = 1ULL << C1 | 1ULL << D1;
    U64 queenSideBlockSquares = 1ULL << C1 | 1ULL << D1 | 1ULL << B1;
    U64 blackAttack = _atkTables->getAttacksBlack();

    if(decodeCastlingRight(castlingRights, whiteKingSide)){
        // checking if f1 and g1 are empty
        if((kingSideSquares & _board->getAllPieces()) == 0){
            // checking if f1 or g1 is attacked
            if(!(blackAttack & kingSideSquares)){
                int move = encodeMove(E1,G1,nWhiteKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
            }
        }
    }
    if(decodeCastlingRight(castlingRights, whiteQueenSide)){
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

void MoveGenerator::generateCastlesBlack(int castlingRights) {
    U64 kingSideSquares = 1ULL << G8 | 1ULL << F8;
    U64 queenSideSquares = 1ULL << C8 | 1ULL << D8;
    U64 queenSideBlockSquares = 1ULL << C8 | 1ULL << D8 | 1ULL << B8;
    U64 whiteAttack = _atkTables->getAttacksWhite();

    if(decodeCastlingRight(castlingRights, blackKingSide)){
        // checking if f8 and g8 are empty
        if((kingSideSquares & _board->getAllPieces()) == 0){
            // checking if f1 or g1 is attacked
            if(!(whiteAttack & kingSideSquares)){
                int move = encodeMove(E8,G8,nBlackKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
            }
        }
    }
    if(decodeCastlingRight(castlingRights, blackQueenSide)){
        // checking if b8, c8 and d8 are empty
        if((queenSideBlockSquares & _board->getAllPieces()) == 0){
            // checking if c8 or d8 is attacked
            if(!(whiteAttack & queenSideSquares)){
                int move = encodeMove(E8,C8,nBlackKing,0,0,0,0,1);
                pseudoLegal.push_back(move);
            }
        }
    }
}

bool MoveGenerator::isCapture(int targetSquare, bool whiteToMove) const{
    enumPieceBB captureColor = nWhite;
    if(whiteToMove)
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

void MoveGenerator::printLegalMoves(){
    std::cout << "Quiet moves: " << std::endl;
    for (int &move: pseudoLegal) {
        std::cout << BitBoard::indexToCoordinate(getMoveStart(move)) << "-" << BitBoard::indexToCoordinate(getMoveTarget(move)) << std::endl;
    }

    std::cout << "Captures: " << std::endl;
    for (int &move: pseudoLegalCapture) {
        std::cout << BitBoard::indexToCoordinate(getMoveStart(move)) << "-"
                  << BitBoard::indexToCoordinate(getMoveTarget(move)) << std::endl;
    }
}

bool MoveGenerator::decodeCastlingRight(int castlingRights, int enumRight) {
        return (castlingRights & enumRight) != 0;
    }


