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

    int move = encodeMove(D2,C1,nWhiteBishop,0,0,0,0,0);
    int move2 = encodeMove(E7,D6,nBlackQueen,0,0,0,0,0);
    int move3 = encodeMove(E1,D1,nWhiteKing,0,0,0,0,0);
//    int move4 = encodeMove(C8,A6,nBlackBishop,0,0,0,0,0);

    fenParser.loadStartingPosition();
    // fenParser.loadFenPosition("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

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