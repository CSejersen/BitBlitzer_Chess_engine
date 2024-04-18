#include "move.h"
int getMoveStart(int move){
    return (move & 0x3f);
}
int getMoveTarget(int move){
    return((move & 0xfc0) >> 6);
}
int getMovePiece(int move){
    return ((move & 0xf000) >> 12);
}
int getMovePromotion(int move){
    return ((move & 0xf0000) >> 16);
}
bool getMoveCapture(int move) {
    return (move & 0x100000) != 0;
}
bool getMoveDoublePush(int move){
    return (move & 0x200000) != 0;
}
bool getMoveEnPassant(int move){
    return (move & 0x400000) != 0;
}
bool getMoveCastling(int move){
    return (move & 0x800000) != 0;
}
int encodeMove(int start, int target, int piece, int promotion, int capture, int doublePush,int enPassant, int castling ){
    return (start) | (target << 6) |(piece << 12) |(promotion << 16) | (capture << 20) |(doublePush << 21) | (enPassant << 22) | (castling << 23);
}

void printMove(int move) {
    std::cout << "start: " << BitBoard::indexToCoordinate(getMoveStart(move)) << std::endl;
    std::cout << "target: " << BitBoard::indexToCoordinate(getMoveTarget(move)) << std::endl;
    std::cout << "Piece: " << getMovePiece(move) << std::endl;
    std::cout << "Promotion: " << getMovePromotion(move) << std::endl;
    std::cout << "Capture: " << getMoveCapture(move) << std::endl;
    std::cout << "Double push: " << getMoveDoublePush(move) << std::endl;
    std::cout << "En Passant: " << getMoveEnPassant(move) << std::endl;
    std::cout << "Castling: " << getMoveCastling(move) << std::endl;
}
