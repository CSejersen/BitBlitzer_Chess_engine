#include <iostream>
#include "BitBoard.h"
#include "CurrentPosition.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "CurrentPosition.h"
#include "FenParser.h"
#include "Search.h"

int main(int argc, char* argv[]) {

    BitBoard board;
    CurrentPosition state(&board);
    AttackTables atkTables(&board, &state);
    MoveGenerator moveGenerator(&board, &state, &atkTables);


    FenParser fenParser(&board, &state);
    // Load position and print board
//    fenParser.loadStartingPosition();



//    fenParser.loadFenPosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    fenParser.loadStartingPosition();
    int depth;
    std::cout << "input search depth: ";
    std::cin >> depth;
    moveGenerator.perft(5);

}