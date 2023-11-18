#include "move.h"

Move::Move(std::string start, std::string target, bool capture){
    startSquare = start;
    targetSquare = target;
    isCapture = capture;
}

std::string Move::getStartSquare() const {
    return startSquare;
}

std::string Move::getTargetSquare() const {
    return targetSquare;
}


