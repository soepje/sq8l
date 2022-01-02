#pragma once

// Size: 0xe
struct Env_settings {
    char /* FIELD_00 */ levels[5]; // first entry is start level of envelope
    char /* FIELD_05 */ levelModulation; // Lv (modulation of levels by velocity, 0-63 linear, 63-127 exponential)
    char /* FIELD_06 */ attackTimeModulation; // T1v (modulation of attack time by velocity) (0-63)
    char /* FIELD_07 */ times[4];
    char /* FIELD_0b */ timeScaling; // Scale envelope times by key position (0-63)
    char /* FIELD_0c */ options; // Envelope shaping curve (off, exp, exp2, exp3, exp4, tan, tan2, tan3) odd = cyc, even = no cyc, Cyc envelope plays full cycle without sustain stage, actually some kind of bitset, first bit is cyc or not, then the shape, finally is t1v mode
    char /* FIELD_0d */ outputSmoothness; // Smoothness of envelope output (0-64)
};

class SqEnv {
private:
    int /* field_4  */ mLevels[5]; // L0, L1, L2, L3, L4 
    int /* field_18 */ mTimes[4]; // T1, T2, T3, T4
    int /* field_28 */ mLevelModulation;
    int /* field_2c */ mTimeScaling;
    bool /* field_30 */ mSecondRelease; 
    bool /* field_31 */ mCycle; // play full cycle without sustain stage
    int /* field_34 */ mLevel; // level (has 16 bit width, upper and lower are shifted each step)
    int /* field_38 */ mTargetLevel; // target level
    int /* field_3c */ mStepSize; // increment per step
    int /* field_40 */ mCounter; // mTime
    int /* field_44 */ mOutput; // mOutputLevel
    int /* field_48 */ mSmoothness;
    int /* field_4c */ mPreviousLevel;
    int /* field_50 */ mBias;
    int /* field_54 */ mInverseBias;
    int /* field_58 */ mShapingCurveIndex;
    char* /* field_5c */ mShapingCurve;
    bool /* field_60 */ mRunning;
    bool /* field_61 */ field_61; // # TODO there is probably also a getter for this member, same as mRunning, find out where in the engine this member is used
    bool /* field_62 */ mReleased;
    bool /* field_63 */ mSustaining;
    bool /* field_64 */ mFinalStep;
    void /* field_68 */ (SqEnv::*mStepFunction)();
    void /* field_70 */ (SqEnv::*mAdvanceStage)();
    float /* field_78 */ mSampleRate;
    float /* field_7c */ mStepsPerCentiSecond;

public:
    /* FUN_0045dbcc */ explicit SqEnv(float sampleRate);
    /* FUN_0045dc14 */ // ~SqEnv();
    void /* FUN_0045dcb8 */ setSampleRate(float sampleRate);
    void /* FUN_0045dd2c */ triggerAttack(Env_settings* settings, int pitch, SqEnv* other, bool cycle, bool clearLevels, int velocity);
    void /* FUN_0045dfb4 */ triggerRelease(bool sustain);
    int /* FUN_0045e1d4 */ getOutput();
    int /* FUN_0045e210 */ getOutputAndStep(bool sustain);
    bool isRunning() { return mRunning; }

private:
    void /* FUN_0045dc3c */ init();
    void /* FUN_0045dce4 */ setOutputSmoothness(int smoothness);
    void /* FUN_0045df18 */ setTargetLevelAndTime(int level, int time);
    void /* FUN_0045dfd4 */ stepUp();
    void /* FUN_0045e030 */ stepDown();
    void /* FUN_0045e08c */ stepLevel();
    void /* FUN_0045e0bc */ emptyFunction();
    void /* FUN_0045e0c0 */ startDecayStage();
    void /* FUN_0045e0f8 */ startSustainStage();
    void /* FUN_0045e130 */ startReleaseStage();
    void /* FUN_0045e160 */ startSecondReleaseStage();
    void /* FUN_0045e17c */ stop();    
    void /* FUN_0045e190 */ selectReleaseStage();
};
