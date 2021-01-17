#include <cmath>

#include "SqEnv.h"
#include "Data.h"
#include "MidiParser.h"

SqEnv::SqEnv(float sampleRate) {
    setSampleRate(sampleRate);
}

void SqEnv::init() {
    mStepFunction = &SqEnv::emptyFunction;
    mAdvanceStage = &SqEnv::startDecayStage;

    mRunning = false;
    field_61 = false; // is only set, never read ???
    field_62 = false;
    field_63 = false;
    mFinalStep = false;

    for (int i = 0; i < 5; i++) {
        mLevels[i] = 0;
    }
  
    for (int i = 0; i < 4; i++) {
        mTimes[i] = 0;
    }

    mLevelModulation = 0;
    mTimeScaling = 0;
    mSecondRelease = false;
    mLevel = 0;
    mTargetLevel = 0;
    mStepSize = 0;
    mCounter = 0;
    mSmoothness = 0;
    mPreviousLevel = 0;
    mOutput = 0;
}

void SqEnv::setSampleRate(float sampleRate) {
    mSampleRate = sampleRate;
    mStepsPerCentiSecond = sampleRate / 83.59257507324219;

    init();
}

void SqEnv::setOutputSmoothness(int smoothness) {
    if (smoothness < 0) {
        this->mSmoothness = 0;
        return;
    }

    if (smoothness > 255) {
        smoothness = 255;
    }

    int bias = 255 - (((255 - smoothness) * (255 - smoothness)) / 256);

    this->mSmoothness = bias;
    this->mBias = bias;
    this->mInverseBias = 256 - bias;
}

void SqEnv::triggerAttack(Env_settings* settings, int pitch, SqEnv* other, bool cycle, bool clearLevels, int velocity) {
    velocity = limitVelocity(velocity);

    mCycle = cycle;

    for (int i = 0; i < 5; i++) {
        int level = settings->levels[i];

        if (level < 0) {
            mLevels[i] = (level - 1) * 2;
        } else {
            mLevels[i] = level * 2;
        }
    }

    for (int i = 0; i < 4; i++) {
        mTimes[i] = settings->times[i] & 0x3f;
    }

    int iVar4 = (pitch - 36) * 2;
    if (iVar4 < 0) {
        iVar4 = 0;
    }
    
    mTimeScaling = (settings->timeScaling * iVar4) / 256;

    mSecondRelease = settings->times[3] > 63;

    char bVar2;
    char cVar1 = settings->levelModulation;
    if (cVar1 < 64) {
        bVar2 = envLinearVelocityCurve[velocity];
    } else {
        bVar2 = envExponentialVelocityCurve[velocity];
    }

    mLevelModulation = (cVar1 * bVar2) / 32 + (63 - cVar1) * 4;
    mShapingCurveIndex = ((settings->options >> 1) & 7) - 1;
    
    if (mShapingCurveIndex < 0 || mShapingCurveIndex > 7) {
        mShapingCurve = nullptr;
    } else {
        mShapingCurve = envShapingCurves[mShapingCurveIndex];
    }

    int uVar5;
    if (settings->options & 0x10 == 0) {
        uVar5 = (settings->attackTimeModulation * velocity) / 128;
        setOutputSmoothness(settings->outputSmoothness * 4);
    } else {
        uVar5 = 0;
        setOutputSmoothness(settings->outputSmoothness * 4 - ((settings->attackTimeModulation * velocity) / 32));
    }

    mStepFunction = &SqEnv::emptyFunction;
    mAdvanceStage = &SqEnv::startDecayStage;

    mPreviousLevel = 0;
    mOutput = 0;

    if (clearLevels) {
        mLevel = mLevels[0] / 256;
        mPreviousLevel = mLevels[0] / 256;
        mOutput = mLevels[0];
    } else {
        if (other != nullptr) {
            mLevel = other->mLevel;
            mPreviousLevel = other->mPreviousLevel;
            mOutput = other->mOutput;
        }
    }

    mRunning = true;
    field_61 = false;
    field_62 = false;
    field_63 = false;
    mFinalStep = false;

    setTargetLevelAndTime((mLevels[1] * mLevelModulation) / 256, mTimes[0] - uVar5);
}

void SqEnv::setTargetLevelAndTime(int level, int time) {
    mTargetLevel = level;

    if (time < 0) {
        time = 0;
    } else if (time > 63) {
        time = 63;    
    }

    int steps = round(envEnvelopeTimes[time] * mStepsPerCentiSecond);
    if (steps < 1) {
        mCounter = 1;
    } else {
        mCounter = steps;
    }

    // compute difference between current level and target level
    int difference = level - (mLevel / 256);
    
    // if we need to go down up, or stay the same
    if (difference == 0) {
        mStepFunction = &SqEnv::stepLevel;
    } else if (difference < 0) {
        mStepFunction = &SqEnv::stepDown;
        difference = -difference;
    } else {
        mStepFunction = &SqEnv::stepUp;
    }

    mStepSize = (difference * 256) / mCounter;
}

void SqEnv::triggerRelease(bool sustain) {
    if(!mCycle) {
        if (sustain) {
            field_63 = true;
        } else {
            field_62 = true;
            field_63 = false;
            selectReleaseStage();
        }
    }
}

void SqEnv::stepUp() {
    mLevel += mStepSize;

    if (mTargetLevel < (mLevel / 256)) {
        mLevel = mTargetLevel * 256 + ((mLevel / 256) & 0xff);
    }

    mCounter -= 1;

    if (mCounter < 1) {
        mLevel = mTargetLevel * 256 + (mLevel & 0xff);

        if (mAdvanceStage != nullptr) {
            (this->*mAdvanceStage)();
        }
    }
}

void SqEnv::stepDown() {
    mLevel -= mStepSize;

    if ((mLevel / 256) < mTargetLevel) {
        mLevel = mTargetLevel * 256 + ((mLevel / 256) & 0xff);
    }

    mCounter -= 1;

    if (mCounter < 1) {
        mLevel = mTargetLevel * 256 + (mLevel & 0xff);

        if (mAdvanceStage != nullptr) {
            (this->*mAdvanceStage)();
        }
    }
}

void SqEnv::stepLevel() {
    mCounter -= 1;

    if (mCounter < 1) {
        mLevel = mTargetLevel * 256 + (mLevel & 0xff);

        if (mAdvanceStage != nullptr) {
            (this->*mAdvanceStage)();
        }
    }
}

void SqEnv::emptyFunction() {
    
}

void SqEnv::startDecayStage() {
    mAdvanceStage = &SqEnv::startSustainStage;

    int time = mTimes[1] - mTimeScaling;
    if (time < 0) {
        time = 0;
    }

    setTargetLevelAndTime((mLevels[2] * mLevelModulation) / 256, time);
}

void SqEnv::startSustainStage() {
    mAdvanceStage = &SqEnv::startReleaseStage;

    int time = mTimes[2] - mTimeScaling;
    if (time < 0) {
        time = 0;
    }

    setTargetLevelAndTime((mLevels[3] * mLevelModulation) / 256, time);
}

void SqEnv::startReleaseStage() {
    if (mLevel == 0) {
        stop();
        return;
    }

    // TODO verify this check
    if (!field_62 && !mCycle) {
        field_61 = true;
        mStepFunction = &SqEnv::emptyFunction;
    } else {
        selectReleaseStage();
    }
}

void SqEnv::startSecondReleaseStage() {
    field_61 = false;
    mAdvanceStage = &SqEnv::stop;

    setTargetLevelAndTime(0, 39);
}

void SqEnv::stop() {
    mStepFunction = &SqEnv::emptyFunction;
    mFinalStep = true;
    field_61 = false;
}

void SqEnv::selectReleaseStage() {
    if (this->mSecondRelease) {
        mAdvanceStage = &SqEnv::startSecondReleaseStage;

        setTargetLevelAndTime((this->mLevel / 512) - 12, mTimes[3]);
    } else {
        mAdvanceStage = &SqEnv::stop;

        setTargetLevelAndTime(0, mTimes[3]);
    }
}

int SqEnv::getOutput() {
    if (mShapingCurve == nullptr) {
        return mOutput;
    }

    int output = mShapingCurve[(((mOutput * 256) / mLevelModulation) + 128) & 255] * mLevelModulation;

    return output / 256;
}

int SqEnv::getOutputAndStep(bool sustain) {
    if (!mRunning) {
        return 0;
    }

    if (!sustain && field_63) {
        triggerRelease(false);
    }

    if (mStepFunction != nullptr) {
        (this->*mStepFunction)();
    }

    int output;
    if (mSmoothness > 0) {
        mPreviousLevel = ((mPreviousLevel * mBias) + (mLevel * mInverseBias)) / 256;

        if (mFinalStep && mPreviousLevel / 256 == 0) {
            mRunning = false;
            mFinalStep = false;
            output = 0;           
        } else {
            output = mPreviousLevel / 256;
        }
    } else {
        if (mFinalStep) {
            mRunning = false;
            mFinalStep = false;
            output = 0;
        } else {
            output = mLevel / 256;
        }
    }

    mOutput = output;

    if (mShapingCurve != nullptr) {
        return getOutput();
    }

    return output;
}
