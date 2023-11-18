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

    Move move("e2","e4");
    Move move2("e7","e5");

    board.makeMove(move);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    board.makeMove(move2);
    BitBoard::printBB(board.getPieceSet(nWhite) | board.getPieceSet(nBlack));

    for(auto & it : board.gameHistory){
        std::cout << it.getStartSquare() << "-" << it.getTargetSquare() << std::endl;
    }

}