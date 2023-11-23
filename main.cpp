#include <iostream>
#include "bitboards.h"
#include "game_state.h"
#include "move.h"
#include "move_generator.h"
#include "attack_tables.h"
#include "game_state.h"
#include "fen_parser.h"
#include "search.h"

int main(int argc, char* argv[]) {

    GameState state;
    BitBoard board(&state);
    AttackTables atkTables(&board, &state);
    MoveGenerator moveGenerator(&board, &state, &atkTables);
    FenParser fenParser(&board, &state);

    // Load position and print board
    fenParser.loadFenPosition("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
    BitBoard::printBB(board.getPieceSet(nBlack) | board.getPieceSet(nWhite));

    moveGenerator.generateMoves();
    moveGenerator.printLegalMoves();

    state.printCastlingRights();

}
