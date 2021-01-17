#include "PackRle.h"

int /* FUN_00452f48 */ PackRle25::getRepeatedByteCount(int maxCount) {
    int count = 1;

    while (mInputBufferIndex + count < mInputBufferSize && (maxCount <= 0 || count < maxCount) &&
           mInputBuffer[mInputBufferIndex] == mInputBuffer[mInputBufferIndex + count]) {
        count += 1;
    }

    if (maxCount > 0 && maxCount < count) {
        count = maxCount;
    }

    return count;
}

void /* FUN_0045356c */ PackRle25::pack(unsigned char*  inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) {
    SimPack::pack(inputBuffer, inputBufferLength, outputBufferLength, outputBuffer);

    auto FUN_004534c0 /* FUN_004534c0 */ = [&](unsigned char param_1) {
        writeBit(0);
        writeByte(param_1);

        mInputBufferIndex += 1;
    };

    auto /* FUN_004534ec */ FUN_004534ec = [&](int param_1, unsigned char param_2) {
        writeBit(1);

        if (param_1 < 4) {
            writeBit(0);
            writeBits(param_1, 2);
        } else {
            if (param_1 > 31) {
                param_1 = 3;
            }

            writeBit(1);
            writeBits(param_1, 5);
        }

        writeByte(param_2);

        mInputBufferIndex += param_1 + 2;
    };

    while (mInputBufferIndex < mInputBufferSize) {
        unsigned char var1 = inputBuffer[mInputBufferIndex];
        int repeats = getRepeatedByteCount(31);

        if (repeats < 2) {
            if (repeats - 1 >= 0) {
                while (repeats > 0) {
                    FUN_004534c0(var1);

                    repeats -= 1;
                }
            }
        } else {
            FUN_004534ec(repeats - 2, var1);
        }
    }

    finalizePack(outputBuffer, outputBufferLength);
}

void /* FUN_00453628 */ PackRle25::unpack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) {
    SimPack::unpack(inputBuffer, inputBufferLength, outputBufferLength, outputBuffer);

    // pretty sure this is implemented in delphi as a nested function or
    // an anonymous function, parent stack frame is giving as final argument
    // in bytecode, need to compile some examples to know for sure, also,
    // this lambda method is directly before the defining method in bytecode
    // writes param_2 to the output buffer param_1 times
    auto writeRepeatingBytes /* FUN_004535e8 */ = [&](int repeats, unsigned char repeatingByte) {
        // make sure repeats is not negative
        if (repeats - 1 >= 0) {
            // does not exactly match the bytecode, but has the same effect
            for (int i = 0; i < repeats; i++) {
                mOutputBuffer[mOutputBufferIndex + i] = repeatingByte;
            }
        }

        mOutputBufferIndex += repeats;
    };

    while (mOutputBufferIndex < mOutputBufferSize) {
        unsigned char isRepeatingBit = readBit();

        if (isRepeatingBit == 0) { // no repeats?
            mOutputBuffer[mOutputBufferIndex] = readByte();
            mOutputBufferIndex += 1;
        } else {
            // small repeats number or large repeats numbers
            unsigned char bit = readBit();

            // amount of repeats
            unsigned char repeats;
            if (bit == 0) {
                repeats = readBits(2);
            } else {
                repeats = readBits(5);
            }

            // repeating char
            unsigned char repeatingByte = readByte();

            // there is a buffer overflow possible here, but you can only
            // write the same character beyond the buffer
            writeRepeatingBytes(repeats + 2, repeatingByte);
        }
    }

    finalizeUnpack(outputBuffer, outputBufferLength);
}
