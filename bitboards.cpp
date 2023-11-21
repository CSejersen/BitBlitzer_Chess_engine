#include "bitboards.h"
#include "magics.h"

// constants for using ranks and files with bitboards
const U64 FILE_A = 0x8080808080808080ULL;
const U64 FILE_B = 0x4040404040404040ULL;
const U64 FILE_C = 0x2020202020202020ULL;
const U64 FILE_D = 0x1010101010101010ULL;
const U64 FILE_E = 0x0808080808080808ULL;
const U64 FILE_F = 0x0404040404040404ULL;
const U64 FILE_G = 0x0202020202020202ULL;
const U64 FILE_H = 0x0101010101010101ULL;
const U64 FILE_AB = FILE_A | FILE_B;
const U64 FILE_GH = FILE_G | FILE_H;

const U64 RANK_1 = 0xffULL;
const U64 RANK_2 = 0xff00ULL;
const U64 RANK_3 = 0xff0000ULL;
const U64 RANK_4 = 0xff000000ULL;
const U64 RANK_5 = 0xff00000000ULL;
const U64 RANK_6 = 0xff0000000000ULL;
const U64 RANK_7 = 0xff000000000000ULL;
const U64 RANK_8 = 0xff00000000000000ULL;

// rook relevant blocker bits
int rookRelevantBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

// bishop relevant blocker bits
int bishopRelevantBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

// Bitboards constructor
BitBoard::BitBoard(){

    // generate attack masks for magic bitboard generation
    // results used by loadAttackTables().
    generateBishopAttackMasks();
    generateRookAttackMasks();

    // Calculate all attack-tables
    loadAttackTables();

    // init starting position + game-state variables
    loadStartingPosition();
}

// returns the bitboard for the requested piece
U64 BitBoard::getPieceSet(enumPieceBB pieceType) const {
    return pieceBB[pieceType];
}

// places piece on a given square of passed Bitboard
void BitBoard::placePiece(int pieceType, int square){
    U64 placementMask = 1ULL << square;
    pieceBB[pieceType] |= placementMask;
    if (pieceType < 7) {
        pieceBB[nWhite] |= placementMask;
    }
    else{
        pieceBB[nBlack] |= placementMask;
    }
}

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
// prints a nice view of bitboard for a given piece.
void BitBoard::printBB(const U64& bb) {
    U64 shiftMe = 1ULL;
    for(int file = 7; file >= 0; file--){
        for(int i = 0; i < 8; i++){
            if((shiftMe << ((file * 8) + i)) & bb){
                std::cout << "x ";
            }
            else{
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void BitBoard::generateBishopAttackMasks() {
    for (int sq = A1; sq <= H8; sq++) {
        // making sure we start at 0
        this->bishopAttackMask[sq] = 0ULL;

        int file = sq % 8; // file ranging from 0-7
        int rank = sq / 8; // rank ranging from 0-7

        for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++) {
            this->bishopAttackMask[sq] |= (1ULL << ((r * 8) + f));
        }
        for (int r = rank + 1, f = file - 1; r <= 6 && f > 0; r++, f--) {
            this->bishopAttackMask[sq] |= 1ULL << ((r * 8) + f);
        }
        for (int r = rank - 1, f = file + 1; r > 0 && f <= 6; r--, f++) {
            this->bishopAttackMask[sq] |= 1ULL << ((r * 8) + f);
        }
        for (int r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--) {
            this->bishopAttackMask[sq] |= 1ULL << ((r * 8) + f);
       }
    }
}

U64 BitBoard::bishopAttacksOnTheFly(int sq, U64 blockers) {
    U64 attacks = 0ULL;

    int file = sq % 8; // file ranging from 0-7
    int rank = sq / 8; // rank ranging from 0-7

    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << ((r * 8) + f));
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= 1ULL << ((r * 8) + f);
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= 1ULL << ((r * 8) + f);
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= 1ULL << ((r * 8) + f);
        if (blockers & (1ULL << ((r * 8) + f))) break;
    }
    return attacks;
}

void BitBoard::generateRookAttackMasks() {
    for (int sq = A1; sq <= H8; sq++) {
        // making sure we start at 0
        this->rookAttackMask[sq] = 0ULL;

        int file = sq % 8; // file ranging from 0-7
        int rank = sq / 8; // rank ranging from 0-7

        //generating attack mask
        for (int r = rank + 1; r <= 6; r++) {
            this->rookAttackMask[sq] |= (1ULL << ((r * 8) + file));
        }
        for (int f = file - 1; f > 0; f--) {
            this->rookAttackMask[sq] |= 1ULL << ((rank * 8) + f);
        }
        for (int f = file + 1; f <= 6; f++) {
            this->rookAttackMask[sq] |= 1ULL << ((rank * 8) + f);
        }
        for (int r = rank - 1; r > 0; r--) {
            this->rookAttackMask[sq] |= 1ULL << ((r * 8) + file);
        }
    }
}

U64  BitBoard::rookAttacksOnTheFly(int sq, U64 blockers) {
    U64  attacks = 0ULL;

    int file = sq % 8; // file ranging from 0-7
    int rank = sq / 8; // rank ranging from 0-7

    //generating attack mask
    // North movement
    for (int r = rank + 1; r <= 7; r++) {
        attacks |= (1ULL << ((r * 8) + file));
        if((blockers & (1ULL << ((r*8) + file)))) break;
    }
    // East movement
    for (int f = file - 1; f >= 0; f--) {
        attacks |= 1ULL << ((rank * 8) + f);
        if((blockers & (1ULL << ((rank*8) + f)))) break;
    }
    // West movement
    for (int f = file + 1; f <= 7; f++) {
        attacks |= 1ULL << ((rank * 8) + f);
        if((blockers & (1ULL << ((rank *8 ) + f)))) break;
    }
    // South movement
    for (int r = rank - 1; r >= 0; r--) {
        attacks |= 1ULL << ((r * 8) + file);
        if((blockers & (1ULL << ((r*8) + file)))) break;
    }

    return attacks;
}

U64 BitBoard::generateBlockers(int patternIndex, int bitsInMask, U64 mask) const{
    U64 blockers = 0ULL;
    U64 attackMask = mask;
    for (int i = 0; i < bitsInMask; i++) {
        U64 square = getLSB(attackMask);
        clearBit(attackMask, square);
        if (patternIndex & (1 << i)) {
            blockers |= 1ULL << square;
        }
    }
    return blockers;
}

void BitBoard::loadAttackTables() {
    // Leaping pieces (king, nights, pawns) are implemented using simple 1D-array look-up tables.
    // Sliding pieces will require another solution as they can be blocked.

    // Generating lookup table for knights
    U64 knight = 0ULL;
    for (int i = 0; i <= 63; i++) {
        setBit(knight, i);
        knightAttacks[i] =  (((knight >> 6) | (knight << 10)) & ~FILE_GH) |
                            (((knight >> 10) | (knight << 6)) & ~FILE_AB) |
                            (((knight >> 15) | (knight << 17)) & ~FILE_H) |
                            (((knight >> 17) | (knight << 15)) & ~FILE_A);
        clearBit(knight, i);
    }
    // Kings
    U64 king = 0LL;
    for (int i = 0; i <= 63; i++) {
        setBit(king, i);
        this->kingAttacks[i] = (((king << 9) | (king << 1) | (king >> 7)) & ~FILE_H) |
                               ((king << 8) | (king >> 8)) |
                               (((king << 7) | (king >> 1) | (king >> 9)) & ~FILE_A);
        clearBit(king, i);
    }
    // Pawns
    U64 pawn = 0ULL;
    for(int color = white; color <= black; color++){
        for(int i = A1; i <= H8; i++){
            if(color == white){
                setBit(pawn,i);
                this->pawnAttacks[i][color] = ((pawn << 7 ) & ~FILE_A) | ((pawn << 9) & ~FILE_H);
                clearBit(pawn,i);
            }
            else{
                setBit(pawn,i);
                this->pawnAttacks[i][color] = ((pawn >> 7 ) & ~FILE_H) | ((pawn >> 9) & ~FILE_A);
                clearBit(pawn,i);

            }
        }
    }
    // implement sliding pieces (bishops, rooks, queens) with 2D-array lookup tables,
    // every square can be looked up for all possible blocker patterns.

    // Rooks
    // looping over all 64 squares
    for(int i = A1; i <= H8; i++){
        //getting relevant attack mask
        U64 mask = rookAttackMask[i];
        // counting number of attacked squares
        int bitCount = countBits(mask);
        // calculating number of possible blocker patterns
        int blockerVariations = 1 << bitCount;

        // looping over all possible blocker variations. think of index as a binary mask.
        for (int index = 0; index < blockerVariations; index++){

            U64 blockers = generateBlockers(index, bitCount,mask);
            U64 magicIndex = (blockers * rookMagics[i]) >> (64 - rookRelevantBits[i]);
            rookAttacks[i][magicIndex] = rookAttacksOnTheFly(i,blockers);
        }
    }
    // Bishops

    for(int i = A1; i <=H8; i++){
        U64 mask = bishopAttackMask[i];
        int bitCount = countBits(mask);
        int blockerVariations = 1 << bitCount;

        for (int index = 0; index < blockerVariations; index++){
            U64 blockers = generateBlockers(index,bitCount,mask);
            U64 magicIndex = (blockers * bishopMagics[i]) >> (64 - bishopRelevantBits[i]);
            bishopAttacks[i][magicIndex] = bishopAttacksOnTheFly(i,blockers);
        }
    }
}

void BitBoard::clearBoard(){
    for(U64& pieceSet : pieceBB){
        pieceSet = 0ULL;
    }
}

void BitBoard::loadStartingPosition() {
    loadFenPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void BitBoard::loadFenPosition(const std::string& fenString) {
    clearBoard();
    std::string fen = fenString;
    int index = 0;
    index = fenParsePieces(fen);
    index = fenParseTurn(index, fen);
    index = fenParseCastleingRights(index, fen);
    index = fenParseEnPassant(index, fen);
    index = fenParseHalfmove(index, fen);
    fenParseMoveNum(index, fen);

}

int BitBoard::fenParsePieces(std::string& fen){
    int currentSquare = A8;
    int file = 0;
    int rank = 7;
    int index = 0;
    bool quit = false;

    for (int i = 0; i < fen.size() && !quit; ++i) {
        currentSquare = (rank * 8) + file;
        if (isdigit(fen[i])) {
            int emptySquares = fen[i] - '0';  // Character to integer
//            std::cout << "Adding " << emptySquares << " empty squares" << std::endl;
            file += emptySquares; // Skipping empty squares
//            std::cout << "file now equal to: " << file << std::endl;
        } else {
            switch (fen[i]) {
                case 'P':
//                    std::cout << "placing white pawn on: " << currentSquare << std::endl;
                    placePiece(nWhitePawn,currentSquare);
                    placePiece(nWhite, currentSquare);
                    break;
                case 'R':
//                    std::cout << "placing white rook on: " << currentSquare << std::endl;
                    placePiece(nWhiteRook,currentSquare);
                    placePiece(nWhite, currentSquare);
                    break;
                case 'N':
//                    std::cout << "placing white Knight on: " << currentSquare << std::endl;
                    placePiece(nWhiteKnight,currentSquare);
                    placePiece(nWhite, currentSquare);
                    break;
                case 'B':
//                    std::cout << "placing white Bishop on: " << currentSquare << std::endl;
                    placePiece(nWhiteBishop,currentSquare);
                    placePiece(nWhite, currentSquare);
                    break;
                case 'Q':
//                    std::cout << "placing white Queen on: " << currentSquare << std::endl;
                    placePiece(nWhiteQueen,currentSquare);
                    placePiece(nWhite, currentSquare);
                    break;
                case 'K':
//                    std::cout << "placing white King on: " << currentSquare << std::endl;
                    placePiece(nWhiteKing,currentSquare);
                    placePiece(nWhite, currentSquare);
                    break;

                case 'p':
//                    std::cout << "placing black pawn on: " << currentSquare << std::endl;
                    placePiece(nBlackPawn,currentSquare);
                    placePiece(nBlack, currentSquare);
                    break;
                case 'r':
//                    std::cout << "placing black Rook on: " << currentSquare << std::endl;
                    placePiece(nBlackRook,currentSquare);
                    placePiece(nBlack, currentSquare);
                    break;
                case 'n':
//                    std::cout << "placing black Knight on: " << currentSquare << std::endl;
                    placePiece(nBlackKnight,currentSquare);
                    placePiece(nBlack, currentSquare);
                    break;
                case 'b':
//                    std::cout << "placing black Bishop on: " << currentSquare << std::endl;
                    placePiece(nBlackBishop,currentSquare);
                    placePiece(nBlack, currentSquare);
                    break;
                case 'q':
//                    std::cout << "placing black Queen on: " << currentSquare << std::endl;
                    placePiece(nBlackQueen,currentSquare);
                    placePiece(nBlack, currentSquare);
                    break;
                case 'k':
//                    std::cout << "placing black King on: " << currentSquare << std::endl;
                    placePiece(nBlackKing,currentSquare);
                    placePiece(nBlack, currentSquare);
                    break;

                case '/':
                    if(!file % 7 == 0){
//                        std::cout << "true" << std::endl;
                        rank --;
                        file = -1;
//                        std::cout << "rank now equal to: " << rank << std::endl;
                        break;
                    }
                    else{
                        throw std::invalid_argument("invalid fen string");
                    }
                case ' ':
                    quit = true;

                default:
                    break;
            }
            file += 1;
        }
        index += 1;
    }
    return index;
}

int BitBoard::fenParseTurn(int index, std::string& fen){
    if(fen[index] == 'w'){
        whiteToMove = true;
    }
    else{
        whiteToMove = false;
    }
    return index += 2;
}

int BitBoard::fenParseCastleingRights(int index, std::string& fen) {
    int indexTracker = index;
    bool quit = false;
    // reset castleing rights
    for(bool & castleingRight : castleingRights) {
        castleingRight = false;
    }

    for(int i = index; i < fen.size() && !quit; i++) {
        switch (fen[i]) {
            case 'K':
                castleingRights[whiteKingside] = true;
                break;

            case 'Q':
                castleingRights[whiteQueenSide] = true;
                break;

            case 'k':
                castleingRights[blackKingside] = true;
                break;

            case 'q':
                castleingRights[blackQueenside] = true;
                break;

            case '-':
                break;

            case ' ':
                quit = true;
                break;
        }
        indexTracker += 1;
    }
    return indexTracker;
}

int BitBoard::fenParseEnPassant(int index, std::string& fen) {
    int rank = 0;
    int file = 0;
    bool quit = false;
    bool enPassant = true;
    int indexTracker = index;

    for(int i = index; i <= fen.size() && !quit; i++){
        switch (fen[i]) {
            case 'a':
                file = 0;
                break;
            case 'b':
                file = 1;
                break;
            case 'c':
                file = 2;
                break;
            case 'd':
                file = 3;
                break;
            case 'e':
                file = 4;
                break;
            case 'f':
                file = 5;
                break;
            case 'g':
                file = 6;
                break;
            case 'h':
                file = 7;
                break;
            case '1':
                rank = 0;
                break;
            case '2':
                rank = 1;
                break;
            case '3':
                rank = 2;
                break;
            case '4':
                rank = 3;
                break;
            case '5':
                rank = 4;
                break;
            case '6':
                rank = 5;
                break;
            case '7':
                rank = 6;
                break;
            case '8':
                rank = 7;
                break;
            case '-':
                enPassant = false;
                break;
            case ' ':
                quit = true;
                break;

            default:
                throw std::invalid_argument("invalid FEN string En-passant error");
        }
        indexTracker += 1;

    }
    if(enPassant){
        int square = (rank*8) + file;
        enPassantSquares |= 1ULL << square;
    }
    return indexTracker;
}

int BitBoard::fenParseHalfmove(int index, std::string &fen) {
    std::string halfmoves;
    int indexTracker = index;
    bool quit = false;

    for(int i = index; i < fen.size() && !quit; i++){
        if(isdigit(fen[i])){
            halfmoves += fen[i];
        }
        else{
            quit = true;
        }
        indexTracker ++;
    }
    halfmoveClock = stoi(halfmoves);
    return indexTracker;
}

void BitBoard::fenParseMoveNum(int index, std::string &fen) {
    std::string moves;
    bool quit = false;

    for(int i = index; i < fen.size() && !quit; i++){
        if(isdigit(fen[i])){
            moves += fen[i];
        }
        else{
            quit = true;
        }
    }
    moveNum = stoi(moves);
}

U64 BitBoard::getAttacks(bool whitePieces) const {
    U64 attacks = 0ULL;
    U64 blockers = (pieceBB[nWhite] | pieceBB[nBlack]);
    if(whitePieces){
        U64 pawns = getPieceSet(nWhitePawn);
        U64 knights = getPieceSet(nWhiteKnight);
        U64 bishops = getPieceSet(nWhiteBishop);
        U64 rooks = getPieceSet(nWhiteRook);
        U64 queens = getPieceSet(nWhiteQueen);
        U64 king = getPieceSet(nWhiteKing);
        while(pawns){
            int square = getLSB(pawns);
            attacks |= pawnAttacks[square][white];
            clearBit(pawns,square);
        }
        while(knights){
            int square = getLSB( knights);
            attacks |= knightAttacks[square];
            clearBit(knights,square);
        }
        while(bishops){
            int square = getLSB( bishops);
            attacks |= getBishopAttacks(square);
            clearBit(bishops,square);
        }
        while(rooks){
            int square = getLSB(rooks);
            attacks |= getRookAttacks(square);
            clearBit(rooks,square);
        }
        while(queens){
            int square = getLSB(queens);
            attacks |= (getRookAttacks(square) | getBishopAttacks(square));
            clearBit(queens,square);
        }
        while(king){
            int square = getLSB(king);
            attacks |= kingAttacks[square];
            clearBit(king,square);
        }
        }
    else {
        U64 pawns = getPieceSet(nBlackPawn);
        U64 knights = getPieceSet(nBlackKnight);
        U64 bishops = getPieceSet(nBlackBishop);
        U64 rooks = getPieceSet(nBlackRook);
        U64 queens = getPieceSet(nBlackQueen);
        U64 king = getPieceSet(nBlackKing);
        while (pawns) {
            int square = getLSB(pawns);
            attacks |= pawnAttacks[square][black];
            clearBit(pawns, square);
        }
        while (knights) {
            int square = getLSB(knights);
            attacks |= knightAttacks[square];
            clearBit(knights, square);
        }
        while (bishops) {
            int square = getLSB(bishops);
            attacks |= getBishopAttacks(square);
            clearBit(bishops, square);
        }
        while (rooks) {
            int square = getLSB(rooks);
            attacks |= getRookAttacks(square);
            clearBit(rooks, square);
        }
        while (queens) {
            int square = getLSB(queens);
            attacks |= (getRookAttacks(square) | getBishopAttacks(square));
            clearBit(queens, square);
        }
        while (king) {
            int square = getLSB(king);
            attacks |= kingAttacks[square];
            clearBit(king, square);
        }
    }
    if (whiteToMove){
        return (attacks & ~getPieceSet(nWhite));
    }
    else{
        return (attacks & ~getPieceSet(nBlack));
    }
}

U64 BitBoard::getRookAttacks(int square) const {
    U64 blockers = getPieceSet(nWhite) | getPieceSet(nBlack);
        blockers &= rookAttackMask[square];
    U64 magicIndex = (blockers * rookMagics[square]) >> (64 - rookRelevantBits[square]);

    if(whiteToMove){
        return rookAttacks[square][magicIndex] & ~getPieceSet(nWhite);
    }
    else{
        return rookAttacks[square][magicIndex] & ~getPieceSet(nBlack);
    }
}

U64 BitBoard::getBishopAttacks(int square) const {
    U64 blockers = getPieceSet(nWhite) | getPieceSet(nBlack);
    blockers &= bishopAttackMask[square];
    U64 magicIndex = (blockers * bishopMagics[square]) >> (64 - bishopRelevantBits[square]);

    if(whiteToMove){
        return bishopAttacks[square][magicIndex] & ~getPieceSet(nWhite);
    }
    else{
        return bishopAttacks[square][magicIndex] & ~getPieceSet(nBlack);
    }
}

U64 BitBoard::getKnightAttacks(int square) const{
    if(whiteToMove){
        return knightAttacks[square] & ~getPieceSet(nWhite);
    }
    else{
        return knightAttacks[square] & ~getPieceSet(nBlack);
    }
}

U64 BitBoard::getPawnAttacks(int square) const{
    if(whiteToMove){
        return pawnAttacks[square][white];
    }
    else{
        return pawnAttacks[square][black];
    }
}

U64 BitBoard::getKingAttacks(int square) const{
    if(whiteToMove){
        return kingAttacks[square] & ~getPieceSet(nWhite);
    }
    else{
        return kingAttacks[square] & ~getPieceSet(nBlack);
    }
}

void BitBoard::makeMove(const Move& move) {
    int startingSquare = move.getStartSquare();
    int targetSquare = move.getTargetSquare();
    int flag = move.getFlag();
    int pieceToMove = 0;

    // Looping over all pieceTypes to find pieceToMove.
    for(int pieceType = nWhitePawn; pieceType <= nBlackKing; pieceType++){
        // skipping board with all black pieces (all white board already skipped by starting loop at nWhitePawn);
        if(pieceType == nBlack){
            continue;
        }
        // Finding piece on starting square
        if(pieceBB[pieceType] & 1ULL << startingSquare){
            pieceToMove = pieceType;
        }

    }
    // checking if starting square is valid
    if(whiteToMove && (pieceToMove > 7)){
        throw std::invalid_argument("Trying to move black piece on whites turn!");
    }
    if(!whiteToMove && (pieceToMove < 7)){
        throw std::invalid_argument("Trying to move white piece on blacks turn!");
    }
    if(!pieceToMove){
        throw std::invalid_argument("No piece on starting square!");
    }

    // making move on the board
    pieceBB[pieceToMove] &= ~(1ULL << startingSquare); // removing piece from starting square
    placePiece(pieceToMove,targetSquare);

    if(whiteToMove){
        pieceBB[nWhite] &= ~(1ULL << startingSquare);
    }
    else{
        pieceBB[nBlack] &= ~(1ULL << startingSquare);
    }

    if(flag == nDoublePawnPush){
        std::cout << "Double pawn push! " << std::endl;
        std::cout << indexToCoordinate(startingSquare) << " - " << indexToCoordinate(targetSquare) << std::endl;
    }
    if(flag == nQuietMoves){
        std::cout << "Quiet Move " << std::endl;
        std::cout << indexToCoordinate(startingSquare) << " - " << indexToCoordinate(targetSquare) << std::endl;
    }

    whiteToMove = !whiteToMove;
    gameHistory.emplace_back(move);
}

int BitBoard::coordinateToIndex(std::string coordinate) {

    int file = 0;
    int rank = 0;
    int squareIndex = 0;

    if(coordinate.size() != 2){
        throw std::invalid_argument("invalid coordinate size");
    }

    else{
        for(int i = 0; i < 2; i++) {
            switch (coordinate[i]) {
                case 'a':
                    file = 0;
                    break;
                case 'b':
                    file = 1;
                    break;
                case 'c':
                    file = 2;
                    break;
                case 'd':
                    file = 3;
                    break;
                case 'e':
                    file = 4;
                    break;
                case 'f':
                    file = 5;
                    break;
                case 'g':
                    file = 6;
                    break;
                case 'h':
                    file = 7;
                    break;
                case '1':
                    rank = 0;
                    break;
                case '2':
                    rank = 1;
                    break;
                case '3':
                    rank = 2;
                    break;
                case '4':
                    rank = 3;
                    break;
                case '5':
                    rank = 4;
                    break;
                case '6':
                    rank = 5;
                    break;
                case '7':
                    rank = 6;
                    break;
                case '8':
                    rank = 7;
                    break;
                default:
                    throw std::invalid_argument("invalid coordinate");

            }
        }
        squareIndex = (rank * 8) + file;

    }
    return squareIndex;
}

void BitBoard::undoMove() {
    auto it = gameHistory.end();
    it --;
    int startSquare = it->getStartSquare();
    int targetSquare = it->getTargetSquare();
    int pieceToMove = 0;

    for(int pieceType = nWhitePawn; pieceType <= nBlackKing; pieceType++){
        if(pieceType == nBlack){
            continue;
        }
        if(pieceBB[pieceType] & 1ULL << targetSquare){
            pieceToMove = pieceType;
        }
        if(pieceToMove){
            break;
        }
    }

    // Undoing move on the board
    pieceBB[pieceToMove] &= (~1ULL << targetSquare);
    placePiece(pieceToMove,startSquare);

    if(whiteToMove){
        pieceBB[nBlack] &= ~(1ULL << targetSquare);
    }
    else{
        pieceBB[nWhite] &= ~(1ULL << targetSquare);
    }
    // passing turn back
    whiteToMove = !whiteToMove;
    it = gameHistory.erase(it);
};

void BitBoard::generateKnightMoves() {
    U64 knights = 0ULL;
    if(whiteToMove)
        knights = getPieceSet(nWhiteKnight);
    else
        knights = getPieceSet(nBlackKnight);

    while(knights){
        U16 startSquare = getLSB(knights);
        clearBit(knights,startSquare);
        U64 attacks = getKnightAttacks(startSquare);
        while(attacks){
            U16 targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            U16 flags = 0;

                if(isCapture(targetSquare)) {
                    flags = nCapture;
                    captures.emplace_back(startSquare,targetSquare,flags);
                }
                else
                    pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }
        }
    }
void BitBoard::generateBishopMoves() {
    U64 bishops = 0ULL;
    if(whiteToMove)
        bishops = getPieceSet(nWhiteBishop);
    else
        bishops = getPieceSet(nBlackBishop);

    while(bishops){
        int startSquare = getLSB(bishops);
        clearBit(bishops,startSquare);
        U64 attacks = getBishopAttacks(startSquare);
        U16 flags = 0;
        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                captures.emplace_back(startSquare,targetSquare,flags);

            }
            else{
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }


        }

    }
}

void BitBoard::generateRookMoves() {
    U64 rooks = 0ULL;
    if(whiteToMove)
        rooks = getPieceSet(nWhiteRook);
    else
        rooks = getPieceSet(nBlackRook);

    while(rooks){
        int startSquare = getLSB(rooks);
        clearBit(rooks, startSquare);
        U64 attacks = getRookAttacks(startSquare);
        U16 flags = 0;
        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                captures.emplace_back(startSquare,targetSquare,flags);

            }
            else{
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }


        }

    }

}

void BitBoard::generateKingMoves() {
    U64 king = 0ULL;
    if (whiteToMove)
        king = getPieceSet(nWhiteKing);
    else
        king = getPieceSet(nBlackKing);
    U16 startSquare = getLSB(king);
    U64 attacks = getKingAttacks(startSquare);

    // adding castling squares
    U64 whiteKingSideSquares = 1ULL << G1 | 1ULL << F1;
    U64 whiteQueenSideSquares = 1ULL << C1 | 1ULL << D1;
    U64 blackKingSideSquares = 1ULL << G8 | 1ULL << F8;
    U64 blackQueenSideSquares = 1ULL << C8 | 1ULL << D8;

    U64 allPieces = getPieceSet(nWhite) & getPieceSet(nBlack);

    if (whiteToMove && startSquare == E1 && whiteKingSideSquares & ~allPieces && castleingRights[whiteKingside]) // DOES NOT WORK! can prob castle with piece on F1 and empty square G1
        attacks |=  (1ULL << G1) & ~allPieces;

    else if (whiteToMove && startSquare == E1 && whiteQueenSideSquares & ~allPieces && castleingRights[whiteQueenSide])
        attacks |=  (1ULL << C1) & ~allPieces;

    else if (!whiteToMove && startSquare == E8 && blackKingSideSquares & ~allPieces && castleingRights[blackKingside])
        attacks += (1ULL << G8) & ~allPieces;

    else if (!whiteToMove && startSquare == E8 && blackQueenSideSquares & ~allPieces && castleingRights[blackQueenside])
    attacks += (1ULL << C8) & ~getPieceSet(nBlack) | (1ULL << C8) & ~getPieceSet(nBlack);


    U16 flags = 0;
    while (attacks) {
        int targetSquare = getLSB(attacks);
        clearBit(attacks, targetSquare);

        if (startSquare == E1 && targetSquare == G1) {
            if (1ULL << F1 & ~allPieces)
                flags = nKingCastle;
        }
        else if (startSquare == E1 && targetSquare == C1){
            flags = nQueenCastle;
        }
        else if (startSquare == E8 && targetSquare == G8){
            flags = nKingCastle;
        }
        else if (startSquare == E8 && targetSquare == C8){
            flags = nQueenCastle;
        }


        if (isCapture(targetSquare)) {
            flags = nCapture;
            captures.emplace_back(startSquare, targetSquare, flags);
        }
        else {
            pseudoLegal.emplace_back(startSquare, targetSquare, flags);

        }
    }
}


void BitBoard::generateQueenMoves() {
    U64 queens = 0ULL;
    if(whiteToMove)
        queens = getPieceSet(nWhiteQueen);
    else
        queens = getPieceSet(nBlackQueen);

    while(queens){
        int startSquare = getLSB(queens);
        clearBit(queens, startSquare);
        U64 straightAttacks = getRookAttacks(startSquare);
        U64 diagonalAttacks = getBishopAttacks(startSquare);
        U64 attacks = straightAttacks | diagonalAttacks;
        U16 flags = 0;
        while(attacks){
            int targetSquare = getLSB(attacks);
            clearBit(attacks,targetSquare);
            if(isCapture(targetSquare)){
                flags = nCapture;
                captures.emplace_back(startSquare,targetSquare,flags);

            }
            else{
                pseudoLegal.emplace_back(startSquare,targetSquare,flags);
            }


        }

    }

}


void BitBoard::removeCastlingRight(castleingSide side) {

    if(whiteToMove)
        castleingRights[side] = false;
    else
        castleingRights[side + 2] = false;



}

std::string BitBoard::indexToCoordinate(int index){
    std::string rank;
    std::string file;

    rank += std::to_string(index/8 + 1);

    switch (index % 8) {

        case 0:
            file = "a";
            break;
        case 1:
            file = "b";
            break;
        case 2:
            file = "c";
            break;
        case 3:
            file = "d";
            break;
        case 4:
            file = "e";
            break;
        case 5:
            file = "f";
            break;
        case 6:
            file = "g";
            break;
        case 7:
            file = "h";
            break;


        default:
            throw std::invalid_argument("error in indexToCoordinate switch case");
    }
    return file+rank;

    }


bool BitBoard::isCapture(int targetSquare) const{
     enumPieceBB captureColor = nWhite;
     if(whiteToMove)
         captureColor = nBlack;

     if((1ULL << targetSquare) & getPieceSet(captureColor))
         return true;
     else
         return false;
}
