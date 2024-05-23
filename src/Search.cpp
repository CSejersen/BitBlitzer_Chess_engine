#include "Search.h"
#include "Eval_Tables.h"

Search::Search(BitBoard *board, MoveGenerator *generator,
               AttackTables *atkTables, Position *position) {
  this->board = board;
  this->moveGen = generator;
  this->atkTables = atkTables;
  this->position = position;
  evalTables = Eval_tables();
}

// returns evaluation of board _position
// from perspective of active player
int Search::evaluate() {
  int mg[2];
  int eg[2];
  int gamePhase = 0;

  mg[WHITE] = 0;
  mg[BLACK] = 0;
  eg[WHITE] = 0;
  eg[BLACK] = 0;

  for (int piece = nWhitePawn; piece <= nBlackKing; piece++) {
    // skipping board with all black pieces
    if (piece == nBlack) {
      continue;
    }
    U64 pieces = board->getPieceSet(piece);

    // PIECE PLACEMENT MAPS
    while (pieces) {
      int square = getLSB(pieces);
      clearBit(pieces, square);

      switch (piece) {
      case nWhitePawn:
        mg[WHITE] += evalTables.get_mg_table(WHITE_PAWN, square);
        eg[WHITE] += evalTables.get_eg_table(WHITE_PAWN, square);
        gamePhase += evalTables.get_gamephaseInc(WHITE_PAWN);
        break;
      case nBlackPawn:
        mg[BLACK] += evalTables.get_mg_table(BLACK_PAWN, square);
        eg[BLACK] += evalTables.get_eg_table(BLACK_PAWN, square);
        gamePhase += evalTables.get_gamephaseInc(BLACK_PAWN);
        break;
      case nWhiteKnight:
        mg[WHITE] += evalTables.get_mg_table(WHITE_KNIGHT, square);
        eg[WHITE] += evalTables.get_eg_table(WHITE_KNIGHT, square);
        gamePhase += evalTables.get_gamephaseInc(WHITE_KNIGHT);
        break;
      case nBlackKnight:
        mg[BLACK] += evalTables.get_mg_table(BLACK_KNIGHT, square);
        eg[BLACK] += evalTables.get_eg_table(BLACK_KNIGHT, square);
        gamePhase += evalTables.get_gamephaseInc(BLACK_KNIGHT);
        break;
      case nWhiteBishop:
        mg[WHITE] += evalTables.get_mg_table(WHITE_BISHOP, square);
        eg[WHITE] += evalTables.get_eg_table(WHITE_BISHOP, square);
        gamePhase += evalTables.get_gamephaseInc(WHITE_BISHOP);
        break;
      case nBlackBishop:
        mg[BLACK] += evalTables.get_mg_table(BLACK_BISHOP, square);
        eg[BLACK] += evalTables.get_eg_table(BLACK_BISHOP, square);
        gamePhase += evalTables.get_gamephaseInc(BLACK_BISHOP);
        break;
      case nWhiteRook:
        mg[WHITE] += evalTables.get_mg_table(WHITE_ROOK, square);
        eg[WHITE] += evalTables.get_eg_table(WHITE_ROOK, square);
        gamePhase += evalTables.get_gamephaseInc(WHITE_ROOK);
        break;
      case nBlackRook:
        mg[BLACK] += evalTables.get_mg_table(BLACK_ROOK, square);
        eg[BLACK] += evalTables.get_eg_table(BLACK_ROOK, square);
        gamePhase += evalTables.get_gamephaseInc(BLACK_ROOK);
        break;
      case nWhiteQueen:
        mg[WHITE] += evalTables.get_mg_table(WHITE_QUEEN, square);
        eg[WHITE] += evalTables.get_eg_table(WHITE_QUEEN, square);
        gamePhase += evalTables.get_gamephaseInc(WHITE_QUEEN);
        break;
      case nBlackQueen:
        mg[BLACK] += evalTables.get_mg_table(BLACK_QUEEN, square);
        eg[BLACK] += evalTables.get_eg_table(BLACK_QUEEN, square);
        gamePhase += evalTables.get_gamephaseInc(BLACK_QUEEN);
        break;
      case nWhiteKing:
        mg[WHITE] += evalTables.get_mg_table(WHITE_KING, square);
        eg[WHITE] += evalTables.get_eg_table(WHITE_KING, square);
        gamePhase += evalTables.get_gamephaseInc(WHITE_KING);
        break;
      case nBlackKing:
        mg[BLACK] += evalTables.get_mg_table(BLACK_KING, square);
        eg[BLACK] += evalTables.get_eg_table(BLACK_KING, square);
        gamePhase += evalTables.get_gamephaseInc(BLACK_KING);
        break;
      default:
        throw std::invalid_argument(
            "invalid piece in piece placement evaluation");
      }
    }
  }

  int side2move = position->getWhiteToMove() ? 0 : 1;

  int mgScore = mg[side2move] - mg[OTHER(side2move)];
  int egScore = eg[side2move] - eg[OTHER(side2move)];
  int mgPhase = gamePhase;
  if (mgPhase > 24)
    mgPhase = 24; /* in case of early promotion */
  int egPhase = 24 - mgPhase;
  return (mgScore * mgPhase + egScore * egPhase) / 24;
}

int Search::search(int depth) {
  // For move generation
  std::vector<int> moves;
  bool inCheck = position->getWhiteToMove() ? position->whiteInCheck
                                            : position->blackInCheck;
  // generate all starting moves
  moveGen->generateMoves(moves);

  int bestScore = -INF;
  int bestMove;

  // evaluate all moves and save the best one
  for (int &move : moves) {
    if (position->makeMove(move)) {
      int max = -negamax(-INF, INF, depth - 1);
      if (max > bestScore) {
        bestScore = max;
        bestMove = move;
      }
      position->undoMove();
    }
  }

  std::cout << "Evaluation: " << bestScore << std::endl;
  return bestMove;
}

int Search::quiesce(int alpha, int beta) {
  int stand_pat = evaluate();
  if (stand_pat >= beta)
    return beta;
  if (alpha < stand_pat)
    alpha = stand_pat;

  std::vector<int> captures;
  // generate all starting moves
  moveGen->generateCaptures(captures, position->getWhiteToMove(),
                            position->getCastlingRighs());
  for (int &capture : captures) {
    if (position->makeMove(capture)) {
      int score = -quiesce(-beta, -alpha);
      position->undoMove();

      if (score >= beta)
        return beta;
      if (score > alpha)
        alpha = score;
    }
  }
  return alpha;
}

int Search::negamax(int alpha, int beta, int depth) {

  if (depth == 0) {
    // keep going until no more captures in position
    return quiesce(alpha, beta);
  }

  std::vector<int> moves;
  int legalMoves = 0;
  moveGen->generateMoves(moves);

  for (int &move : moves) {
    if (position->makeMove(move)) {
      legalMoves++;
      int score = -negamax(-beta, -alpha, depth - 1);
      position->undoMove();
      if (score >= beta)
        return beta; //  fail hard beta-cutoff
      if (score > alpha)
        alpha = score; // alpha acts like max in MiniMax
    }
  }

  bool inCheck = position->getWhiteToMove() ? position->whiteInCheck
                                            : position->blackInCheck;
  if (!legalMoves && inCheck) {
    return -16000;
  }
  if (!legalMoves && !inCheck) {
    return 0;
  }

  return alpha;
}

// test functions
void Search::negamaxTest(int depth) {
  std::vector<int> moves;
  // generate all starting moves
  moveGen->generateMoves(moves);

  // evaluate all moves and save the best one
  for (int &move : moves) {
    if (position->makeMove(move)) {
      int eval = -negamax(-INF, INF, depth - 1);
      std::cout << BitBoard::indexToCoordinate(getMoveStart(move)) << "-"
                << BitBoard::indexToCoordinate(getMoveTarget(move)) << " "
                << eval << std::endl;
      position->undoMove();
    }
  }
}
