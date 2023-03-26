#include <cmath>

#include "Amp.h"

float ampVolumeCurve[256] = {
    0.0,           0.00387464953, 0.00406694505, 0.00427471288, 0.00449132221, 0.00470793108, 0.00492453994, 0.00514114881,
    0.00536438916, 0.00569814397, 0.00603189832, 0.00636565359, 0.0066994084,  0.00703316275, 0.00746417092, 0.00790180918,
    0.00834828895, 0.00879476964, 0.00914841611, 0.00950206444, 0.0100060115,  0.0105033275,  0.0110050645,  0.0115068033,
    0.0120195914,  0.0125323804,  0.0131689459,  0.0138055114,  0.0144752311,  0.0151449507,  0.0160091761,  0.0168667715,
    0.0177243669,  0.0185819604,  0.0195920672,  0.0205822792,  0.02155702,    0.0225229189,  0.0236855336,  0.0248879362,
    0.026090337,   0.0272861086,  0.0285172425,  0.0297859535,  0.0313309506,  0.0328803658,  0.0344408378,  0.0359173156,
    0.0375330374,  0.0391509756,  0.0407799669,  0.0424155854,  0.0442832857,  0.0460117385,  0.0478175506,  0.0497581922,
    0.0516988337,  0.0536284186,  0.0556928366,  0.0578368232,  0.0601775311,  0.0625248626,  0.0648721978,  0.0669874474,
    0.0693325773,  0.0716776997,  0.0740338787,  0.0764143616,  0.0790866092,  0.0818141177,  0.0845438316,  0.0872757658,
    0.090228714,   0.093219243,   0.0962097719,  0.0992555618,  0.102593109,   0.105722897,   0.108852677,   0.1121637,
    0.115768693,   0.119263172,   0.122748807,   0.126413479,   0.130374327,   0.134142891,   0.137951225,   0.141887754,
    0.145912722,   0.149984062,   0.154055446,   0.158230692,   0.16251424,    0.166647494,   0.170754224,   0.175079793,
    0.179809824,   0.184464693,   0.189132839,   0.194072857,   0.19887805,    0.203590393,   0.208274022,   0.21321182,
    0.218366221,   0.223487481,   0.228617579,   0.234269306,   0.240007237,   0.245736331,   0.251396894,   0.257556975,
    0.263763517,   0.27006948,    0.276340067,   0.283114642,   0.289995283,   0.296935618,   0.303836167,   0.311209738,
    0.318375498,   0.325313598,   0.332260579,   0.339819789,   0.347281754,   0.354768038,   0.362698555,   0.370772809,
    0.378758609,   0.386618406,   0.394979924,   0.40332824,    0.411510766,   0.420033664,   0.42893675,    0.437934846,
    0.446851194,   0.455705613,   0.464820892,   0.473938346,   0.482786179,   0.491722435,   0.500753701,   0.50993526,
    0.518749893,   0.527781188,   0.536706328,   0.545883536,   0.554709196,   0.563663125,   0.572583914,   0.581898093,
    0.590688467,   0.599854529,   0.609491408,   0.619362593,   0.628637016,   0.638293743,   0.647937298,   0.657527804,
    0.667027652,   0.67730993,    0.687658548,   0.697699964,   0.707778871,   0.718693256,   0.729561329,   0.740444899,
    0.751206756,   0.76257211,    0.774021447,   0.785406709,   0.796743274,   0.808751822,   0.820729434,   0.832636356,
    0.84451884,    0.857163966,   0.869603515,   0.882310569,   0.894765496,   0.907919049,   0.921331048,   0.934515476,
    0.947452426,   0.961103201,   0.974930763,   0.988568306,   1.00197375,    1.01607537,    1.03009534,    1.04422569,
    1.05794728,    1.07222354,    1.0859164,     1.09981906,    1.11340809,    1.12785673,    1.14184356,    1.15590322,
    1.16945899,    1.18406451,    1.19854867,    1.21314549,    1.22731781,    1.24222195,    1.25680315,    1.27172709,
    1.28653169,    1.30196404,    1.31654525,    1.33207262,    1.34703195,    1.36299467,    1.37810433,    1.39321387,
    1.40829909,    1.42422879,    1.43957472,    1.45534086,    1.47090137,    1.48712933,    1.50295734,    1.51916993,
    1.53496671,    1.55171204,    1.56791341,    1.58448195,    1.60076952,    1.61749709,    1.63351727,    1.6500746,
    1.66622305,    1.68306327,    1.69889343,    1.7148186,     1.73042333,    1.74764597,    1.76444197,    1.78232324,
    1.79951727,    1.81648564,    1.83270037,    1.84886658,    1.86383688,    1.87880719,    1.89277399,    1.9067409,
    1.92002475,    1.93323123,    1.94544768,    1.95753348,    1.96864688,    1.97968507,    1.98999834,    2.0,
};
float ampSaturationParameters[2][4] = { { 0.73, 12.0, 0.584, 0.58 }, {1.0, 70.0, 1.0, 1.0 } };
float ampSaturationTable[2][65][2];
float ampPanningTable[127][2];

void /* FUN_0045e33c */ initAmpSaturationTable() {
    for (int i = 0; i < 2; i++) {
        ampSaturationTable[i][0][0] = 1.0;
        ampSaturationTable[i][0][1] = ampSaturationParameters[i][2];

        for (int j = 0; j < 64; j++) {
            float fVar1 = j * (1.0 / 63.0);
            float fVar2 = (ampSaturationParameters[i][1] - ampSaturationParameters[i][0]) *
                          fVar1 * fVar1 + ampSaturationParameters[i][0];
            float fVar3 = (ampSaturationParameters[i][3] - 1.0f) * fVar1 + 1.0f;

            if (fVar2 < 0.1) {
                fVar2 = 0.1;
            }

            ampSaturationTable[i][j + 1][0] = fVar2;
            ampSaturationTable[i][j + 1][1] = fVar3;
        }
    }
}

void /* FUN_0045e9d0 */ getAmpPanning(float channels[2], float panning) {
    if (panning < -1.0) {
        panning = -1.0;
    } else if (panning > 1.0) {
        panning = 1.0;
    }

    float x = sqrtf(1.0f / (panning * panning * 2.0f + 2.0f));

    channels[0] = x - x * panning;
    channels[1] = x + x * panning;
}

void /* FUN_0045ea54 */ initAmpPanningTable() {
    for (int i = -63, j = 0; i <= 63; i++, j++) {
        getAmpPanning(ampPanningTable[j], (float) i / 63.0f);
    }
}

/* FUN_0045e4a0 */ Amp::Amp() {
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
    mField_34 = 1.4142135381698608; //, possibly sqrt(2)
    mField_38 = 1.0606601238250732; //, possibly 3 / (2 * sqrt(2))
    mField_3c = 0.1666666716337204; //, 1.0 / 6.0
    mField_2c = 1.0;
    mField_30 = -1.0;
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

void /* FUN_0045e5dc */ Amp::setParameterValues(unsigned int volumeIndex, int panningIndex, bool reset) {
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
            unsigned int index = mPreviousVolumeIndex * mVolumeFactor + volumeIndex * mVolumeFactor * 256;

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

void /* FUN_0045e680 */ Amp::setParameters(unsigned int volumeIndex, int panningIndex, bool reset) {
    setParameters(volumeIndex, panningIndex, reset, 1.0);
}

void /* FUN_0045e69c */ Amp::setParameters(unsigned int volume, int panning, bool reset, float gain) {
    setParameterValues(volume, panning, reset);

    float totalGain = mSaturation.gain * 1.22756f * gain;

    if (!reset && mSmoothingCounter < 0) {
        mVolume[0] = mTargetVolume[0];
        mVolume[1] = mTargetVolume[1];
    }

    mTargetVolume[0] = ampPanningTable[mPanningIndex + 63][0] * ampVolumeCurve[volume] * totalGain;
    mTargetVolume[1] = ampPanningTable[mPanningIndex + 63][1] * ampVolumeCurve[volume] * totalGain;

    if (mTargetVolume[0] < 1e-06) {
        mTargetVolume[0] = 1e-06;
    }

    if (mTargetVolume[1] < 1e-06) {
        mTargetVolume[1] = 1e-06;
    }

    if (reset) {
        mVolume[0] = mTargetVolume[0];
        mVolume[1] = mTargetVolume[1];

        mVolumeIncrement[0] = 0;
        mVolumeIncrement[1] = 0;

        mSmoothingCounter = 0;
    } else {
        mVolumeIncrement[0] = (mTargetVolume[0] - mVolume[0]) * mSmoothingSamplesInverse;
        mVolumeIncrement[1] = (mTargetVolume[1] - mVolume[1]) * mSmoothingSamplesInverse;

        mVolume[0] += mVolumeIncrement[0];
        mVolume[1] += mVolumeIncrement[1];

        mSmoothingCounter = mSmoothingSamples;
    }

    mTotalGain = totalGain;
}

void /* FUN_0045e7b8 */ Amp::setSampleRate(float sample_rate) {
    mSampleRate = sample_rate;

    updateSmoothing();
}

void /* FUN_0045e7cc */ Amp::setSmoothingSamples(int samples) {
    if (samples > 0) {
        mSmoothingSamples = samples;
        mSmoothingSamplesInverse = 1.0f / (float) samples;
    } else {
        mSmoothingSamples = 0;
        mSmoothingSamplesInverse = 0;
    }
}

void /* FUN_0045e7f8 */ Amp::setSmoothing(float seconds) {
    if (seconds <= 0.0) {
        mSmoothingDuration = 0.0;

        setSmoothingSamples(0);
    } else {
        mSmoothingDuration = seconds;

        setSmoothingSamples(static_cast<int>(std::floor(mSampleRate * (double) seconds)));
    }
}

void /* FUN_0045e848 */ Amp::setVolumeResponse(int responseIndex) {
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

void /* FUN_0045e890 */ Amp::updateSmoothing() {
    setSmoothing(mSmoothingDuration);
}

void /* FUN_0045e89c */ Amp::setSaturation(int saturationIndex) {
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

void /* FUN_0045e8ec */ Amp::process(float *channels, float sample) {
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

void /* FUN_0045e95c */ Amp::process(float *channels, float sample, float gain) {
    if (mSaturation.level > -1) {
        if (fabsf(sample) >= mSaturation.threshold) {
            sample = (sample > 0) ? 1 : -1;
        } else {
            sample = mSaturation.parameterB * sample + mSaturation.parameterA * sample * sample * sample;
        }
    }

    channels[0] += mVolume[0] * sample * gain;
    channels[1] += mVolume[1] * sample * gain;

    if (mSmoothingCounter >= 0) {
        mVolume[0] += mVolumeIncrement[0];
        mVolume[1] += mVolumeIncrement[1];
    }

    mSmoothingCounter--;
}
