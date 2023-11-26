#pragma once
#include "board_constants.h"

/*
          binary move bits                               hexidecimal

    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

int encodeMove(int start, int target, int piece, int promotion, int capture, int doublePush, int enPassant, int castling );
// extract source square
uint8_t getMoveStart(int move);
uint8_t getMoveTarget(int move);
uint8_t getMovePiece(int move);
uint8_t getMovePromotion(int move);
bool getMoveCapture(int move);
bool getMoveDoublePush(int move);
bool getMoveEnPassant(int move);
bool getMoveCastling(int move);
