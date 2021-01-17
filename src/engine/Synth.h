#pragma once

#include "Amp.h"
#include "FilterSq.h"
#include "ModFoll.h"
#include "SqLfo.h"
#include "SqEnv.h"

// Size: 0x10
struct SynthLfoSettings {
    unsigned char /* field_0 */ frequency;
    unsigned char /* field_1 */ field_1;
    char /* field_2 */ humanize;
    unsigned char /* field_3 */ wave;
    char /* field_4 */ startAmplitude;
    unsigned char /* field_5 */ fading; // fading speed + delay mode
    char /* field_6 */ finalAmplitude;
    short /* field_7 */ amplitudeModulationSource;
    char /* field_9 */ amplitudeModulationAmount;
    short /* field_a */ frequencyModulationSource;
    char /* field_c */ frequencyModulationAmount;
    unsigned char /* field_d */ playMode; // TODO figure out bit patterns
    unsigned char /* field_e */ phase;
    unsigned char /* field_f */ smoothness;
};

// Size: 0xc
struct ModulationMatrixSettings {
    short /* field_0 */ modulationSource1;
    char /* field_2 */ modulationAmount1;
    short /* field_3 */ modulationAmount2;
    char /* field_5 */ modulationSource2;
    short /* field_6 */ modulationAmount3;
    char /* field_8 */ modulationSource3;
    short /* field_9 */ amplitudeModulationSource;
    char /* field_b */ amplitudeModulationAmount;
};

// Size: ???
struct FilterSettings {
    char /* field_0 */ cutoff;
    char /* field_1 */ resonance;
    char /* field_2 */ noteModulationAmount;
    short /* field_4 */ modulationSource1;
    char /* field_6 */ modulationAmount1;
    short /* field_7 */ modulationSource2;
    char /* field_9 */ modulationAmount2;
};

// Size: 0x9
struct AmpSettings {
    char /* field_0 */ envelopeAmplitudeModulationAmount;
    char /* field_1 */ stereoPanning;
    short /* field_2 */ amplitudeModulationSource;
    char /* field_4 */ amplitudeModulationAmount;
    short /* field_5 */ panningModulationSource;
    char /* field_7 */ panningModulationAmount;
    char /* field_8 */ saturation;
};

// Size: 0x18
struct SynthOscSettings {
    char /* field_0 */ octave;
    char /* field_1 */ semi;
    char /* field_2 */ fine;
    unsigned short /* field_3 */ wave;
    short /* field_5 */ modulationSource1;
    char /* field_7 */ modulationAmount1;
    short /* field_8 */ modulationSource2;
    char /* field_a */ modulationAmount2;

    // b c d

    char /* field_e */ level;
    char /* field_f */ enabled;
    short /* field_10 */ modulationSource3;
    char /* field_12 */ modulationAmount3;
    short /* field_13 */ modulationSource4;
    char /* field_15 */ modulationAmount4;

    // 16 17
};

// Size: still unknown
struct Synth_settings {
    // field_0
    // field_1

    char /* field_2 */ presetNameLength;
    char /* field_3 */ presetName[31];

    SynthOscSettings /* field_0x22 */ oscSettings[3];
    Env_settings /* field_0x6a */ envSettings[4];
    SynthLfoSettings /* field_0xa2*/ lfoSettings[4];
    ModulationMatrixSettings /* field_0xe2 */ modulationMatrixSettings[3];
    FilterSettings /* field_106 */ filterSettings; // probably [2]
    AmpSettings /* field_132 */ ampSettings;

    char /* field_13b */ muffle; // could also be options
    char /* field_13c */ field_13c;

    // here is still stuff missing

    char /* field_170 */ sync;
    char /* field_171 */ am;
    char /* field_172 */ mono;
    char /* field_173 */ glide;
    char /* field_174 */ restartVoice;
    char /* field_175 */ restartEnvelopes;
    char /* field_176 */ restartOscillators;
    char /* field_177 */ cycle;

    // here is still stuff missing

    char /* field_0x190 */ pitchBendRange;
    char /* field_0x191 */ pitchBendMode;
    unsigned char /* field_0x192 */ emuOptions; // emulation options (am bug etc.)
    char /* field_195 */ voiceStealingMode;
    unsigned char /* field_196 */ field_196; // more emulation options
};

// Size: 0xe8
struct SynthVoice {
    int /* 0x0 */ playing;
    int /* 0x4 */ field_4;
    int /* 0x8 */ field_8;
    int /* 0xc */ field_c; // settings Updated?
    int /* 0x10 */ voiceIndex;
    char /* 0x14 */ note;
    int /* 0x18 */ field_18;
    int /* 0x1c */ field_1c;
    int /* 0x20 */ group; // maybe this should be ant int // TODO
    Synth_settings* /* 0x24 */ settings;
    SynthVoice* /* 0x28 */ field_28;
    int /* 0x2c */ field_2c;
    int /* 0x30 */ field_30;
    int /* 0x38 */ field_34;
    int /* 0x38 */ field_38;
    float /* 0x3c */ ampGain;
    float /* 0x40 */ ampGainIncrement;
    int /* 0x44 */ fadeOutSamples;
    float /* 0x48 */ ampFadeOutIncrement;
    int /* 0x4c */ field_4c;
    float /* 0x50 */ ampAttackIncrement;
    SqLfo* /* 0x54 */ lfos[4];
    SqEnv* /* 0x64 */ envs[4];
    FilterSq* /* 0x74 */ filters[2];
    Amp* /* 0x7c */ amp;
    int /* 0x80 */ dca4Smoothing;
    int /* 0x84 */ field_84;
    int /* 0x88 */ field_88;
    ModFoll* /* 0x8c */ modFoll;
    int /* field_90 */ mPitchBend;
    int /* field_94 */ field_94;
    int /* field_98 */ modulation[16];
    int /* field_d8 */ field_d8;
    int /* field_dc */ field_dc;
    int /* field_e0 */ field_e0;
    int /* 0xe4 */ field_e4;
};

struct VoiceAssignment {
    int /* 0x0 */ playing;
    int /* 0x4 */ voiceIndex;
};

struct TriggerOptions {
    short /* 0x0 */ field_0; // semitones offset
    short /* 0x2 */ field_2;
    short /* 0x4 */ field_4; // used for amp amplitude
    short /* 0x6 */ field_6; // used for amp panning
    short /* 0x8 */ field_8; // used for lfo phase
    short /* 0xa */ field_a;
    SynthVoice* /* 0xc */ field_c; // stolenVoice / previousVoice
    unsigned int /* 0x10 */ voiceStealSoft; // voiceStealSoft
};

// Size: 0x10
struct MidiModulationSource {
    short /* 0x0 */ modulation;
    // 0x4
    // 0x8
    // 0xc
};

// Name: CplugMaster
// this class is unfortunately not an exact reversing from SQ80.dll as it is tangled with vst3 specific code
// Size: 0x12054
class Synth {
public:
    /* FUN_00461cac */ Synth(int sampleRate);
    /* FUN_00461eb4 */ ~Synth();

    void /* FUN_00461fa8 */ FUN_00461fa8();
    void /* FUN_00461fb0 */ FUN_00461fb0();

    void /* FUN_0046215c */ FUN_0046215c();
    void /* FUN_00462174 */ FUN_00462174();
    void /* FUN_004621fc */ FUN_004621fc();

    int /* FUN_00462214 */ setSampleRate(int sampleRate);
    void /* FUN_00462380 */ setUpdateRate(float updateRate);

    void /* FUN_00462488 */ FUN_00462488();
    void /* FUN_004624a8 */ FUN_004624a8(short* param_2, unsigned char note, bool mono);
    void /* FUN_004624f4 */ FUN_004624f4(short* param_2, unsigned char note);

    void /* FUN_00462544 */ initVoice(int voiceIndex);
    void /* FUN_00462570 */ initVoices();
    void /* FUN_004625d4 */ triggerVoice2(int voiceIndex, int voiceGroup, char param_4, TriggerOptions* triggerOptions, char param_6, char param_7, unsigned char velocity, char previousNote, char note);
    void /* FUN_00462c04 */ triggerVoice(int voiceIndex, int voiceGroup, char param_4, TriggerOptions* triggerOptions, char restartOscillators, char param_7, unsigned char velocity, int previousNote, int note);
    void /* FUN_00462ca0 */ setGlide(int voiceIndex, int note);
    void /* FUN_00462da4 */ setNoteModulation(int voiceIndex, int param_3);
    void /* FUN_00462de4 */ triggerRelease(int voiceIndex);
    void /* FUN_00462e30 */ stopVoice(int voiceIndex);
    void /* FUN_00462e6c */ stopVoices(Synth_settings* settings);
    void /* FUN_00462e9c */ releaseVoices(Synth_settings* settings);
    void /* FUN_00462ecc */ setFadeOut(int voiceIndex);
    void /* FUN_00462f18 */ setFadeIn(int voiceIndex);
    void /* FUN_00462f64 */ reserveVoice(int voiceIndex);
    void /* FUN_00462f90 */ freeVoice(int voiceIndex);
    SynthVoice* /* FUN_00462ffc */ getVoice(int voiceIndex);
    int /* FUN_0046300c */ selectVoice();
    int /* FUN_004630cc */ FUN_004630cc();
    void /* FUN_004632e4 */ setNumVoices(int numVoices);
    int /* FUN_00463328 */ getNumPlayingVoices();
    void /* FUN_00463358 */ setNumVoices(int param_2, int param_3);

    void /* FUN_00463388 */ triggerNote(unsigned char param_2, unsigned char note, unsigned char velocity);
    void /* FUN_00463420 */ triggerNote(unsigned char note, unsigned char velocity, TriggerOptions *triggerOptions, unsigned int voiceGroup);

    int /* FUN_004637c4 */ getSustainModulation();
    int /* FUN_004637d0 */ getMidiModulation(MidiModulationSource* modulationSource);
    int /* FUN_004637f0*/ getMidiModulation(int modulationIndex);
    int /* FUN_00463808 */ getModulation(SynthVoice* voice, int modulationSource);

    void /* FUN_00463890 */ updateState(SynthVoice* voice);

    void /* FUN_00464250 */ initMuffleParameters();
    void /* FUN_004643c8 */ setMuffle(bool muffle);

    bool /* FUN_00464410 */ getVoiceOutput(SynthVoice *voice, float output[2]);
    void /* FUN_004644f4 */ muffle(float output[2]);
    void /* FUN_004645c8 */ getSamples(bool reset, float *param_3, int numSamples, float *param_5);

private:
    SynthVoice /* FIELD_4 */ mVoices[16];
    int /* mField_e84 */ mNumVoices;
    VoiceAssignment /* FIELD_e88 */ mVoiceAssignments[16];
    int /* FIELD_f08 */ mNumUsedVoices;
    int /* FIELD_f0c */ mVoiceMapping[16];
    int /* FIELD_f4c */ mField_f4c;
    int /* FIELD_f50 */ mField_f50;
    short /* FIELD_f54 */ mField_f54[8]; // notes, might be actually [2][4]
    float /* FIELD_f64 */ mField_f64;
    int /* FIELD_f68 */ mField_f68;
    int /* FIELD_f6c */ mField_f6c;
    int /* FIELD_f70 */ mSampleRate;
    float /* FIELD_f74 */ mSamplesPerSecond;
    float /* FIELD_f78 */ mSampleRate2;
    float /* FIELD_f7c */ mUpdateRate;
    int /* FIELD_f80 */ mField_f80;
    int /* FIELD_f84 */ mField_f84;
    int /* FIELD_f88 */ mField_f88;
    int /* FIELD_f8c */ mField_f8c;

    // FIELD_f90

    bool /* FLOAT_f94 */ mMuffle;
    float /* FIELD_f98 */ mField_f98;
    float /* FIELD_f9c */ mField_f9c;
    float /* FIELD_fa0 */ mField_fa0;
    float /* FIELD_fa4 */ mField_fa4;
    float /* FIELD_fa8 */ mField_fa8;
    float /* FLOAT_fac */ mField_fac;
    float /* FLOAT_fb0 */ mField_fb0;
    float /* FLOAT_fb4 */ mField_fb4;
    float /* FLOAT_fb8 */ mField_fb8;
    float /* FLOAT_fbc */ mField_fbc;
    float /* FLOAT_fc0 */ mField_fc0;
    float /* FLOAT_fc4 */ mField_fc4;
    float /* FLOAT_fc8 */ mField_fc8;

    // FIELD_fcc
    // FIELD_fd0, FIELD_fd4 function pointer TODO find out the class types for these function pointers
    // FIELD_fd8, FIELD_fdc function pointer
    // FIELD_fe0

    int /* FIELD_fe4 */ mGlobalVoiceStealingMode; // 0 = no override, 1 = hard, 2 = soft
    int /* FIELD_fe8 */ mGlobalMuffle; // 0 = no override, 1 = muffle off, 2 = muffle on
    int /* FIELD_fec */ mGlobalDca13Smoothing; // 0 = no override, 1 = emu, 2 = hard
    int /* FIELD_ff0 */ mGlobalDca4Smoothing; // 0 = no override, 1 = emu, 2 = hard
    int /* FIELD_ff4 */ mGlobalDcBlockingFilter; // 0 = no override, 1 = smart, 2 = on, 3 = off

    // mField_ff8 is an editBuf
    // mField_ffc is a midiparser
    /*  FIELD_1000 */ Doc* mDoc;

    unsigned  int /* FIELD_11004 */ mField_11004;
    // mFIELD_11008
    // mFIELD_1100c

    MidiModulationSource /* FIELD_11010 */ mMidiModulation[131];
    MidiModulationSource /* FIELD_11840 */ mNoteModulation[128]; // TODO find out where these are initialized
    MidiModulationSource /* FIELD_12040 */ mPitchBendModulation; // TODO find out what this is

    bool /* FIELD_12050 */ mField_12050;


public:
    // Added
    Synth_settings* mSettings;
};

/*

FUN_00461cac 208 [x]
FUN_00461eb4 f4 [x]
FUN_00461fa8 8 [x]
FUN_00461fb0 8 [x]
FUN_00461fb8 20 wrapper for fd0
FUN_00461fd8 11 wrapper for fd8
FUN_00462004 40 wrapper for FUN_00461fd8
FUN_00462044 60
FUN_004620a4 40
FUN_004620e4 78 global emulations overrides
FUN_0046215c 18 [x]
FUN_00462174 88 [x]
FUN_004621fc 18 [x]
FUN_00462214 16c [x]
FUN_00462380 108 [x]
FUN_00462488 20 [x]
FUN_004624a8 4c [x]
FUN_004624f4 50 [x]
FUN_00462544 2c [x]
FUN_00462570 64 [x]
FUN_004625d4 630
FUN_00462c04 9c [x]
FUN_00462ca0 104 [x]
FUN_00462da4 40 [x]
FUN_00462de4 4c [x]
FUN_00462e30 3c [x]
FUN_00462e6c 30 [x]
FUN_00462e9c 30 [x]
FUN_00462ecc 4c [x]
FUN_00462f18 4c [x]
FUN_00462f64 2c [x]
FUN_00462f90 6c [x]
FUN_00462ffc 10 [x]
FUN_0046300c c0 [x]
FUN_004630cc 78 [x]
FUN_00463144 64 // weird
FUN_004631a8 18
FUN_004631c0 1c
FUN_004631dc 14
FUN_004631f0 40
FUN_00463230 5c
FUN_0046328c 2c
FUN_004632b8 c
FUN_004632c4 8
FUN_004632cc 8
FUN_004632d4 8
FUN_004632dc 8
FUN_004632e4 44 [x]
FUN_00463328 30 [x]
FUN_00463358 30 [x]
FUN_00463388 98 [x]
FUN_00463420 2f4 [x]
FUN_00463714 b0
FUN_004637c4 8 [x]
FUN_004637cc 4
FUN_004637d0 4 [x]
FUN_004637d4 1c
FUN_004637f0 18 [x]
FUN_00463808 88 [x]
FUN_00463890 9c0 [x]
FUN_00464250 178 [x]
FUN_004643c8 48 [x]
FUN_00464410 e4 [x]
FUN_004644f4 d4 [x]
FUN_004645c8 0 [x]

*/