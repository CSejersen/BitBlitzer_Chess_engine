#include <iostream>
#include <string>
#include "BitBoard.h"
#include "Position.h"
#include "MoveGenerator.h"
#include "AttackTables.h"
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
    
    // program loop
    bool run = true;
    while(run) {
        std::string input;
        std::cout << "WELCOME TO BIT BLITZER:" << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << "COMMANDS:" << std::endl;
        std::cout << "- play" << std::endl;
        std::cout << "- eval" << std::endl;
        std::cout << "- perft" << std::endl;
        std::cout << "- exit" << std::endl;
        std::cout << "-----------------------" << std::endl;
        std::cout << ">> ";

        std::cin >> input;

        std::cout << "-----------------------" << std::endl;

        if (input == "print"){
            fenParser.loadStartingPosition();
            for(int piece = nWhitePawn; piece <= nWhiteKing; piece++){
                BitBoard::printBB(board.getPieceSet(piece));
            }
        }

        if (input == "play") {
            bool play = true;
            fenParser.loadStartingPosition();
            while (play) {
                BitBoard::printBB(board.getAllPieces());

                std::string start, target;
                std::cout << "input move: " << std::endl;
                std::cin >> start >> target;

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

        if (input == "eval") {
            // get position and depth
            std::string fen;
            std::cout << "Input fen string (\"startpos\" for starting position): ";
            std::cin.ignore();
            std::getline(std::cin, fen);
            if (fen == "startpos") {
                fenParser.loadStartingPosition();
            } else {
                fenParser.loadFenPosition("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
            }
            int depth;
            std::cout << "input depth: ";
            std::cin >> depth;

            // run evaluation
            std::cout << "\nALPHA BETA TEST AT DEPTH " << depth << std::endl;
            search.negamaxTest(depth);
        }

        if (input == "perft") {
            // get position and depth
            /* std::string fen; */
            /* std::cout << "Input fen string (\"startpos\" for starting position): "; */
            /* std::cin.ignore(); */
            /* std::getline(std::cin,fen); */
            /* if (fen == "startpos") { */
            /*     fenParser.loadStartingPosition(); */
            /* } else { */
            /*     fenParser.loadFenPosition(fen); */
            /* } */
            std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0";
            fenParser.loadFenPosition(fen);
            int depth;
            std::cout << "Input depth: ";
            std::cin >> depth;
            std::cout << "\nPerft at depth: " << depth << std::endl;
            perft.perft(depth);
        }

        if(input == "exit"){
            run = false;
        }
    }
    std::cout << "Shutting down" << std::endl;
}
