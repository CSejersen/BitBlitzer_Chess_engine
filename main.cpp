#include <iostream>
#include "bitboards.h"
#include <iostream>
#include <string>

int main()
{
    BitBoard bitBoards;

    std::cout << "printing attacks for knight on E4:" << std::endl;
    BitBoard::printBB(bitBoards.knightAttacks[E4]);

    std::cout << "printing attacks for king on f3:" << std::endl;
    BitBoard::printBB(bitBoards.kingAttacks[F3]);

    std::cout << "printing attacks for pawn on E2:" << std::endl;
    BitBoard::printBB(bitBoards.pawnAttacks[E2]);

    return 0;
}
