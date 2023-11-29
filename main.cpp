#include <iostream>
#include "BitBoard.h"
#include "Position.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "Position.h"
#include "FenParser.h"
#include "Search.h"
#include "perft.h"

int main(int argc, char* argv[]) {

    BitBoard board;
    AttackTables atkTables(&board);
    Position position(&board, &atkTables);
    MoveGenerator moveGenerator(&board, &atkTables);

    Perft perft(&moveGenerator,&position, &board);
    FenParser fenParser(&board, &position);
    // Load position and print board
//    fenParser.loadStartingPosition();

    int move = encodeMove(D5,E6,nWhitePawn,0,1,0,0,0);
    int move2 = encodeMove(H3,G2,nBlackPawn,0,1,0,0,0);
//    int move3 = encodeMove(D5,E6,nWhitePawn,0,1,0,0,0);
//    int move4 = encodeMove(C8,A6,nBlackBishop,0,0,0,0,0);

    fenParser.loadStartingPosition();
//    fenParser.loadFenPosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

//    position.makeMove(move);
//    position.makeMove(move2);
//    position.makeMove(move3);
//    position.makeMove(move4);
//    BitBoard::printBB(board.getAllPieces());

    int depth;
    std::cout << "input search depth: ";
    std::cin >> depth;

    perft.perft(depth);

}