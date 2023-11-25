#pragma once

class Piece {
public:
    Piece(int pieceType, int square);
    Piece();
    int getPieceType() const;
    int getSquare() const;
    void setPieceType(int pieceType);
    void setSquare(int square);

    virtual ~Piece();

private:
    int _pieceType;
    int _square;
};


