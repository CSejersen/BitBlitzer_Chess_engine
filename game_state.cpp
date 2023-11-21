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
bool GameState::whiteKingMoved() {
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == E1)
            moved = true;
        else
            moved = false;
    }
       return moved;
}

bool GameState::blackKingMoved() {
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == E8)
            moved = true;
        else
            moved = false;
    }
    return moved;
}

bool GameState::blackKingsRookMoved(){
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == H8)
            moved = true;
        else
            moved = false;
    }
    return moved;

}
bool GameState::blackQueensRookMoved(){
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == A8)
            moved = true;
        else
            moved = false;
    }
    return moved;
}
bool GameState::whiteKingsRookMoved(){
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == H1)
            moved = true;
        else
            moved = false;
    }
    return moved;
}
bool GameState::whiteQueensRookMoved(){
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == A1)
            moved = true;
        else
            moved = false;
    }
    return moved;
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

void GameState::updateCastlingRights() {
    if(whiteKingMoved()){
        castlingRights[whiteKingSide] = false;
        castlingRights[whiteQueenSide] = false;
    }
    else{
        castlingRights[whiteKingSide] = true;
        castlingRights[whiteQueenSide] = true;
    }
    if(whiteKingsRookMoved())
        castlingRights[whiteKingSide] = false;
    if(whiteQueensRookMoved())
        castlingRights[whiteQueenSide] = false;

    if(blackKingMoved()){
        castlingRights[blackKingSide] = false;
        castlingRights[blackQueenSide] = false;
    }
    else{
        castlingRights[blackKingSide] = true;
        castlingRights[blackQueenSide] = true;
    }
    if(blackKingsRookMoved())
        castlingRights[blackKingSide] = false;
    if(blackQueensRookMoved())
        castlingRights[blackQueenSide] = false;
}
