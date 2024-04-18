#include "bit_manipulation.h"

void setBit(U64 &bb, int i){
    (bb) |= (1ULL << i);
}
U64 getBit(U64 &bb, int i) {
    return (bb) & (1ULL << i);
}
void clearBit(U64 &bb, int i) {
    (bb) &= ~(1ULL << i);
}
int getLSB(const U64& bb) {
    return (__builtin_ctzll(bb));
}
int countBits(U64 bb) {
    // bit count
    int count = 0;
    // pop bits until bitboard is empty
    while (bb) {
        // increment count
        count++;
        // consecutively reset least significant 1st bit
        bb &= bb - 1;

    }
    return count;
}
