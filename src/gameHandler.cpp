#include "gameHandler.hpp"
#include "BitBoard.h"
#include "FenParser.h"
#include "move.h"
#include <cassert>
#include <ostream>
#include <string>

GameHandler::GameHandler(Search *s, MoveGenerator *m, Position *p, FenParser *f)
    : search_(s), moveGenerator_(m), position_(p), fenParser_(f) {}

std::string GameHandler::getLegalTargetSquares(int startSquare) {
  std::cout << "Searching for legal target squares - input: " << startSquare
            << std::endl;
  std::string legalTargetSquares;
  std::vector<int> moves;
  bool inCheck = position_->getWhiteToMove() ? position_->whiteInCheck
                                             : position_->blackInCheck;
  moveGenerator_->generateMoves(moves, position_->getWhiteToMove(),
                                position_->getCastlingRighs(),
                                position_->getEnPassantSquare(), inCheck);

  for (int move : moves) {
    if (getMoveStart(move) == startSquare) {
      printMove(move);
      legalTargetSquares.append(
          BitBoard::indexToCoordinate(getMoveTarget(move)));
    }
  }
  return legalTargetSquares;
}

int GameHandler::stringToMove(std::string input) {
  assert(input.length() == 4);

  int inputStartSquare = BitBoard::coordinateToIndex(input.substr(0, 2));
  int inputTargetSquare = BitBoard::coordinateToIndex(input.substr(2, 2));

  std::vector<int> moves;
  bool inCheck = position_->getWhiteToMove() ? position_->whiteInCheck
                                             : position_->blackInCheck;
  moveGenerator_->generateMoves(moves, position_->getWhiteToMove(),
                                position_->getCastlingRighs(),
                                position_->getEnPassantSquare(), inCheck);

  for (int move : moves) {
    if (getMoveStart(move) == inputStartSquare &&
        getMoveTarget(move) == inputTargetSquare) {
      return move;
    }
  }
  // Illegal move
  return 1;
}

std::string GameHandler::handleInput(std::string input) {

  std::cout << "FROM SERVER: " << input << std::endl;
  char command = input[0];
  input.erase(0, 1);
  // TASK PROTOCOL
  // l = legalSquares
  // m = make move
  // s = search for move
  // c = check if move is castling
  // f = load fenstring

  if (command == 'c') {
    int move = stringToMove(input);
    if (getMoveCastling(move)) {
      return "true";
    } else {
      return "false";
    }
  }

  if (command == 'l') {
    int sq = BitBoard::coordinateToIndex(input);

    std::string legalTargets = getLegalTargetSquares(sq);
    std::cout << "Legal targetsquares: " << legalTargets << std::endl;
    return legalTargets;
  }
  
  // returns end if game is done after move
  else if (command == 'm') {
    int move = stringToMove(input);
    std::cout << "making move: " << input << std::endl;
    position_->makeMove(move);

    std::vector<int> moves;
    bool inCheck = position_->getWhiteToMove() ? position_->whiteInCheck
                                               : position_->blackInCheck;

    moveGenerator_->generateMoves(moves, position_->getWhiteToMove(),
                                  position_->getCastlingRighs(),
                                  position_->getEnPassantSquare(), inCheck);
    if(moves.size() == 0){
      return "end";
    } else {
      return "done";
    }
  }

  else if (command == 's') {
    std::string uciNotation;
    int engineMove = search_->search(6);
    uciNotation += BitBoard::indexToCoordinate(getMoveStart(engineMove));
    uciNotation += BitBoard::indexToCoordinate(getMoveTarget(engineMove));

    std::cout << "Search result: " << uciNotation << std::endl;
    return uciNotation;
  }

  else if (command == 'f') {
    fenParser_->loadFenPosition(input);
    std::cout << "loaded position:" << std::endl;
    return "fen loaded";
  }

  return "";
}
