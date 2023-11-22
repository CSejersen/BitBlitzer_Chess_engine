#pragma once
#include "piece.h"
#include "board_constants.h"

enum enumMoveFlags{
    nQuietMoves,
    nDoublePawnPush,
    nKingCastle,
    nQueenCastle,
    nCapture,
    nEnPassantCapture,
    nKnightPromo,
    nBishopPromo,
    nRookPromo,
    nQueenPromo,
    nKnightPromoCapture,
    nBishopPromoCapture,
    nRookPromoCapture,
    nQueenPromoCapture,
};

class Move{
public:
    Move(U16 start, U16 target, U16 flags);
    int getStartSquare() const;
    int getTargetSquare() const;
    int getFlag() const;
    Piece getCapturedPiece() const;
    void setCapturedPiece(Piece);

private:
    U16 _move;
    Piece _capturedPiece;
};
