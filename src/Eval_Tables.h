#pragma once

// evaluation function and piece_lookup_tables taken from chessprogramming.org
#define FLIP(sq) ((sq)^56)
#define OTHER(side) ((side)^ 1)

#define PAWN   0
#define KNIGHT 1
#define BISHOP 2
#define ROOK   3
#define QUEEN  4
#define KING   5

/* board representation */
#define WHITE  0
#define BLACK  1

#define WHITE_PAWN      (2*PAWN   + WHITE)
#define BLACK_PAWN      (2*PAWN   + BLACK)
#define WHITE_KNIGHT    (2*KNIGHT + WHITE)
#define BLACK_KNIGHT    (2*KNIGHT + BLACK)
#define WHITE_BISHOP    (2*BISHOP + WHITE)
#define BLACK_BISHOP    (2*BISHOP + BLACK)
#define WHITE_ROOK      (2*ROOK   + WHITE)
#define BLACK_ROOK      (2*ROOK   + BLACK)
#define WHITE_QUEEN     (2*QUEEN  + WHITE)
#define BLACK_QUEEN     (2*QUEEN  + BLACK)
#define WHITE_KING      (2*KING   + WHITE)
#define BLACK_KING      (2*KING   + BLACK)

#define PCOLOR(p) ((p)&1)


class Eval_tables {
private:
  int mg_table[12][64];
  int eg_table[12][64];
  int gamephaseInc[12] = {0,0,1,1,1,1,2,2,4,4,0,0};
  void init_tables();

public:
  Eval_tables();
  int get_mg_table(int, int);
  int get_eg_table(int, int);
  int get_gamephaseInc(int);

  int eval();
  void print_table(int piece);
};

