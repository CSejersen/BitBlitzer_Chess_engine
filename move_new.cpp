#include "move_new.h"
uint8_t getMoveStart(int move){
    return (move & 0x3f);
}
uint8_t getMoveTarget(int move){
    return((move & 0xfc0) >> 6);
}
uint8_t getMovePiece(int move){
    return ((move & 0xf000) >> 12);
}
uint8_t getMovePromotion(int move){
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
