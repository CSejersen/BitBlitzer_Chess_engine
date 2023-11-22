//
// Created by Christian Sejersen on 22/11/2023.
//

#include "piece.h"

Piece::Piece(int pieceType, int square) {
    _pieceType = pieceType;
    _square = square;
}
Piece::Piece() = default;

int Piece::getPieceType() const{
    return _pieceType;
}

int Piece::getSquare() const{
    return _square;
}
