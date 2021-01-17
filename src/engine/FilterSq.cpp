#include <cmath>

#include "FilterSq.h"

float table1[32];
float table2[32];
float table3[256];

void initFilterSQTable1();
void initFilterSQTable2();
void initFilterSQTable3();

/*  FUN_0045ef38 */ FilterSq::FilterSq(float *table4, float sampleRate) : FilterBase(sampleRate) {
    initFilterSQTable1();
    initFilterSQTable2();
    initFilterSQTable3();

    init();

    if (table4 == nullptr) {
        mTable4 = new float[16384];
        mTable4Allocated = true;
    } else {
        mTable4 = table4;
        mTable4Allocated = false;
    }

    field_0x78 = 0.017944177612662315;
    field_0x18 = 1.0;
    field_0x1c = -2;

    setField1c(-1);

    field_0x14 = 1.0;

    // here the constructor of FilterBase is called, but that is not how c++ works
    // so we call the functionality of that constructor here
    FilterSq::setSampleRate(sampleRate);
    FilterSq::setCutoffAndResonance(0, 0, true);
}

/* FUN_0045efc8 */ FilterSq::~FilterSq() {
    if (mTable4Allocated) {
        delete[] mTable4;
    }
}

void /* FUN_0045f000 */ FilterSq::init() {
    field_0x6c = 1.075;
    field_0x70 = 0.5;
    field_0x74 = 1.0 / 6.0;
}

void /* FUN_0045f018 */ FilterSq::copyState(FilterBase *otherBase) {
    FilterSq* other = dynamic_cast<FilterSq*>(otherBase);

    if (other == nullptr) {
        mBuf0 = 0;
        mBuf1 = 0;
        mBuf2 = 0;
        mBuf3 = 0;
        mBuf4 = 0;
        return;
    }

    field_0x18 = other->field_0x18;
    field_0x1c = other->field_0x1c;

    mBuf0 = other->mBuf0;
    mBuf1 = other->mBuf1;
    mBuf2 = other->mBuf2;
    mBuf3 = other->mBuf3;
    mBuf4 = other->mBuf4;

    field_0x3c = other->field_0x3c;
    field_0x40 = other->field_0x40;
    field_0x44 = other->field_0x44;

    field_0x60 = other->field_0x60;
    field_0x64 = other->field_0x64;

    field_0x48 = other->field_0x48;
    field_0x4c = other->field_0x4c;
    field_0x50 = other->field_0x50;
    field_0x54 = other->field_0x54;
    field_0x58 = other->field_0x58;
    field_0x5c = other->field_0x5c;

    field_0x68 = other->field_0x68;
    field_0x78 = other->field_0x78;

}

void /* FUN_0045f0f8 */ FilterSq::setSampleRate(float sampleRate) {
    FilterBase::setSampleRate(sampleRate);
    FUN_0045f120();
    calculateFeedbackAmount(true);
}

void /* FUN_0045f120 */ FilterSq::FUN_0045f120() {
    // TODO needs checking
    field_0x48 = field_0x50 = field_0x58 = pow(1.0 / (field_0x78 * 0.5 * mSampleRate + 1), 0.01);
    field_0x4c = field_0x54 = field_0x5c = 1 - field_0x48;
}

void /* FUN_0045f1ec */ FilterSq::setCutoff(int cutoff, bool reset) {
    if (cutoff < 1) {
        mCutoff = 0;
    } else if (cutoff > 0xff) {
        mCutoff = 0xff;
    } else {
        mCutoff = cutoff;
    }

    calculateFeedbackAmount(reset);
}

void /* FUN_0045f214 */ FilterSq::setResonance(int resonance, bool reset) {
    if (resonance < 1) {
        mResonance = 0;
    } else if (resonance > 0x1f) {
        mResonance = 0x1f;
    } else {
        mResonance = resonance;
    }

    calculateFeedbackAmount(reset);
}

void /* FUN_0045f238 */ FilterSq::setCutoffAndResonance(int cutoff, int resonance, bool reset) {
    if (cutoff < 1) {
        mCutoff = 0;
    } else if (cutoff > 0xff) {
        mCutoff = 0xff;
    } else {
        mCutoff = cutoff;
    }

    if (resonance < 1) {
        mResonance = 0;
    } else if (resonance > 0x1f) {
        mResonance = 0x1f;
    } else {
        mResonance = resonance;
    }

    calculateFeedbackAmount(reset);
}

void /* FUN_0045f284 */ FilterSq::setCutoffAndResonance2(int cutoff, int resonance, bool reset) {
    setCutoffAndResonance(cutoff, resonance, reset);
}

void /* FUN_0045f3d4 */ FilterSq::initTable4() {
    auto /* FUN_0045f29c */ FUN_0045f29c = [&](float x) {
        float a = x / mSampleRate;

        if (0.490 < a) {
            a = 0.490;
        }

        return (float) (a * (4.0 + M_PI));
    };

    auto /* FUN_0045f2f */ FUN_0045f2f = [](float x) {
        float a = tanf(x / 4);
        float b = a - 1;
        float c = (a * 2) / (b * b);

        return (c - 1) / (c + 1);
    };

    auto /* FUN_0045f358 */ FUN_0045f358 = [](float x, float y) {
        float a = exp(-0.90194209275175 * x) * 1.78879830303240001;
        float b = exp(3.777826974574 * x) * -0.015449152554;

        return (a + b) * y;
    };

    for (int j = 0; j < 256; j++) {
        float a = FUN_0045f2f(FUN_0045f29c(table3[j]));

        for (int i = 0; i < 32; i++) {
            mTable4[i * 512 + j * 2] = a;
            mTable4[i * 512 + j * 2 + 1] = FUN_0045f358(a, table1[i]);
        }
    }
}

void /* FUN_0045f4dc */ FilterSq::calculateFeedbackAmount(bool reset) {
    field_0x64 = mTable4[mResonance * 512 + mCutoff * 2];
    field_0x60 = mTable4[mResonance * 512 + mCutoff * 2 + 1];

    if (reset) {
        field_0x44 = field_0x64;
        field_0x40 = field_0x60;
        field_0x3c = (field_0x64 + 1.0f) * 0.5f;
    }

    field_0x64 *= field_0x4c;
    field_0x60 *= field_0x5c;
    field_0x68 = table2[mResonance];

    if (reset) {
        field_0x18 = field_0x68;
    }

    field_0x68 *= field_0x5c;
}

void /* FUN_0045f560 */ FilterSq::setField1c(int param_2) {
    if (param_2 < 0) {
        param_2 = -1;
    } else if (param_2 > 0x3e) {
        param_2 = 0x3f;
    }

    field_0x1c = param_2;
}

float /* FUN_0045f578 */ FilterSq::process(float sample) {
    float oldBuf0 = mBuf0;
    float oldBuf1 = mBuf1;
    float oldBuf2 = mBuf2;
    float oldBuf3 = mBuf3;

    mBuf0 = (field_0x40 * field_0x6c + 1.0f) * sample - mBuf4 * field_0x40;
    mBuf1 = mBuf0 * field_0x3c + (oldBuf0 * field_0x3c - mBuf1 * field_0x44);
    mBuf2 = mBuf1 * field_0x3c + (oldBuf1 * field_0x3c - mBuf2 * field_0x44);
    mBuf3 = mBuf2 * field_0x3c + (oldBuf2 * field_0x3c - mBuf3 * field_0x44);
    mBuf4 = mBuf3 * field_0x3c + (oldBuf3 * field_0x3c - mBuf4 * field_0x44);

    field_0x3c = (field_0x44 + 1.0f) * field_0x70;
    field_0x44 = field_0x44 * field_0x48 + field_0x64;
    field_0x40 = field_0x40 * field_0x50 + field_0x60;
    field_0x18 = field_0x18 * field_0x58 + field_0x68;

    return mBuf4;
}

void /* FUN_0045f61c */ initFilterSQTable3() {
    float a = -logf(42.394f / 309.662f) / 63.0f;
    for (int i = 0; i < 64; i++) {
        table3[i] = expf((float) i * a) * 42.394f * 0.9925f;
    }

    float b = -logf(309.662f / 2231.04f) / 63.0f;
    for (int i = 0; i < 64; i++) {
        table3[i + 64] = expf((float) i * b) * 309.662f * 0.9925f;
    }

    float c = -logf(2231.04f / 13638.1f) / 63.0f;
    for (int i = 0; i < 64; i++) {
        table3[i + 128] = expf((float) i * c) * 2231.04f * 0.9925f;
    }

    float d = -logf(13638.1f / 45767.9f) / 63.0f;
    for (int i = 0; i < 64; i++) {
        table3[i + 192] = expf((float) i * d) * 13638.1f * 0.9925f;
    }
}

void /* FUN_0045f79c */ initFilterSQTable1() {
    table1[0] = 0;

    for (int i = 1; i < 32; i++) {
        double x = i / 31.0;

        table1[i] = -0.572034995441400018201 * x * x + 1.641754288 * x - 0.0466502765;
    }
}

void /* FUN_0045f830 */ initFilterSQTable2() {
    for (int i = 0; i < 32; i++) {
        table2[i] = exp(sqrt(i / 31.0) * log(0.330)); // log(0.415)
    }
}
