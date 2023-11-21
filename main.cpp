#include <iostream>
#include "bitboards.h"
#include "move.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include "SDL_functions.h"
//#include <SDL_image.h>

int main(int argc, char* argv[]){

    // Init board
    BitBoard board;
    board.loadStartingPosition();
    Move move(E2,E4, nDoublePawnPush);
    Move move2(E7,E5, nDoublePawnPush);
    Move move3(G1,F3, nQuietMoves);
    Move move4(B8,C6, nQuietMoves);
    Move move5(F1,C4,nQuietMoves);
    Move move6(G8,F6,nQuietMoves);
    Move move7(E2,E4,nQuietMoves);
    Move move8(D7,D6,nQuietMoves);

    board.makeMove(move);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    board.makeMove(move2);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    board.makeMove(move3);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    board.makeMove(move4);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    board.makeMove(move5);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    board.makeMove(move6);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
//    board.makeMove(move7);
//    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
//    board.makeMove(move8);
//    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));

    board.generateKnightMoves();
    board.generateBishopMoves();
    board.generateRookMoves();
    board.generateKingMoves();
    board.generateQueenMoves();
    board.generatePawnAdvances();
    board.generatePawnCaptures();

    std::cout << "Moves: " << std::endl;
    for(auto & it : board.pseudoLegal){
        std::cout << board.indexToCoordinate(it.getStartSquare()) << "-" << board.indexToCoordinate(it.getTargetSquare()) << std::endl;
    }

    std::cout << "captures: " << std::endl;
    for(auto & it : board.captures){
        std::cout << board.indexToCoordinate(it.getStartSquare()) << "-" << board.indexToCoordinate(it.getTargetSquare()) << std::endl;
    }
}