#pragma once
#include "AttackTables.h"
#include "BitBoard.h"
#include "bit_manipulation.h"
#include "board_constants.h"
#include "magics.h"
#include "move.h"
#include <vector>

enum Piece { Pawn = 1, Knight, Bishop, Rook, Queen, King };

class MoveGenerator {

public:
  MoveGenerator(BitBoard *board, AttackTables *attackTables);

  // Generate all pseudo-legal moves
  void generateMoves(std::vector<int> &moves, bool whiteToMove,
                     int castlingRights, const U64 &enPassantSquare,
                     bool inCheck);
  bool generateCaptures(std::vector<int> &captures, bool whiteToMove,
                        const U64 &enPassantSquare);
  // Generate all moves the evade the check
  void generateEvasionMoves(std::vector<int> &moves, bool whiteToMove,
                            const U64 &enPassantSquare);

  // move-lists
  std::vector<int> pseudoLegal;
  std::vector<int> pseudoLegalCapture;

  // Debug function
  void printLegalMoves();

private:
  BitBoard *_board;
  AttackTables *_atkTables;


  // generate pseudo-legal moves
  void generateMovesForPieceSet(int p, U64 (AttackTables::*)(int) const, bool whiteToMove);
  void generateCapturesForPieceSet(int p, U64 (AttackTables::*)(int) const, bool whiteToMove);
  void generatePawnCaptures(bool whiteToMove);
  void generatePawnAdvances(bool whiteToMove);
  void generatePawnAdvancesWhite();
  void generatePawnAdvancesBlack(); 
  void generateCastlesWhite(int castlingRights, bool inCheck);
  void generateCastlesBlack(int castlingRights, bool inCheck);
  void generateEnPassant(bool whiteToMove, const U64 &enPassantSquare);

  // utility
  bool isCapture(int targetSquare, bool whiteToMove) const;
  bool decodeCastlingRight(int castlingRights, int right);
};
