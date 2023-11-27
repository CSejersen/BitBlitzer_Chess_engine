#include "CurrentPosition.h"

CurrentPosition::CurrentPosition(BitBoard* board){
    _board = board;
};

void CurrentPosition::setCastlingRight(int enumRight) {
    castlingRights |= enumRight;
}

bool CurrentPosition::getCastlingRight(int enumRight) const{
    return (castlingRights & enumRight) != 0;
}

void CurrentPosition::setEnPassantSquare(int square){
    enPassantSquare = 1ULL << square;
}

void CurrentPosition::setHalfMoveClock(int value){
    halfmoveClock = value;
}

void CurrentPosition::setMoveNum(int value){
    moveNum = value;
}

bool CurrentPosition::getWhiteToMove() const {
    if (whiteToMove)
        return true;
    else
        return false;
}

void CurrentPosition::setWhiteToMove(bool yes) {
    if(yes)
        whiteToMove = true;
    else
        whiteToMove = false;
}

U64 CurrentPosition::getEnPassantSquare() const {
    return enPassantSquare;
}
void CurrentPosition::resetEnPassantSquare() {
    enPassantSquare = 0ULL;
}

void CurrentPosition::passTurn() {
    whiteToMove = !whiteToMove;
}


void CurrentPosition::addPositionToHistory(int move) {

    Position posToSave{};
    posToSave.previousMove = move;
    posToSave.whiteInCheck = whiteInCheck;
    posToSave.blackInCheck = blackInCheck;
    posToSave.whiteToMove = whiteToMove;
    posToSave.enPassantSquare = enPassantSquare;
    posToSave.castlingRights = castlingRights;

    for(int i = nWhite; i <= nBlackKing; i++){
        posToSave.boardPosition[i] = _board->pieceBB[i];
    }

    gameHistory.push_back(posToSave);
}

Position CurrentPosition::getPrevPosition(){
    auto it = gameHistory.end();
    it --;
    return *it;
}

void CurrentPosition::deleteLastPositionFromHistory() {
    auto it = gameHistory.end();
    it --;
    gameHistory.erase(it);
}

void CurrentPosition::printCastlingRights() {
    std::cout << "castling Rights: " << std::endl;
    for (int i = 1; i <= 4; i++) {
        std::cout << "at index " << i << " = " << getCastlingRight(i) << std::endl;
    }
}

void CurrentPosition::revertToPreviousPosition() {

   Position prevPos = getPrevPosition();
   castlingRights = prevPos.castlingRights;
   enPassantSquare = prevPos.enPassantSquare;
   whiteToMove = prevPos.whiteToMove;
   blackInCheck = prevPos.blackInCheck;
   whiteInCheck = prevPos.whiteInCheck;
   previousMove = prevPos.previousMove;

   for(int i = 0; i <= nBlackKing; i++){
       _board->pieceBB[i] = prevPos.boardPosition[i]; // maybe optimize to only copy the necessary bitboards;
   }
}
void CurrentPosition::makeMove(int& move) {
    // first saving current position
    addPositionToHistory(move);
    // decoding move
    int startingSquare = getMoveStart(move);
    int targetSquare = getMoveTarget(move);
    int piece = getMovePiece(move);
    int promotion = getMovePromotion(move);
    bool doublePush = getMoveDoublePush(move);
    bool capture = getMoveCapture(move);
    bool enPassant = getMoveEnPassant(move);
    bool castling = getMoveCastling(move);

    // making move on the _board
    _board->pieceBB[piece] &= ~(1ULL << startingSquare); // removing Piece from starting square
    _board->placePiece(piece,targetSquare);
    if(whiteToMove){
        _board->pieceBB[nWhite] &= ~(1ULL << startingSquare);
    }
    else{
        _board->pieceBB[nBlack] &= ~(1ULL << startingSquare);
    }

    // Checking for En Passant
    if(enPassant) {
        handleEnPassantFlag(targetSquare);
    }

    // don't make move if it is illegal
    //
    //
    // insert here

    if(capture){
        handleCaptureFlag(targetSquare);
    }


    // Checking for king side castling
    if(castling && (targetSquare == G1 || targetSquare == G8)) {
        handleCastlesKingSide();
    }

    // Checking for queens side castling
    if(castling && (targetSquare == C1 || targetSquare == C8)) {
        handleCastlesQueenSide();
    }

    if(piece == nWhiteKing){
        removeCastlingRight(whiteKingSide);
        removeCastlingRight(whiteQueenSide);
    }
    if(piece == nBlackKing){
        removeCastlingRight(blackKingSide);
        removeCastlingRight(blackQueenSide);
    }


    // reset En-Passant square and set a new one if double pawn push was played.
    resetEnPassantSquare();
    if(doublePush){
        if(whiteToMove){
            setEnPassantSquare(targetSquare - 8);
        }
        else{
            setEnPassantSquare(targetSquare + 8);
        }
    }

    passTurn();
}
void CurrentPosition::handleCaptureFlag(int targetSquare) {
    Piece pieceToCapture;
    int pieceType = 20;

    // searching for Piece to capture
    if (getWhiteToMove()) {
        for (int piece = nBlackPawn; piece <= nBlackKing; piece++) {
            if (_board->pieceBB[piece] & (1ULL << targetSquare)) {
                pieceType = piece;
                break;
            }
        }
    }
    else {
        for (int piece = nWhitePawn; piece <= nWhiteKing; piece++) {
            if (_board->pieceBB[piece] & (1ULL << targetSquare)) {
                pieceType = piece;
                break;
            }
        }
    }
    // removing captured Piece from _board
    if(pieceType == 20){
        throw std::invalid_argument("no piece to capture - handle capture flag");
    }
    _board->pieceBB[pieceType] &= ~(1ULL << targetSquare);

    // removing from nWhite and nBlack pieceBB's
    if(getWhiteToMove())
        _board->pieceBB[nBlack] &= ~(1ULL << targetSquare);
    else
        _board->pieceBB[nWhite] &= ~(1ULL << targetSquare);
}

void CurrentPosition::handleEnPassantFlag(uint8_t targetSquare) {
    int pawnToCapture = 0;
    // searching for Piece to capture
    if (getWhiteToMove()) {
        pawnToCapture = getLSB(getEnPassantSquare() >> 8);
        // removing captured Piece from _board
        _board->pieceBB[nBlackPawn] &= ~(1ULL << pawnToCapture);
        _board->pieceBB[nBlack] &= ~(1ULL << pawnToCapture);
    } else {
        pawnToCapture = getLSB(getEnPassantSquare() << 8);
        // removing captured Piece from _board
        _board->pieceBB[nWhitePawn] &= ~(1ULL << pawnToCapture);
        _board->pieceBB[nWhite] &= ~(1ULL << pawnToCapture);
    }
}
void CurrentPosition::handleCastlesKingSide() {

    if(whiteToMove){
        // Removing rook from H1
        _board->pieceBB[nWhiteRook] &= ~(1ULL << H1);
        _board->pieceBB[nWhite] &= ~(1ULL << H1);

        // Placing rook on F1
        _board->placePiece(nWhiteRook,F1);
        removeCastlingRight(whiteKingSide);
        removeCastlingRight(whiteQueenSide);
    }
    else{
        // removing rook from H8
        _board->pieceBB[nBlackRook] &= ~(1ULL << H8);
        _board->pieceBB[nBlack] &= ~(1ULL << H8);

        // Placing rook on F8
        _board->placePiece(nWhiteRook,F8);
        removeCastlingRight(blackKingSide);
        removeCastlingRight(blackQueenSide);
    }
}

void CurrentPosition::handleCastlesQueenSide() {
    if(whiteToMove){
        // Removing rook from H1
        _board->pieceBB[nWhiteRook] &= ~(1ULL << A1);
        _board->pieceBB[nWhite] &= ~(1ULL << A1);

        // Placing rook on F1
        _board->placePiece(nWhiteRook,D1);
        removeCastlingRight(whiteKingSide);
        removeCastlingRight(whiteQueenSide);
    }
    else{
        // removing rook from H8
        _board->pieceBB[nBlackRook] &= ~(1ULL << A8);
        _board->pieceBB[nBlack] &= ~(1ULL << A8);
        // Placing rook on F8
        _board->placePiece(nWhiteRook,D8);
        removeCastlingRight(blackKingSide);
        removeCastlingRight(blackQueenSide);
    }
}

void CurrentPosition::removeCastlingRight(int enumRight) {
    castlingRights &= ~enumRight;
}

void CurrentPosition::undoMove() {
    // Give turn back
    passTurn();

    Position prevPos = getPrevPosition();

    previousMove = prevPos.previousMove;
    whiteInCheck = prevPos.whiteInCheck;
    blackInCheck = prevPos.blackInCheck;
    whiteToMove = prevPos.whiteToMove;
    enPassantSquare = prevPos.enPassantSquare;
    castlingRights = prevPos.castlingRights;

    for(int i = nWhite; i <= nBlackKing; i++){
        _board->pieceBB[i] = prevPos.boardPosition[i];
    }
    //

    deleteLastPositionFromHistory();
}

void CurrentPosition::resetCastlingRights() {
    castlingRights = 0;
};
