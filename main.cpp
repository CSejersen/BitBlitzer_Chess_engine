#include <iostream>
#include "BitBoard.h"
#include "GameState.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
#include "GameState.h"
#include "FenParser.h"
#include "Search.h"




int main(int argc, char* argv[]) {

    auto* state = new GameState;
    auto* board = new BitBoard(state);
    auto* atkTables = new AttackTables(board, state);
    auto* moveGenerator = new MoveGenerator(board, state, atkTables);

    FenParser fenParser(board, state);

    // Load position and print board
    fenParser.loadFenPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    BitBoard::printBB(board->getPieceSet(nBlack) | board->getPieceSet(nWhite));

    // counting nodes
    int depth;
    std::cout << "Input depth: ";
    std::cin >> depth;
    U64 nodes = moveGenerator->countNodes(depth);
    std::cout << "Search at depth " << depth << " hit " << nodes << " nodes" << std::endl;
}
