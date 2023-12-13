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

    // initializing Engine
    BitBoard board;
    AttackTables atkTables(&board);
    Position position(&board, &atkTables);
    MoveGenerator moveGenerator(&board, &atkTables);
    Search search(&board, &moveGenerator, &atkTables, &position);
    Perft perft(&moveGenerator, &position, &board);
    FenParser fenParser(&board, &position);

    int move1 = encodeMove(D2, D3, nWhitePawn, 0, 0, 0, 0, 0);
    // Load position and print board
    fenParser.loadStartingPosition();
    bool play = true;
    while (play) {
        BitBoard::printBB(board.getAllPieces());

        std::string start, target;
        std::cout << "input move: " << std::endl;
        std::cin >> start >> target;
        std::cout << start << " " << target << std::endl;

        if (start == "q") {
            play = false;
            continue;
        }

        std::vector<int> moves;
        bool inCheck = position.getWhiteToMove() ? position.whiteInCheck : position.blackInCheck;
        moveGenerator.generateMoves(moves, position.getWhiteToMove(), position.getCastlingRighs(),
                                    position.getEnPassantSquare(), inCheck);

        for (int &move: moves) {
            if (BitBoard::indexToCoordinate(getMoveStart(move)) == start &&
                BitBoard::indexToCoordinate(getMoveTarget(move)) == target) {
                position.makeMove(move);
                BitBoard::printBB(board.getAllPieces());
                break;
            }
        }
        std::cout << "Thinking..." << std::endl;
        int robotMove = search.search(5);
        position.makeMove(robotMove);
    }
}