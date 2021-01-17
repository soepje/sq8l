#include "FilterBase.h"

/* FUN_0045ed88 */ FilterBase::FilterBase(float sampleRate) {
    this->FilterBase::setField8(0);
    this->FilterBase::setSampleRate(sampleRate);
    this->FilterBase::setCutoffAndResonance(0, 0, true);
    this->FilterBase::copyState(nullptr);
}

void /* FUN_0045ee18 */ FilterBase::setSampleRate(float sampleRate) {
    mSampleRate = sampleRate;
}

void /* FUN_0045ee28 */ FilterBase::copyState(FilterBase *other) {

}

void /* FUN_0045ee2c */ FilterBase::setField8(int field8) {
    mField8 = field8;
}

void /* FUN_0045ee38 */ FilterBase::setCutoffAndResonance(int cutoff, int resonance, bool reset) {
    setCutoff(cutoff, reset);
    setResonance(resonance, reset);
}

void /* LAB_0045ee30 */ FilterBase::setCutoff(int cutoff, bool reset) {
    mCutoff = cutoff;
}

void /* LAB_0045ee34 */ FilterBase::setResonance(int resonance, bool reset) {
    mResonance = resonance;
}

void /* FUN_0045ee70 */ FilterBase::setCutoffAndResonance2(int cutoff, int resonance, bool reset) {
    setCutoffAndResonance(cutoff, resonance, reset);
}
