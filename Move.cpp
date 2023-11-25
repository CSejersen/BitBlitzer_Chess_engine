#include "Move.h"
#include <iostream>

Move::Move(U16 startSquare, U16 targetSquare, U16 flags) {
    _move = (flags << 12) | (startSquare << 6) | targetSquare;
}

int Move::getTargetSquare() const {return _move & 0x3f;}
int Move::getStartSquare() const {return _move >> 6 & 0x3f;}
int Move::getFlag() const {return _move >> 12 & 0x0f;}
Piece Move::getCapturedPiece() const {return _capturedPiece;}

void Move::setCapturedPiece(Piece piece) {
    _capturedPiece = piece;
}

Move::~Move() = default;

