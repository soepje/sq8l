#pragma once

#include "SimPack.h"

class PackRle25 : public SimPack {
private:
    int /* FUN_00452f48 */ getRepeatedByteCount(int maxCount);

public:
    void /* FUN_0045356c */ pack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) override;
    void /* FUN_00453628 */ unpack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) override;
};
