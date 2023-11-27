//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include <string>
#include "BitBoard.h"
#include "CurrentPosition.h"

class FenParser {
public:
    FenParser(BitBoard*, CurrentPosition*);
    void loadFenPosition(const std::string& fenString);
    void loadStartingPosition();
    BitBoard* _board;
    CurrentPosition* _state;
private:
    // FEN parsing utility functions
    int fenParsePieces(std::string& fen); // returns index number reached in fen-string
    int fenParseTurn(int index, std::string& fen); // returns index number reached in fen-string
    int fenParseCastlingRights(int index, std::string& fen); // returns index number reached in fen-string
    int fenParseEnPassant(int index, std::string& fen); // returns index number reached in fen-string
    int fenParseHalfmove(int index, std::string& fen); // returns index number reached in fen-string
    void fenParseMoveNum(int index, std::string& fen); // returns index number reached in fen-string
};
