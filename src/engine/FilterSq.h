#pragma once

#include "FilterBase.h"

// CfilterSQ
// Size:
// VMT entries: 8
class FilterSq : public FilterBase {
    float field_0x18; // 18h
    int field_0x1c; // 1ch

    float* mTable4; // 20h
    bool mTable4Allocated; //24h

    // these seem to store the state of the filter
    float mBuf0; // 28h
    float mBuf1; // 2ch
    float mBuf2; // 30h
    float mBuf3; // 34h
    float mBuf4; // 38h

    float field_0x3c; // 3ch
    float field_0x40; // 40h
    float field_0x44; // 44h

    // these seem to be constants
    float field_0x48; // 48h
    float field_0x4c; // 4ch
    float field_0x50; // 50h
    float field_0x54; // 54h
    float field_0x58; // 58h
    float field_0x5c; // 5ch

    float field_0x60; // 60h
    float field_0x64; // 64h
    float field_0x68; // 68h

    // these seem to be constants
    float field_0x6c; // 6ch
    float field_0x70; // 70h
    float field_0x74; // 74h

    float field_0x78; // 78h

public:
    FilterSq(float *other, float sampleRate);
    ~FilterSq();

    void init();

    float process(float sample);
    void calculateFeedbackAmount(bool reset);
    void FUN_0045f120();

    void initTable4();

    void setField1c(int param_2);

    void setSampleRate(float sampleRate) override; // 00
    void copyState(FilterBase *other) override; // 0c
    void setCutoffAndResonance(int cutoff, int resonance, bool reset) override; // 10
    void setCutoff(int cutoff, bool reset) override; // 14
    void setResonance(int resonance, bool reset) override; // 18
    void setCutoffAndResonance2(int cutoff, int resonance, bool reset) override; // 1c
};
