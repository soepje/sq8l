#pragma once

#include "SimPack.h"

class PackDlt36 : public SimPack {
public:
    void /* FUN_00453378 */ pack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) override;
    void /* FUN_00453434 */ unpack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer) override;
};
