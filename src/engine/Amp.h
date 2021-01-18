#pragma once

// Size: 0x14
struct Saturation {
    int /* field_0 */ level;
    float /* field_4 */ threshold;
    float /* field_8 */ parameterA;
    float /* field_c */ parameterB;
    float /* field_10 */ gain;
};

// Name: Camp
// Size: 0x74
class Amp {
private:
    float /* field_4 */ mVolume[2];
    float /* field_c */ mVolumeIncrement[2];
    int /* field_14 */ mSmoothingCounter;
    Saturation /* field_18 */ mSaturation;
    float /* field_2c */ mField_2c;
    float /* field_30 */ mField_30;
    float /* field_34 */ mField_34;
    float /* field_38 */ mField_38;
    float /* field_3c */ mField_3c;
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
    void copyState(Amp *other);
    void setParameters(unsigned int volume, int panning, bool reset);
    void setParameters(unsigned int volume, int panning, bool reset, float gain);
    void updateSmoothing();
    void setParameterValues(unsigned int volumeIndex, int panningIndex, bool reset);
    void setSampleRate(float sampleRate);
    void setSmoothingSamples(int samples);
    void setSmoothing(float seconds);
    void setVolumeResponse(int responseIndex);
    void setSaturation(int saturationIndex);
    void process(float *channels, float sample);
    void process(float *channels, float sample, float gain);
    int smoothingCounter() { return mSmoothingCounter; }
};
