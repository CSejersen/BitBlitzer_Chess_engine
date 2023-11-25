//
// Created by Christian Sejersen on 22/11/2023.
//

#include <stdexcept>
#include "Piece.h"

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

void Piece::setPieceType(int pieceType) {
    if(pieceType > 13)
        throw std::invalid_argument("invalid pieceType fed to set PieceType");
    _pieceType = pieceType;
}

void Piece::setSquare(int square) {
    _square = square;
}

Piece::~Piece() {

}

