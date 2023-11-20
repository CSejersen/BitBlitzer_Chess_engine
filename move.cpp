#include "move.h"
#include <iostream>

Move::Move(U16 startSquare, U16 targetSquare, U16 flags){
    _move = (flags << 12) | (startSquare <<6) | targetSquare;
//    _move = ((flags & 0xf)<<12) | ((startSquare & 0x3f)<<6) | (targetSquare & 0x3f);


}

int Move::getTargetSquare() const {return _move & 0x3f;}
int Move::getStartSquare() const {return _move >> 6 & 0x3f;}
int Move::getFlag() const {return _move >> 12 & 0x0f;}