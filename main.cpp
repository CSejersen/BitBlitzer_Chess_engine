#include <iostream>
#include "bitboards.h"
#include <iostream>
#include <string>

int main()
{
    BitBoard bitBoards;

    std::cout << "printing attacks for knight on E4:" << std::endl;
    BitBoard::printBB(bitBoards.knightAttacks[B3]);

    std::cout << "printing attacks for king on f3:" << std::endl;
    BitBoard::printBB(bitBoards.kingAttacks[F3]);

    std::cout << "printing attacks for pawn on E2:" << std::endl;
    BitBoard::printBB(bitBoards.pawnAttacks[E2]);

    std::cout << "printing bishop attack mask for E4:" << std::endl;
    BitBoard::printBB(bitBoards.bishopAttackMask[E4]);

    std::cout << "printing all rook attacks with white bishops on the board:" << std::endl;

    // bitBoards.placePiece(nWhitePawn,E4);

   for(int square = A1;square <= H8; square++){
        U64 moves = bitBoards.getRookAttacks(square,bitBoards.getPieceSet(nWhiteBishop));
        BitBoard::printBB(moves);
    }
   U64 test = 0b100000;
   std::cout << BitBoard::getLSB(test) << std::endl;
   BitBoard::clearBit(test,BitBoard::getLSB(test));
    std::cout << test << std::endl;



    // BitBoard::printBB(bitBoards.bishopAttacksOnTheFly(E4,bitBoards.getPieceSet(nWhitePawn)));



    }

