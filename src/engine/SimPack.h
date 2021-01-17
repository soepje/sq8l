#pragma once

class SimPack {
protected:
    unsigned char* /* FIELD_4 */ mInputBuffer;
    unsigned char* /* FIELD_8 */ mOutputBuffer;

    unsigned int /* FIELD_c */ mInputBufferSize;
    unsigned int /* FIELD_10 */ mOutputBufferSize;

    bool /* FIELD_14 */ mOutputBufferAllocated;

    unsigned int /* FIELD_18 */ mInputBufferIndex;
    unsigned int /* FIELD_1c */ mInputBitIndex;
    unsigned int /* FIELD_20 */ mOutputBufferIndex;
    unsigned int /* FIELD_24 */ mOutputBitIndex;

    void /* FUN_00452f14 */ incrementOutputBufferIndex();

    unsigned char /* FUN_00452f98 */ readBit();
    unsigned char /* FUN_00452fc0 */ readByte();
    unsigned char /* FUN_00453034 */ readBits(int nbits);

    void /* FUN_004530b8 */ writeBit(unsigned char bit);
    void /* FUN_004530e4 */ writeByte(unsigned char param_2);
    void /* FUN_00453154 */ writeBits(unsigned char param_2, int nBits);

public:
    virtual void /* FUN_004531ec */ pack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer);
    virtual void /* FUN_00453250 */ unpack(unsigned char* inputBuffer, int inputBufferLength, int* outputBufferLength, unsigned char** outputBuffer);

    virtual void /* FUN_004532b8 */ finalizePack(unsigned char** outputBuffer, int* outputBufferLength);
    virtual void /* FUN_00453304 */ finalizeUnpack(unsigned char** outputBuffer, int* outputBufferLength);

    virtual /* FUN_00452ed8 */ ~SimPack();

private:
    void /* FUN_004531d4 */ resetBufferIndices(); //reset input and output buffer indices
};
