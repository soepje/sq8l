#pragma once

// CfilterBase
// Size: 0x18
// VMT Size: 8
class FilterBase {
public:
    float mSampleRate; // 4h
    int mField8; // 8h
    int mCutoff; // ch
    int mResonance; //10h
    float mOutputGain; // 14h

public:
    virtual void setSampleRate(float sampleRate); // 00
    virtual void setField8(int field8); // 04
    explicit FilterBase(float sampleRate); // 08
    virtual void copyState(FilterBase* other); // 0c
    virtual void setCutoffAndResonance(int cutoff, int resonance, bool reset); // 10
    virtual void setCutoff(int cutoff, bool reset); // 14
    virtual void setResonance(int resonance, bool reset); // 18
    virtual void setCutoffAndResonance2(int cutoff, int resonance, bool reset); // 1c
};
