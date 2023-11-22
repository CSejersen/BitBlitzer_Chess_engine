#include "game_state.h"

GameState::GameState() = default;

void GameState::resetCastlingRights() {
    for(bool & right : castlingRights) {
        right = false;
    }
}

void GameState::setCastlingRight(int index) {
    castlingRights[index] = true;
}

bool GameState::getCastlingRight(int index) const{
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

bool GameState::whiteKingsRookCaptured() const{
    bool captured = false;
    for(auto& it: gameHistory ){
        if(it.getTargetSquare() == H1){
            captured = true;
            break;
        }
    }
    return captured;
}
bool GameState::whiteQueensRookCaptured() const{
    bool captured = false;
    for(auto& it: gameHistory ){
        if(it.getTargetSquare() == A1){
            captured = true;
            break;
        }
    }
    return captured;
}
bool GameState::blackKingsRookCaptured() const{
    bool captured = false;
    for(auto& it: gameHistory ){
        if(it.getTargetSquare() == H8){
            captured = true;
            break;
        }
    }
    return captured;
}
bool GameState::blackQueensRookCaptured() const{
    bool captured = false;
    for(auto& it: gameHistory ){
        if(it.getTargetSquare() == A8){
            captured = true;
            break;
        }
    }
    return captured;
}

bool GameState::whiteKingMoved() const {
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == E1){
            moved = true;
            break;
        }
    }
       return moved;
}

bool GameState::blackKingMoved() const {
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == E8){
            moved = true;
            break;
        }
    }
    return moved;
}

bool GameState::blackKingsRookMoved() const{
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == H8){
            moved = true;
            break;
        }
    }
    return moved;

}
bool GameState::blackQueensRookMoved() const{
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == A8){
            moved = true;
            break;
        }
    }
    return moved;
}
bool GameState::whiteKingsRookMoved() const{
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == H1){
            moved = true;
            break;
        }
    }
    return moved;
}
bool GameState::whiteQueensRookMoved() const{
    bool moved = false;
    for(auto& it: gameHistory ){
        if(it.getStartSquare() == A1){
            moved = true;
            break;
        }
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

    // start by setting everything true
    castlingRights[whiteKingSide] = true;
    castlingRights[whiteQueenSide] = true;
    castlingRights[blackKingSide] = true;
    castlingRights[blackQueenSide] = true;

    //white king movement
    if(whiteKingMoved()){
        castlingRights[whiteKingSide] = false;
        castlingRights[whiteQueenSide] = false;
    }

    //white king side rook movement or capture
    if(whiteKingsRookMoved())
        castlingRights[whiteKingSide] = false;
    if(whiteKingsRookCaptured())
        castlingRights[whiteKingSide] = false;

    //white queen side rook movement or capture
    if(whiteQueensRookMoved())
        castlingRights[whiteQueenSide] = false;
    if(whiteQueensRookCaptured())
        castlingRights[whiteQueenSide] = false;

    // Black king movement
    if(blackKingMoved()){
        castlingRights[blackKingSide] = false;
        castlingRights[blackQueenSide] = false;
    }

    // Black king side rook movement or capture
    if(blackKingsRookMoved())
        castlingRights[blackKingSide] = false;
    if(blackKingsRookCaptured())
        castlingRights[blackKingSide] = false;

    // Black queen side rook movement or capture
    if(blackQueensRookMoved())
        castlingRights[blackQueenSide] = false;
    if(blackQueensRookCaptured())
        castlingRights[blackQueenSide] = false;
}
