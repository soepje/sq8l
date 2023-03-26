#pragma once

// Size: 0x70
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
    float /* field_2c */ targetLevel;
    float /* field_30 */ level;
    float /* field_34 */ field_34;
    float /* field_38 */ field_38;
    unsigned int /* field_3c */ field_3c;
    unsigned int /* field_40 */ rom;
    unsigned int /* field_44 */ field_44; // rom Offset
    int /* field_48 */ field_48;
    unsigned int /* field_4c */ field_4c;
    int /* field_50 */ field_50;
    
    unsigned int /* field_54 */ field_54; // max sample index
    unsigned int /* field_58 */ field_58; // sample index shift
    unsigned int /* field_5c */ sample_index;
    unsigned int /* field_60 */ field_60; // sample index increment
    bool /* field_64 */ field_64; // sample index wraparound

    unsigned int /* field_68 */ sync_state; // playmode (0=regular, 1=oneshot, 2=sync)
    bool /* field_6c */ am_enabled;
};

// Size: 0x200
struct Voice {
    Osc /* field_0 */ oscillators[3];

    bool /* field_150 */ am_enabled;
    unsigned int /* field_154 */ field_154; // counter ???
    unsigned int /* field_158 */ field_158;
    unsigned int /* field_15c */ field_15c;
    unsigned int /* field_160 */ field_160;
    unsigned int /* field_164 */ field_164;
    unsigned int /* field_168 */ field_168;


    float /* field_16c */ field_16c; // filterBuf[4]
    float /* field_170 */ field_170;
    float /* field_174 */ field_174;
    float /* field_178 */ field_178;

    float /* field_180 */ field_180[4];

    bool /* field_190 */ dcBlock;
    float /* field_194 */ dcBlockBuf[4];

    unsigned int /* field_1a4 */ note;
    unsigned int /* field_1a8 */ glideNote;
    unsigned int /* field_1ac */ smoothingMode; // { 0 = no output ; 1 = emulated ; 2 = fast }
};

struct OscSettings {
    bool /* field_0 */ enabled;
    unsigned int /* field_4 */ wave;
    int /* field_8 */ level;
    int /* field_c */ semi;
    int /* field_10 */ fine;
    int /* field_14 */ pitchModulation;
    int /* field_18 */ field_18;
};

// Size: 0x80
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
    bool /* field_7c */ dcBlock;
};

//Cdoc
// Size: 0x28a0
class Doc {
private:
    unsigned int /* field_4 */ mNumVoices;
    Voice /* field_8 */ voices[16];

    unsigned char* /* field_2008 */ mWaves;

    const float /* field_200c */ field_200c[2] = { 1e-10, -1e-10 };
    const float /* field_2014 */ field_2014 = 9.313226e-10;
    const float /* field_2018 */ field_2018 = 0.001;

    const float /* field_201c */ field_201c = 0.00472441;
    const float /* field_2020 */ field_2020 = 0.4;
    const float /* field_2024 */ field_2024 = 0.00787402; // 1/127

    const float /* field_2028 */ field_2028 = 0.25;
    const float /* field_202c */ field_202c = 0.5;
    const float /* field_2030 */ field_2030 = 0.75;
    const float /* field_2034 */ field_2034 = 1.5;
    const float /* field_2038 */ field_2038 = 1.75;
    const float /* field_203c */ field_203c = 2.0;
    const float /* field_2040 */ field_2040 = 2.5;
    const float /* field_2044 */ field_2044 = 3.0;
    const float /* field_2048 */ field_2048 = 4.0;
    const float /* field_204c */ field_204c = 0.16666667; // 1/6
    const float /* field_2050 */ field_2050 = 0.11111111; // 1/9
    const float /* field_2054 */ field_2054 = 0.05555556; // 1/18
    const float /* field_2058 */ field_2058 = 1.66666663; // 5/3
    const float /* field_205c */ field_205c = 0.30555555; // 11/36
    const float /* field_2060 */ field_2060 = 1.83333337; // 11/6
    const float /* field_2064 */ field_2064 = 2.36111116; // 85/36

    unsigned int /* field_2068 */ field_2068; // clock ratio

    float /* field_206c */ mDcBlockCoefficients[5];

    float /* field_2080 */ mLevel;
    float /* field_2084 */ mLevelInv;
    float /* field_2088 */ mLevelAm;
    float /* field_208c */ mLevelAmInv;

    float /* field_2090 */ mClockRate;
    float /* field_2094 */ mSampleRate;
    float /* field_2098 */ mSampleDuration;
    unsigned int /* field_209c */ mTailSamples;
    VoiceSettings /* field_20a0 */ mVoiceSettings[16];

    // for debugging
    float mOutput = 0.0;

public:
    static unsigned int pitchToFrequency(int pitch);

    explicit Doc(unsigned int numVoices);
    ~Doc();
    void init(); // FUN_0045baac
    void initVoice(unsigned int voiceIndex);
    void initVoices();
    void initVoices2();
    void setNumVoices(unsigned int numVoices);
    void setClockRate(float clockRate);
    void setSampleRate(float sampleRate);
    void triggerVoice(unsigned voiceIndex, unsigned int note, int prevVoiceIndex, bool param_5, bool restartOsc);
    void setGlideStart(unsigned int voiceIndex, unsigned int note, bool trigger);
    void stopVoice(unsigned int voiceIndex);
    void updateVoiceSettings(unsigned int voiceIndex);
    void updateSmoothingMode(unsigned int voiceIndex);
    void setGlideStart(unsigned int voiceIndex, unsigned int note);

    void fun_45c470(int* semi, unsigned char* wsr);
    void getWaveParameters(unsigned int note, unsigned int glide_note, int* pitch, unsigned char* wsr, unsigned char* page, unsigned int wave, int fine, int semi);
    void getWaveData(int note, unsigned int wave, unsigned int *waveDataSize, unsigned char **waveData);

    unsigned int getFrequency(int pitchIndex);
    float getVolume(int volumeIndex);
    void setLevel(Voice *voice, unsigned int oscIndex, float level);
    void updateLevels();
    void updateDcBlockCoefficients();
    void clearDcBlockBuffer(Voice *voice);
    VoiceSettings* getVoiceSettings(unsigned int voiceIndex);
    float getSample(unsigned int voiceIndex);
    unsigned int getTailSamples() { return mTailSamples; }
};
