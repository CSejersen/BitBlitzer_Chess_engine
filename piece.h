#pragma once

class Piece {
public:
    Piece(int pieceType, int square);
    Piece();
    int getPieceType() const;
    int getSquare() const;
private:
    int _pieceType;
    int _square;
};


