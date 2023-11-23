#include "bitboards.h"

// Bitboards constructor
BitBoard::BitBoard(GameState* state){
    _state = state;
}

// returns the bitboard for the requested Piece
U64 BitBoard::getPieceSet(enumPieceBB pieceType) const {
    return pieceBB[pieceType];
}

// places Piece on a given _square of passed Bitboard
void BitBoard::placePiece(int pieceType, int square){
    U64 placementMask = 1ULL << square;
    pieceBB[pieceType] |= placementMask;
    if (pieceType < 7) {
        pieceBB[nWhite] |= placementMask;
    }
    else{
        pieceBB[nBlack] |= placementMask;
    }
}


// prints a nice view of bitboard for a given Piece.
void BitBoard::printBB(const U64& bb) {
    U64 shiftMe = 1ULL;
    for(int file = 7; file >= 0; file--){
        for(int i = 0; i < 8; i++){
            if((shiftMe << ((file * 8) + i)) & bb){
                std::cout << "x ";
            }
            else{
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void BitBoard::clearBoard(){
    for(U64& pieceSet : pieceBB){
        pieceSet = 0ULL;
    }
}

void BitBoard::makeMove(Move& move) {
    int startingSquare = move.getStartSquare();
    int targetSquare = move.getTargetSquare();
    int flag = move.getFlag();
    int pieceToMove = 0;

    // checking for capture
    if(flag == nCapture || flag == nRookPromoCapture || flag == nBishopPromoCapture || flag == nKnightPromoCapture || flag == nQueenPromoCapture)
        handleCaptureFlag(move);

    // Checking for En Passant
    if(flag == nEnPassantCapture)
        handleEnPassantFlag(move);

    // Checking for king side castling
    if(flag == nKingCastle)
        handleCastlesKingSide();

    // Checking for queens side castling
    if(flag == nQueenCastle)
        handleCastlesQueenSide();





    // Looping over all pieceTypes to find pieceToMove.
    for(int pieceType = nWhitePawn; pieceType <= nBlackKing; pieceType++){
        // skipping _board with all black pieces (all white _board already skipped by starting loop at nWhitePawn);
        if(pieceType == nBlack){
            continue;
        }
        // Finding Piece on starting _square
        if(pieceBB[pieceType] & 1ULL << startingSquare){
            pieceToMove = pieceType;
            break;
        }
    }

    // making move on the _board
    pieceBB[pieceToMove] &= ~(1ULL << startingSquare); // removing Piece from starting square
    placePiece(pieceToMove,targetSquare);
    if(_state->getWhiteToMove()){
        pieceBB[nWhite] &= ~(1ULL << startingSquare);
    }
    else{
        pieceBB[nBlack] &= ~(1ULL << startingSquare);
    }



    // reset En-Passant square and set a new one if double pawn push was played.
    _state->resetEnPassantSquare();
    if(flag == nDoublePawnPush){
        if(_state->getWhiteToMove())
            _state->setEnPassantSquare(targetSquare - 8);
        else
            _state->setEnPassantSquare(targetSquare + 8);
    }

    _state->passTurn();
    _state->addMoveToHistory(move);
    _state->updateCastlingRights();
}

void BitBoard::handleCaptureFlag(Move& move) {
    int targetSquare = move.getTargetSquare();
    Piece pieceToCapture;

    // Checking for capture
    // searching for Piece to capture
    if (_state->getWhiteToMove()) {
        for (int piece = nBlackPawn; piece <= nBlackKing; piece++) {
            if (pieceBB[piece] & 1ULL << targetSquare) {
                pieceToCapture.setPieceType(piece);
                break;
            }
        }
    } else {
        for (int piece = nWhitePawn; piece <= nWhiteKing; piece++) {
            if (pieceBB[piece] & 1ULL << targetSquare) {
                pieceToCapture.setPieceType(piece);
                break;
            }
        }
        pieceToCapture.setSquare(targetSquare);
        move.setCapturedPiece(pieceToCapture);


    }

    // removing captured Piece from _board
    pieceBB[pieceToCapture.getPieceType()] &= ~1ULL << targetSquare;
    // storing capture piece in move member attribute
    move.setCapturedPiece(pieceToCapture);

    // removing from nWhite and nBlack pieceBB's
    if(_state->getWhiteToMove())
        pieceBB[nBlack] &= ~(1ULL << targetSquare);
    else
        pieceBB[nWhite] &= ~(1ULL << targetSquare);
}

void BitBoard::handleEnPassantFlag(Move& move) {
    int startingSquare = move.getStartSquare();
    int targetSquare = move.getTargetSquare();
    int pieceToCapture = 0;
    // searching for Piece to capture
    if (_state->getWhiteToMove()) {
        pieceToCapture = getLSB(getPieceSet(nBlackPawn) & _state->getEnPassantSquare() >> 8);
        // removing captured Piece from _board
        pieceBB[nBlackPawn] &= ~(1ULL << pieceToCapture);
        pieceBB[nBlack] &= ~(1ULL << pieceToCapture);
        // storing captured pawn in move member attribute
        move.setCapturedPiece(Piece(nBlackPawn, pieceToCapture));
    } else {
        pieceToCapture = getLSB(getPieceSet(nWhitePawn) & _state->getEnPassantSquare() << 8);
        // removing captured Piece from _board
        pieceBB[nWhitePawn] &= ~1ULL << pieceToCapture;
        pieceBB[nWhite] &= ~1ULL << pieceToCapture;
        // storing captured pawn in move member attribute
        move.setCapturedPiece(Piece(nWhitePawn, pieceToCapture));
    }
}

int BitBoard::coordinateToIndex(std::string coordinate) {

    int file = 0;
    int rank = 0;
    int squareIndex = 0;

    if(coordinate.size() != 2){
        throw std::invalid_argument("invalid coordinate size");
    }

    else{
        for(int i = 0; i < 2; i++) {
            switch (coordinate[i]) {
                case 'a':
                    file = 0;
                    break;
                case 'b':
                    file = 1;
                    break;
                case 'c':
                    file = 2;
                    break;
                case 'd':
                    file = 3;
                    break;
                case 'e':
                    file = 4;
                    break;
                case 'f':
                    file = 5;
                    break;
                case 'g':
                    file = 6;
                    break;
                case 'h':
                    file = 7;
                    break;
                case '1':
                    rank = 0;
                    break;
                case '2':
                    rank = 1;
                    break;
                case '3':
                    rank = 2;
                    break;
                case '4':
                    rank = 3;
                    break;
                case '5':
                    rank = 4;
                    break;
                case '6':
                    rank = 5;
                    break;
                case '7':
                    rank = 6;
                    break;
                case '8':
                    rank = 7;
                    break;
                default:
                    throw std::invalid_argument("invalid coordinate");

            }
        }
        squareIndex = (rank * 8) + file;

    }
    return squareIndex;
}

void BitBoard::undoMove() {
    // Give turn back
    _state->passTurn();

    Move move = _state->getLastMove();
    int startSquare = move.getStartSquare();
    int targetSquare = move.getTargetSquare();
    int flag = move.getFlag();
    Piece capturedPiece;
    int pieceToMove = 0;

    // finding moved piece
    for(int pieceType = nWhitePawn; pieceType <= nBlackKing; pieceType++){
        if(pieceType == nBlack){
            continue;
        }
        if(pieceBB[pieceType] & 1ULL << targetSquare){
            pieceToMove = pieceType;
        }
        if(pieceToMove){
            break;
        }
    }
    // Undoing move on the _board
    pieceBB[pieceToMove] &= ~(1ULL << targetSquare);
    placePiece(pieceToMove,startSquare);
    if(_state->getWhiteToMove()){
        pieceBB[nWhite] &= ~(1ULL << targetSquare);
    }
    else{
        pieceBB[nBlack] &= ~(1ULL << targetSquare);
    }


    if(flag == nCapture || flag == nEnPassantCapture || flag == nKnightPromoCapture || flag == nBishopPromoCapture || flag == nRookPromoCapture || flag == nQueenPromoCapture){

        capturedPiece = move.getCapturedPiece();
        // replacing captured piece
        placePiece(capturedPiece.getPieceType(),capturedPiece.getSquare());
    }
    // passing turn back
    _state->deleteLastMoveFromHistory();
};

std::string BitBoard::indexToCoordinate(int index){
    std::string rank;
    std::string file;
    rank += std::to_string(index/8 + 1);
    switch (index % 8) {

        case 0:
            file = "a";
            break;
        case 1:
            file = "b";
            break;
        case 2:
            file = "c";
            break;
        case 3:
            file = "d";
            break;
        case 4:
            file = "e";
            break;
        case 5:
            file = "f";
            break;
        case 6:
            file = "g";
            break;
        case 7:
            file = "h";
            break;
        default:
            throw std::invalid_argument("error in indexToCoordinate switch case");
    }
    return file+rank;

}

void BitBoard::handleCastlesKingSide() {

    if(_state->getWhiteToMove()){
        // Removing rook from H1
        pieceBB[nWhiteRook] &= ~(1ULL << H1);
        pieceBB[nWhite] &= ~(1ULL << H1);

        // Placing rook on F1
        placePiece(nWhiteRook,F1);
    }
    else{
        // removing rook from H8
        pieceBB[nBlackRook] &= ~(1ULL << H8);
        pieceBB[nBlack] &= ~(1ULL << H8);

        // Placing rook on F8
        placePiece(nWhiteRook,F8);
    }
}

void BitBoard::handleCastlesQueenSide() {
    if(_state->getWhiteToMove()){
        // Removing rook from H1
        pieceBB[nWhiteRook] &= ~(1ULL << A1);
        pieceBB[nWhite] &= ~(1ULL << A1);

        // Placing rook on F1
        placePiece(nWhiteRook,D1);
    }
    else{
        // removing rook from H8
        pieceBB[nBlackRook] &= ~(1ULL << A8);
        pieceBB[nBlack] &= ~(1ULL << A8);
        // Placing rook on F8
        placePiece(nWhiteRook,D8);
    }
}





