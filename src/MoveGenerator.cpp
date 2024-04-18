//
// Created by Christian Sejersen on 21/11/2023.
//

#include "MoveGenerator.h"
#include "AttackTables.h"
#include "board_constants.h"

MoveGenerator::MoveGenerator(BitBoard *board, AttackTables *attackTables) {
  _board = board;
  _atkTables = attackTables;
}

void MoveGenerator::generateMoves(std::vector<int> &moves, bool whiteToMove,
                                  int castlingRights,
                                  const U64 &enPassantSquare, bool inCheck) {
  // clearing move lists
  pseudoLegal.clear();
  pseudoLegalCapture.clear();

  // generate all pseudo-legal moves (quiet + captures)
  generateMovesForPieceSet(Knight, &AttackTables::getKnightAttacks, whiteToMove);
  generateMovesForPieceSet(Bishop, &AttackTables::getBishopAttacks, whiteToMove);
  generateMovesForPieceSet(Rook, &AttackTables::getRookAttacks, whiteToMove);
  generateMovesForPieceSet(Queen, &AttackTables::getQueenAttacks, whiteToMove);
  generateMovesForPieceSet(King, &AttackTables::getKingAttacks, whiteToMove);
  generatePawnAdvances(whiteToMove);
  generatePawnCaptures(whiteToMove);
  generateEnPassant(whiteToMove, enPassantSquare);
  whiteToMove ? generateCastlesWhite(castlingRights, inCheck)
              : generateCastlesBlack(castlingRights, inCheck);

  // inserting all moves into "moves-vector" passed by reference
  moves.reserve(pseudoLegal.size() + pseudoLegalCapture.size()); // preallocate memory
  moves.insert(moves.end(), pseudoLegalCapture.begin(), pseudoLegalCapture.end());
  moves.insert(moves.end(), pseudoLegal.begin(), pseudoLegal.end());
}

// Only generate captures (for use with quiencient search)
bool MoveGenerator::generateCaptures(std::vector<int> &captures,
                                     bool whiteToMove,
                                     const U64 &enPassantSquare) {

  // clear internal vectors that hold the generated moves
  pseudoLegal.clear();
  pseudoLegalCapture.clear();

  generatePawnCaptures(whiteToMove);
  generateCapturesForPieceSet(Knight, &AttackTables::getKnightAttacks, whiteToMove);
  generateCapturesForPieceSet(Bishop, &AttackTables::getBishopAttacks, whiteToMove);
  generateCapturesForPieceSet(Rook, &AttackTables::getRookAttacks, whiteToMove);
  generateCapturesForPieceSet(Queen, &AttackTables::getQueenAttacks, whiteToMove);
  generateCapturesForPieceSet(King, &AttackTables::getKingAttacks, whiteToMove);
  generateEnPassant(whiteToMove, enPassantSquare);

  if (!pseudoLegalCapture.empty()) {
    captures.reserve(pseudoLegalCapture.size()); // preallocate memory
    captures.insert(captures.end(), pseudoLegalCapture.begin(),
                    pseudoLegalCapture.end());
    return true;
  } else {
    return false;
  }
}

void MoveGenerator::generateMovesForPieceSet(
    int p, U64 (AttackTables::*attackFunc)(int) const, bool whiteToMove) {
  U64 pieces;
  U64 attacks;
  pieces = whiteToMove ? _board->getPieceSet(p) : _board->getPieceSet(p + 7);

  // generating pseudo-legal moves for every piece
  while (pieces) {
    int startSquare = getLSB(pieces);
    clearBit(pieces, startSquare);

    // Generate all possible target squares
    if (whiteToMove)
      attacks =
          (_atkTables->*attackFunc)(startSquare) & ~_board->getPieceSet(nWhite);
    else
      attacks =
          (_atkTables->*attackFunc)(startSquare) & ~_board->getPieceSet(nBlack);

    while (attacks) {
      int targetSquare = getLSB(attacks);
      clearBit(attacks, targetSquare);

      // In case of capture --> append to PseudoLegal capture
      if (isCapture(targetSquare, whiteToMove)) {
        if (whiteToMove) {
          int move = encodeMove(startSquare, targetSquare, p, 0, 1, 0, 0, 0);
          pseudoLegalCapture.push_back(move);
        } else {
          int move =
              encodeMove(startSquare, targetSquare, p + 7, 0, 1, 0, 0, 0);
          pseudoLegalCapture.push_back(move);
        }
        // If not capture --> append move to PseudoLegal capture
      } else {
        if (whiteToMove) {
          int move = encodeMove(startSquare, targetSquare, p, 0, 0, 0, 0, 0);
          pseudoLegal.push_back(move);
        } else {
          int move =
              encodeMove(startSquare, targetSquare, p + 7, 0, 0, 0, 0, 0);
          pseudoLegal.push_back(move);
        }
      }
    }
  }
}

void MoveGenerator::generatePawnAdvances(bool whiteToMove) {
  whiteToMove ? generatePawnAdvancesWhite() : generatePawnAdvancesBlack();
}

void MoveGenerator::generatePawnAdvancesWhite() {
  U64 pawns = _board->getPieceSet(nWhitePawn);
  while (pawns) {
    int startSquare = getLSB(pawns);
    bool blocked = ((1ULL << (startSquare + 8)) & ~_board->getAllPieces()) == 0;
    clearBit(pawns, startSquare);

    // adding single advances
    if (!blocked) {
      int targetSquare = startSquare + 8;
      // checking for promotion
      if ((1ULL << targetSquare) & RANK_8) {
        for (int piece = nWhiteKnight; piece <= nWhiteQueen; piece++) {
          int move = encodeMove(startSquare, targetSquare, nWhitePawn, piece, 0,
                                0, 0, 0);
          pseudoLegal.push_back(move);
        }
      }
      // encode move with no promotion
      else {
        int move =
            encodeMove(startSquare, targetSquare, nWhitePawn, 0, 0, 0, 0, 0);
        pseudoLegal.push_back(move);
      }
    } else
      continue;

    // getting double pawn advances
    if (1ULL << startSquare & RANK_2) {
      blocked = ((1ULL << (startSquare + 16)) & ~_board->getAllPieces()) == 0;
      if (!blocked) {
        int targetSquare = startSquare + 16;
        int move =
            encodeMove(startSquare, targetSquare, nWhitePawn, 0, 0, 1, 0, 0);
        pseudoLegal.push_back(move);
      }
    }
  }
}

void MoveGenerator::generatePawnAdvancesBlack() {
  U64 pawns = _board->getPieceSet(nBlackPawn);
  while (pawns) {
    int startSquare = getLSB(pawns);
    bool blocked = ((1ULL << (startSquare - 8)) & ~_board->getAllPieces()) == 0;
    clearBit(pawns, startSquare);

    // adding single advances
    if (!blocked) {
      int targetSquare = startSquare - 8;
      // checking for promotion
      if ((1ULL << targetSquare) & RANK_1) {
        for (int piece = nBlackKnight; piece <= nBlackQueen; piece++) {
          int move = encodeMove(startSquare, targetSquare, nBlackPawn, piece, 0,
                                0, 0, 0);
          pseudoLegal.push_back(move);
        }
      } else {
        int move =
            encodeMove(startSquare, targetSquare, nBlackPawn, 0, 0, 0, 0, 0);
        pseudoLegal.push_back(move);
      }
    } else
      continue;

    // getting double pawn advances
    if (1ULL << startSquare & RANK_7) {
      blocked = ((1ULL << (startSquare - 16)) & ~_board->getAllPieces()) == 0;
      if (!blocked) {
        int targetSquare = startSquare - 16;
        int move =
            encodeMove(startSquare, targetSquare, nBlackPawn, 0, 0, 1, 0, 0);
        pseudoLegal.push_back(move);
      }
    }
  }
}

void MoveGenerator::generatePawnCaptures(bool whiteToMove) {
  // getting active color pawns
  U64 pawns = whiteToMove ? _board->getPieceSet(nWhitePawn)
                          : _board->getPieceSet(nBlackPawn);
  U64 attacks;
  U64 captures;

  while (pawns) {
    // getting first pawn starting-square and remove it from the board
    int startSquare = getLSB(pawns);
    clearBit(pawns, startSquare);

    // Look-up pawn attacks from starting-square and generate captures
    // for white:
    if (whiteToMove) {
      attacks = (_atkTables->getPawnAttacksWhite(startSquare) &
                 ~_board->getPieceSet(nWhite));
      captures = attacks & _board->getPieceSet(nBlack);
    }
    // for black:
    else {
      attacks = (_atkTables->getPawnAttacksBlack(startSquare) &
                 ~_board->getPieceSet(nBlack));
      captures = attacks & _board->getPieceSet(nWhite);
    }

    // encode and add captures to move-lists
    while (captures) {
      int targetSquare = getLSB(captures);
      clearBit(captures, targetSquare);

      // Check for promotion
      // for white:
      if (whiteToMove) {
        if (1ULL << targetSquare & RANK_8) {
          for (int piece = nWhiteKnight; piece <= nWhiteQueen; piece++) {
            int move = encodeMove(startSquare, targetSquare, nWhitePawn, piece,
                                  1, 0, 0, 0);
            pseudoLegalCapture.push_back(move);
          }
        } else {
          int move =
              encodeMove(startSquare, targetSquare, nWhitePawn, 0, 1, 0, 0, 0);
          pseudoLegalCapture.push_back(move);
        }
      }

      // For black
      else {
        if (1ULL << targetSquare & RANK_1) {
          for (int piece = nBlackKnight; piece <= nBlackQueen; piece++) {
            int move = encodeMove(startSquare, targetSquare, nBlackPawn, piece,
                                  1, 0, 0, 0);
            pseudoLegalCapture.push_back(move);
          }
        } else {
          int move =
              encodeMove(startSquare, targetSquare, nBlackPawn, 0, 1, 0, 0, 0);
          pseudoLegalCapture.push_back(move);
        }
      }
    }
  }
}

void MoveGenerator::generateEnPassant(bool whiteToMove,
                                      const U64 &enPassantSquare) {

  // can be made faster by getting all pawn attacks then anding with en-Passant
  // square and getting opposite color pawn attacks from resulting square. then
  // overlap with white pawns are legal En-Passant moves

  U64 pawns = whiteToMove ? _board->getPieceSet(nWhitePawn)
                          : _board->getPieceSet(nBlackPawn);
  U64 attacks;

  while (pawns) {
    int startSquare = getLSB(pawns);
    clearBit(pawns, startSquare);
    if (whiteToMove) {
      attacks = _atkTables->getPawnAttacksWhite(startSquare) &
                ~_board->getPieceSet(nWhite) & enPassantSquare;
    } else {
      attacks = _atkTables->getPawnAttacksBlack(startSquare) &
                ~_board->getPieceSet(nBlack) & enPassantSquare;
    }

    if (attacks) {
      int targetSquare = getLSB(attacks);
      if (whiteToMove) {
        int move =
            encodeMove(startSquare, targetSquare, nWhitePawn, 0, 0, 0, 1, 0);
        pseudoLegalCapture.push_back(move);
      } else {
        int move =
            encodeMove(startSquare, targetSquare, nBlackPawn, 0, 0, 0, 1, 0);
        pseudoLegalCapture.push_back(move);
      }
    }
  }
}

void MoveGenerator::generateCastlesWhite(int castlingRights, bool inCheck) {

  // Castling illegal if in check
  if (inCheck) {
    return;
  }

  // defining square for later use (MAYBE PUT IN BOARD CCONSTANTS)
  U64 kingSideSquares = 1ULL << G1 | 1ULL << F1;
  U64 queenSideSquares = 1ULL << C1 | 1ULL << D1;
  U64 queenSideBlockSquares = 1ULL << C1 | 1ULL << D1 | 1ULL << B1;
  // get all black attacks
  U64 blackAttack = _atkTables->getAttacksBlack();

  if (decodeCastlingRight(castlingRights, whiteKingSide)) {
    // checking if f1 and g1 are empty
    if ((kingSideSquares & _board->getAllPieces()) == 0) {
      // checking if f1 or g1 is attacked
      if (!(blackAttack & kingSideSquares)) {
        int move = encodeMove(E1, G1, nWhiteKing, 0, 0, 0, 0, 1);
        pseudoLegal.push_back(move);
      }
    }
  }
  if (decodeCastlingRight(castlingRights, whiteQueenSide)) {
    // checking if b1, c1 and d1 are empty
    if ((queenSideBlockSquares & _board->getAllPieces()) == 0) {
      // checking if c1 or d1 is attacked
      if (!(blackAttack & queenSideSquares)) {
        int move = encodeMove(E1, C1, nWhiteKing, 0, 0, 0, 0, 1);
        pseudoLegal.push_back(move);
      }
    }
  }
}

void MoveGenerator::generateCastlesBlack(int castlingRights, bool inCheck) {
  if (inCheck) {
    return;
  }

  U64 kingSideSquares = 1ULL << G8 | 1ULL << F8;
  U64 queenSideSquares = 1ULL << C8 | 1ULL << D8;
  U64 queenSideBlockSquares = 1ULL << C8 | 1ULL << D8 | 1ULL << B8;
  U64 whiteAttack = _atkTables->getAttacksWhite();

  if (decodeCastlingRight(castlingRights, blackKingSide)) {
    // checking if f8 and g8 are empty
    if ((kingSideSquares & _board->getAllPieces()) == 0) {
      // checking if f1 or g1 is attacked
      if (!(whiteAttack & kingSideSquares)) {
        int move = encodeMove(E8, G8, nBlackKing, 0, 0, 0, 0, 1);
        pseudoLegal.push_back(move);
      }
    }
  }
  if (decodeCastlingRight(castlingRights, blackQueenSide)) {
    // checking if b8, c8 and d8 are empty
    if ((queenSideBlockSquares & _board->getAllPieces()) == 0) {
      // checking if c8 or d8 is attacked
      if (!(whiteAttack & queenSideSquares)) {
        int move = encodeMove(E8, C8, nBlackKing, 0, 0, 0, 0, 1);
        pseudoLegal.push_back(move);
      }
    }
  }
}

bool MoveGenerator::isCapture(int targetSquare, bool whiteToMove) const {
  enumPieceBB captureColor = whiteToMove ? nBlack : nWhite;

  if ((1ULL << targetSquare) & _board->getPieceSet(captureColor))
    return true;
  else
    return false;
}

void MoveGenerator::printLegalMoves() {
  std::cout << "Quiet moves: " << std::endl;
  for (int &move : pseudoLegal) {
    std::cout << BitBoard::indexToCoordinate(getMoveStart(move)) << "-"
              << BitBoard::indexToCoordinate(getMoveTarget(move)) << std::endl;
  }

  std::cout << "Captures: " << std::endl;
  for (int &move : pseudoLegalCapture) {
    std::cout << BitBoard::indexToCoordinate(getMoveStart(move)) << "-"
              << BitBoard::indexToCoordinate(getMoveTarget(move)) << std::endl;
  }
}

bool MoveGenerator::decodeCastlingRight(int castlingRights, int enumRight) {
  return (castlingRights & enumRight) != 0;
}

void MoveGenerator::generateEvasionMoves(std::vector<int> &moves,
                                         bool whiteToMove,
                                         const U64 &enPassantSquare) {

  // TO DO! All moves that evade the check
}

void MoveGenerator::generateCapturesForPieceSet(
    int p, U64 (AttackTables::*attackFunc)(int) const, bool whiteToMove) {
  // get pieceSet
  U64 pieces;
  pieces = whiteToMove ? _board->getPieceSet(p) : _board->getPieceSet(p + 7);

  // generating captures for every piece
  U64 captures;
  while (pieces) {
    int startSquare = getLSB(pieces);
    clearBit(pieces, startSquare);

    if (whiteToMove)
      captures =
          (_atkTables->*attackFunc)(startSquare)&_board->getPieceSet(nBlack);
    else
      captures =
          (_atkTables->*attackFunc)(startSquare)&_board->getPieceSet(nWhite);

    while (captures) {
      int targetSquare = getLSB(captures);
      clearBit(captures, targetSquare);

      if (isCapture(targetSquare, whiteToMove)) {
        if (whiteToMove) {
          int move = encodeMove(startSquare, targetSquare, p, 0, 1, 0, 0, 0);
          pseudoLegalCapture.push_back(move);
        } else {
          int move =
              encodeMove(startSquare, targetSquare, p + 7, 0, 1, 0, 0, 0);
          pseudoLegalCapture.push_back(move);
        }
      } else {
        throw std::invalid_argument(
            "generated non-captures in generate captures");
      }
    }
  }
}
