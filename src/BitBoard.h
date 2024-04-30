#pragma once
#include "bit_manipulation.h"
#include "board_constants.h"
#include "move.h"
#include <iostream>
#include <string>


class BitBoard {
public:
  explicit BitBoard();
  // Bitboards for all piece sets (1 for white pawns, 1 for black pawns and so
  // on...)
  U64 pieceBB[14];
  // Places piece on the given square
  void placePiece(int piece, int square);
  // Remoces piece from a given square
  void removePiece(int piece, int square);
  // returns the bitboard for the requested piece
  U64 getPieceSet(int pieceType) const;
  // return a bitboard with all pieces
  U64 getAllPieces() const;
  // clears entire board
  void clearBoard();
  // Generate zobrist hash of position
  U64 generateHash();

  // Board indexing util functions
  // converts chess-coordinate to square-ndex function, example A1 --> 0 or B1 --> 1
  static int coordinateToIndex(std::string coordinate);
  // conversion function, example 0 --> a1 or 1 --> b1
  static std::string indexToCoordinate(int);

  // Debug
  // prints a nice view of the given bitboard.
  static void printBB(const U64 &bb);

private:
  void printBB_helper(const U64 &bb, char piece);
  U64 pieceKeys[12][64];
  /* unsigned int get_random_U32_number(); */
  /* U64 get_random_U64_number(); */
  /* unsigned int random_state = 1804289383; */
};
