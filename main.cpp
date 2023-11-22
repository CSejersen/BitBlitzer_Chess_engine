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

    // Init _board
    GameState state;
    BitBoard board(&state);
    AttackTables atkTables(&board, &state);
    MoveGenerator moveGenerator(&board, &state, &atkTables);
    FenParser fenParser(&board, &state);
    Search search(&board, &moveGenerator, &atkTables, &state);

    fenParser.loadFenPosition("r1bqkb1r/pppp1ppp/2n5/PB2p3/4P3/5Nn1/1PPP1PPP/RNBQK2R b KQkq - 0 6");
    BitBoard::printBB(board.getPieceSet(nBlack) | board.getPieceSet(nWhite));

    moveGenerator.generateMoves();
    moveGenerator.pruneIllegalMoves();

    std::cout << "Moves" << std::endl;
    for (Move &m: moveGenerator.legalMoves) {
        std::cout << board.indexToCoordinate(m.getStartSquare()) << "-" << board.indexToCoordinate(m.getTargetSquare())
                  << std::endl;
    }

    std::cout << "Captures: " << std::endl;
    for (Move &move: moveGenerator.pseudoLegalCapture) {
        std::cout << board.indexToCoordinate(move.getStartSquare()) << "-"
                  << board.indexToCoordinate(move.getTargetSquare()) << std::endl;
    }

    std::cout << "castling Rights: " << std::endl;
    for (int i = 0; i <= 3; i++) {
        std::cout << "at index " << i << " = " << state.getCastlingRight(i) << std::endl;
    }

}