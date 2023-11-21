#include "game_state.h"

GameState::GameState() = default;

void GameState::resetCastlingRights() {
    for(bool & right : castlingRights) {
        right = false;
    }
}

void GameState::setCastlingRight(nCastleingRight enumIndex) {
    castlingRights[enumIndex] = true;
}

bool GameState::getCastlingRight(nCastleingRight index) const{
    return castlingRights[index];
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

bool GameState::getWhiteToMove() const {
    if (whiteToMove)
        return true;
    else
        return false;
}

void GameState::setWhiteToMove(bool yes) {
    if(yes)
        whiteToMove = true;
    else
        whiteToMove = false;
}

U64 GameState::getEnPassantSquare() const {
    return enPassantSquare;
}
void GameState::resetEnPassantSquare() {
    enPassantSquare = 0ULL;
}

void GameState::passTurn() {
    whiteToMove = !whiteToMove;
}

void GameState::addMoveToHistory(Move move) {
    gameHistory.emplace_back(move);
}

Move GameState::getLastMove(){
    auto it = gameHistory.end();
    it --;
    return *it;
}

void GameState::deleteLastMove() {
    auto it = gameHistory.end();
    it --;
    gameHistory.erase(it);
}