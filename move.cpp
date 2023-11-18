#include "move.h"

Move::Move(std::string start, std::string target){
    startSquare = start;
    targetSquare = target;
}

std::string Move::getStartSquare() const {
    return startSquare;
}

std::string Move::getTargetSquare() const {
    return targetSquare;
}


