#pragma once

// Cmod_foll
// Size: 0x20
class ModFoll {
    float mSampleRate;
    float mField8;
    float mFieldc;
    int mField10;
    int mField14;
    int mField18;
    int mField1c;

public:
    void FUN_0045ebe8(float param_4);
    void FUN_0045ec1c(int param_2);
    void FUN_0045ec68(int param_2);


    void setSampleRate(float sampleRate);

    int getOutput();

    ModFoll(float sampleRate);
};
