//
// Created by Christian Sejersen on 21/11/2023.
//
// Bit manipulation functions
void BitBoard::setBit(U64 &bb, int i){
    (bb) |= (1ULL << i);
}
U64 BitBoard::getBit(U64 &bb, int i) {
    return (bb) & (1ULL << i);
}
void BitBoard::clearBit(U64 &bb, int i) {
    (bb) &= ~(1ULL << i);
}
int BitBoard::getLSB(const U64& bb) {
    return (__builtin_ctzll(bb));
}
int BitBoard::countBits(U64 bb) {
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
