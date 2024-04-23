#include "Search.h"
#include "piece_square_tables.h"
#include <cmath>
#include <random>

Search::Search(BitBoard *board, MoveGenerator *generator,
               AttackTables *atkTables, Position *position) {
  this->board = board;
  this->moveGen = generator;
  this->atkTables = atkTables;
  this->position = position;
}

// returns evaluation of board _position
// from perspective of active player
int Search::evaluate() {
  int score = 0;

  for (int piece = nWhitePawn; piece <= nBlackKing; piece++) {
    // skipping board with all black pieces
    if (piece == nBlack) {
      continue;
    }

    // MATERIAL COUNT
    // Adding up all values of all pieces on the board
    U64 pieces = board->getPieceSet(piece);
    int pieceNum = std::popcount(pieces);
    score += pieceNum * pieceValue[piece];

    // PIECE PLACEMENT MAPS
    while (pieces) {
      int square = getLSB(pieces);
      clearBit(pieces, square);

      switch (piece) {
      case nWhitePawn:
        score += whitePawnPlacementScore[square];
        break;
      case nBlackPawn:
        score -= blackPawnPlacementScore[square];
        break;
      case nWhiteKnight:
        score += whiteKnightPlacementScore[square];
        break;
      case nBlackKnight:
        score -= blackKnightPlacementScore[square];
        break;
      case nWhiteBishop:
        score += whiteBishopPlacementScore[square];
        break;
      case nBlackBishop:
        score -= blackBishopPlacementScore[square];
        break;
      case nWhiteRook:
        score += whiteRookPlacementScore[square];
        break;
      case nBlackRook:
        score -= blackRookPlacementScore[square];
        break;
      case nWhiteQueen:
        score += whiteQueenPlacementScore[square];
        break;
      case nBlackQueen:
        score -= blackQueenPlacementScore[square];
        break;
      case nWhiteKing:
        score += whiteKingPlacementScore[square];
        break;
      case nBlackKing:
        score -= blackKingPlacementScore[square];
        break;
      default:
        throw std::invalid_argument(
            "invalid piece in piece placement evaluation");
      }
    }
  }
  // return score from the right perspective
  return position->getWhiteToMove() ? score : -score;
}

int Search::search(int depth) {
  // For move generation
  std::vector<int> moves;
  bool inCheck = position->getWhiteToMove() ? position->whiteInCheck
                                            : position->blackInCheck;
  // generate all starting moves
  moveGen->generateMoves(moves, position->getWhiteToMove(),
                         position->getCastlingRighs(),
                         position->getEnPassantSquare(), inCheck);

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

  std::cout << "My move: "
            << BitBoard::indexToCoordinate(getMoveStart(bestMove)) << "-"
            << BitBoard::indexToCoordinate(getMoveTarget(bestMove))
            << std::endl;
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
    return quiesce(alpha, beta);
    return evaluate();
  }

  std::vector<int> moves;

  bool inCheck = position->getWhiteToMove() ? position->whiteInCheck
                                            : position->blackInCheck;
  moveGen->generateMoves(moves, position->getWhiteToMove(),
                         position->getCastlingRighs(),
                         position->getEnPassantSquare(), inCheck);

  for (int &move : moves) {
    if(position->makeMove(move)){
      int score = -negamax(-beta, -alpha, depth - 1);
      position->undoMove();
      if (score >= beta)
        return beta; //  fail hard beta-cutoff
      if (score > alpha)
        alpha = score; // alpha acts like max in MiniMax
    }
  }
  return alpha;
}

// test functions
void Search::negamaxTest(int depth) {
  std::vector<int> moves;
  bool inCheck = position->getWhiteToMove() ? position->whiteInCheck
                                            : position->blackInCheck;
  // generate all starting moves
  moveGen->generateMoves(moves, position->getWhiteToMove(),
                         position->getCastlingRighs(),
                         position->getEnPassantSquare(), inCheck);

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
