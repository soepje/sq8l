#include <cmath>

#include "Amp.h"
#include "Data.h"

float ampSaturationTable[2][65][2];
float ampPanningTable[127][2];

void /* fun_45e33c */ initAmpSaturationTable() {
    float amp_saturation_parameters[2][4] = { { 0.73, 12.0, 0.584, 0.58 }, { 1.0, 70.0, 1.0, 1.0 } };

    for (int i = 0; i < 2; i++) {
        ampSaturationTable[i][0][0] = 1.0;
        ampSaturationTable[i][0][1] = amp_saturation_parameters[i][2];

        for (int j = 0; j < 64; j++) {
            float fVar1 = j * (1.0 / 63.0);
            float fVar2 = (amp_saturation_parameters[i][1] - amp_saturation_parameters[i][0]) * 
                fVar1 * fVar1 + amp_saturation_parameters[i][0];
            float fVar3 = (amp_saturation_parameters[i][3] - 1.0f) * fVar1 + 1.0f;

            if (fVar2 < 0.1) {
                fVar2 = 0.1;
            }

            ampSaturationTable[i][j + 1][0] = fVar2;
            ampSaturationTable[i][j + 1][1] = fVar3;
        }
    }
}

void /* fun_45e9d0 */ getAmpPanning(float channels[2], float panning) {
    if (panning < -1.0) {
        panning = -1.0;
    } else if (panning > 1.0) {
        panning = 1.0;
    }

    float x = sqrtf(1.0f / (panning * panning * 2.0f + 2.0f));

    channels[0] = x - x * panning;
    channels[1] = x + x * panning;
}

void /* fun_45ea54 */ initAmpPanningTable() {
    for (int i = -63, j = 0; i <= 63; i++, j++) {
        getAmpPanning(ampPanningTable[j], (float) i / 63.0f);
    }
}

/* fun_45e4a0 */ Amp::Amp() {
    // These are probably called in the Delphi initialization section
    initAmpSaturationTable();
    initAmpPanningTable();

    init();
    updateSmoothing();
    setSmoothing(0.0);
    mSaturation.level = -2;
    setSaturation(0);
}

void /* FUN_0045e52c */ Amp::init() {
    mFIELD_34 = 1.4142135381698608; //, possibly sqrt(2)
    mFIELD_38 = 1.0606601238250732; //, possibly 3 / (2 * sqrt(2))
    mFIELD_3c = 0.1666666716337204; //, 1.0 / 6.0
    mFIELD_2c = 1.0;
    mFIELD_30 = -1.0;
}

void /* FUN_0045e554 */ Amp::copyState(Amp* other) {
    if (other == nullptr) {
        setParameters(0, mPanningIndex, true);
        return;
    }

    mVolumeIndex = other->mVolumeIndex;
    mPanningIndex = other->mPanningIndex;
    mVolumeResponseIndex = other->mVolumeResponseIndex;
    mPreviousVolumeIndex = other->mPreviousVolumeIndex;
    mVolumeFactorPrevious = other->mVolumeFactorPrevious;
    mVolumeFactor = other->mVolumeFactor;
    mSaturation = other->mSaturation;
    mVolume[0] = other->mVolume[0];
    mVolume[1] = other->mVolume[1];
    mTargetVolume[0] = other->mTargetVolume[0];
    mTargetVolume[1] = other->mTargetVolume[1];
    mVolumeIncrement[0] = other->mVolumeIncrement[1]; // ???
    mVolumeIncrement[1] = other->mVolumeIncrement[1];
    mSmoothingCounter = other->mSmoothingCounter;
    mSmoothingSamples = other->mSmoothingSamples;
    mSmoothingSamplesInverse = other->mSmoothingSamplesInverse;
    mTotalGain = other->mTotalGain;
}

void /* fun_45e5dc */ Amp::setParameterValues(unsigned int volumeIndex, int panningIndex, bool reset) {
    if (volumeIndex > 255) {
        volumeIndex = 255;
    }
  
    if (panningIndex < -63) {
        panningIndex = -63;
    } else if (panningIndex > 63) {
        panningIndex = 63;
    }

    if (mVolumeResponseIndex == 0) {
        mVolumeIndex = volumeIndex;
    } else {
        if (!reset) {
            unsigned int index = mPreviousVolumeIndex * mVolumeFactor +
                                 volumeIndex * mVolumeFactor * 256;

            index /= 256;

            mPreviousVolumeIndex = index;

            index /= 256;
            
            if (index > 255) {
                mVolumeIndex = 255;
            } else {
                mVolumeIndex = index;
            }
        } else {
            mPreviousVolumeIndex = mVolumeIndex * 256;
            mVolumeIndex = volumeIndex;
        }
    }

    mPanningIndex = panningIndex;
}

void /* fun_45e680 */ Amp::setParameters(unsigned int volumeIndex, int panningIndex, bool reset) {
    setParameters(volumeIndex, panningIndex, reset, 1.0);
}

void /* fun_45e69c */ Amp::setParameters(unsigned int volume, int panning, bool reset, float gain) {
    setParameterValues(volume, panning, reset);

    float total_gain_local = mSaturation.gain * 1.61378f * gain;

    if (!reset && mSmoothingCounter < 0) {
        mVolume[0] = mTargetVolume[0];
        mVolume[1] = mTargetVolume[1];
    }

    mTargetVolume[0] = ampPanningTable[mPanningIndex][0] * ampVolumeCurve[volume] * total_gain_local;
    mTargetVolume[1] = ampPanningTable[mPanningIndex][1] * ampVolumeCurve[volume] * total_gain_local;

    if (mTargetVolume[0] < 1e-06) {
        mTargetVolume[0] = 1e-06;
    }

    if (mTargetVolume[1] < 1e-06) {
        mTargetVolume[1] = 1e-06;
    }
    
    if (reset) {
        mVolumeIncrement[0] = mTargetVolume[0] - mVolume[0] * mSmoothingSamplesInverse;
        mVolumeIncrement[1] = mTargetVolume[1] - mVolume[1] * mSmoothingSamplesInverse;

        mVolume[0] *= mVolumeIncrement[0];
        mVolume[1] *= mVolumeIncrement[1];

        mSmoothingCounter = mSmoothingSamples;
    } else {
        mVolume[0] = mTargetVolume[0];
        mVolume[1] = mTargetVolume[1];

        mVolumeIncrement[0] = 0;
        mVolumeIncrement[1] = 0;

        mSmoothingCounter = 0;
    }

    mTotalGain = total_gain_local;
}

void /* fun_45e7b8 */ Amp::setSampleRate(float sample_rate) {
    mSampleRate = sample_rate;

    updateSmoothing();
}

void /* fun_45e7cc */ Amp::setSmoothingSamples(int samples) {
    if (samples > 0) {
        mSmoothingSamples = samples;
        mSmoothingSamplesInverse = 1.0f / (float) samples;
    } else {
        mSmoothingSamples = 0;
        mSmoothingSamplesInverse = 0;
    }
}

void /* fun_45e7f8 */ Amp::setSmoothing(float seconds) {
    if (seconds <= 0.0) {
        mSmoothingDuration = 0.0;

        setSmoothingSamples(0);
    } else {
        mSmoothingDuration = seconds;

        setSmoothingSamples((int) roundf(seconds * mSampleRate));
    }
}

void /* fun_45e848 */ Amp::setVolumeResponse(int responseIndex) {
    if (responseIndex < 0) {
        mVolumeResponseIndex = 0;
        return;
    }

    if (responseIndex > 255) {
        responseIndex = 255;
    }

    int response = 255 - (((255 - responseIndex) * (255 - responseIndex)) / 256);

    mVolumeResponseIndex = responseIndex;
    mVolumeFactorPrevious = response;
    mVolumeFactor = 256 - response;
}

void /* fun_45e890 */ Amp::updateSmoothing() {
    setSmoothing(mSmoothingDuration);
}

void /* fun_45e89c */ Amp::setSaturation(int saturationIndex) {
    if (saturationIndex < -1) {
        saturationIndex = -1;
    } else if (saturationIndex > 63) {
        saturationIndex = 63;
    }

    mSaturation.level = saturationIndex;

    float x = ampSaturationTable[0][mSaturation.level + 1][0];
    float gain = ampSaturationTable[0][mSaturation.level + 1][1];
    float c = (float) (3.0 * sqrt(2) / 4.0);

    mSaturation.threshold = sqrt(2) / x;
    mSaturation.parameterA = -(x * x * x * c) / 6.0f;
    mSaturation.parameterB = x * c;
    mSaturation.gain = gain;
}

void /* fun_45e8ec */ Amp::process(float channels[2], float sample) {
    if (mSaturation.level > -1) {
        if (fabsf(sample) >= mSaturation.threshold) {
            sample = (sample > 0) ? 1 : -1;
        } else {
            sample = mSaturation.parameterB * sample + mSaturation.parameterA * sample * sample * sample;
        }
    }

    channels[0] += mVolume[0] * sample;
    channels[1] += mVolume[1] * sample;

    if (mSmoothingCounter >= 0) {
        mVolume[0] += mVolumeIncrement[0];
        mVolume[1] += mVolumeIncrement[1];
    }

    mSmoothingCounter--;
}

void /* FUN_0045e95c */ Amp::process(float channels[2], float sample, float outputGain) {
    if (mSaturation.level > -1) {
        if (fabsf(sample) >= mSaturation.threshold) {
            sample = (sample > 0) ? 1 : -1;
        } else {
            sample = mSaturation.parameterB * sample + mSaturation.parameterA * sample * sample * sample;
        }
    }

    channels[0] += mVolume[0] * sample * outputGain;
    channels[1] += mVolume[1] * sample * outputGain;

    if (mSmoothingCounter >= 0) {
        mVolume[0] += mVolumeIncrement[0];
        mVolume[1] += mVolumeIncrement[1];
    }

    mSmoothingCounter--;
}
