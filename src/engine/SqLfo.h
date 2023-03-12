#pragma once

#include <functional>
#include <cstdint>

#include "Doc.h"

struct Sq_lfo_settings {
    std::function<void(int, unsigned int, unsigned int*, unsigned char**)> /* FIELD_a8 */ loadWave;

    int /* FIELD_b0 */ frequency; // frequency
    int /* FIELD_b4 */ humanize; // humanize
    int /* FIELD_b8 */ wave; // wave
    int /* FIELD_bc */ startAmplitude; // start amplitude of LFO (L1)

    int /* FIELD_c0 */ finalAmplitude; // final amplitude of LFO (L2)
    int /* FIELD_c4 */ delay; // speed of LFO amplitude fading (Delay)
    int /* FIELD_c8 */ phase1;
    int /* FIELD_cc */ phase2;

    int /* FIELD_d0 */ amplitudeModulation;
    int /* FIELD_d4 */ delayMode; // LFO delay mode
    bool /* FIELD_d8 */ reverse; // reverse
    bool /* FIELD_dc */ oneShot; // one shot

    bool /* FIELD_e0 */ twinMode; // twin mode
    int /* FIELD_e4 */ smoothness; // smoothness of LFO output
    // TODO FIELD_e8
    int /* FIELD_ec */ FIELD_ec;
};

// size 0xf0
class SqLfo {
private:
    int /* FIELD_4 */ mOutput;
    bool /* FIELD_8 */ mRunning;
    int /* FIELD_c */ mSmoothness;

    int /* FIELD_10 */ mPreviousSample;
    int /* FIELD_14 */ mPreviousSampleMix;
    int /* FIELD_18 */ mCurrentSampleMix;

    int /* FIELD_1c */ mPhase;
    int /* FIELD_20 */ mPhaseIncrement;
    int /* FIELD_24 */ mPhaseOffsets[2];
    int /* FIELD_2c */ mPhaseIncrementShift;

    int /* FIELD_30 */ mTwinMode;
    int /* FIELD_34 */ mFrequency;
    int /* FIELD_38 */ mFrequencyHumanization;
    bool /* FIELD_3c */ mReverse;
    bool /* FIELD_40 */ mOneShot;

    int /* FIELD_44 */ mStartAmplitude;
    int /* FIELD_48 */ mFinalAmplitude;
    int /* FIELD_4c */ mDelaySetting;
    int /* FIELD_50 */ mDelayMode;
    int /* FIELD_54 */ mAmplitude;
    int /* FIELD_58 */ mFinalAmplitude2;
    int /* FIELD_5c */ mDelay;
    int /* FIELD_60 */ mDelayCounter;
    int /* FIELD_64 */ mAmplitudeModulation;

    int /* FIELD_68 */ mWave;
    uint8_t* /* FIELD_6c */ mWaveData;
    int /* FIELD_70 */ mWaveMask;
    int /* FIELD_74 */ mWaveShift;

    int /* FIELD_78 */ mHumanize;
    int /* FIELD_7c */ mHumanizationCounter;
    int /* FIELD_80 */ mHumanization;
    int /* FIELD_84 */ mHumanizationAmount;

    int /* FIELD_88 */ mUpdateCounter;
    int /* FIELD_8c */ mUpdateInterval;

    char /* FIELD_90 */ mAmplitudeShift;
    char /* FIELD_90 */ mAmplitudeModulationShift;

    float /* FIELD_94 */ mSampleRate;
    float /* FIELD_98 */ mClockRatio;
    float /* FIELD_9c */ mLowFrequencyFactor;
    float /* FIELD_a0 */ mFrequencyFactor;
    bool /* FIELD_a4 */ mOriginalSampleRate;

public:
   Sq_lfo_settings mSettings;

private:
    void /* FUN_0045d1a8 */ init();
    int /* FUN_0045d110 */ getPhaseIncrement(int frequency);
    void /* FUN_0045d218 */ setDelayMode(bool smooth);
    void /* FUN_0045d25c */ setOutputSmoothness(int smoothness);
    void /* FUN_0045d3e0 */ updateSettings();
    int /* FUN_0045d824 */ updateState();

public:
    explicit /* FUN_0045cf9c */ SqLfo(float sampleRate);
    void /* FUN_0045d020 */ setSampleRate(float sampleRate);
    void /* FUN_0045d2a4 */ reset(int resetToPhase, bool resetPhase);
    int /* FUN_0045db2c */ nextSample();
};
