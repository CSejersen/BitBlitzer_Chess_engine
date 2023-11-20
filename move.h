#pragma once
#include <string>

typedef uint16_t U16;

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
    nKightPromoCapture,
    nBishopPromoCapture,
    hRookPromoCapture,
    nQueenPromoCapture,
};

class Move{
public:
    Move(U16 start, U16 target, U16 flags);
    int getStartSquare() const;
    int getTargetSquare() const;
    int getFlag() const;

private:
    U16 _move;

};
