#include <iostream>
#include "bitboards.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include "SDL_functions.h"
//#include <SDL_image.h>

int main(int argc, char* argv[])
{
    // Init board
    BitBoard board;
    board.loadStartingPosition();

    Move move;
    move.startSquare = "e2";
    move.targetSquare = "e4";

    BitBoard::printBB(board.getPieceSet(nWhitePawn));
    board.makeMove(move);
    BitBoard::printBB(board.getPieceSet(nWhite));

}