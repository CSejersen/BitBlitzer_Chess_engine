#pragma once
#include "MoveGenerator.h"
#include "Position.h"
#include "Search.h"
#include "FenParser.h"

class GameHandler{
private:
  Search* search_; 
  MoveGenerator* moveGenerator_;
  Position* position_;
  FenParser* fenParser_;


public:
  GameHandler(Search*, MoveGenerator*, Position*, FenParser*);
  std::string getLegalTargetSquares(int startSquare);
  std::string handleInput(std::string input);
  int stringToMove(std::string);
};
