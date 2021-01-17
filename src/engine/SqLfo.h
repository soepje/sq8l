#pragma once

#include "Doc.h"

typedef void (Doc::*DocFunc) (int note, unsigned int wave, char* waveData, unsigned int* waveDataSize);

struct Sq_lfo_settings {
    DocFunc /* FIELD_a8 */  FIELD_a8; // FUNCTION pointer to doc loads waveform
    Doc* /* FIELD_ac */ FIELD_ac;

    int /* FIELD_b0 */ frequency; // frequency
    int /* FIELD_b4 */ humanize; // humanize
    int /* FIELD_b8 */ wave; // wave
    int /* FIELD_bc */ startAmplitude; // start amplitude of LFO (L1)

    int /* FIELD_c0 */ finalAmplitude; // final amplitude of LFO (L2)
    int /* FIELD_c4 */ fadingSpeed; // speed of LFO amplitude fading (Delay)
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
    int /* FIELD_8 */ mRunning;
    int /* FIELD_c */ mSmoothness;

    int /* FIELD_10 */ mPreviousSample;
    int /* FIELD_14 */ mPreviousSampleMix;
    int /* FIELD_18 */ mCurrentSampleMix;
    int /* FIELD_1c */ mCounter;
   
    int /* FIELD_20 */ mCounterIncrement;
    int /* FIELD_24 */ mWaveOffsets[2];
    int /* FIELD_2c */ FIELD_2c;

    int /* FIELD_30 */ mTwinMode;
    int /* FIELD_34 */ mFrequency;
    int /* FIELD_38 */ mFrequencyHumanization;
    bool /* FIELD_3c */ mReverse;

    bool /* FIELD_40 */ mOneShot;
    int /* FIELD_44 */ mStartAmplitude;
    int /* FIELD_48 */ mFinalAmplitude;
    int /* FIELD_4c */ mFadingSpeed;

    int /* FIELD_50 */ mSmootherFading;
    int /* FIELD_54 */ mAmplitude;
    int /* FIELD_58 */ mFinalAmplitude2;
    int /* FIELD_5c */ mFading;

    int /* FIELD_60 */ FIELD_60;
    int /* FIELD_64 */ mAmplitudeModulation;
    int /* FIELD_68 */ mWave;
    char* /* FIELD_6c */ mWaveData;

    int /* FIELD_70 */ FIELD_70;
    int /* FIELD_74 */ FIELD_74;
    int /* FIELD_78 */ mHumanize;
    int /* FIELD_7c */ FIELD_7c;

    int /* FIELD_80 */ mHumanization;
    int /* FIELD_84 */ mHumanizationFactor;
    int /* FIELD_88 */ FIELD_88; // some internal state
    int /* FIELD_8c */ FIELD_8c; // increment for internal state

    char /* FIELD_90 */ mAmplitudeFactor;
    char /* FIELD_90 */ FIELD_91;
    float /* FIELD_94 */ mSampleRate;
    float /* FIELD_98 */ mFadingFactor;
    float /* FIELD_9c */ FIELD_9c;

    float /* FIELD_a0 */ mFrequencyFactor;
    bool /* FIELD_a4 */ mDefaultSampleRate;

public:
   Sq_lfo_settings mSettings;

private:
    void /* FUN_0045d1a8 */ init();
    int /* FUN_0045d110 */ getCounterIncrement(int frequency);
    void /* FUN_0045d218 */ setDelayMode(bool x);
    void /* FUN_0045d25c */ setOutputSmoothness(int x);
    void /* FUN_0045d3e0 */ updateSettings();
    int /* FUN_0045d824 */ calculateSample();

public:
    /* FUN_0045cf9c */ SqLfo(float sampleRate);
    void /* FUN_0045d020 */ setSampleRate(float sampleRate);
    void /* FUN_0045d2a4 */ FUN_0045d2a4(int x, bool resetCounter);
    int /* FUN_0045db2c */ nextSample();
};

