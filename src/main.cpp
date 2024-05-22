#include "AttackTables.h"
#include "BitBoard.h"
#include "FenParser.h"
#include "MoveGenerator.h"
#include "Position.h"
#include "Search.h"
#include "Socket_Handler.hpp"
#include "board_constants.h"
#include "gameHandler.hpp"
#include "perft.h"
#include <iostream>
#include <string>

int main() {
  // initialize Engine
  BitBoard board;
  AttackTables atkTables(&board);
  Position position(&board, &atkTables);
  FenParser fenParser(&board, &position);
  MoveGenerator moveGenerator(&board, &atkTables);
  Search search(&board, &moveGenerator, &atkTables, &position);
  GameHandler gameHandler(&search, &moveGenerator, &position);
  SocketHandler socketHandler;

  // for test purposes
  Perft perft(&moveGenerator, &position, &board);

  bool run = true;
  while (run) {
    std::string input;
    std::cout << "welcome to bit blitzer:" << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "commands:" << std::endl;
    std::cout << "- (p)erft" << std::endl;
    std::cout << "- (m)agiChess" << std::endl;
    std::cout << "- (e)xit" << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << ">> ";
    std::cin >> input;
    std::cout << "-----------------------" << std::endl;

    if (input == "p") {
      std::cout << "Entered Perft mode with position: " << std::endl;
      std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0" ;
      fenParser.loadFenPosition(fen);
      /* fenParser.loadStartingPosition(); */
      BitBoard::printBB(board.getAllPieces());
      int depth;
      std::cout << "Input depth: ";
      std::cin >> depth;
      perft.perft(depth);
    }

    if (input == "m") {
      std::cout << "Entered MagiChess mode\nConnecting to socket..." << std::endl;
      socketHandler.connectToSocket(
          "/Users/sejersen/dev/magichess/magiChess_server_V2/chess.sock");
      socketHandler.writeToSocket("engine");

      std::string input;
      fenParser.loadStartingPosition();
      while (run) {
        std::string input = socketHandler.readFromSocket();
        std::string resp = gameHandler.handleInput(input);
        if(resp == "fen loaded"){
          BitBoard::printBB(board.getAllPieces());
        }
        socketHandler.writeToSocket(resp);
      }
    }

    if (input == "e") {
    std::cout << "Shutting down" << std::endl;
      run = false;
    }
  }
}
