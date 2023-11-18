#include <iostream>
#include "bitboards.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include "SDL_functions.h"
//#include <SDL_image.h>

int main(int argc, char* argv[])
{
    BitBoard board;

    std::string fen = "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b KQk e4 99 50";

    board.loadFenPosition(fen);
    U64 blockers = (board.getPieceSet(nWhite) | board.getPieceSet(nBlack));
    U64 attacks = board.getAttacks(blockers);

    if(board.whiteToPlay){
        std::cout << "White to move!: " << std::endl;
        std::cout << "White pieces: " << std::endl;
        BitBoard::printBB(board.getPieceSet(nWhite));
        std::cout << "White attacks: " << std::endl;
        BitBoard::printBB(attacks);
    }
    else{
        std::cout << "Black to move!: " << std::endl;
        std::cout << "Black pieces: " << std::endl;
        BitBoard::printBB(board.getPieceSet(nBlack));
        std::cout << "Black attacks: " << std::endl;
        BitBoard::printBB(attacks);
    }
    std::cout << "En-passant squares: " << std::endl;
    BitBoard::printBB(board.enPassantSquares);

    std::cout << "Castleing Rights: " << std::endl;
    for(int i = 0; i < 4; i++){
        std::cout << "at index " << i << " " << board.castleingRights[i] << std::endl;
    }
    std::cout << "Halfmove clock: " << board.halfmoveClock << std::endl;
    std::cout << "Move number: " << board.moveNum << std::endl;
}