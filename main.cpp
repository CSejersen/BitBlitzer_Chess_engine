#include <iostream>
#include "bitboards.h"
#include <iostream>
#include <string>
#include <SDL.h>


int main(int argc, char* argv[])
{
    SDL_INIT_EVERYTHING;
    BitBoard bitBoards;

    bool whiteToPlay = true;
    bitBoards.placePiece(nWhiteRook,F4);
    bitBoards.placePiece(nWhiteBishop,A3);
    bitBoards.placePiece(nWhiteKing,D6);
    bitBoards.placePiece(nBlackKnight,F6);
    bitBoards.placePiece(nBlackBishop,A4);

    if(whiteToPlay){
        std::cout << "white pieces: " << std::endl;
        BitBoard::printBB(bitBoards.getPieceSet(nWhite));
    }
    else{
        std::cout << "black pieces: " << std::endl;
        BitBoard::printBB(bitBoards.getPieceSet(nBlack));
    }
    U64 position = (bitBoards.getPieceSet(nWhite) | bitBoards.getPieceSet(nBlack));
    U64 attacks = bitBoards.getAttacks(whiteToPlay,position);

    if(whiteToPlay){
        std::cout << "white attacks: " << std::endl;
        BitBoard::printBB(attacks);
    }
    else{
        std::cout << "black attacks: " << std::endl;
        BitBoard::printBB(attacks);
    }




    // BitBoard::printBB(bitBoards.bishopAttacksOnTheFly(E4,bitBoards.getPieceSet(nWhitePawn)));



    }

