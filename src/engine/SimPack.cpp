#include <cstring>
#include <string>

#include "SimPack.h"

/* FUN_00452ed8 */ SimPack::~SimPack() {
    if (mOutputBufferAllocated && mOutputBuffer != nullptr) {
        std::free(mOutputBuffer);
    }
}

// increments output buffer index and enlarges output buffer if necessary
void SimPack::incrementOutputBufferIndex() {
    mOutputBufferIndex += 1;

    if (mOutputBufferSize <= mOutputBufferIndex) {
        mOutputBufferSize += 1024;
        mOutputBuffer = (unsigned char*) std::realloc(mOutputBuffer, mOutputBufferSize);
    }

    mOutputBuffer[mOutputBufferIndex] = 0;
    mOutputBitIndex = 0;
}

// reads a bit from the input buffer
unsigned char SimPack::readBit() {
    unsigned int inputBitIndex = mInputBitIndex;
    unsigned char inputByte = mInputBuffer[mInputBufferIndex];

    mInputBitIndex += 1;

    if (mInputBitIndex > 7) {
        mInputBitIndex = 0;
        mInputBufferIndex += 1;
    }

    unsigned char bit = (inputByte >> inputBitIndex) & 1;

    return bit;
}

// reads a byte from the input buffer
unsigned char SimPack::readByte() {
    char var1 = 8 - mInputBitIndex;
    unsigned char var2 = ((1 << var1) - 1) & (mInputBuffer[mInputBufferIndex] >> mInputBitIndex);

    mInputBitIndex += var1;

    if (mInputBitIndex > 7) {
        mInputBitIndex = 0;
        mInputBufferIndex += 1;
    }

    if (var1 < 8) {
        char var3 = 8 - var1;

        var2 = var2 | ((((1 << var3) - 1) & mInputBuffer[mInputBufferIndex]) << (8 - var3));

        mInputBitIndex += var3;
    }

    return var2;
}

// reads nbit number from the input buffer
unsigned char SimPack::readBits(int nbits) {
    char var1 = 8 - mInputBitIndex;

    if (nbits < var1) {
        var1 = nbits;
    }

    unsigned char var2 = ((1 << var1) - 1) & (mInputBuffer[mInputBufferIndex] >> mInputBitIndex);

    mInputBitIndex += var1;

    if (mInputBitIndex > 7) {
        mInputBitIndex = 0;
        mInputBufferIndex += 1;
    }

    if (var1 < nbits) {
        char var3 = nbits - var1;

        var2 = var2 | ((((1 << var3) - 1) & mInputBuffer[mInputBufferIndex]) << (nbits - var3));

        mInputBitIndex += var3;
    }

    return var2;
}

// write a single bit to the output buffer
void SimPack::writeBit(unsigned char bit) {
    if (mOutputBitIndex > 7) {
        incrementOutputBufferIndex();
    }

    mOutputBuffer[mOutputBufferIndex] |= (bit & 1) << mOutputBitIndex;
    mOutputBitIndex += 1;
}

// write nbit number to the output buffer
void /* FUN_00453154 */ SimPack::writeBits(unsigned char param_2, int nBits) {
    if (mOutputBitIndex > 7) {
        incrementOutputBufferIndex();
    }

    // the amount of bits available in the current output byte
    char var1 = 8 - mOutputBitIndex;

    if (nBits < var1) {
        var1 = nBits;
    }

    // creates a bitmask for the amount of bits left in the current output byte
    // that bitmask is applied, the result is shifted to correct location
    // and xor'ed with the byte in the output buffer
    mOutputBuffer[mOutputBufferIndex] |= (((1 << var1) - 1) & param_2) << mOutputBitIndex;

    mOutputBitIndex += var1;

    // if the bits do not fit in current output byte
    if (var1 < nBits) {
        // advance the output buffer index
        incrementOutputBufferIndex();

        mOutputBuffer[mOutputBufferIndex] |= param_2 >> var1;
        mOutputBitIndex += (nBits - var1);
    }
}

// write a byte to the output buffer
void /* FUN_004530e4 */ SimPack::writeByte(unsigned char param_2) {
    if (mOutputBitIndex > 7) {
        incrementOutputBufferIndex();
    }

    // the amount of bits available in the current output byte
    unsigned int var1 = 8 - mOutputBitIndex;

    // creates a bitmaskfor the amount of bits left in the current output byte
    // that bitmask is applied, the result is shifted to correct location
    // and xor'ed with the byte in the output buffer
    mOutputBuffer[mOutputBufferIndex] |= (((1 << var1) - 1) & param_2) << mOutputBitIndex;

    mOutputBitIndex += var1;

    // if the bits do not fit in current output byte
    if (var1 < 8) {
        // advance the output buffer index
        incrementOutputBufferIndex();

        mOutputBuffer[mOutputBufferIndex] |= param_2 >> var1;
        mOutputBitIndex += (8 - var1);
    }
}

void SimPack::resetBufferIndices() {
    mInputBufferIndex = 0;
    mInputBitIndex = 0;
    mOutputBufferIndex = 0;
    mOutputBitIndex = 0;
}

void /* FUN_004531ec */ SimPack::pack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferSize, unsigned char** outputBuffer) {
    resetBufferIndices();

    mInputBufferSize = inputBufferLength;
    mInputBuffer = inputBuffer;
    mOutputBufferSize = inputBufferLength + 4;
    mOutputBufferAllocated = true;

    if (mOutputBuffer == nullptr) {
        mOutputBuffer = (unsigned char*) std::malloc(mOutputBufferSize);
    } else {
        mOutputBuffer = (unsigned char*) std::realloc(mOutputBuffer, mOutputBufferSize);
    }

    mOutputBuffer[0] = inputBufferLength & 0xff;
    mOutputBuffer[1] = (inputBufferLength >> 8) & 0xff;
    mOutputBuffer[2] = (inputBufferLength >> 16) & 0xff;
    mOutputBuffer[3] = (inputBufferLength >> 24) & 0xff;

    mOutputBufferIndex = 4;

    if (mOutputBufferSize > 4) {
        mOutputBuffer[4] = 0;
    }
}

void SimPack::unpack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) {
    resetBufferIndices();

    mInputBufferSize = inputBufferLength;
    mInputBuffer = inputBuffer;
    mOutputBufferSize = inputBuffer[0] + (inputBuffer[1] << 8) + (inputBuffer[2] << 16) + (inputBuffer[3] << 24); // = *param_2;

    mInputBufferIndex += 4;

    if (*outputBuffer == nullptr) {
        mOutputBufferAllocated = true;
        if (mOutputBuffer == nullptr) {
            mOutputBuffer = (unsigned char*) std::malloc(mOutputBufferSize);
        } else {
            mOutputBuffer = (unsigned char*) std::realloc(mOutputBuffer, mOutputBufferSize);
        }
    } else {
        mOutputBufferAllocated = false;
        mOutputBuffer = *outputBuffer;
    }
}

void SimPack::finalizePack(unsigned char** outputBuffer, int* outputBufferLength) {
    *outputBuffer = nullptr;
    *outputBufferLength = 0;

    if (mOutputBuffer != nullptr && mOutputBufferSize > 0 && mOutputBufferIndex > 0) {
        *outputBufferLength = mOutputBufferIndex + 1;

        if (mOutputBufferAllocated) {
            mOutputBuffer = (unsigned char*) std::realloc(mOutputBuffer, *outputBufferLength);
        }

        *outputBuffer = mOutputBuffer;

        mOutputBuffer = nullptr;
        mOutputBufferAllocated = 0;
        mOutputBufferSize = 0;
    }
}

void SimPack::finalizeUnpack(unsigned char** outputBuffer, int* outputBufferLength) {
    int var1 = mOutputBufferIndex;

    if (mOutputBitIndex > 0) {
        var1 += 1;
    }

    if (*outputBuffer == nullptr) {
        if (!mOutputBufferAllocated) {
            *outputBuffer = ((unsigned char*) std::malloc(var1));
            std::memmove(mOutputBuffer, outputBuffer, var1);
        } else {
            *outputBuffer = mOutputBuffer;
        }
    } else {
        if (mOutputBufferAllocated) {
            std::memmove(mOutputBuffer, outputBuffer, var1);
            std::free(mOutputBuffer);
        }        
    }

    if (outputBufferLength != nullptr) {
        *outputBufferLength = var1;
    }

    mOutputBuffer = nullptr;
    mOutputBufferAllocated = false;
    mOutputBufferSize = 0;
}
