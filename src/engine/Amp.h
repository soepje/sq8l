#pragma once

struct Saturation {
    int /* field_18 */ level;
    float /* field_1c */ threshold;
    float /* field_20 */ parameterA;
    float /* field_24 */ parameterB;
    float /* field_28 */ gain;
};

// Camp
class Amp {
    float /* field_4 */ mVolume[2];
    float /* field_c */ mVolumeIncrement[2];

public:
    int /* field_14 */ mSmoothingCounter;
private:

    Saturation /* field_18 */ mSaturation;

    float /* field_2c */ mFIELD_2c;
    float /* field_30 */ mFIELD_30;
    float /* field_34 */ mFIELD_34;
    float /* field_38 */ mFIELD_38;
    float /* field_3c */ mFIELD_3c;

    float /* field_40 */ mTargetVolume[2];
    int /* field_48 */ mSmoothingSamples;
    float /* field_4c */ mSmoothingSamplesInverse;
    unsigned int /* field_50 */ mVolumeIndex;
    int /* field_54 */ mPanningIndex;
    int /* field_58 */ mVolumeResponseIndex;
    unsigned int /* field_5c */ mPreviousVolumeIndex;
    int /* field_60 */ mVolumeFactorPrevious;
    int /* field_64 */ mVolumeFactor;
    float /* field_68 */ mSmoothingDuration; // smoothing duration in seconds
    float /* field_6c */ mTotalGain;
    float /* field_70 */ mSampleRate;

public:
    Amp();
    void init();
    void /* FUN_0045e554 */ copyState(Amp *other);
    void /* fun_45e680 */ setParameters(unsigned int volume, int panning, bool reset);
    void /* fun_45e69c */ setParameters(unsigned int volume, int panning, bool reset, float gain);
    void updateSmoothing();
    void setParameterValues(unsigned int volumeIndex, int panningIndex, bool reset);
    void setSampleRate(float sampleRate);
    void setSmoothingSamples(int samples);
    void setSmoothing(float seconds);
    void setVolumeResponse(int responseIndex);
    void setSaturation(int saturationIndex);
    void process(float outputs[2], float sample);
    void process(float *channels, float sample, float outputGain);
};
