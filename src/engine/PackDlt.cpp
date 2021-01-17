#include "PackDlt.h"

void /* FUN_00453378 */ PackDlt36::pack(unsigned char*  inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) {
    SimPack::pack(inputBuffer, inputBufferLength, outputBufferLength, outputBuffer);

    char previous = 0;

    while (mInputBufferIndex < mInputBufferSize) {
        char var4 = inputBuffer[mInputBufferIndex];

        mInputBufferIndex += 1;

        char delta = var4 - previous;

        if (delta < -3 || delta > 3) {
            if (delta < -32 || delta > 31) {
                writeBit(0);
                writeBits(0, 3);
                writeByte(var4);
            } else {
                writeBit(1);
                writeBits(delta + 32, 6);
            }

        } else {
            writeBit(0);
            writeBits(delta + 4, 3);
        }

        previous = var4;
    }

    finalizePack(outputBuffer, outputBufferLength);
}

void /* FUN_00453434 */ PackDlt36::unpack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) {
    SimPack::unpack(inputBuffer, inputBufferLength, outputBufferLength, outputBuffer);

    char var3 = 0;
    while (mOutputBufferIndex < mOutputBufferSize) {
        char var1 = readBit();

        if (var1 == 0) {
            char var2 = readBits(3); // read delta

            if (var2 == 0) {
                var3 = readByte(); // read byte
            } else {
                var3 += (var2 - 4);
            }
        } else {
            char var4 = readBits(6); // read delta

            var3 += (var4 - 32);
        }

        mOutputBuffer[mOutputBufferIndex] = var3;
        mOutputBufferIndex += 1;
    }

    finalizeUnpack(outputBuffer, outputBufferLength);
}
