#include <cmath>

#include "ModFoll.h"

/* FUN_0045eb28 */ ModFoll::ModFoll(float sampleRate) {
    setSampleRate(sampleRate);
    FUN_0045ebe8(0);
}

void /* FUN_0045eba0 */ ModFoll::setSampleRate(float sampleRate) {
    if (sampleRate < 1.0) {
        mSampleRate = 1.0;
    } else {
        mSampleRate = sampleRate;
    }

    mFieldc = 4096.0f / mSampleRate;

    // TODO find out where mField8 is initialized
    FUN_0045ebe8(mField8);
}

void /* FUN_0045ebe8 */ ModFoll::FUN_0045ebe8(float param_4) {
    int iVar1 = (int) roundf(param_4 * mFieldc);

    mField10 = iVar1;
    if (mField18 < 0) {
        mField18 = -iVar1;
    } else {
        mField18 = iVar1;
    }
}

void /* FUN_0045ec1c */ ModFoll::FUN_0045ec1c(int param_2) {
    if (param_2 >= 0x80000) {
        param_2 = 0x7ffff;
    } else if (param_2 < -0x7ffff) {
        param_2 = -0x7ffff;
    }

    param_2 = param_2 * 0x1000;

    int iVar1 = mField14;
    if (param_2 == iVar1) {
        mField18 = 0;
    } else if (param_2 > iVar1) {
        mField18 = mField10;
    } else if (param_2 < iVar1) {
        mField18 = -mField10;
    }

    mField1c = param_2;
}

void /* FUN_0045ec68 */ ModFoll::FUN_0045ec68(int param_2) {
    if (param_2 >= 0x80000) {
        param_2 = 0x7ffff;
    } else if (param_2 < -0x7ffff) {
        param_2 = -0x7ffff;
    }

    mField18 = 0;
    mField14 = param_2 * 4096;
}

int /* FUN_0045ec90 */ ModFoll::getOutput() {
    if (mField14 != mField1c) {
        int iVar1 = mField18;

        if (iVar1 < 0) {
            mField14 = mField14 + iVar1;
            if (mField14 <= mField1c) {
                mField14 = mField1c;
                mField18 = 0;
            }
        } else {
            mField14 = mField14 + iVar1;
            if (mField1c <= mField14) {
                mField14 = mField1c;
                mField18 = 0;
            }
        }
    }

    return mField14 / 4096;
}
