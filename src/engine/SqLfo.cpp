#include <cmath>

#include "SqLfo.h"
#include "Data.h"

unsigned char waveParams[70][4] {
        {0, 0, 0,0 },
        {0x01, 0x00, 0x03, 0x00},
        {0x02, 0x00, 0x00, 0x00},
        {0x03, 0x00, 0x00, 0x00},
        {0x04, 0x38, 0x04, 0x00},
        {0x05, 0x00, 0x04, 0x00},
        {0x06, 0x00, 0x04, 0x00},
        {0x07, 0x00, 0x04, 0x00},
        {0x08, 0x00, 0x03, 0x00},
        {0x09, 0x40, 0x03, 0x00},
        {0x0a, 0x00, 0x02, 0x00},
        {0x0b, 0x00, 0x02, 0x00},
        {0x0c, 0x00, 0x01, 0x00},
        {0x0d, 0x00, 0x03, 0x00},
        {0x0e, 0x48, 0x03, 0x00},
        {0x0f, 0x00, 0x03, 0x00},
        {0x10, 0x00, 0x04, 0x00},
        {0x11, 0x00, 0x04, 0x00},
        {0x12, 0x00, 0x04, 0x00},
        {0x13, 0x00, 0x04, 0x00},
        {0x14, 0x48, 0x04, 0x00},
        {0x15, 0x00, 0x03, 0x00},
        {0x16, 0x00, 0x02, 0x00},
        {0x17, 0x00, 0x01, 0x00},
        {0x18, 0x00, 0x02, 0x00},
        {0x19, 0x00, 0x00, 0x00},
        {0x1a, 0x00, 0x02, 0x00},
        {0x1b, 0x48, 0x02, 0x00},
        {0x1c, 0x00, 0x02, 0x00},
        {0x1d, 0x00, 0x00, 0x00},
        {0x1e, 0x00, 0x01, 0x00},
        {0x1f, 0x00, 0x01, 0x00},
        {0x20, 0x00, 0x00, 0x00},
        {0x21, 0x00, 0x00, 0x00},
        {0x22, 0x00, 0x00, 0x00},
        {0x23, 0x00, 0x00, 0x00},
        {0x24, 0x00, 0x02, 0x00},
        {0x25, 0x48, 0x02, 0x00},
        {0x26, 0x58, 0x02, 0x00},
        {0x27, 0x00, 0x00, 0x00},
        {0x28, 0x00, 0x00, 0x00},
        {0x29, 0x00, 0x00, 0x00},
        {0x2a, 0x50, 0x02, 0x00},
        {0x2b, 0x00, 0x01, 0x00},
        {0x2c, 0x00, 0x01, 0x00},
        {0x2d, 0x00, 0x03, 0x00},
        {0x2e, 0x00, 0x03, 0x00},
        {0x2f, 0x00, 0x03, 0x00},
        {0x30, 0x00, 0x04, 0x00},
        {0x31, 0x00, 0x05, 0x00},
        {0x32, 0x00, 0x06, 0x00},
        {0x33, 0x00, 0x07, 0x00},
        {0x34, 0x00, 0x07, 0x00},
        {0x35, 0x00, 0x06, 0x00},
        {0x36, 0x00, 0x05, 0x00},
        {0x37, 0x00, 0x05, 0x00},
        {0x38, 0x00, 0x05, 0x00},
        {0x39, 0x00, 0x05, 0x00},
        {0x3a, 0x00, 0x05, 0x00},
        {0x3b, 0x00, 0x05, 0x00},
        {0x3c, 0x00, 0x05, 0x00},
        {0x3d, 0x00, 0x05, 0x00},
        {0x3e, 0x00, 0x05, 0x00},
        {0x3f, 0x00, 0x05, 0x00},
        {0x40, 0x00, 0x05, 0x00},
        {0x41, 0x00, 0x05, 0x00},
        {0x42, 0x00, 0x05, 0x00},
        {0x43, 0x00, 0x05, 0x00},
        {0x44, 0x00, 0x05, 0x00},
        {0x45, 0x00, 0x05, 0x00}
};


char humanizationData[128] = {
        1,  0,  0,  0,
        0,  0, -1,  0,
        0,  0,  0,  1,
        0,  0,  0,  0,
        -1,  0,  0,  0,
        0,  0,  0,  0,
        0,  1,  0,  0,
        0,  0,  0,  0,
        1,  0,  0,  0,
        0, -1,  0,  0,
        0,  0,  0, -1,
        0,  0,  0,  0,
        1,  0,  0,  0,
        0,  0,  0,  0,
        -1,  0,  0,  1,
        0,  0,  0, -1,
        0,  0,  0,  1,
        0,  0,  0,  0,
        0,  0, -1,  0,
        0,  0,  0,  0,
        1,  0,  0,  0,
        0,  0,  0,  0,
        0,  0, -1,  0,
        0,  0,  0,  0,
        1,  0,  0,  0,
        0,  0,  0,  0,
        0,  0, -1,  0,
        0,  0,  0,  0,
        1,  0,  0,  0,
        -1,  0,  0,  0,
        1,  0,  0,  0,
        0, -1,  0,  0
};


unsigned char bipData[256] = {
        0x0, 0xF5, 0xEE, 0xEC,
        0xDF, 0xEB, 0xEF, 0xF3,
        0x1D, 0x25, 0x2F, 0x38,
        0x2D, 0x30, 0x38, 0x3D,
        0x3F, 0x39, 0x31, 0x35,
        0x3E, 0x4E, 0x45, 0x4F,
        0x45, 0x3B, 0x2C, 0x36,
        0x3E, 0x4E, 0x3D, 0x45,
        0x3C, 0x3A, 0x2C, 0xDF,
        0xEA, 0xE5, 0xE4,  0xF,
        0x5,  0xF, 0x15, 0x17,
        0x26, 0x1B, 0x20, 0x2E,
        0x32, 0x2C, 0x23, 0x15,
        0x17, 0x10,  0x8, 0xFB,
        0xF3,  0x1, 0xFD, 0xFB,
        0x7,  0xA,  0x4, 0xFA,
        0xF2, 0xEC, 0xF6, 0xF4,
        0xE9, 0xDA, 0xD1, 0xDA,
        0xE5, 0xEA, 0xDE, 0xD9,
        0xDE, 0xE7, 0xF1, 0xE8,
        0xF2, 0xEC, 0xE4, 0xF4,
        0xFB, 0xF8,  0x2,  0x7,
        0xC, 0x1C, 0x23, 0x28,
        0x33, 0x3D, 0x36, 0x2C,
        0x1F, 0x2A, 0x26, 0x2F,
        0x34, 0x31, 0x22, 0x1A,
        0x12,  0x8, 0xFF, 0xF5,
        0xEC, 0xE0, 0xD2, 0xCC,
        0xC6, 0xC2, 0xC8, 0xD6,
        0xCB, 0xD3, 0xC8, 0xBD,
        0xB9, 0xC3, 0xBE, 0xBE,
        0xCC, 0xCF, 0xCF, 0xD3,
        0x1,  0x0,  0x0,  0x0,
        0x0, 0xFF,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x0,
        0x0,  0x1,  0x0,  0x0,
        0xFF,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x1,
        0x0,  0x0, 0xFF,  0x0,
        0x1,  0x0,  0x0,  0x0,
        0xFF,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x1,  0x0,
        0xFF,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x1,  0x0,
        0x0,  0x0,  0x0, 0xFF,
        0x0,  0x0,  0x0,  0x0,
        0x1,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x0, 0xFF,
        0x0,  0x0,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x0,
        0x1,  0x0,  0x0,  0x0,
        0xFF,  0x0,  0x0,  0x0,
        0x0,  0x1,  0x0,  0x0,
        0x0,  0x0, 0xFF,  0x0,
        0x1,  0x0,  0x0,  0x0,
        0x0,  0x0, 0xFF,  0x0,
        0x0,  0x0,  0x0,  0x0,
        0x0,  0x1,  0x0,  0x0,
        0xFF,  0x0,  0x0,  0x1,
        0x0,  0x0,  0x0,  0x0,
        0x0, 0xFF,  0x0,  0x0,
        0x0,  0x0,  0x0,  0x0


};

SqLfo::SqLfo(float sampleRate) {
    FIELD_8c = 7296;
    mSampleRate = sampleRate;
    mDefaultSampleRate = true;

    init();
}

void SqLfo::setSampleRate(float sampleRate) {
    if (sampleRate == 83.59257507324219) {
        mDefaultSampleRate = true;
        mSampleRate = 83.59257507324219;
        mFadingFactor = 1.0;
        mFrequencyFactor = 1.0;
        FIELD_9c = 1.0 / 7.0;
        FIELD_8c = 7296;
    } else {
        mDefaultSampleRate = false;

        if (sampleRate < 0.01) {
            sampleRate = 0.01;
        }

        mSampleRate = sampleRate;
        mFadingFactor = 83.59257507324219 / sampleRate;
        mFrequencyFactor = mFadingFactor;
        FIELD_9c = mFrequencyFactor / 7.0;
        FIELD_8c = (int) round(mFadingFactor * 7296);
    }
  
    init();
}

int SqLfo::getCounterIncrement(int frequency) {
    bool negativeFrequency = frequency < 0;
    if (negativeFrequency) {
        frequency = -frequency;
    }

    int counterIncrement;
    if (frequency < 0x700) {
        counterIncrement = (int) round(frequency * 16384 * (double) FIELD_9c);
    } else if (frequency < 0x8600) {
        counterIncrement = (int) round((frequency - 0x600) * 16384 * (double) mFrequencyFactor);
    } else {
        counterIncrement = (int) round((double) mFrequencyFactor * 16384 * 32768);
    }

    if (mReverse != negativeFrequency) {
        counterIncrement = -counterIncrement;
    }

    return counterIncrement;
}

void SqLfo::init() {
    mOutput = 0;
    mRunning = 0;
    mCounter = 0;
    mCounterIncrement = 0;
    mFrequency = 0;
    mFrequencyHumanization = 0;
    mAmplitude = 0;
    mFinalAmplitude2 = 0;
    mFading = 0;
    FIELD_60 = 0;
    mSmootherFading = 0;
    mReverse = false;
    mOneShot = false;
    mWave = 0;
    mHumanize = 0;
    FIELD_7c = 0;
    mHumanization = 0;
    mHumanizationFactor = 0;
    FIELD_88 = 0;
    mAmplitudeModulation = 0;
}

// LFO Delay Mode (EMU=like SQ80, SMTH=smoother fading)
void SqLfo::setDelayMode(bool smootherFading) {
    mAmplitude = mAmplitude >> mAmplitudeFactor;

    if (smootherFading) {
        mSmootherFading = 1;
    } else {
        mSmootherFading = 0;
    }

    char DAT_004c1e88[2] = { 1, 4 };
    char DAT_004c1e8c[2] = { 0, 3 };

    mAmplitudeFactor = DAT_004c1e88[mSmootherFading];
    FIELD_91 = DAT_004c1e8c[mSmootherFading];
    mAmplitude = mAmplitude << mAmplitudeFactor;
}

// Smoothness of LFO output (0=neutral)
void SqLfo::setOutputSmoothness(int smoothness) {
    if (smoothness < 1) {
        mSmoothness = 0;
        return;
    }

    mSmoothness = smoothness;

    if (smoothness > 255) {
        smoothness = 255;
    }

    int mix = 255 - (((255 - smoothness) * (255 - smoothness)) / 256);

    mPreviousSampleMix = mix;
    mCurrentSampleMix = 256 - mix;
}

void SqLfo::FUN_0045d2a4(int x, bool resetCounter) {
    mRunning = -1;

    if (resetCounter) {
        mCounter = 0;
    }

    if (x != 0) {
        mCounter += x * 16777216; // what is this effectively?
    }

    setDelayMode(mSettings.delayMode);

    mStartAmplitude = mSettings.startAmplitude;
    mFinalAmplitude = mSettings.finalAmplitude;
    mFadingSpeed = mSettings.fadingSpeed;

    mAmplitude = mSettings.startAmplitude << mAmplitudeFactor;
    mFinalAmplitude2 = mSettings.finalAmplitude << mAmplitudeFactor;

    int fadingSpeed = mSettings.fadingSpeed;
    if (fadingSpeed < 1 || mSettings.startAmplitude == mSettings.finalAmplitude) {
        mFading = 0;
    } else if (mSettings.startAmplitude < mSettings.finalAmplitude) {
        mFading = fadingSpeed;
    } else {
        mFading = -fadingSpeed;
    }

    if (!mDefaultSampleRate && mSmootherFading > 0) {
        mFading = (int) round(mFading * (double) mFadingFactor);
    }

    FIELD_60 = 0;
    FIELD_88 = 0;
    mAmplitudeModulation = 0;
    mFrequency = 0x80000000; // check this
    mFrequencyHumanization = 0;
    mReverse = mSettings.reverse;
    mOneShot = mSettings.oneShot;
    mWave = 0;
    FIELD_2c = 0;

    FIELD_7c = FIELD_7c % 256;

    if (mSettings.humanize < 2) {
        mHumanization = 0;
        mHumanizationFactor = 0;

        if (FIELD_7c > 0x7f) {
            FIELD_7c -= 0x80;
        }
    }

    mOutput = 0;
    mSmoothness = 0;
}

void SqLfo::updateSettings() {
    mAmplitudeModulation = mSettings.amplitudeModulation;

    if (mSettings.startAmplitude != mStartAmplitude || mSettings.finalAmplitude != mFinalAmplitude || mSettings.fadingSpeed != mFadingSpeed || mSettings.delayMode != mSmootherFading) {
        setDelayMode(mSettings.delayMode);

        mFinalAmplitude2 = mSettings.finalAmplitude << mAmplitudeFactor;
        
        int iVar2 = mSettings.startAmplitude;
        if (mSettings.finalAmplitude == iVar2 || mSettings.fadingSpeed < 1) {
            mFading = 0;
            if (mSettings.fadingSpeed < 1) {
                mAmplitude = iVar2 << mAmplitudeFactor;
            }
        } else if (mSettings.finalAmplitude < iVar2) {
            if (mSmootherFading < 1) {
                int local_c = FIELD_60 * mSettings.fadingSpeed;
                mAmplitude = (iVar2 << mAmplitudeFactor) - round(local_c * (8.0 / 65.0)); // 0.123076923076923083755
            } else {
                mAmplitude = (iVar2 << mAmplitudeFactor) - (FIELD_60 * mSettings.fadingSpeed);
            }

            if (mFinalAmplitude2 < mAmplitude) {
                if (!mDefaultSampleRate && mSmootherFading > 0) {
                    mFading = -round(mSettings.fadingSpeed * mFadingFactor);
                } else {
                    mFading = -mSettings.fadingSpeed;
                }
            } else {
                mAmplitude = mFinalAmplitude2;
                mFading = 0;
            }
        } else {
            if (mSmootherFading < 1) {
                int local_c = FIELD_60 * mSettings.fadingSpeed;
                mAmplitude = round(local_c * (8 / 65)) + (iVar2 << mAmplitudeFactor);
            } else {
                mAmplitude = (iVar2 << mAmplitudeFactor) + (FIELD_60 * mSettings.fadingSpeed);
            }

            if (mAmplitude < mFinalAmplitude2) {
                if (!mDefaultSampleRate && mSmootherFading > 0) {
                    mFading = round(mSettings.fadingSpeed * mFadingFactor);
                } else {
                    mFading = mSettings.fadingSpeed;
                }
            } else {
                mAmplitude = mFinalAmplitude2;
                mFading = 0;
            }
        }

        mStartAmplitude = mSettings.startAmplitude;
        mFinalAmplitude = mSettings.finalAmplitude;
        mFadingSpeed = mSettings.fadingSpeed;
    }

    int wave = mSettings.wave;
    if (wave != mWave) {
        mWave = wave;

        if (mSettings.wave < 5) {
            FIELD_2c = 0;
        } else if (wave < 0x4b) {

            if (mSettings.loadWave != nullptr) {
                int wave = mSettings.wave - 5;

                int iVar1; // TODO
                int local_10; // TODO

                if (iVar1 < 0 || iVar1 > 0x45) {
                    mWaveData = 0;
                } else {
//                    FIELD_a8(waveParams[wave][1] + FIELD_e8, waveParams[wave][0], &local_10, &mWaveData); // TODO
                }

                if (mWaveData) {
                    FIELD_70 = (1 << local_10) - 1;
                    FIELD_74 = 0x1e - local_10;
                    FIELD_2c = waveParams[wave][2] + mSettings.FIELD_ec;
                } else {
                    mWave = 0;
                    FIELD_2c = 0;
                }
            } else {
                mWave = 0;
            }
        } else {
          FIELD_2c = 0;
          
            wave = mSettings.wave - 0x4b;
            if (wave < 8) {
                mWaveData = envShapingCurves[wave];
            } else {
                mWave = 0;
            }
        }
    }

    mFrequency = mSettings.frequency;
    mReverse = mSettings.reverse;

    if (mSettings.oneShot != mOneShot) {
        mOneShot = mSettings.oneShot;
        mRunning = -1;
    }

    if (mHumanize < 1) {
        mCounterIncrement = getCounterIncrement(mSettings.frequency);
    } else if (mHumanize < 2) {
        if (mSettings.frequency / 256 < 7) {
            mCounterIncrement = getCounterIncrement(mSettings.frequency);
        } else if ((mSettings.frequency + mFrequencyHumanization) / 256 < 7) {
            mCounterIncrement = 0;
        } else {
            mCounterIncrement = getCounterIncrement(mSettings.frequency + mFrequencyHumanization);
        }
    } else {
        mCounterIncrement = getCounterIncrement(mSettings.frequency + mFrequencyHumanization);
    }

    if (FIELD_2c > 0) {
        if (mCounterIncrement < 0) {
            mCounterIncrement = -mCounterIncrement >> FIELD_2c;
        } else {
            mCounterIncrement = mCounterIncrement >> FIELD_2c;
        }
    }

    mWaveOffsets[0] = mSettings.phase1 * 16384;
    if (mSettings.twinMode) {
        mWaveOffsets[1] = mSettings.phase2 * 16384;
        mTwinMode = 1;
    } else {
        mTwinMode = 0;
    }

    if (mReverse || mCounterIncrement < 0) {
        mWaveOffsets[0] = -mWaveOffsets[0];
        mWaveOffsets[1] = -mWaveOffsets[1];
    }

    if (mSettings.humanize != mHumanize) {
        mHumanize = mSettings.humanize;
        mFrequencyHumanization = 0;
        mHumanization = 0;
        mHumanizationFactor = 0;
    }

    if (mSettings.smoothness != mSmoothness) {
        setOutputSmoothness(mSettings.smoothness);
    }
}

int SqLfo::calculateSample() {
    char var14;
    char samples[2] = {0, 0};

    if (mRunning == 0) {
        return mOutput;
    }

    for (int i = 0; i < mTwinMode + 1; i++) {
        int x = mWaveOffsets[i] + mCounter;

        // tri, saw, square, noise, bip
        switch (mWave) {
            case 0:
                var14 = x >> 22;
                if (var14 < 0) {
                    samples[i] = (~var14 - 0x40) * 2;
                } else {
                    samples[i] = (var14 + 0x40) * 2;
                }
                break;
            case 1:
                samples[i] = (x >> 22) + 0x80;
                break;
            case 2:
                var14 = x >> 22;
                if (var14 < 0) {
                    samples[i] = 0;
                } else {
                    samples[i] = 0x7f;
                }
                break;
            case 3:
                var14 = x >> 22;
//                    buf[i] = byte_4c1f10[var14];
                break;
            case 4:
                var14 = x >> 22;
                if (var14 < 0) {
                    samples[i] = 0x81;
                } else {
                    samples[i] = 0x7f;
                }
                break;
            default:

                break;
        }
    }

    int x = (int) samples[0] + (int) samples[1];
    if (x < -0x7e) {
        samples[0] = -0x7f;
    } else if (x < 0x7f) {
        samples[0] = (char) x;
    } else {
        samples[0] = 0x7f;
    }

    if (mSmoothness > 0) {
        int sample = (mPreviousSample * mPreviousSampleMix + samples[0] * mCurrentSampleMix) * 256;
        samples[0] = sample / 65536;
        mPreviousSample = sample / 256;
        // TODO
    }

    int output = (mAmplitudeModulation << FIELD_91) + mAmplitude;
    if (output < 1) {
        output = 0;
    } else if (mSmootherFading > 0) {
        output = (samples[0] * output) / 2048;
    } else {
        output = (samples[0] * output) / 256;
    }

    mCounter += mCounterIncrement;
    if (mOneShot && mCounter > 0x40000000) {
        mRunning = 0;
    }

    int iVar6;
    if (FIELD_88 < 1) {
        FIELD_88 += FIELD_8c;
        iVar6 = -1;
    } else {
        FIELD_88 -= 0x400;
        iVar6 = 0;
    }

    if (mSmootherFading > 0 || iVar6 != 0) {
        int fading = mFading;
        if (fading != 0) {
            if (fading < 1) {
                mAmplitude += fading;
                if (mAmplitude < mFinalAmplitude2) {
                    mAmplitude = mFinalAmplitude2;
                    mFading = 0;
                }
            } else {
                mAmplitude += fading;
                if (mFinalAmplitude2 < mAmplitude) {
                    mAmplitude = mFinalAmplitude2;
                    mFading = 0;
                }
            }
        }
        if (FIELD_60 < 0x7fffffff) {
            FIELD_60 += 1;
        }
    }

    if (mHumanize > 0 && iVar6 != 0) {
        if (mHumanize > 1 || mFading == 0) {
            char humanization = humanizationData[FIELD_7c];
            if (humanization != 0) {
                if (mHumanizationFactor > 0) {
                    mFrequencyHumanization += mHumanization * mHumanizationFactor;
                }
                mHumanization = humanization * 256;
                if (mHumanize < 3) {
                    mHumanizationFactor = 1;
                } else {
                    mHumanizationFactor = 1 << (mHumanize - 2);
                }
            }

            if (mHumanizationFactor > 0) {
                mFrequencyHumanization += mHumanization;
                mHumanizationFactor -= 1;
            }
        }

        FIELD_7c += 1;
    }

    mOutput = output;

    return output;
}

int SqLfo::nextSample() {
    updateSettings();
 
    return calculateSample();
}
