#pragma once
#include "AttackTables.h"
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "Position.h"
#include "board_constants.h"
#include "Eval_Tables.h"
#include <bit>
#include <bitset>

const int INF = 2147483647;

class Search {
public:
  Search(BitBoard *board, MoveGenerator *generator, AttackTables *atkTables,
         Position *position);
  BitBoard *board;
  MoveGenerator *moveGen;
  AttackTables *atkTables;
  Position *position;
  Eval_tables evalTables;
  int evaluate();
  int search(int depth);

  void negamaxTest(int depth);

private:
  //  Negamax search with alpha beta pruning and quiescent search
  int quiesce(int alpha, int beta);
  int negamax(int alpha, int beta, int depth);

};
