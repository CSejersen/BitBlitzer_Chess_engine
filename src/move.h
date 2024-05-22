#pragma once
#include "board_constants.h"
#include "BitBoard.h"
#include <iostream>

/*
          move bits protocol                               hexidecimal

    0000 0000 0000 0000 0011 1111    start square       0x3f
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
int getMoveStart(int move);
int getMoveTarget(int move);
int getMovePiece(int move);
int getMovePromotion(int move);
bool getMoveCapture(int move);
bool getMoveDoublePush(int move);
bool getMoveEnPassant(int move);
bool getMoveCastling(int move);
void printMove(int move);

void printMove(int move);
