#include "perft.h"
#include <iostream>


U64 Perft::countNodes(int depth) {
    // stop recursion when depth is reached and count node.
    if (depth == 0){
        return 1ULL;
    }

    U64 nodes = 0;
    std::vector<int> moves;
    moveGenerator->generateMoves(moves);

    for(int move : moves){
        if(position->makeMove(move)){
            nodes += countNodes(depth - 1);
            position->undoMove();
        }
    }
    return nodes;
}

void Perft::perft(int depth){
    std::vector<int> moves;
    moveGenerator->generateMoves(moves);
    U64 nodes = 0;
    U64 totalNodes = 0;

    for(int& move : moves){
        if(position->makeMove(move)){
            nodes = countNodes(depth - 1);
            totalNodes += nodes;
            std::cout << BitBoard::indexToCoordinate(getMoveStart(move)) << BitBoard::indexToCoordinate(getMoveTarget(move)) << ": " << nodes << std::endl;
            position->undoMove();
            }
    }
    std::cout << "\nNodes searched: " << totalNodes << std::endl;
}

Perft::Perft(MoveGenerator* moveGen, Position* pos, BitBoard* bb) {
    position = pos;
    moveGenerator = moveGen;
    board = bb;
}

