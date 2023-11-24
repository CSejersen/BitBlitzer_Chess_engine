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

    GameState state;
    BitBoard board(&state);
    AttackTables atkTables(&board, &state);
    MoveGenerator moveGenerator(&board, &state, &atkTables);
    FenParser fenParser(&board, &state);

    // Load position and print board
    fenParser.loadFenPosition("r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
    BitBoard::printBB(board.getPieceSet(nBlack) | board.getPieceSet(nWhite));

    moveGenerator.generateMoves();
    moveGenerator.printLegalMoves();

    state.printCastlingRights();

}
