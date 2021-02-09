#include <cmath>
#include <cstring>
#include <algorithm>
#include <functional>

#include "Synth.h"
#include "MidiParser.h"
#include "Data.h"

unsigned short /* 0x4c3158 */ glideDurations[64] = {
        0x1,   0x1,   0x1,   0x2,
        0x2,   0x3,   0x3,   0x3,
        0x4,   0x4,   0x4,   0x5,
        0x6,   0x6,   0x7,   0x8,
        0x9,   0xA,   0xB,   0xD,
        0xE,   0x10,   0x12,   0x14,
        0x17,   0x19,   0x1D,   0x20,
        0x24,   0x28,   0x2D,   0x33,
        0x39,   0x40,   0x48,   0x51,
        0x5B,   0x66,   0x72,   0x80,
        0x90,   0xA1,   0xB5,   0xCB,
        0xE4,   0x100,   0x11F,   0x143,
        0x16A,   0x196,   0x1C8,   0x200,
        0x23F,   0x285,   0x2D4,   0x32D,
        0x390,   0x400,   0x47D,   0x50A,
        0x5A8,   0x659,   0x721,   0x800
};

float /* DAT_004c2530 */ smoothingDurations[2][4] = {
        { 0.0025, 0.004, 0.005, 0.01 },
        { 0.0006, 0.0014, 0.004, 0.01 }
};

/* FUN_00461cac */ Synth::Synth(int sampleRate) {
//    FIELD_fd0 = param_6; FUN_00487c28, 
//    FIELD_fd4 = param_7; // reference to a CSynthEditor
//    FIELD_fd8 = param_4; FUN_00487bb8
//    FIELD_fdc = param_5; // rerefence to a CSynthEditor

//    FUN_00462044();

//    FIELD_ff8 = new EditBuf{};
//    FIELD_ff8->FUN_004610f4();

//    FIELD_ffc = new MidiParser{};

// callbacks for MidiParser
// FUN_00463380 reset ?
// FUN_00463388 note callback
// FUN_00463714 other events, for example pitch bend and after touch

//    FIELD_ffc->FUN_0044f7b0(); sets callbacks?

    memset(mVoices, 0, sizeof(mVoices)); // NOT ORIGINAL

    mDoc = new Doc{16};

    for (int i = 0; i < 16; i++) {
        mVoices[i].modFoll = new ModFoll{83.59257507};

        for (int j = 0; j < 4; j++) {
            mVoices[i].lfos[j] = new SqLfo{83.59257507};

//            mVoices[i].lfos[j]->mSettings.FIELD_a8 = &Doc::FUN_0045c4f8; TODO
            mVoices[i].lfos[j]->mSettings.FIELD_ac = mDoc;
        }

        for (int j = 0; j < 4; j++) {
            mVoices[i].envs[j] = new SqEnv{83.59257507};
        }

        for (int j = 0; j < 2; j++) {
            mVoices[i].filters[j] = new FilterSq{nullptr, (float) mSampleRate};
        }

        mVoices[i].amp = new Amp;
    }

    // TODO
//    (mDoc->*(mVoices[0].lfos[0]->mSettings.FIELD_a8))(0, 0, 0, 0);

    setNumVoices(8, 8);
    setSampleRate(sampleRate);
    setUpdateRate(83.59257507);
    FUN_0046215c();

//    mField_ff8->FUN_00460db8(0, -1);
}

/* FUN_00461eb4 */ Synth::~Synth() {
//    FUN_004620a4();

    for (int i = 0; i < 16; i++) {

    }

    // TODO
    delete mDoc;
}

void /* FUN_00461fa8 */ Synth::FUN_00461fa8() {
    mField_f8c += 1;
}

void /* FUN_00461fb0 */ Synth::FUN_00461fb0() {
    mField_f8c -= 1;
}

void /* FUN_0046215c */ Synth::FUN_0046215c() {
//    FUN_0044f7fc(mField_ffc); call something in midi parser
    FUN_004621fc();
}

void /* FUN_00462174 */ Synth::FUN_00462174() {
    mField_f8c += 1;
    
    mField_f84 = 0;
    initVoices();

    mDoc->initVoices2();

    memset(mMidiModulation, 0, 0x830);
    memset((char*) &mPitchBendModulation, 0, 0x10);
    memset(mNoteModulation, 0, 0x800);

    mMidiModulation[8].modulation = 127;
    mMidiModulation[9].modulation = 64; // panning
    mMidiModulation[75].modulation = 64; // brightness (filter frequency)
    mMidiModulation[76].modulation = 64;

    mField_f8c -= 1;
}

void /* FUN_004621fc */ Synth::FUN_004621fc() {
    mField_f8c += 1;
    FUN_00462174();
    mField_f8c -= 1;
}

int /* FUN_00462214 */ Synth::setSampleRate(int sampleRate) {
    int result = 0;
    if (sampleRate != mSampleRate) {
        mSampleRate = sampleRate;
        mSampleRate2 = (float) sampleRate * 0.001f;

        if (sampleRate < 40000) {
            mField_f88 = -1;
            result = -44100;
        } else {
            mSamplesPerSecond = 1.0f / (float) mSampleRate;

            FUN_00461fa8(); // increment lock

            initMuffleParameters();
            setMuffle(mMuffle);

            mDoc->setSampleRate(mSampleRate);

            for (auto voice : mVoices) {
                voice.amp->setSampleRate(mSampleRate);
            }

            mVoices[0].filters[1]->initTable4();

            setUpdateRate(mUpdateRate);
            FUN_00462174();

            mField_f88 = 0;

            FUN_00461fb0(); // decrement lock

            result = 0;
        }
    }

    return result;
}

void /* FUN_00462380 */ Synth::setUpdateRate(float updateRate) {
    if (updateRate < 1.0) {
        updateRate = 1.0;
    } else if (updateRate >= (float) mSampleRate) {
        updateRate = (float) mSampleRate;
    }

    mField_f80 = round((mSampleRate / updateRate) * 512) - 1024;

    if (updateRate != mUpdateRate) {
        mUpdateRate = updateRate;

        for (auto voice : mVoices) {
            // TODO check
            if (mField_f80 < voice.field_8) {
                voice.field_8 = mField_f80;
            }

            voice.modFoll->setSampleRate(updateRate);

            for (auto lfo : voice.lfos) {
                lfo->setSampleRate(updateRate);
            }

            for (auto env : voice.envs) {
                env->setSampleRate(updateRate);
            }
        }

        // TODO
//        FUN_00417a78((char)param_1 + '\f',(char)param_1 + '\b', 1.00000000 / param_4, param_4);
    }
}

void /* FUN_00462488 */ Synth::FUN_00462488() {
    for (int i = 0; i < 4; i++) {
        mField_f54[i] = -1;
        mField_f54[4 + i] = -1;
    }
}

void /* FUN_004624a8 */ Synth::FUN_004624a8(short* param_2, unsigned char note, bool mono) {
    if (param_2 != nullptr && note < 0x80) {
        for (int i = 3; i > 0; i--) {
            param_2[i] = param_2[i - 1];

            if (mono) {
                param_2[4 + i] = param_2[3 + i];
            }
        }

        param_2[0] = note;
        if (mono) {
            param_2[4] = note;
        }
    }
}

void /* FUN_004624f4 */ Synth::FUN_004624f4(short* param_2, unsigned char note) {
    if (param_2 != nullptr && note < 0x80) {
        int noteIndex = -1;
        for (int i = 0; i < 4; i++) {
            if (param_2[4 + i] == note) {
                noteIndex = i;
                break;
            }
        }

        if (noteIndex > -1 && noteIndex < 3) {
            for (int i = noteIndex; i < 3; i++) {
                param_2[4 + i] = param_2[5 + i];
            }

            param_2[7] = -1;
        }
    }
}

void /* FUN_00462544 */ Synth::initVoice(int voiceIndex) {
    if (voiceIndex > -1 && voiceIndex < 16) {
        SynthVoice* voice = &mVoices[voiceIndex];
        voice->playing = 0;
        voice->field_4 = 0;
        voice->voiceIndex = voiceIndex;
        voice->note = -1;
        voice->field_e4 = 0;
    }
}

void /* FUN_00462570 */ Synth::initVoices() {
    FUN_00462488();

    mField_f6c = 0;
    mField_f68 = -1;

    for (int i = 0; i < mNumVoices; i++) {
        initVoice(i);

        mNumUsedVoices = 0;

        mVoiceAssignments[i].voice = nullptr;
        mVoiceAssignments[i].voiceIndex = -1;

        mVoiceMapping[i] = i;
    }
}

// TODO i think param 4 is reset counter
void /* FUN_004625d4 */ Synth::triggerVoice2(int voiceIndex, int voiceGroup, char param_4, TriggerOptions* triggerOptions, char param_6, char param_7, unsigned char velocity, char previousNote, char note) {
    if (voiceIndex >= 0 && voiceIndex < mNumVoices) {
        SynthVoice* voice = &mVoices[voiceIndex];

        voice->settings = mSettings; // mEditBuf->field_a94;
        voice->field_e4 = 0;

        if (!voice->playing) {
            reserveVoice(voiceIndex);
        }

        voice->field_2c = triggerOptions->voiceStealSoft;
        voice->field_30 = 0;
        voice->field_34 = 0;
        voice->field_38 = 0;
        voice->ampGain = 1.0;
        voice->ampGainIncrement = 0.0;

        int fadeOutSamples = round(mSampleRate * 0.005);

        voice->fadeOutSamples = fadeOutSamples;
        if (fadeOutSamples < 1) {
            voice->ampFadeOutIncrement = 0;
        } else {
            voice->ampFadeOutIncrement = 1.0 / fadeOutSamples;
        }

        int dca4Attack = voice->settings->envSettings[3].times[0];
        if (dca4Attack > 3) {
            dca4Attack = 3;
        }

        float dca4AttackTimes[4] = {0.0009, 0.0025, 0.0032, 0.005 };
        int dca4AttackSamples = round(mSampleRate * dca4AttackTimes[dca4Attack]);
        voice->field_4c = dca4AttackSamples;
        if (dca4AttackSamples < 1) {
            voice->ampAttackIncrement = 0;
        } else {
            voice->ampAttackIncrement = 1.0 / dca4AttackSamples;
        }


        bool bVar4 = false;
        if (triggerOptions->field_c == nullptr) {
            voice->field_28 = nullptr;
        } else {
            voice->field_28 = triggerOptions->field_c;

            if (voice->settings->restartVoice > 0) {
                if (voice->field_28->group == voiceGroup && voice->field_28->note == note) {
                    bVar4 = true;
                    setFadeIn(voice->voiceIndex);
                }
            }
        }

        voice->group = voiceGroup;

        int local_20;
        if (triggerOptions->field_0 == 0) { // TODO check if field_0 (playing) is checked here or nullptr check
            voice->field_d8 = 0;
            voice->field_dc = 0;
            local_20 = 0;
        } else {
            voice->field_d8 = triggerOptions->field_4;
            voice->field_dc = triggerOptions->field_6;
            local_20 = triggerOptions->field_8;
        }

        int limitedNote = limitVelocity(note);
        int limitedPreviousNote = limitedNote;
        if (previousNote > -1) {
            limitedPreviousNote = limitVelocity(previousNote);
        }
        previousNote = limitedPreviousNote;

        voice->field_4 = -1;
        voice->field_18 = 0;
        voice->field_1c = voice->settings->mono > 0; // TODO check
        voice->field_8 = 0;
        voice->field_c = 0;

        if (param_7) {
            for (int i = 0; i < 4; i++) {
                SynthLfoSettings lfoSettings = voice->settings->lfoSettings[i];

                // TODO still a bit confused about this bit
                int iVar6 = lfoSettings.field_1;
                bool resetCounter = iVar6 >= 0;
                if (!resetCounter) {
                    iVar6 = 0;
                }

                SqLfo* lfo = voice->lfos[i];
                lfo->mSettings.frequency = (lfoSettings.frequency & 0x7f) * 256;
                lfo->mSettings.humanize = lfoSettings.humanize;
                lfo->mSettings.wave = lfoSettings.wave;
                lfo->mSettings.startAmplitude = lfoSettings.startAmplitude;
                lfo->mSettings.fadingSpeed = lfoSettings.fading & 0x3f;
                lfo->mSettings.finalAmplitude = lfoSettings.finalAmplitude;
                lfo->mSettings.delayMode = (lfoSettings.fading >> 6) & 1;
                lfo->mSettings.reverse = (lfoSettings.playMode >> 4) & 1;
                lfo->mSettings.oneShot = (lfoSettings.playMode >> 5) & 1;
                lfo->mSettings.smoothness = (lfoSettings.smoothness & 0x3f) * 4;
                lfo->FUN_0045d2a4(local_20 + iVar6, resetCounter);
            }

            for (int i = 0; i < 4; i++) {
                Env_settings* envSettings = &voice->settings->envSettings[i];
                bool cycle = voice->settings->cycle > 0 || (envSettings->options & 1) > 0;

                if (bVar4 && voice->field_28 != nullptr) {
                    bool restartEnvelope = voice->settings->restartEnvelopes > 0;

                    voice->envs[i]->triggerAttack(envSettings, limitedNote, voice->field_28->envs[i], cycle, restartEnvelope, velocity);
                } else {
                    voice->envs[i]->triggerAttack(envSettings, limitedNote, nullptr, cycle, true, velocity);
                }
            }
        }

        voice->filters[0]->setField1c(voice->settings->field_13c - 1);
        voice->amp->setSaturation(voice->settings->ampSettings.saturation);

        if (param_4 == 0) {
            voice->field_e0 = -1;
        } else {
            voice->field_e0 = 0;

            if (bVar4) {
                voice->filters[0]->copyState(voice->field_28->filters[0]);
                voice->amp->copyState(voice->field_28->amp);
            } else {
                voice->filters[0]->copyState(nullptr);
                voice->amp->copyState(nullptr);
            }

            if (mGlobalDca4Smoothing < 1) {
                voice->dca4Smoothing = (voice->settings->field_196 >> 1) & 1;
            } else {
                voice->dca4Smoothing = (mGlobalDca4Smoothing - 1) & 1;
            }

            int envAttackTime = voice->settings->envSettings[3].times[0];
            if (envAttackTime > 3) {
                envAttackTime = 3;
            }

            voice->amp->setSmoothing(smoothingDurations[voice->dca4Smoothing][envAttackTime]);
        }

        std::fill_n(voice->modulation, 16, 0);

        voice->modulation[8] = envLinearVelocityCurve[limitVelocity(velocity)];
        voice->modulation[9] = envExponentialVelocityCurve[limitVelocity(velocity)];

        setNoteModulation(voiceIndex, limitedNote);

        if (voice->field_28 == nullptr) {
            mDoc->triggerVoice(voiceIndex, limitedNote, -1, param_4 != 0, param_6 != 0);
        } else {
            mDoc->triggerVoice(voiceIndex, limitedNote, voice->field_28->voiceIndex, param_4 != 0, param_6 != 0);
        }

        voice->mPitchBend = (getMidiModulation(&mPitchBendModulation) * voice->settings->pitchBendRange) / 32;

        if (voice->settings->restartVoice < 1 || limitedNote == previousNote) {
            voice->field_84 = 0;
            voice->modFoll->FUN_0045ec68(0);
            previousNote = limitedNote;
        } else {
            voice->field_84 = -1;
            voice->field_88 = previousNote;

            int noteDifference = limitedNote - previousNote;
            float glideDuration = ((float) (noteDifference * 0x100) * 83.59257598) / (float) glideDurations[voice->settings->glide];

            voice->modFoll->FUN_0045ebe8(glideDuration);
            voice->modFoll->FUN_0045ec68(0);
            voice->modFoll->FUN_0045ec1c((limitedNote - previousNote) * 256);
        }

        if (voice->field_84 != 0) {
            mDoc->setGlideStart(voiceIndex, previousNote);
        }

        voice->note = note;
        voice->playing = -1;
    }
}

void /* FUN_00462c04 */ Synth::triggerVoice(int voiceIndex, int voiceGroup, char param_4, TriggerOptions* triggerOptions, char restartOscillators, char param_7, unsigned char velocity, int previousNote, int note) {
    if (voiceIndex > -1 && voiceIndex < 16) {
        int i = mVoiceMapping[voiceIndex];
        triggerOptions->field_c = &mVoices[i];

        if (triggerOptions->voiceStealSoft != 0) {
            int j = FUN_004630cc();

            if (j > -1) {
                int uVar3 = mVoiceMapping[j];
                mVoiceMapping[j] = i;
                mVoiceMapping[voiceIndex] = uVar3;

                setFadeOut(i);
            }
        }

        triggerVoice2(i, voiceGroup, param_4, triggerOptions, restartOscillators, param_7, velocity, previousNote, note);
    }
}

void /* FUN_00462ca0 */ Synth::setGlide(int /* voiceIndex */ voiceIndex, int /* note */ note) {
    if (voiceIndex > -1 && voiceIndex < mNumVoices) {
        note = limitVelocity(note);

        SynthVoice* voice = &mVoices[voiceIndex];

        if (voice->settings->glide < 1 || note == voice->note) {
            voice->field_84 = 0;
            voice->modFoll->FUN_0045ec68(0);
            mDoc->setGlideStart(voiceIndex, note);
        } else {
            voice->field_84 = -1;

            int noteDifference = abs(note - (int) voice->note);

            // or duration per glide step
            float glideDuration = ((float) (noteDifference * 0x100) * 83.59257598) / (float) glideDurations[voice->settings->glide];

            voice->modFoll->FUN_0045ebe8(glideDuration);
            voice->modFoll->FUN_0045ec1c((note - voice->field_88) * 0x100);
        }

        voice->note = note;

        setNoteModulation(voiceIndex, note);
    }
}

void /* FUN_00462da4 */ Synth::setNoteModulation(int voiceIndex, int param_3) {
    SynthVoice* voice = &mVoices[voiceIndex];

    voice->modulation[10] = param_3; // TODO check

    if (param_3 < 33) {
        voice->modulation[11] = -128;
    } else if (param_3 < 95) {
        voice->modulation[11] = (param_3 - 64) * 4;
    } else {
        voice->modulation[11] = 124;
    }
}

void /* FUN_00462de4 */ Synth::triggerRelease(int voiceIndex) {
    if (voiceIndex > -1 && voiceIndex < mNumVoices) {
        SynthVoice* voice = &mVoices[voiceIndex];

        if (voice->playing != 0 && voice->field_18 == 0) {
            for (int i = 0; i < 4; i++) {
                bool sustain = getSustainModulation() != 0;
                voice->envs[i]->triggerRelease(sustain);
            }
            voice->field_18 = -1;
        }
    }
}

void /* FUN_00462e30 */ Synth::stopVoice(int voiceIndex) {
    if (voiceIndex >= 0 && voiceIndex < mNumVoices) {
        mDoc->stopVoice(voiceIndex);

        SynthVoice* voice = &mVoices[voiceIndex];
        voice->playing = 0;
        voice->field_4 = 0;

        freeVoice(voiceIndex);
    }
}

void /* FUN_00462e6c */ Synth::stopVoices(Synth_settings* settings) {
    for (int i = 0; i < 16; i++) {
        SynthVoice* voice = &mVoices[i];

        if (voice->playing != 0 && settings == voice->settings) {
            stopVoice(i);
        }
    }
}

void /* FUN_00462e9c */ Synth::releaseVoices(Synth_settings* settings) {
    for (int i = 0; i < 16; i++) {
        SynthVoice* voice = &mVoices[i];

        if (voice->playing != 0 && settings == voice->settings) {
            triggerRelease(i);
        }
    }
}

void /* FUN_00462ecc */ Synth::setFadeOut(int voiceIndex) {
    if (voiceIndex > -1 && voiceIndex < mNumVoices) {
        SynthVoice* voice = &mVoices[voiceIndex];

        voice->field_30 = -1;
        voice->field_34 = -1;
        voice->field_38 = voice->fadeOutSamples;

        if (voice->fadeOutSamples < 1) {
            voice->ampGain = 0.0;
            voice->ampGainIncrement = 0;
        } else {
            voice->ampGainIncrement = -voice->ampFadeOutIncrement;
            voice->ampGain = 1.0;
        }
    }
}

void /* FUN_00462f18 */ Synth::setFadeIn(int voiceIndex) {
    if (voiceIndex > -1 && voiceIndex < mNumVoices) {
        SynthVoice* voice = &mVoices[voiceIndex];

        if (voice->field_34 == 0) {
            voice->field_38 = voice->field_4c;

            if (voice->field_4c < 1) {
                voice->field_30 = 0;
                voice->ampGain = 1.0;
                voice->ampGainIncrement = 0;
            } else {
                voice->field_30 = -1;
                voice->ampGainIncrement = voice->ampAttackIncrement;
                voice->ampGain = 0.0;
            }
        }
    }
}

void /* FUN_00462f64 */ Synth::reserveVoice(int voiceIndex) {
    if (mNumUsedVoices < mNumVoices) {
        VoiceAssignment* unknown1 = &mVoiceAssignments[mNumUsedVoices];

        unknown1->voice = &mVoices[voiceIndex];
        unknown1->voiceIndex = voiceIndex;

        mNumUsedVoices += 1;
    }
}

void /* FUN_00462f90 */ Synth::freeVoice(int voiceIndex) {
    int iVar2;

    for (int i = 0; i < 16; i++) {
        iVar2 = i;
        if (voiceIndex == mVoiceAssignments[i].voiceIndex) {
            break;
        }
        iVar2 = -1;
    }

    if (iVar2 >= 0) {
        mNumUsedVoices -= 1;

        if (iVar2 < mNumVoices - 1 && iVar2 < 15) {
            while (iVar2 != 15) {
                mVoiceAssignments[iVar2].voice = mVoiceAssignments[iVar2 + 1].voice;
                mVoiceAssignments[iVar2].voiceIndex = mVoiceAssignments[iVar2 + 1].voiceIndex;
                iVar2++;
            }
        }

        mVoiceAssignments[15].voice = nullptr;
        mVoiceAssignments[15].voiceIndex = -1;
    }
}

SynthVoice* /* FUN_00462ffc */ Synth::getVoice(int voiceIndex) {
    return &mVoices[mVoiceMapping[voiceIndex]];
}

int /* FUN_0046300c */ Synth::selectVoice() {
    int voiceIndex;

    if (mField_f4c < 2) {
        voiceIndex = 0;
    } else {
        int local_c = -1;
        voiceIndex = -1;

        for (int i = 0; i < mField_f4c; i++) {
            SynthVoice* voice = getVoice(i);

            if (voice->playing == 0 || voice->field_4 == 0) {
                voiceIndex = i;
                break;
            }

            if (voice->field_18 != 0 && voice->field_4 <= local_c) {
                local_c = voice->field_4;
                voiceIndex = i;
            }
        }

        if (voiceIndex < 0) {
            voiceIndex = 0;

            SynthVoice* firstVoice = getVoice(0);
            local_c = firstVoice->field_4;

            for (int i = 1; i < mField_f4c; i++) {
                SynthVoice* voice = getVoice(i);
                if (voice->field_4 <= local_c) {
                    local_c = voice->field_4;
                    voiceIndex = i;
                }
            }
        }
    }

    return voiceIndex;
}


int /* FUN_004630cc */ Synth::FUN_004630cc() {
    int local_8 = mField_f4c;
    int local_c = getVoice(local_8)->field_4;

    SynthVoice* voice = getVoice(local_8);

    if (voice->playing != 0) {
        int iVar1 = mField_f4c + 1;
        int iVar3 = mField_f4c + mField_f50 - 1;

        for (int i = iVar1; i < iVar3; i++) {
            voice = getVoice(i);

            if (voice->playing == 0) {
                return i;
            }

            if (voice->field_4 <= local_c) {
                local_c = voice->field_4;
                local_8 = i;
            }
        }
    }

    return local_8;
}

void /* FUN_004632e4 */ Synth::setNumVoices(int numVoices) {
    mField_f8c += 1;
    if (numVoices > 16) {
        numVoices = 16;
    }

    mNumVoices = numVoices;
    mField_f64 = 0.8;

    FUN_00462174();

    mDoc->setNumVoices(mNumVoices);

    mField_f8c -= 1;
}

int /* FUN_00463328 */ Synth::getNumPlayingVoices() {
    int result = 0;

    for (int i = 0; i < mField_f4c; i++) {
        SynthVoice* voice = getVoice(i);
        if (voice->playing != 0) {
            result += 1;
        }
    }

    return result;
}

void /* FUN_00463358 */ Synth::setNumVoices(int param_2, int param_3) {
    mField_f8c += 1;
    mField_f4c = param_2;
    mField_f50 = param_3;
    setNumVoices(param_3 + param_2);
    mField_f8c += 1;
}

void /* FUN_00463388 */ Synth::triggerNote(unsigned char param_2, unsigned char note, unsigned char velocity) {
    TriggerOptions triggerOptions = {
        0,
        0,
        63,
        0,
        0,
        0,
        nullptr,
        0,
    };

    int voiceStealingMode;
    if (mGlobalVoiceStealingMode < 1) {
        voiceStealingMode = mSettings->voiceStealingMode; // = mEditBuf.mField_a94.mField_195
    } else {
        voiceStealingMode = mGlobalVoiceStealingMode - 1;
    }

    triggerOptions.voiceStealSoft = voiceStealingMode != 0;

    int voiceGroup = 0; // = mEditBuf->FUN_0046093c();

    triggerNote(note + triggerOptions.field_0, velocity, &triggerOptions, voiceGroup);
}

void /* FUN_00463420 */ Synth::triggerNote(unsigned char note, unsigned char velocity, TriggerOptions *triggerOptions, unsigned int voiceGroup) {
    limitVelocity(note); // not used ???

    short* local_1c = mField_f54;
    Synth_settings* local_14 = mSettings; // mEditBuf->field_a94;

    if (velocity < 1 || triggerOptions == nullptr) {
        for (int i = 0; i < mField_f4c; i++) {
            SynthVoice* voice = getVoice(i);
            if (voice->playing != 0 && voice->field_18 == 0 && voice->note == note && voice->group == voiceGroup) {
                if (voice->field_1c == 0 || local_1c[5] < 0 || local_1c[5] == note) {
                    triggerRelease(mVoiceMapping[i]);
                } else {
                    setGlide(mVoiceMapping[i], local_1c[5]);
                }
            }
        }

        FUN_004624f4(local_1c, note);
    } else {
        FUN_004624f4(local_1c, note);

        if (local_14->mono > 0) {
            bool bVar7 = false;
            bool bVar8 = false;
            int voiceIndex = -1;

            for (int i = 0; i < mField_f4c; i++) {
                SynthVoice* voice = getVoice(i);

                if (voice->playing != 0 && voice->group == voiceGroup) {
                    bVar7 = voice->field_18 != 0;
                    bVar8 = voice->field_1c != 0;
                    voiceIndex = i;
                }
            }

            // TODO check references to local_1c

            if (voiceIndex < 0) {
                bool restartOscillators = local_14->restartOscillators; // TODO check
                int newVoiceIndex = selectVoice();
//
                triggerVoice(newVoiceIndex, voiceGroup, true, triggerOptions, restartOscillators, true, false, *local_1c,
                             note);
            } else {
                if (bVar7 || !bVar8) {
                    bool restartOscillators = local_14->restartOscillators;

                    triggerVoice(voiceIndex, voiceGroup, true, triggerOptions, restartOscillators, true, velocity,
                                 mField_f54[3], note);
                } else {
                    triggerVoice(mVoiceMapping[voiceIndex], voiceGroup, false, triggerOptions, false, false, velocity,
                                 mField_f54[3], note);
                }
            }

            FUN_004624a8(local_1c, note, true);
        } else {
            char restartVoice = local_14->restartVoice;
            int voiceIndex = -1;

            for (int i = 0; i < mField_f4c; i++) {
                SynthVoice* voice = getVoice(i);

                // might be nullptr check, TODO check everywhere :(
                if (voice->playing != 0 && voice->note == note && voice->group == voiceGroup) {
                    if (voice->field_18 == 0) {
                        triggerRelease(mVoiceMapping[i]);
                    } else {
                        voiceIndex = i;
                    }
                }
            }

            if (restartVoice < 1 || voiceIndex < 0) {
                bool restartOscillators = local_14->restartOscillators > 0;
                int newVoiceIndex = selectVoice();

                triggerVoice(newVoiceIndex, voiceGroup, true, triggerOptions, restartOscillators, true, velocity,
                             *local_1c, note);
            } else {
                bool restartOscillators = local_14->restartOscillators > 0;

                triggerVoice(voiceIndex, voiceGroup, true, triggerOptions, restartOscillators, true, velocity, *local_1c,
                             note);
            }

            FUN_004624a8(local_1c, note,false);
        }
    }
}

int /* FUN_004637c4 */ Synth::getSustainModulation() {
    return mMidiModulation[65].modulation;
}

int /* FUN_004637d0 */ Synth::getMidiModulation(MidiModulationSource* modulationSource) {
    return modulationSource->modulation;
}

int /* FUN_004637f0*/ Synth::getMidiModulation(int modulationIndex) {
    if (modulationIndex > -1) {
        return getMidiModulation(&mMidiModulation[modulationIndex + 1]);
    }

    return 0;
}

int /* FUN_00463808 */ Synth::getModulation(SynthVoice* voice, int modulationSource) {
    int modulation;

    if ((modulationSource < -1) || (0x91 < modulationSource)) {
        modulation = 0;
    } else {
        int modulationIndex = modulationSources[modulationSource + 1][1];
        if (modulationSources[modulationSource + 1][0] == 1) {
            if (voice == nullptr) {
                modulation = 0;
            } else {
                modulation = voice->modulation[modulationIndex];
            }
        } else {
            if (modulationSource == 0x81) {
                modulation = 0;
            } else if (modulationIndex == 0x81) {
                if (voice->field_94 == 0) {
                    modulation = 0;
                } else {
                    modulation = getMidiModulation(0x81);
                }
            } else {
                modulation = getMidiModulation(modulationIndex);
            }
        }
    }

    return modulation;
}

void /* FUN_00463890 */ Synth::updateState(SynthVoice* voice) {
    if (voice->field_c == 0) {
        voice->field_c = 1;

        mField_11004 = (mField_11004 + 1) % 2;

        Synth_settings* synthSettings = voice->settings;

        voice->modulation[12] = getMidiModulation(&mNoteModulation[voice->note % 128]);

        int pitchModulation;
        if (voice->field_84 == 0) {
            pitchModulation = 0;
        } else {
            pitchModulation = voice->modFoll->getOutput();
        }

        char pitchBendMode = synthSettings->pitchBendMode;
        bool keepPitch = pitchBendMode < 4;
        if (!keepPitch) {
            pitchBendMode -= 3;
        }

        bool applyPitchBend;
        if (pitchBendMode == 1) {
            applyPitchBend = voice->field_18 == 0;
        } else if (pitchBendMode == 2) {
            applyPitchBend = voice->note == mField_f54[0];
        } else if (pitchBendMode == 3) {
            applyPitchBend = voice->field_18 == 0 && voice->note == mField_f54[0];
        } else {
            applyPitchBend = true;
        }

        int pitchBend;
        if (applyPitchBend) {
            pitchBend = (getMidiModulation(&mPitchBendModulation) * synthSettings->pitchBendRange) / 32;

            if (keepPitch) {
                voice->mPitchBend = pitchBend;
            }

            voice->field_94 = -1;
        } else {
            pitchBend = voice->mPitchBend;
            voice->field_94 = 0;
        }

        pitchModulation += pitchBend;

        for (int i = 0; i < 4; i++) {
            bool sustain = getSustainModulation() != 0;
            voice->modulation[i + 4] = voice->envs[i]->getOutputAndStep(sustain);
        }

        for (int i = 0; i < 4; i++) {
            SynthLfoSettings synthLfoSettings = voice->settings->lfoSettings[i];
            Sq_lfo_settings* lfoSettings = &voice->lfos[i]->mSettings;

            lfoSettings->frequency = (synthLfoSettings.frequency & 0x7f) * 256;
            lfoSettings->humanize = synthLfoSettings.humanize;
            lfoSettings->wave = synthLfoSettings.wave;
            lfoSettings->startAmplitude = synthLfoSettings.startAmplitude;
            lfoSettings->fadingSpeed = synthLfoSettings.fading & 0x3f;
            lfoSettings->finalAmplitude = synthLfoSettings.finalAmplitude;
            lfoSettings->delayMode = (synthLfoSettings.fading >> 6) & 0x1;
            lfoSettings->reverse = (synthLfoSettings.playMode >> 4) & 0x1;
            lfoSettings->oneShot = (synthLfoSettings.playMode >> 5) & 0x1;

            int phase = 0;

            lfoSettings->smoothness = 0;
            lfoSettings->amplitudeModulation = 0;

            // TODO this bit needs some checking
            int frequencyModulation = getModulation(voice, synthLfoSettings.frequencyModulationSource);
            int playMode = (synthLfoSettings.playMode >> 2) & 0x3;

            if (playMode < 2) {
                if (frequencyModulation > 0 || (playMode != 0)) {
                    lfoSettings->frequency += synthLfoSettings.frequencyModulationAmount * frequencyModulation * 2;
                }
            } else if (playMode == 2) {
                phase = synthLfoSettings.frequencyModulationAmount * frequencyModulation * 4;
            } else {
                lfoSettings->smoothness = synthLfoSettings.frequencyModulationAmount * frequencyModulation;
            }

            int amplitudeModulation = getModulation(voice, synthLfoSettings.amplitudeModulationSource);
            int playMode2 = synthLfoSettings.playMode & 0x3;

            if (playMode2 < 2) {
                if (0 < amplitudeModulation > 0 || (playMode2 != 0)) {
                    lfoSettings->amplitudeModulation += synthLfoSettings.amplitudeModulationAmount * amplitudeModulation * 2;
                }
            } else if (playMode2 == 2) {
                phase = synthLfoSettings.amplitudeModulationAmount * amplitudeModulation * 4;
            } else {
                lfoSettings->smoothness = synthLfoSettings.amplitudeModulationAmount * amplitudeModulation;
            }

            phase += (synthLfoSettings.phase & 0x3f) * 1024;

            if ((synthLfoSettings.phase & 0x40) == 0) {
                lfoSettings->phase1 = phase;
                lfoSettings->twinMode = false;
            } else {
                lfoSettings->phase1 = 0;
                lfoSettings->phase2 = phase;
                lfoSettings->twinMode = true;
            }

            if (lfoSettings->smoothness == 0) {
                lfoSettings->smoothness = (synthLfoSettings.smoothness & 0x3f) * 4;
            } else {
                lfoSettings->smoothness = ((synthLfoSettings.smoothness & 0x3f) * 256 + lfoSettings->smoothness) / 64;
            }

            voice->modulation[i] = voice->lfos[i]->nextSample();
        }

        for (int i = 0; i < 3; i++) {
            ModulationMatrixSettings modulationMatrixSettings = synthSettings->modulationMatrixSettings[i];

            int modulationAmount1 = modulationMatrixSettings.modulationAmount1;
            int modulation1 = getModulation(voice, modulationMatrixSettings.modulationSource1);
            int modulationAmount2 = modulationMatrixSettings.modulationAmount2;
            int modulation2 = getModulation(voice, modulationMatrixSettings.modulationSource2);
            int modulationAmount3 = modulationMatrixSettings.modulationAmount3;
            int modulation3 = getModulation(voice, modulationMatrixSettings.modulationSource3);

            int modulation = modulationAmount1 * modulation1 +
                    modulationAmount2 * modulation2 +
                    modulationAmount3 * modulation3;

            int amplitudeModulationAmount = modulationMatrixSettings.amplitudeModulationAmount;
            int amplitudeModulation = getModulation(voice, modulationMatrixSettings.amplitudeModulationSource);

            amplitudeModulation = (amplitudeModulation - 127) * amplitudeModulationAmount + 8001;

            if (modulation == 0 && amplitudeModulation == 0) {
                voice->modulation[i + 13] = 0;
            } else {
                voice->modulation[i + 13] = (modulation * amplitudeModulation) / 504063;
            }
        }

        VoiceSettings* voiceSettings = mDoc->getVoiceSettings(voice->voiceIndex);
        voiceSettings->am_enabled = synthSettings->am;
        voiceSettings->sync_enabled = synthSettings->sync < 1;
        voiceSettings->am_bug_enabled = synthSettings->emuOptions & 1;
        voiceSettings->field_70 = synthSettings->field_196 & 1;

        if (mGlobalDca13Smoothing < 1) {
            voiceSettings->smoothingMode = ((synthSettings->emuOptions >> 1) & 3) + 1;
        } else {
            voiceSettings->smoothingMode = mGlobalDca13Smoothing;
        }

        int dcBlock;
        if (mGlobalDcBlockingFilter < 1) {
            dcBlock = (synthSettings->emuOptions >> 3) & 3;
        } else {
            dcBlock = mGlobalDcBlockingFilter - 1;
        }

        if (dcBlock == 0) {
            if (synthSettings->sync < 1) {
                voiceSettings->field_7c = false;
            } else {
                voiceSettings->field_7c = true;
            }
        } else if (dcBlock == 1) {
            voiceSettings->field_7c = true;
        } else {
            voiceSettings->field_7c = false;
        }

        for (int i = 0; i < 3; i++) {
            OscSettings* oscSettings = &voiceSettings->oscSettings[i];
            SynthOscSettings* synthOscSettings = &synthSettings->oscSettings[i];

            int modulation1 = getModulation(voice, synthOscSettings->modulationSource1);
            int modulationAmount1 = synthOscSettings->modulationAmount1;
            int modulation2 = getModulation(voice, synthOscSettings->modulationSource2);
            int modulationAmount2 = synthOscSettings->modulationAmount2;
            int modulation3 = getModulation(voice, synthOscSettings->modulationSource3);
            int modulationAmount3 = synthOscSettings->modulationAmount3;
            int modulation4 = getModulation(voice, synthOscSettings->modulationSource4);
            int modulationAmount4 = synthOscSettings->modulationAmount4;

            oscSettings->enabled = synthOscSettings->enabled;
            oscSettings->wave = synthOscSettings->wave;
            oscSettings->semi = synthOscSettings->octave * 12 + synthOscSettings->semi;
            oscSettings->fine = synthOscSettings->fine;

            oscSettings->field_14 = pitchModulation + modulation1 * modulationAmount1 + modulation2 * modulationAmount2;
            oscSettings->level = (modulation3 * modulationAmount3 + modulation4 * modulationAmount4) * 4 + ((int) synthOscSettings->level) * 512;
        }

        mDoc->updateVoiceSettings(voice->voiceIndex);
        voice->filters[0]->setField1c(voice->settings->field_13c - 1);

        int cutoffModulation1 = getModulation(voice, synthSettings->filterSettings.modulationSource1);
        int cutoffModulationAmount1 = synthSettings->filterSettings.modulationAmount1;

        int cutoffModulation2 = getModulation(voice, synthSettings->filterSettings.modulationSource2);
        int cutoffModulationAmount2 = synthSettings->filterSettings.modulationAmount2;

        int cutoffModulation3 = voice->note;
        int cutoffModulationAmount3 = synthSettings->filterSettings.noteModulationAmount;

        int cutoffModulation4 = getModulation(voice, 0x5a);

        int cutoffModulation = cutoffModulation1 * cutoffModulationAmount1 +
                cutoffModulation2 * cutoffModulationAmount2 +
                cutoffModulation3 * cutoffModulationAmount3 * 2 +
                (cutoffModulation4 - 64) * 192;

        bool resetFilter = voice->field_e0 == 0;
        int cutoff = synthSettings->filterSettings.cutoff * 2 + (cutoffModulation / 64);
        voice->filters[0]->setCutoffAndResonance(cutoff, synthSettings->filterSettings.resonance, resetFilter);

        // Process amp settings
        // TODO this needs some checking
        voice->amp->setSaturation(synthSettings->ampSettings.saturation);

        int amplitudeModulation = getModulation(voice, synthSettings->ampSettings.amplitudeModulationSource);
        int amplitudeModulationAmount = synthSettings->ampSettings.amplitudeModulationAmount;
        int amplitudeModulation2 = amplitudeModulation * amplitudeModulationAmount;

        int panningModulation1 = getModulation(voice, synthSettings->ampSettings.panningModulationSource);
        int panningModulationAmount1 = synthSettings->ampSettings.panningModulationAmount;
        int panningModulation2 = getModulation(voice, 0x18);
        int panningModulation = panningModulation1 * panningModulationAmount1 + panningModulation2 - 64;

        int x = round((float) voice->field_d8 * (float) synthSettings->ampSettings.envelopeAmplitudeModulationAmount * 0.0163809523809523814586);
        x = (x + getModulation(voice, 0x1b) / 2) * voice->envs[3]->getOutput() * 2;
        int iVar15 = getModulation(voice, 0x17);

        int ampVolume = round(((x / 64) * (iVar15 + (amplitudeModulation2 / 64)) * (1.0 / 127)));
        int ampPanning = synthSettings->ampSettings.stereoPanning + voice->field_dc + (panningModulation / 128);

        ampVolume = voice->envs[3]->getOutput(); // TODO remove, for debugging

        int /* DAT_4C2550 */ DAT_4C2550[2] = {0, 0};

        voice->amp->setVolumeResponse(DAT_4C2550[voice->dca4Smoothing]);
        voice->amp->setParameters(ampVolume, ampPanning, false); // TODO check 4th param, voice->filters[0]->field_0x14);

        if (mField_f6c <= voice->field_4 || voice->voiceIndex == mField_f68) {
            mField_f6c = voice->field_4;
            mField_f68 = voice->voiceIndex;

            bool muffle;
            if (mGlobalMuffle < 1) {
                muffle = (voice->settings->muffle >> 2) & 1;
            } else {
                muffle = mGlobalMuffle - 1;
            }

            if (mMuffle != muffle) {
                setMuffle(muffle);
            }
        }

        if (voice->field_e0 == 1) {
            voice->amp->setSmoothing(smoothingDurations[voice->dca4Smoothing][3]);
        }

        if (voice->field_e0 > -1 && voice->field_e0 < 2147483647) {
            voice->field_e0 += 1;
        }
    } else {
        mDoc->updateSmoothingMode(voice->voiceIndex);

        if (voice->field_c < 1) {
            voice->field_c += 1;
        } else {
            voice->field_c = 0;
        }
    }
}

void /* FUN_00464250 */ Synth::initMuffleParameters() {
    float fVar1 = cos(mSamplesPerSecond * 6214 * M_PI);
    float fVar2 = pow(2.5625, -1.5); // TODO could be switched
    float fVar3 = sqrt(fVar2 / 0.71);
    float fVar4 = ((fVar2 + 1.0) - (fVar2 - 1.0) * fVar1) + fVar3;

    mField_f98 = ((fVar2 + 1.0 + (fVar2 - 1.0) * fVar1 + fVar3) * fVar2) / fVar4;
    mField_f9c = (((fVar2 - 1.0) + (fVar2 + 1.0) * fVar1) * fVar2 * -2.0) / fVar4;
    mField_fa0 = (((fVar2 + 1.0 + (fVar2 - 1.0) * fVar1) - fVar3) * fVar2) / fVar4;
    mField_fa4 = (((fVar2 - 1.0) - (fVar2 + 1.0) * fVar1) * 2.0) / fVar4;
    mField_fa8 =  (((fVar2 + 1.0) - (fVar2 - 1.0) * fVar1) - fVar3) / fVar4;
}

void /* FUN_004643c8 */ Synth::setMuffle(bool muffle) {
    if (!muffle) {
        mMuffle = false;
        return;
    }

    mMuffle = true;
    mField_fac = 0;
    mField_fb0 = 0;
    mField_fb4 = 0;
    mField_fb8 = 0;
    mField_fbc = 0;
    mField_fc0 = 0;
    mField_fc4 = 0;
    mField_fc8 = 0;
}

bool /* FUN_00464410 */ Synth::getVoiceOutput(SynthVoice* voice, float output[2]) {
    if (voice->field_4 > 0) {
        voice->field_4 -= 1;
    }

    if (voice->field_8 < 1) {
        voice->field_8 += mField_f80;

        if (voice->envs[3]->isRunning()) {
            updateState(voice);
        }

        if (!voice->envs[3]->isRunning()) {
            mDoc->stopVoice(voice->voiceIndex);
        }
    } else {
        voice->field_8 -= 1024;
    }

    if (!voice->envs[3]->isRunning()) {
        if (voice->amp->smoothingCounter() == -mDoc->field_209c()) {
            return true;
        }
    }

    bool result = false;

    float sample = mDoc->getSample(voice->voiceIndex);
    float filteredSample = voice->filters[0]->process(sample);

    if (voice->field_30 == 0) {
        voice->amp->process(output, filteredSample);
    } else {
        voice->amp->process(output, filteredSample, voice->ampGain);

        if (voice->field_38 <= 0) {
            if (voice->field_34 == 0) {
                result = true;
            }

            voice->field_30 = 0;
        } else {
            voice->field_38 -= 1;

            voice->ampGain += voice->ampGainIncrement;
        }
    }

    return result;
}

void /* FUN_004644f4 */ Synth::muffle(float *output) {
    float fVar1 = mField_f98 * output[0] +
                  (((mField_fa0 * mField_fb4 +
               mField_f9c * mField_fac) -
              mField_fa4 * mField_fbc) -
             mField_fa8 * mField_fc4);

    mField_fb4 = mField_fac;
    mField_fac = output[0];
    mField_fc4 = mField_fbc;
    mField_fbc = fVar1;

    output[0] = fVar1;

    float fVar2 = mField_f98 * output[1] +
            (((mField_fa0 * mField_fb8 +
               mField_f9c * mField_fb0) -
              mField_fa4 * mField_fc0) -
             mField_fa8 * mField_fc8);

    mField_fb8 = mField_fb0;
    mField_fb0 = output[1];
    mField_fc8 = mField_fc0;
    mField_fc0 = fVar2;

    output[1] = fVar2;
}

void /* FUN_004645c8 */ Synth::getSamples(bool reset, float *param_3, int numSamples, float *param_5) {
    // TODO fix error handling
    // function has exception handling
    // see https://reverseengineering.stackexchange.com/questions/1911/whats-fs0-doing-and-how-can-i-execute-it-step-by-step

//    mField_ff8->FUN_00460488();

    mField_12050 = true;

    //
    // Set8087CW
    // set FPU mode to truncate

//    if (param_3 == nullptr || param_5 == nullptr) {
//        // finally?
//    }

    if (mField_f88 == 0 /* && mField_f8c < 1 */ && mNumVoices > 0) {
        int finishedVoices[16];

        for (int i = 0; i < 16; i++) {
            finishedVoices[i] = -1;
        }

        for (int i = 0; i < numSamples; i++) {
//            mField_ffc->FUN_0044fe88();

            if (mNumUsedVoices > 0) {
                float outputs[2] = {0.0, 0.0};
                int numFinishedVoices = 0;

                for (int j = 0; j < mNumUsedVoices; j++) {
                    bool voiceFinished = getVoiceOutput(mVoiceAssignments[j].voice, outputs);

                    if (voiceFinished) {
                        finishedVoices[numFinishedVoices] = mVoiceAssignments[j].voiceIndex;
                        numFinishedVoices++;
                    }
                }

                for (int j = 0; j < numFinishedVoices; j++) {
                    stopVoice(finishedVoices[j]);
                    finishedVoices[j] = -1;
                }

                if (mMuffle) {
                    muffle(outputs);
                }

                if (!reset) {
                    param_3[i] += outputs[0] * mField_f64;
                    param_5[i] += outputs[1] * mField_f64;
                } else {
                    param_3[i] = outputs[0] * mField_f64;
                    param_5[i] = outputs[1] * mField_f64;
                }
            } else {
                if (reset) {
                    param_3[i] = 0.0;
                    param_5[i] = 0.0;
                }
            }
        }

//         mField_ffc->FUN_0044f7fc();

//         this is maybe the finally clause
//         restore FPU mode
//        mField_ff8->FUN_0046048c();

        mField_12050 = false;

        return;
    }

    if (reset) {
        std::fill_n(param_3, numSamples, 0.0);
        std::fill_n(param_5, numSamples, 0.0);
    }
}
