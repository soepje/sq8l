#include <cmath>

#include "SqLfo.h"
#include "Data.h"

uint8_t waveParams[70][4] {
    {0x00, 0x00, 0x00, 0x00},
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

int8_t randomWaveData[256] = {
    -41,  -50,  -65,  -50,
    -46,  -54,  -60,  -65,
    -70,  -65,  -57,  -50,
    -36,  -26,  -23,  -30,
    -39,  -37,  -32,  -20,
    -10,    0,   10,   23,
     56,   78,   86,   96,
     69,   46,   38,   35,
     29,   43,   52,   59,
     47,   35,   40,   20,
      8,   15,   32,   49,
     56,   43,   33,   22,
     39,   53,   62,   77,
     68,   73,   78,   80,
     76,   83,   65,   62,
     71,   67,   58,   45,
     43,   41,   32,   35,
     29,   20,   26,   20,
     12,   30,   41,   50,
     58,   48,   32,   40,
     30,   24,   16,    2,
     -9,  -14,   -8,    5,
     -1,  -17,  -25,  -31,
    -36,  -43,  -52,  -66,
    -77,  -89,  -95, -100,
    110, -106, -111, -120,
    110, -100,  -90,  -86,
    -72,  -61,  -51,  -49,
    -30,  -40,  -37,  -47,
    -48,  -33,  -28,  -39,
    -28,  -14,  -22,  -14,
     -2,  -11,  -19,   -6,
      0,  -11,  -18,  -20,
    -33,  -21,  -17,  -13,
     29,   37,   47,   56,
     45,   48,   56,   61,
     63,   57,   49,   53,
     62,   78,   69,   79,
     69,   59,   44,   54,
     62,   78,   61,   69,
     60,   58,   44,  -33,
    -22,  -27,  -28,   15,
      5,   15,   21,   23,
     38,   27,   32,   46,
     50,   44,   35,   21,
     23,   16,    8,   -5,
    -13,    1,   -3,   -5,
      7,   10,    4,   -6,
    -14,  -20,  -10,  -12,
    -23,  -38,  -47,  -38,
    -27,  -22,  -34,  -39,
    -34,  -25,  -15,  -24,
    -14,  -20,  -28,  -12,
     -5,   -8,    2,    7,
     12,   28,   35,   40,
     51,   61,   54,   44,
     31,   42,   38,   47,
     52,   49,   34,   26,
     18,    8,   -1,  -11,
    -20,  -32,  -46,  -52,
    -58,  -62,  -56,  -42,
    -53,  -45,  -56,  -67,
    -71,  -61,  -66,  -66,
    -52,  -49,  -49,  -45
};

int8_t humanizationData[256] = {
      1,  0,  0,  0,
      0, -1,  0,  0,
      0,  0,  0,  0,
      0,  1,  0,  0,
     -1,  0,  0,  0,
      0,  0,  0,  0,
      0,  0,  0,  1,
      0,  0, -1,  0,
      1,  0,  0,  0,
     -1,  0,  0,  0,
      0,  0,  0,  0,
      0,  0,  1,  0,
     -1,  0,  0,  0,
      0,  0,  1,  0,
      0,  0,  0, -1,
      0,  0,  0,  0,
      1,  0,  0,  0,
      0,  0,  0, -1,
      0,  0,  0,  0,
      0,  0,  0,  0,
      1,  0,  0,  0,
     -1,  0,  0,  0,
      0,  1,  0,  0,
      0,  0, -1,  0,
      1,  0,  0,  0,
      0,  0, -1,  0,
      0,  0,  0,  0,
      0,  1,  0,  0,
     -1,  0,  0,  1,
      0,  0,  0,  0,
      0, -1,  0,  0,
      0,  0,  0,  0,
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
      0, -1,  0,  0,
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

int SqLfo::getPhaseIncrement(int frequency) {
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
    mPhase = 0;
    mPhaseIncrement = 0;
    mFrequency = 0;
    mFrequencyHumanization = 0;
    mAmplitude = 0;
    mFinalAmplitude2 = 0;
    mFading = 0;
    mFadingCounter = 0;
    mDelayMode = 0;
    mReverse = false;
    mOneShot = false;
    mWave = 0;
    mHumanize = 0;
    mHumanizationIndex = 0;
    mHumanization = 0;
    mHumanizationFactor = 0;
    FIELD_88 = 0;
    mAmplitudeModulation = 0;
}

// LFO Delay Mode (EMU=like SQ80, SMTH=smoother fading)
void SqLfo::setDelayMode(bool smooth) {
    mAmplitude = mAmplitude >> mAmplitudeFactor;

    if (smooth) {
        mDelayMode = 1;
    } else {
        mDelayMode = 0;
    }

    char DAT_004c1e88[2] = { 1, 4 };
    char DAT_004c1e8c[2] = { 0, 3 };

    mAmplitudeFactor = DAT_004c1e88[mDelayMode];
    FIELD_91 = DAT_004c1e8c[mDelayMode];
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

void SqLfo::reset(int phase, bool resetPhase) {
    mRunning = -1;

    if (resetPhase) {
        mPhase = 0;
    }

    if (phase != 0) {
        mPhase += phase * 0x1000000;
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

    if (!mDefaultSampleRate && mDelayMode > 0) {
        mFading = (int) round(mFading * (double) mFadingFactor);
    }

    mFadingCounter = 0;
    FIELD_88 = 0;
    mAmplitudeModulation = 0;
    mFrequency = 0x80000000; // check this
    mFrequencyHumanization = 0;
    mReverse = mSettings.reverse;
    mOneShot = mSettings.oneShot;
    mWave = 0;
    FIELD_2c = 0;

    mHumanizationIndex = mHumanizationIndex % 256; // TODO not modulo

    if (mSettings.humanize < 2) {
        mHumanization = 0;
        mHumanizationFactor = 0;

        if (mHumanizationIndex > 0x7f) {
            mHumanizationIndex -= 0x80;
        }
    }

    mOutput = 0;
    mSmoothness = 0;
}

void SqLfo::updateSettings() {
    mAmplitudeModulation = mSettings.amplitudeModulation;

    if (mSettings.startAmplitude != mStartAmplitude || mSettings.finalAmplitude != mFinalAmplitude || mSettings.fadingSpeed != mFadingSpeed || mSettings.delayMode != mDelayMode) {
        setDelayMode(mSettings.delayMode);

        mFinalAmplitude2 = mSettings.finalAmplitude << mAmplitudeFactor;
        
        int iVar2 = mSettings.startAmplitude;
        if (mSettings.finalAmplitude == iVar2 || mSettings.fadingSpeed < 1) {
            mFading = 0;
            if (mSettings.fadingSpeed < 1) {
                mAmplitude = iVar2 << mAmplitudeFactor;
            }
        } else if (mSettings.finalAmplitude < iVar2) {
            if (mDelayMode < 1) {
                int local_c = mFadingCounter * mSettings.fadingSpeed;
                mAmplitude = (iVar2 << mAmplitudeFactor) - round(local_c * (8.0 / 65.0)); // 0.123076923076923083755
            } else {
                mAmplitude = (iVar2 << mAmplitudeFactor) - (mFadingCounter * mSettings.fadingSpeed);
            }

            if (mFinalAmplitude2 < mAmplitude) {
                if (!mDefaultSampleRate && mDelayMode > 0) {
                    mFading = -round(mSettings.fadingSpeed * mFadingFactor);
                } else {
                    mFading = -mSettings.fadingSpeed;
                }
            } else {
                mAmplitude = mFinalAmplitude2;
                mFading = 0;
            }
        } else {
            if (mDelayMode < 1) {
                int local_c = mFadingCounter * mSettings.fadingSpeed;
                mAmplitude = round(local_c * (8 / 65)) + (iVar2 << mAmplitudeFactor);
            } else {
                mAmplitude = (iVar2 << mAmplitudeFactor) + (mFadingCounter * mSettings.fadingSpeed);
            }

            if (mAmplitude < mFinalAmplitude2) {
                if (!mDefaultSampleRate && mDelayMode > 0) {
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
                    mWaveMask = (1 << local_10) - 1;
                    mWaveShift = 0x1e - local_10;
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
//                mWaveData = envShapingCurves[wave]; TODO
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
        mPhaseIncrement = getPhaseIncrement(mSettings.frequency);
    } else if (mHumanize < 2) {
        if (mSettings.frequency / 256 < 7) {
            mPhaseIncrement = getPhaseIncrement(mSettings.frequency);
        } else if ((mSettings.frequency + mFrequencyHumanization) / 256 < 7) {
            mPhaseIncrement = 0;
        } else {
            mPhaseIncrement = getPhaseIncrement(mSettings.frequency + mFrequencyHumanization);
        }
    } else {
        mPhaseIncrement = getPhaseIncrement(mSettings.frequency + mFrequencyHumanization);
    }

    if (FIELD_2c > 0) {
        if (mPhaseIncrement < 0) {
            mPhaseIncrement = -mPhaseIncrement >> FIELD_2c;
        } else {
            mPhaseIncrement = mPhaseIncrement >> FIELD_2c;
        }
    }

    mPhaseOffsets[0] = mSettings.phase1 * 16384;
    if (mSettings.twinMode) {
        mPhaseOffsets[1] = mSettings.phase2 * 16384;
        mTwinMode = 1;
    } else {
        mTwinMode = 0;
    }

    if (mReverse || mPhaseIncrement < 0) {
        mPhaseOffsets[0] = -mPhaseOffsets[0];
        mPhaseOffsets[1] = -mPhaseOffsets[1];
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

int SqLfo::updateState() {
    if (mRunning == 0) {
        return mOutput;
    }

    int8_t samples[2] = {0, 0};
    int8_t var14 = 0;

    for (int i = 0; i < mTwinMode + 1; i++) {
        int x = mPhaseOffsets[i] + mPhase;

        // tri, saw, square, noise, bip
        switch (mWave) {
            case 0:
                var14 = static_cast<int8_t>(x >> 22);
                if (var14 < 0) {
                    samples[i] = (~var14 - 0x40) * 2;
                } else {
                    samples[i] = (var14 - 0x40) * 2;
                }
                break;
            case 1:
                var14 = x >> 22;
                samples[i] = var14 - 0x80;
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
                samples[i] = randomWaveData[128 + var14];
                break;
            case 4:
                var14 = x >> 22;
                if (var14 < 0) {
                    samples[i] = -0x7f;
                } else {
                    samples[i] = 0x7f;
                }
                break;
            default:
                if (mWave < 75) {
                    uint8_t sample = mWaveData[(x >> mWaveShift) & mWaveMask];
                    if (sample == 0) {
                        samples[i] = 0;
                    } else {
                        samples[i] = sample - 128;
                    }
                } else {
                    samples[i] = mWaveData[(x >> 16) & 0xff];
                }
                break;
        }
    }

    if (mTwinMode > 0) {
        int32_t x = (int32_t) samples[0] - (int32_t) samples[1];
        if (x < -127) {
            samples[0] = -127;
            samples[1] = 0;
        } else if (x < 127) {
            samples[0] = (int8_t) x;
            samples[1] = 0;
        } else {
            samples[0] = 127;
            samples[1] = 0;
        }
    }

    if (mSmoothness > 0) {
        int sample = (mPreviousSample * mPreviousSampleMix + samples[0] * mCurrentSampleMix) * 256;
        sample /= 256;
        mPreviousSample = sample;
        sample /= 256;
        samples[0] = sample;
    }

    int output = 0;

    int amplitude = (mAmplitudeModulation << FIELD_91) + mAmplitude;
    if (amplitude < 1) {
        output = 0;
    } else if (mDelayMode > 0) {
        output = (samples[0] * amplitude) / 2048;
    } else {
        output = (samples[0] * amplitude) / 256;
    }

    mPhase += mPhaseIncrement;
    if (mOneShot && mPhase > 0x40000000) {
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

    if (mDelayMode > 0 || iVar6 != 0) {
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
        if (mFadingCounter < 0x7fffffff) {
            mFadingCounter += 1;
        }
    }

    if (mHumanize > 0 && iVar6 != 0) {
        if (mHumanize > 1 || mFading == 0) {
            int8_t humanization = humanizationData[128 + (int8_t) mHumanizationIndex];
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

        mHumanizationIndex += 1;
    }

    mOutput = output;

    return output;
}

int SqLfo::nextSample() {
    updateSettings();
 
    return updateState();
}
