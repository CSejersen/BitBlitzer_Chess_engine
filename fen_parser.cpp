//
// Created by Christian Sejersen on 21/11/2023.
//
#include "fen_parser.h"
#include "bitboards.h"
#include <string>
#include <iostream>
FenParser::FenParser(BitBoard* board,GameState* state) {
    _board = board;
    _state = state;

}
int FenParser::fenParsePieces(std::string& fen){
    int currentSquare = A8;
    int file = 0;
    int rank = 7;
    int index = 0;
    bool quit = false;

    for (int i = 0; i < fen.size() && !quit; ++i) {
        currentSquare = (rank * 8) + file;
        if (isdigit(fen[i])) {
            int emptySquares = fen[i] - '0';  // Character to integer
//            std::cout << "Adding " << emptySquares << " empty squares" << std::endl;
            file += emptySquares; // Skipping empty squares
//            std::cout << "file now equal to: " << file << std::endl;
        } else {
            switch (fen[i]) {
                case 'P':
//                    std::cout << "placing white pawn on: " << currentSquare << std::endl;
                    _board->placePiece(nWhitePawn, currentSquare);
                    _board->placePiece(nWhite, currentSquare);
                    break;
                case 'R':
//                    std::cout << "placing white rook on: " << currentSquare << std::endl;
                    _board->placePiece(nWhiteRook, currentSquare);
                    _board->placePiece(nWhite, currentSquare);
                    break;
                case 'N':
//                    std::cout << "placing white Knight on: " << currentSquare << std::endl;
                    _board->placePiece(nWhiteKnight, currentSquare);
                    _board->placePiece(nWhite, currentSquare);
                    break;
                case 'B':
//                    std::cout << "placing white Bishop on: " << currentSquare << std::endl;
                    _board->placePiece(nWhiteBishop, currentSquare);
                    _board->placePiece(nWhite, currentSquare);
                    break;
                case 'Q':
//                    std::cout << "placing white Queen on: " << currentSquare << std::endl;
                    _board->placePiece(nWhiteQueen, currentSquare);
                    _board->placePiece(nWhite, currentSquare);
                    break;
                case 'K':
//                    std::cout << "placing white King on: " << currentSquare << std::endl;
                    _board->placePiece(nWhiteKing, currentSquare);
                    _board->placePiece(nWhite, currentSquare);
                    break;

                case 'p':
//                    std::cout << "placing black pawn on: " << currentSquare << std::endl;
                    _board->placePiece(nBlackPawn, currentSquare);
                    _board->placePiece(nBlack, currentSquare);
                    break;
                case 'r':
//                    std::cout << "placing black Rook on: " << currentSquare << std::endl;
                    _board->placePiece(nBlackRook, currentSquare);
                    _board->placePiece(nBlack, currentSquare);
                    break;
                case 'n':
//                    std::cout << "placing black Knight on: " << currentSquare << std::endl;
                    _board->placePiece(nBlackKnight, currentSquare);
                    _board->placePiece(nBlack, currentSquare);
                    break;
                case 'b':
//                    std::cout << "placing black Bishop on: " << currentSquare << std::endl;
                    _board->placePiece(nBlackBishop, currentSquare);
                    _board->placePiece(nBlack, currentSquare);
                    break;
                case 'q':
//                    std::cout << "placing black Queen on: " << currentSquare << std::endl;
                    _board->placePiece(nBlackQueen, currentSquare);
                    _board->placePiece(nBlack, currentSquare);
                    break;
                case 'k':
//                    std::cout << "placing black King on: " << currentSquare << std::endl;
                    _board->placePiece(nBlackKing, currentSquare);
                    _board->placePiece(nBlack, currentSquare);
                    break;

                case '/':
                    if(!file % 7 == 0){
//                        std::cout << "true" << std::endl;
                        rank --;
                        file = -1;
//                        std::cout << "rank now equal to: " << rank << std::endl;
                        break;
                    }
                    else{
                        throw std::invalid_argument("invalid fen string");
                    }
                case ' ':
                    quit = true;

                default:
                    break;
            }
            file += 1;
        }
        index += 1;
    }
    return index;
}

int FenParser::fenParseTurn(int index, std::string& fen){
    if(fen[index] == 'w'){
        _state->setWhiteToMove(true);
    }
    else{
        _state->setWhiteToMove(false);
    }
    return index += 2;
}

int FenParser::fenParseCastlingRights(int index, std::string& fen) {
    int indexTracker = index;
    bool quit = false;

    _state->resetCastlingRights();

    for(int i = index; i < fen.size() && !quit; i++) {
        switch (fen[i]) {
            case 'K':
                _state->setCastlingRight(whiteKingside);
                break;

            case 'Q':
                _state->setCastlingRight(whiteQueenSide);
                break;

            case 'k':
                _state->setCastlingRight(blackKingside);
                break;

            case 'q':
                _state->setCastlingRight(blackQueenside);
                break;

            case '-':
                break;

            case ' ':
                quit = true;
                break;
        }
        indexTracker += 1;
    }
    return indexTracker;
}

int FenParser::fenParseEnPassant(int index, std::string& fen) {
    int rank = 0;
    int file = 0;
    bool quit = false;
    bool enPassant = true;
    int indexTracker = index;

    for(int i = index; i <= fen.size() && !quit; i++){
        switch (fen[i]) {
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
            case '-':
                enPassant = false;
                break;
            case ' ':
                quit = true;
                break;

            default:
                throw std::invalid_argument("invalid FEN string En-passant error");
        }
        indexTracker += 1;

    }
    if(enPassant){
        int square = (rank*8) + file;
        _state->setEnPassantSquare(square);

    }
    return indexTracker;
}

int FenParser::fenParseHalfmove(int index, std::string &fen) {
    std::string halfmoves;
    int indexTracker = index;
    bool quit = false;

    for(int i = index; i < fen.size() && !quit; i++){
        if(isdigit(fen[i])){
            halfmoves += fen[i];
        }
        else{
            quit = true;
        }
        indexTracker ++;
    }
    _state->setHalfMoveClock(stoi(halfmoves));
    return indexTracker;
}

void FenParser::fenParseMoveNum(int index, std::string &fen) {
    std::string moves;
    bool quit = false;

    for(int i = index; i < fen.size() && !quit; i++){
        if(isdigit(fen[i])){
            moves += fen[i];
        }
        else{
            quit = true;
        }
    }
    _state->setMoveNum(stoi(moves));
}

void FenParser::loadFenPosition(const std::string& fenString) {
    _board->clearBoard();
    std::string fen = fenString;
    int index = 0;
    index = fenParsePieces(fen);
    index = fenParseTurn(index, fen);
    index = fenParseCastlingRights(index, fen);
    index = fenParseEnPassant(index, fen);
    index = fenParseHalfmove(index, fen);
    fenParseMoveNum(index, fen);
}
