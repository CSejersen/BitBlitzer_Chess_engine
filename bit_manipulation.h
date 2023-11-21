//
// Created by Christian Sejersen on 21/11/2023.
//
#pragma once
#include "board_constants.h"

// Bit manipulation functions
void setBit(U64&, int);
U64 getBit(U64&, int);
void clearBit(U64&, int i);
int getLSB(const U64&);
int countBits(U64);