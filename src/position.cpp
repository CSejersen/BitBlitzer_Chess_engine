#include "Position.h"

Position::Position(BitBoard *board, AttackTables *atkTables) {
    _board = board;
    _atkTables = atkTables;
};

void Position::setCastlingRight(int enumRight) {
    castlingRights |= enumRight;
}

bool Position::getCastlingRight(int enumRight) const{
    return (castlingRights & enumRight) != 0;
}

void Position::setEnPassantSquare(int square){
    enPassantSquare = 1ULL << square;
}

void Position::setHalfMoveClock(int value){
    halfmoveClock = value;
}

void Position::setMoveNum(int value){
    moveNum = value;
}

bool Position::getWhiteToMove() const {
    if (whiteToMove)
        return true;
    else
        return false;
}

void Position::setWhiteToMove(bool yes) {
    if(yes)
        whiteToMove = true;
    else
        whiteToMove = false;
}

U64 Position::getEnPassantSquare() const {
    return enPassantSquare;
}

void Position::resetEnPassantSquare() {
    enPassantSquare = 0ULL;
}

void Position::passTurn() {
    whiteToMove = !whiteToMove;
}


void Position::addStateToHistory(int move) {

    State posToSave{};
    posToSave.previousMove = move;
    posToSave.whiteInCheck = whiteInCheck;
    posToSave.blackInCheck = blackInCheck;
    posToSave.whiteToMove = whiteToMove;
    posToSave.enPassantSquare = enPassantSquare;
    posToSave.castlingRights = castlingRights;
    posToSave.capturedPiece = capturedPiece;

    gameHistory.push_back(posToSave);
}

State Position::getPrevPosition(){
    auto it = gameHistory.end();
    it --;
    return *it;
}

void Position::deleteLastPositionFromHistory() {
    auto it = gameHistory.end();
    it --;
    gameHistory.erase(it);
}

void Position::printCastlingRights() {
    std::cout << "castling Rights: " << std::endl;
    std::cout << "white Kingside: " << " = " << getCastlingRight(whiteKingSide) << std::endl;
    std::cout << "white Queenside: " << " = " << getCastlingRight(whiteQueenSide) << std::endl;
    std::cout << "black Kingside: " << " = " << getCastlingRight(blackKingSide) << std::endl;
    std::cout << "black Queenside: " << " = " << getCastlingRight(blackQueenSide) << std::endl;
}

bool Position::makeMove(int& move) {
    // decoding move
    int startingSquare = getMoveStart(move);
    int targetSquare = getMoveTarget(move);
    int piece = getMovePiece(move);
    int promotion = getMovePromotion(move);
    bool doublePush = getMoveDoublePush(move);
    bool capture = getMoveCapture(move);
    bool enPassant = getMoveEnPassant(move);
    bool castling = getMoveCastling(move);

    // calculating captured piece before saving state in history
    capturedPiece = 0;
    if(capture){
        handleCaptureFlag(targetSquare);
    }

    // making move on the board
    _board->removePiece(piece,startingSquare);
    if(promotion){
        _board->placePiece(promotion,targetSquare);
    }
    else{
        _board->placePiece(piece,targetSquare);
    }

    // Checking for En Passant
    if(enPassant) {
        handleEnPassantFlag();
    }

    // undo if king in check and return false
    if(whiteToMove){
        if(_atkTables->squareAttackedBy(getLSB(_board->getPieceSet(nWhiteKing)), false)){
            undoIllegal(startingSquare, targetSquare, piece, enPassant, promotion);
            return false;
        }
    }
    else {
        if(_atkTables->squareAttackedBy(getLSB(_board->getPieceSet(nBlackKing)), true)){
            undoIllegal(startingSquare, targetSquare, piece, enPassant, promotion);
            return false;
        }
    }

    // saving current state before changing castling rights and enPassantSquare
    addStateToHistory(move);

    // Checking for king side castling
    if(castling && (targetSquare == G1 || targetSquare == G8)) {
        handleCastlesKingSide();
    }

    // Checking for queens side castling
    if(castling && (targetSquare == C1 || targetSquare == C8)) {
        handleCastlesQueenSide();
    }

    // removing relevant castling rights if they are still there
    if(castlingRights){
        if(piece == nWhiteKing){
            removeCastlingRight(whiteKingSide);
            removeCastlingRight(whiteQueenSide);
        }
        if(piece == nBlackKing){
            removeCastlingRight(blackKingSide);
            removeCastlingRight(blackQueenSide);
        }
        if(startingSquare == A1 || targetSquare == A1){
            removeCastlingRight(whiteQueenSide);
        }
        if(startingSquare == A8 || targetSquare == A8){
            removeCastlingRight(blackQueenSide);
        }
        if(startingSquare == H1 || targetSquare == H1){
            removeCastlingRight(whiteKingSide);
        }
        if(startingSquare == H8 || targetSquare == H8){
            removeCastlingRight(blackKingSide);
        }
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
    if(whiteToMove){
        blackInCheck = givesCheck();
    }
    else{
        whiteInCheck = givesCheck();
    }

    passTurn();
    return true;
}
void Position::handleCaptureFlag(int targetSquare) {
    // searching for Piece to capture
    if (whiteToMove) {
        for (int piece = nBlackPawn; piece <= nBlackKing; piece++) {
            if (_board->pieceBB[piece] & (1ULL << targetSquare)) {
                capturedPiece = piece;
                break;
            }
        }
    }
    else {
        for (int piece = nWhitePawn; piece <= nWhiteKing; piece++) {
            if (_board->pieceBB[piece] & (1ULL << targetSquare)) {
                capturedPiece = piece;
                break;
            }
        }
    }
    // removing captured Piece from board
    if(!capturedPiece){
        throw std::invalid_argument("no piece to capture - handleCaptureFlag()");
    }
    _board->removePiece(capturedPiece,targetSquare);
}

void Position::handleEnPassantFlag() const {
    int pawnToCapture = 0;
    // searching for Piece to capture
    if (getWhiteToMove()) {
        pawnToCapture = getLSB(getEnPassantSquare() >> 8);
        // removing captured Piece from _board
        _board->removePiece(nBlackPawn,pawnToCapture);
    } else {
        pawnToCapture = getLSB(getEnPassantSquare() << 8);
        // removing captured Piece from _board
        _board->removePiece(nWhitePawn,pawnToCapture);
    }
}
void Position::handleCastlesKingSide() {

    if(whiteToMove){
        _board->removePiece(nWhiteRook,H1);
        _board->placePiece(nWhiteRook,F1);

        removeCastlingRight(whiteKingSide);
        removeCastlingRight(whiteQueenSide);
    }
    else{
        _board->removePiece(nBlackRook,H8);
        _board->placePiece(nBlackRook,F8);

        removeCastlingRight(blackKingSide);
        removeCastlingRight(blackQueenSide);
    }
}

void Position::handleCastlesQueenSide() {
    if(whiteToMove){
        // moving rook
        _board->removePiece(nWhiteRook,A1);
        _board->placePiece(nWhiteRook,D1);

        removeCastlingRight(whiteKingSide);
        removeCastlingRight(whiteQueenSide);
    }
    else{
        _board->removePiece(nBlackRook,A8);
        _board->placePiece(nBlackRook,D8);

        removeCastlingRight(blackKingSide);
        removeCastlingRight(blackQueenSide);
    }
}

void Position::removeCastlingRight(int enumRight) {
    castlingRights &= ~enumRight;
}

void Position::undoMove() {
    // Give turn back
    passTurn();

    State prevPos = getPrevPosition();

    previousMove = prevPos.previousMove;
    whiteInCheck = prevPos.whiteInCheck;
    blackInCheck = prevPos.blackInCheck;
    whiteToMove = prevPos.whiteToMove;
    enPassantSquare = prevPos.enPassantSquare;
    castlingRights = prevPos.castlingRights;
    capturedPiece = prevPos.capturedPiece;

    int start = getMoveStart(previousMove);
    int piece = getMovePiece(previousMove);
    int target = getMoveTarget(previousMove);
    bool enPassant = getMoveEnPassant(previousMove);
    int castling = getMoveCastling(previousMove);
    int promotion = getMovePromotion(previousMove);

    // undo move
    _board->placePiece(piece,start);
    if(promotion){
        _board->removePiece(promotion,target);
    }
    else{
        _board->removePiece(piece,target);
    }

    if(capturedPiece){
        _board->placePiece(capturedPiece,target);
    }

    if(enPassant){
        if(whiteToMove){
            _board->placePiece(nBlackPawn, getLSB(enPassantSquare) - 8);
        }
        else{
            _board->placePiece(nWhitePawn, getLSB(enPassantSquare) + 8);
        }
    }

    if(castling){
        if(target == G1){
            _board->removePiece(nWhiteRook,F1);
            _board->placePiece(nWhiteRook,H1);
        }
        if(target == C1){
            _board->removePiece(nWhiteRook,D1);
            _board->placePiece(nWhiteRook,A1);
        }
        if(target == G8){
            _board->removePiece(nBlackRook,F8);
            _board->placePiece(nBlackRook,H8);
        }
        if(target == C8){
            _board->removePiece(nBlackRook,D8);
            _board->placePiece(nBlackRook,A8);
        }
    }

    deleteLastPositionFromHistory();
}

void Position::resetCastlingRights() {
    castlingRights = 0;
}

int Position::getCastlingRighs() {
    return castlingRights;
}

void Position::undoIllegal(int start, int target, int piece, bool enPassant, int promotion) {

    // replace captured piece
    if(capturedPiece){
        _board->placePiece(capturedPiece,target);
    }
    // undo move
    _board->placePiece(piece,start); // also places on nWhite/nBlack board
    if(promotion){
        _board->removePiece(promotion,target);
    }
    else{
        _board->removePiece(piece,target);
    }

    // replace captured enPassantPawn
    if(enPassant){
        if (whiteToMove){
            _board->placePiece(nBlackPawn, (getLSB(enPassantSquare) - 8));
        }
        else {
            _board->placePiece(nWhitePawn, (getLSB(enPassantSquare) + 8));
        }
    }
}

bool Position::givesCheck() const {
    if(whiteToMove && _board->getPieceSet(nBlackKing)){
        if(_atkTables->squareAttackedBy(getLSB(_board->getPieceSet(nBlackKing)), true)){
            return true;
        }
        else{
            return false;
        }
    }
    else if(!whiteToMove && _board->getPieceSet(nWhiteKing)){
        if(_atkTables->squareAttackedBy(getLSB(_board->getPieceSet(nWhiteKing)), false)){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
};
