#include <iostream>
#include "bitboards.h"
#include "game_state.h"
#include "move.h"
#include "move_generator.h"
#include "attack_tables.h"
#include "game_state.h"
#include "fen_parser.h"


//#include <SDL_image.h>

int main(int argc, char* argv[]){

    // Init _board
    GameState state;
    BitBoard board(&state);
    AttackTables atkTables(&board,&state);
    MoveGenerator moveGenerator(&board, &state, &atkTables);
    FenParser fenParser(&board, &state);

    fenParser.loadStartingPosition();

    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    Move move(E2,E4, nDoublePawnPush);
    Move move2(B8,C6, nQuietMoves);
    Move move3(E4,E5, nQuietMoves);
    Move move4(D7,D5, nDoublePawnPush);
    Move move5(E5,D6,nEnPassantCapture);
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
//    _board.makeMove(move6);
//    BitBoard::printBB(_board.getPieceSet(nWhite) | _board.getPieceSet(nBlack));
//    _board.makeMove(move7);
//    BitBoard::printBB(_board.getPieceSet(nWhite) | _board.getPieceSet(nBlack));
//    _board.makeMove(move8);
//    BitBoard::printBB(_board.getPieceSet(nWhite) | _board.getPieceSet(nBlack));

    moveGenerator.generateKnightMoves();
    moveGenerator.generateBishopMoves();
    moveGenerator.generateRookMoves();
    moveGenerator.generateKingMoves();
    moveGenerator.generateQueenMoves();
    moveGenerator.generatePawnAdvancesWhite();
    moveGenerator.generatePawnCaptures();
    moveGenerator.generateEnPassant();

    std::cout << "Moves: " << std::endl;
    for(auto & it : moveGenerator.pseudoLegal){
        std::cout << board.indexToCoordinate(it.getStartSquare()) << "-" << board.indexToCoordinate(it.getTargetSquare()) << std::endl;
    }

    std::cout << "captures: " << std::endl;
    for(auto & it : moveGenerator.captures){
        std::cout << board.indexToCoordinate(it.getStartSquare()) << "-" << board.indexToCoordinate(it.getTargetSquare()) << std::endl;
    }
}