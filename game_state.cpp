#include "game_state.h"

void GameState::resetCastlingRights() {
    for(bool & right : castlingRights) {
        right = false;
    }
}

void GameState::setCastlingRight(nCastleingRight enumIndex) {
    castlingRights[enumIndex] = true;

}
void GameState::setEnPassantSquare(int square){
    enPassantSquare = 1ULL << square;
}

void GameState::setHalfMoveClock(int value){
    halfmoveClock = value;
}

void GameState::setMoveNum(int value){
    moveNum = value;
}
