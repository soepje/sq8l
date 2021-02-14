#pragma once

struct Osc {
    bool /* field_0 */ field_0;
    bool /* field_4 */ field_4;
    unsigned int /* field_8 */ wave;
    int /* field_c */ semi;
    int /* field_10 */ fine;
    unsigned int /* field_14 */ field_14;
    unsigned int /* field_18 */ field_18;
    int /* field_1c */ field_1c;
    unsigned int /* field_20 */ field_20;
    bool /* field_24 */ stopped;
    unsigned int field_28;
    float /* field_2c */ field_2c; // output possibly
    float /* field_30 */ field_30;
    float /* field_34 */ field_34;
    float /* field_38 */ field_38;
    unsigned int /* field_3c */ field_3c;
    unsigned int /* field_40 */ rom;
    unsigned int /* field_44 */ field_44;
    int /* field_48 */ field_48;
    unsigned int /* field_4c */ field_4c;
    bool /* field_50 */ field_50;
    unsigned int /* field_54 */ field_54; // sample end ???
    unsigned int /* field_58 */ field_58;
    unsigned int /* field_5c */ sample_index;
    unsigned int /* field_60 */ field_60;
    bool /* field_64 */ field_64;
    unsigned int /* field_68 */ sync_state;
    bool /* field_6c */ am_enabled;
};

struct Voice {
    Osc /* field_0 */ oscillators[3];

    bool /* field_150 */ am_enabled;
    unsigned int /* field_154 */ field_154; // counter ???
    unsigned int /* field_158 */ field_158;
    unsigned int /* field_15c */ field_15c;
    unsigned int /* field_160 */ field_160;
    unsigned int /* field_164 */ field_164;
    unsigned int /* field_168 */ field_168;
    float /* field_16c */ field_16c;
    float /* field_170 */ field_170;
    float /* field_174 */ field_174;
    float /* field_178 */ field_178;

    float /* field_180 */ field_180[4];

    unsigned int /* field_190 */ field_190;
    unsigned int /* field_194 */ field_194; // output buffer
    unsigned int /* field_198 */ field_198; // output buffer
    unsigned int /* field_19c */ field_19c; // output buffer
    unsigned int /* field_1a0 */ field_1a0; // output buffer
    unsigned int /* field_1a4 */ note;
    unsigned int /* field_1a8 */ glideNote;
    unsigned int /* field_1ac */ smoothingMode; // { 0 = no output ; 1 = emulated ; 2 = fast }
};

struct OscSettings {
    bool /* field_0 */ enabled;
    unsigned int /* field_4 */ wave;
    unsigned int /* field_8 */ level;
    int /* field_c */ semi;
    int /* field_10 */ fine;
    int /* field_14 */ field_14; // glide ???
    int /* field_18 */ field_18;
};

struct VoiceSettings {
    OscSettings /* field_0 */ oscSettings[3];

    bool /* field_48 */ field_48; // indicates whether this is the first time processing settings after trigger
    bool /* field_4c */ restartOsc;
    bool /* field_50 */ field_50; //
    bool /* field_54 */ triggered; // note triggered
    Voice* /* field_58 */ prevVoice;
    unsigned int /* field_5c */ note;
    unsigned int /* field_60 */ glideNote; // the note to glide from
    bool /* field_64 */ am_enabled;
    bool /* field_68 */ sync_enabled;
    bool /* field_6c */ am_bug_enabled;
    bool /* field_70 */ field_70;
    unsigned int /* field_74 */ field_74;
    unsigned int /* field_78 */ smoothingMode;
    bool /* field_7c */ field_7c;
};

//Cdoc
// Size: 0x28a0
class Doc {
private:
    unsigned int /* field_4 */ mNumVoices;
    Voice /* field_8 */ voices[16];

    unsigned char* /* field_2008 */ mWaves;
    float /* field_200c */ field_200c[2]; // hmm
    float /* field_2014 */ field_2014;
    float /* field_2018 */ field_2018;
    float /* field_201c */ field_201c;
    float /* field_2020 */ field_2020;
    float /* field_2024 */ field_2024;
    float /* field_2028 */ field_2028;
    float /* field_202c */ field_202c;
    float /* field_2030 */ field_2030;
    float /* field_2034 */ field_2034;
    float /* field_2038 */ field_2038;
    float /* field_203c */ field_203c;
    float /* field_2040 */ field_2040;
    float /* field_2044 */ field_2044;
    float /* field_2048 */ field_2048;
    float /* field_204c */ field_204c;
    float /* field_2050 */ field_2050;
    float /* field_2054 */ field_2054;
    float /* field_2058 */ field_2058;
    float /* field_205c */ field_205c;
    float /* field_2060 */ field_2060;
    float /* field_2064 */ field_2064;
    unsigned int /* field_2068 */ field_2068;
    float /* field_206c */ field_206c;
    float /* field_2070 */ field_2070;
    float /* field_2074 */ field_2074;
    float /* field_2078 */ field_2078;
    float /* field_207c */ field_207c;
    float /* field_2080 */ field_2080;
    float /* field_2084 */ field_2084;
    float /* field_2088 */ field_2088;
    float /* field_208c */ field_208c;
    float /* field_2090 */ mClockRate;
    float /* field_2094 */ mSampleRate;
    float /* field_2098 */ mSampleDuration;
    unsigned int /* field_209c */ mField_209c;
    VoiceSettings /* field_20a0 */ mVoiceSettings[16];

public:
    explicit Doc(unsigned int numVoices);
    ~Doc();
    void init();
    void initVoice(unsigned int voiceIndex);
    void initVoices();
    void initVoices2();
    void setNumVoices(unsigned int numVoices);
    void setClockRate(float clockRate);


    void setSampleRate(float sampleRate);
    void triggerVoice(unsigned voiceIndex, unsigned int note, int prevVoiceIndex, bool param_5,
                                       bool restartOsc);
    void setGlideStart(unsigned int voiceIndex, unsigned int note, bool trigger);
    void stopVoice(unsigned int voiceIndex);
    void updateVoiceSettings(unsigned int voiceIndex);
    void updateSmoothingMode(unsigned int voiceIndex);
    void setGlideStart(unsigned int voiceIndex, unsigned int note);

    float getVolume(unsigned int volumeIndex);
    void fun_45c5ec(Voice *voice, unsigned int oscIndex, float param_4);
    void updateClockRate();
    void updateSampleRate();
    void resetVoiceOutput(Voice *voice);
    VoiceSettings * getVoiceSettings(unsigned int voiceIndex);
    float getSample(unsigned int voiceIndex);
    void FUN_0045c4f8(int note, unsigned int wave, unsigned int *param_4, char **param_5);
    unsigned int field_209c() { return mField_209c; }
};

void fun_45c470(int* semi, unsigned char* wsr);
void fun_45c49c(unsigned int note, unsigned int glide_note, int* param_4, unsigned char* wsr, unsigned char* page, unsigned int wave, int fine, int semi);
unsigned int fun_45c55c(int param_2);
unsigned int getVolume2(int pitchIndex);