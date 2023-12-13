#include "Search.h"

Search::Search(BitBoard* board, MoveGenerator *generator, AttackTables *atkTables,
               Position *position) {
    this->board = board;
    this->moveGen = generator;
    this->atkTables = atkTables;
    this->position = position;
}

// returns evaluation of current position
// from perspective of active player
int Search::evaluate() {
    int score = 0;

    for(int piece = nWhitePawn; piece <= nBlackKing; piece++){
        if(piece == nBlack){
            continue;
        }

        // adding up all values of all pieces on the board
        U64 pieces = board->getPieceSet(piece);
        int pieceNum = std::popcount(pieces);
        score += pieceNum * pieceValue[piece];
        }

    if(position->getWhiteToMove()){
        return score;
    }
    else{
        return -score;
    }
}

int Search::negaMax(int depth) {
    // don't go deeper if depth is reached
    if(depth == 0){
        int eval = evaluate();
        return eval;
    }
    int max = -1000000000;

    // Move generation
    std::vector<int> moves;
    bool inCheck = position->getWhiteToMove() ? position->whiteInCheck : position->blackInCheck;
    moveGen->generateMoves(moves,position->getWhiteToMove(), position->getCastlingRighs(), position->getEnPassantSquare(),inCheck);

    for (int &move : moves) {
        if(position->makeMove(move)){
            int score = -negaMax(depth - 1);
            if(score > max){
                max = score;
            }
            position->undoMove();
        };
    }
    return max;
}

int Search::search(int depth){
    std::vector<int> moves;
    bool inCheck = position->getWhiteToMove() ? position->whiteInCheck : position->blackInCheck;
    moveGen->generateMoves(moves, position->getWhiteToMove(), position->getCastlingRighs(),position->getEnPassantSquare(),inCheck);
    int bestScore = -10000000;
    int bestMove;
    for(int& move : moves){
        if(position->makeMove(move)){
            int max = -negaMax(depth - 1);
            if(max > bestScore){
                bestScore = max;
                bestMove = move;
            }
            position->undoMove();
        }
    }
    std::cout << "My move: " << BitBoard::indexToCoordinate(getMoveStart(bestMove)) << "-" << BitBoard::indexToCoordinate(
            getMoveTarget(bestMove)) << std::endl;
    return bestMove;
}
