#include <cstdlib>
#include <cstring>
#include <cmath>

#include "Doc.h"
#include "Data.h"
#include "SoundLib.h"

#include "../data/Data.h"

unsigned int /* FUN_0045b8b4 */ Doc::pitchToFrequency(int pitch) {
    while (pitch >= 0x8000) {
        pitch -= 0xc00;
    }

    if (pitch < 0) {
        pitch = 0;
    } else {
        pitch /= 8;
    }

    unsigned int frequency;
    if ((pitch & 1) == 0) {
        pitch >>= 1;

        if (pitch >= 0x800) {
            frequency = docFrequencyCurve[0x800];
        } else {
            frequency = docFrequencyCurve[pitch];
        }
    } else {
        pitch >>= 1;

        if (pitch >= 0x800) {
            frequency = docFrequencyCurve[0x800];
        } else {
            frequency = (docFrequencyCurve[pitch] + docFrequencyCurve[pitch + 1]) / 2;
        }
    }

    return frequency;
}

/* FUN_0045b9e0 */ Doc::Doc(unsigned int numVoices) {
    setNumVoices(numVoices);

    mWaves = sq8l::data::waverom;

    setClockRate(38455.85546875);
}

/* FUN_0045ba78 */ Doc::~Doc() {

}

void /* FUN_0045bb54 */ Doc::initVoice(unsigned int voiceIndex) {
    if (voiceIndex < 16) {
        memset(&mVoiceSettings[voiceIndex], 0, sizeof(VoiceSettings));
        memset(&voices[voiceIndex], 0, sizeof(Voice));
        
        Voice* voice = &voices[voiceIndex];

        voice->smoothingMode = 2;

        for (int i = 0; i < 3; i++) {
            Osc* osc = &voice->oscillators[i];

            osc->field_34 = getVolume(0);
            osc->field_38 = osc->field_34;
            osc->level = osc->field_34;

            setLevel(voice, i, osc->field_34);

            osc->stopped = true;
        }
    }
}

void /* FUN_0045bc00 */ Doc::initVoices() {
    for (int i = 0; i < 16; i++) {
        initVoice(i);
    }
}

void /* FUN_0045bc18 */ Doc::initVoices2() {
    initVoices();
}

void /* FUN_0045bc20 */ Doc::setNumVoices(unsigned int numVoices) {
    if (numVoices > 16) {
        numVoices = 16;
    }

    mNumVoices = numVoices;
}

void /* FUN_0045bc34 */ Doc::setClockRate(float clockRate) {
    mClockRate = clockRate;

    updateLevels();
}

void /* FUN_0045bc4c */ Doc::setSampleRate(float sampleRate) {
    mSampleRate = sampleRate;
    mSampleDuration = 1.0f / mSampleRate;
    field_2068 = floor((mClockRate / mSampleRate) * (2 << 29)); // TODO check floor
    mTailSamples = round(mSampleRate * 0.0975); // TODO check round

    updateDcBlockCoefficients();
}

void /* FUN_0045bcd0 */ Doc::triggerVoice(unsigned voiceIndex, unsigned int note, int prevVoiceIndex, bool param_5, bool restartOsc) {
    if (voiceIndex < mNumVoices) {
        VoiceSettings* voiceSettings = &mVoiceSettings[voiceIndex];

        voiceSettings->field_48 = true;
        voiceSettings->restartOsc = restartOsc;
        voiceSettings->field_50 = param_5;
        voiceSettings->triggered = true;

        if (prevVoiceIndex >= 0 && prevVoiceIndex < 16) {
            voiceSettings->prevVoice = &voices[prevVoiceIndex];
        } else {
            voiceSettings->prevVoice = nullptr;
        }

        voiceSettings->note = note;
        voiceSettings->glideNote = note;

        for (int i = 0; i < 3; i++) {
            voiceSettings->oscSettings[i].pitchModulation = 0;
        }
    }
}

void /* FUN_0045bd44 */ Doc::setGlideStart(unsigned int voiceIndex, unsigned int note, bool trigger) {
    if (voiceIndex < mNumVoices) {
        VoiceSettings* voiceSettings = &mVoiceSettings[voiceIndex];

        if (trigger) {
            voiceSettings->note = note;
            voiceSettings->triggered = true;
        }

        voiceSettings->glideNote = note;

        updateVoiceSettings(voiceIndex);
    }
}

void /* FUN_0045bd78 */ Doc::stopVoice(unsigned int voiceIndex) {
    if (voiceIndex < mNumVoices) {
        for (int i = 0; i < 3; i++) {
            voices[voiceIndex].oscillators[i].stopped = true;
        }

        voices[voiceIndex].field_158 = 0;
    }
}

void /* FUN_0045bda4 */ Doc::updateVoiceSettings(unsigned int voiceIndex) {
    if (voiceIndex < mNumVoices) {
        Voice* voice = &voices[voiceIndex];
        VoiceSettings* voiceSettings = &mVoiceSettings[voiceIndex];

        if (voiceSettings->am_enabled) {
            voiceSettings->oscSettings[0].enabled = true;
            voiceSettings->oscSettings[0].level = 0x7f00;
            voiceSettings->oscSettings[1].enabled = true;
            voiceSettings->oscSettings[1].level = 0x7f00;
        }

        voice->am_enabled = voiceSettings->am_enabled;

        if (voiceSettings->restartOsc) {
            voice->field_154 = 0;
        }

        voice->field_158 = voiceSettings->field_70;

        if (voiceSettings->field_50) {
            if (voiceSettings->dcBlock) {
                clearDcBlockBuffer(voice);

                voice->dcBlock = voiceSettings->dcBlock;
            } else {
                voice->dcBlock = false;
            }
        }

        bool update = voiceSettings->triggered || voice->note != voiceSettings->note ||
                      voice->glideNote != voiceSettings->glideNote;

        if (update) {
            voice->note = voiceSettings->note;
            voice->glideNote = voiceSettings->glideNote;
        }

        if (!voiceSettings->field_48) {
            voice->smoothingMode = voiceSettings->smoothingMode;
        } else {
            voice->smoothingMode = 0;
        }

        for (int i = 0; i < 3; i++) {
            OscSettings* osc_settings = &voiceSettings->oscSettings[i];
            Osc* osc = &voice->oscillators[i];

            if (osc_settings->enabled || (i == 0 && voiceSettings->oscSettings[1].enabled &&
                                          (voiceSettings->am_enabled || voiceSettings->sync_enabled))) {

                bool update_osc_params = update || !osc->field_4 || osc->wave != osc_settings->wave ||
                    osc->semi != osc_settings->semi || osc->fine != osc_settings->fine ||
                    (i == 1 && voiceSettings->sync_enabled != (osc->sync_state == 2));

                int pitch = 0;
                unsigned char wsr = 0;
                unsigned char page = 0;

                if (update_osc_params) {
                    osc->wave = osc_settings->wave;
                    osc->semi = osc_settings->semi;
                    osc->fine = osc_settings->fine;

                    fun_45c49c(voice->note, voice->glideNote, &pitch, &wsr, &page,
                               osc_settings->wave, osc_settings->fine, osc_settings->semi);
                } else {
                    pitch = osc->field_14;
                }

                bool local_24 = voiceSettings->restartOsc;
                unsigned int local_28; // osc play mode

                // im not too sure about this next part
                if (update_osc_params) {
                    if (i == 1) {
                        if (voiceSettings->sync_enabled) {
                            local_28 = 2;
                        } else {
                            if (voiceSettings->am_enabled) {
                                local_28 = 0;
                                if (osc_settings->wave >= 0x36) {
                                    local_24 = true;
                                }
                            } else {
                                if (osc_settings->wave < 0x36) {
                                    local_28 = 0;
                                } else {
                                    local_28 = 1;
                                    local_24 = true;
                                }
                            }
                        }
                    } else {
                        if (osc_settings->wave < 0x36) {
                            local_28 = 0;
                        }
                        else {
                            local_28 = 1;
                            local_24 = true;
                        }
                    }
                } else {
                    local_28 = osc->sync_state; // ???
                }

                bool local_2c;
                int local_30 = 0; // TODO

                if (i == 0) {
                    local_30 = wsr;
                    local_2c = false;
                } else if (i == 1) {
                    local_2c = voiceSettings->am_enabled;
                } else {
                    local_2c = false;
                }

                osc->field_0 = osc_settings->enabled;
                osc->field_4 = true;
                osc->field_14 = pitch;

                pitch += osc_settings->pitchModulation;

                if (pitch < 0) {
                    pitch = 0;
                }

                osc->field_18 = pitch;
                osc->field_1c = osc_settings->pitchModulation;
                osc->field_20 = osc_settings->level;
                osc->field_60 = getFrequency(pitch);

                if (local_24) {
                    osc->sample_index = 0;
                    osc->field_64 = false;
                } else {
                    if (voiceSettings->field_48 && voiceSettings->prevVoice != NULL) {
                        osc->sample_index = voiceSettings->prevVoice->oscillators[i].sample_index;
                        osc->field_64 = voiceSettings->prevVoice->oscillators[i].field_64;
                    }
                }

                osc->stopped = false;

                if (update_osc_params) {
                    osc->rom = page;
                    osc->sync_state = local_28;

                    if (local_2c && voiceSettings->am_bug_enabled) {
                        osc->am_enabled = true;
                        osc->field_3c = ((wsr & 0x40) >> 5) | (local_30 >> 7);
                    } else {
                        osc->am_enabled = false;
                        osc->field_3c = ((wsr & 0x40) >> 5) | (wsr >> 7);
                    }

                    osc->field_48 = ((wsr & 0x38) >> 3) + 8;
                    osc->field_4c = 1 << osc->field_48;
                    osc->field_50 = wsr & 7;
                    osc->field_44 = osc->field_3c * 0x10000 + osc->rom * 0x100;
                    osc->field_54 = (1 << ((osc->field_50 + 0x11) & 0x1f)) - 1;
                    osc->field_58 = (osc->field_50 + 0x11) - osc->field_48;
                }

                float volume = getVolume(osc_settings->level);
                
                if (!voiceSettings->field_48) {
                    if (voice->smoothingMode == 1) {
                        setLevel(voice, i, osc->field_34);

                        osc->field_38 = osc->field_34;
                        osc->field_34 = volume;
                    } else if (voice->smoothingMode == 2) {
                        setLevel(voice, i, volume);

                        osc->field_38 = osc->field_34;
                        osc->field_34 = volume;
                    } else {
                        setLevel(voice, i, volume);
                    }
                } else {
                    setLevel(voice, i, volume);

                    osc->field_34 = volume;
                    osc->field_38 = volume;
                }
            } else {
                osc->field_0 = false;
                osc->field_4 = false;
                osc->semi = 0;
                osc->fine = 0;
                osc->field_14 = 0;
                osc->field_18 = 0;
                osc->field_1c = 0;
                osc->field_20 = 0;
                osc->stopped = true;
            }
        }

        // this next bit has something to do with the highest pitch/octave of the oscs
        // find highest playing pitch ???
        unsigned int z = 0;

        for (int i = 0; i < 3; i++) {
            Osc* osc = &voice->oscillators[i];

            if (!osc->stopped) {
                unsigned int y = osc->field_60 << ((osc->field_48 - 1) - osc->field_50);

                if (y > z) {
                    z = y;
                }
            }
        }

        unsigned int a = 1;
        float b[] = { 576717.0, 1631846.0, 6918636.0 };

        for (int i = 0; i < 3; i++) {
            if (z < b[i]) {
                break;
            }

            a = i + 1;
        }

        voice->field_160 = a;

        voiceSettings->field_48 = false;
        voiceSettings->restartOsc = false;
        voiceSettings->field_50 = false;
        voiceSettings->triggered = false;
    }
}

void /* FUN_0045c378 */ Doc::updateSmoothingMode(unsigned int voiceIndex) {
    if (voiceIndex < mNumVoices) {
        Voice* voice = &voices[voiceIndex];

        if (voice->smoothingMode > 0) {
            for (int i = 0; i < 3; i++) {
                Osc* osc = &voice->oscillators[i];

                if (!osc->stopped) {
                    if (voice->smoothingMode == 1) {
                        setLevel(voice, i, (osc->field_34 + osc->field_38) * 0.5);
                    } else if (voice->smoothingMode == 2) {
                        float local_10 = (osc->field_34 * 3.0 - osc->field_38) * 0.0;

                        if (local_10 < 0.0) {
                            local_10 = 0.0;
                        } else if (local_10 > 1.0) {
                            local_10 = 1.0;
                        }

                        setLevel(voice, i, local_10);
                    }
                }
            }
        }
    }
}

void /* FUN_0045c460 */ Doc::setGlideStart(unsigned int voiceIndex, unsigned int note) {
    setGlideStart(voiceIndex, note, false);
}

void /* FUN_0045c470 */ Doc::fun_45c470(int* semi, unsigned char* wsr) {
    *semi = *semi + 12;

    if (*semi < 12) {
        for (int i = 0; i < 3; i++) {
            *wsr = *wsr + 1;
            *semi = *semi + 12;

            if (*semi >= 12) {
                break;
            }
        }
    } else if (*semi >= 36) {
        *wsr = *wsr - 1;
        *semi = *semi - 12;
    }
}

void /* FUN_0045c49c */ Doc::fun_45c49c(unsigned int note, unsigned int glide_note, int* pitch, unsigned char* wsr, unsigned char* page, unsigned int wave, int fine, int semi) {
    unsigned int /* local_10 */ fine_out;
    int /* local_c */ semi_out;

    getWaveParameters(wave, note + semi, page, &fine_out, &semi_out, wsr);

    semi += semi_out;

    fun_45c470(&semi, wsr);

    *pitch = (glide_note + semi) * 256 + fine * 8 + fine_out;
}

// TODO test this function
void /* FUN_0045c4f8 */ Doc::getWaveData(int note, unsigned int wave, unsigned int *waveDataSize, unsigned char **waveData) {
    unsigned char page;
    unsigned char wsr;

    getWaveParameters(wave, note, &page, &wsr);

    unsigned char x = ((wsr & 0x40) >> 5) | (wsr >> 7);

    *waveDataSize = ((wsr & 0x38) >> 3) + 8;
    *waveData = &mWaves[((x * 65536) + page * 256) & 0x3ffff];
}

unsigned int /* FUN_0045c55c */ Doc::getFrequency(int pitchIndex) {
    return pitchToFrequency(pitchIndex - 16);
}

float /* FUN_0045c568 */ Doc::getVolume(int volumeIndex) {
    float volume;
    
    if (volumeIndex < 0) {
        volume = docVolumeCurve[0];
    } else {
        unsigned int x = volumeIndex >> 7;

        if (x > 0xff) {
            x = 0xff;
        }

        if ((volumeIndex & 0x7f) == 0) {
            volume = docVolumeCurve[x];
        } else {
            unsigned int y = x + 1;

            if (y > 0xff) {
                y = 0xff;
            }

            volume = (docVolumeCurve[y] - docVolumeCurve[x]) * (volumeIndex & 0x7f) * field_2024 + docVolumeCurve[x];
        }
    }

    return volume;
}

void /* FUN_0045c5ec */ Doc::setLevel(Voice* voice, unsigned int oscIndex, float level) {
    Osc* osc = &voice->oscillators[oscIndex];

    if (oscIndex == 1 && voice->am_enabled) {
        osc->targetLevel = level * mLevelAmInv;
    } else {
        osc->targetLevel = level * mLevelAm;
    }
}


void /* FUN_0045c628 */ Doc::updateLevels() {
    mLevel = pow(0.01, 1.0 / (mClockRate * 0.002 + 1.0));
    mLevelAm = 1 - mLevel;
    mLevelInv = pow(0.01, 1.0 / (mClockRate * 0.0002 + 1.0));
    mLevelAmInv = 1 - mLevelInv;
}

void /* FUN_0045c6dc */ Doc::updateDcBlockCoefficients() {
    float theta = mSampleDuration * 30.0f * M_PI;
    float a = std::cos(theta);
    float b = std::sin(theta) / 1.41422f;
    float c = b + 1.0f;

    mDcBlockCoefficients[0] = ((a + 1.0f) * 0.5f) / c;
    mDcBlockCoefficients[1] = -(a + 1.0f) / c;
    mDcBlockCoefficients[2] = ((a + 1.0f) * 0.5f) / c;
    mDcBlockCoefficients[3] = -(a * 2.0f) / c;
    mDcBlockCoefficients[4] = (1.0f - b) / c;
}

void /* FUN_0045c7bc */ Doc::clearDcBlockBuffer(Voice* voice) {
    voice->dcBlockBuf[0] = 0;
    voice->dcBlockBuf[1] = 0;
    voice->dcBlockBuf[2] = 0;
    voice->dcBlockBuf[3] = 0;
}

VoiceSettings* /* FUN_0045c7e0 */ Doc::getVoiceSettings(unsigned int voiceIndex) {
    if (voiceIndex >= mNumVoices) {
        return nullptr;
    }

    return &mVoiceSettings[voiceIndex];
}

float /* FUN_0045c7f4 */ Doc::getSample(unsigned int voiceIndex) {
    if (voiceIndex >= mNumVoices) {
        return 0.0;
    }

    Voice* voice = &voices[voiceIndex];

    if (voice->field_164 <= voice->field_168) {
        int sample_osc1 = 0;
        int sample_osc2 = 0;
        int sample_osc3 = 0;

        Osc* osc1 = &voice->oscillators[0];
        Osc* osc2 = &voice->oscillators[1];
        Osc* osc3 = &voice->oscillators[2];

        if (!osc1->stopped) {
            unsigned char sample = mWaves[(osc1->sample_index >> (osc1->field_58 & 0x1f)) + (osc1->field_44 & 0x3ffff)];
        
            if (sample == 0) {
                osc1->stopped = true;
            } else {
                sample_osc1 = sample - 0x80;

                if (osc1->field_64) {
                    osc1->field_64 = false;

                    if (osc2->sync_state == 2) {
                        osc2->sample_index = 0;
                    }

                    osc1->stopped = osc1->sync_state == 1;
                }

                unsigned int next_sample_index = osc1->sample_index + osc1->field_60;
                unsigned int field_54 = osc1->field_54;

                if (next_sample_index >= field_54) {
                    next_sample_index &= field_54;
                    osc1->field_64 = true;
                }

                osc1->sample_index = next_sample_index;
            }
        }

        if (!osc2->stopped) {
            unsigned char sample = mWaves[(osc2->sample_index >> (osc2->field_58 & 0x1f)) + (osc2->field_44 & 0x3ffff)];
        
            if (sample == 0) {
                osc2->stopped = true;
            } else {
                sample_osc2 = (int) sample - 0x80;

                if (osc2->field_64) {
                    osc2->field_64 = false;
                    osc2->stopped = osc2->sync_state == 1;
                }

                unsigned int next_sample_index = osc2->sample_index + osc2->field_60;
                unsigned int field_54 = osc2->field_54;

                if (next_sample_index >= field_54) {
                    next_sample_index &= field_54;
                    osc2->field_64 = true;
                }

                osc2->sample_index = next_sample_index;
            }
        }

        if (!osc3->stopped) {
            unsigned int sample = mWaves[(osc3->sample_index >> (osc3->field_58 & 0x1f)) + (osc3->field_44 & 0x3ffff)];
        
            if (sample == 0) {
                osc3->stopped = true;
            } else {
                sample_osc3 = sample - 0x80;

                if (osc3->field_64) {
                    osc3->field_64 = false;
                    osc3->stopped = osc3->sync_state == 1;
                }

                unsigned int next_sample_index = osc3->sample_index + osc3->field_60;
                unsigned int field_54 = osc3->field_54;

                if (next_sample_index >= field_54) {
                    next_sample_index &= field_54;
                    osc3->field_64 = true;
                }

                osc3->sample_index = next_sample_index;
            }
        }

        float var_12;
        if (voice->am_enabled) {
            var_12 = sample_osc2 * field_2024 * osc2->level;
            var_12 += sample_osc3 * field_2024 * osc3->level;

            osc2->level = mLevelInv * osc2->level + (sample_osc2 * field_201c + field_2020) * mLevelAmInv;
            osc3->level = mLevel * osc3->level + osc3->targetLevel;
        } else {
            var_12 = (osc1->field_0) ? sample_osc1 * field_2024 * osc1->level : 0.0;
            var_12 += sample_osc2 * field_2024 * osc2->level;
            var_12 += sample_osc3 * field_2024 * osc3->level;

            osc1->level = mLevel * osc1->level + osc1->targetLevel;
            osc2->level = mLevel * osc2->level + osc2->targetLevel;
            osc3->level = mLevel * osc3->level + osc3->targetLevel;
        }

        // some random filter
        voice->field_16c = voice->field_170;
        voice->field_170 = voice->field_174;
        voice->field_174 = voice->field_178;
        voice->field_178 = var_12 + field_200c[voice->field_15c];

        voice->field_15c = (voice->field_15c + 1) % 2;

        if (voice->field_160 == 1) {
            voice->field_180[0] = voice->field_170 - voice->field_16c;
        } else if (voice->field_160 == 2) {
            voice->field_180[0] =
                voice->field_170 * field_203c -
                (voice->field_16c * field_2034 +
                voice->field_174 * field_202c);
            voice->field_180[1] =
                (voice->field_16c + voice->field_174) * 
                field_202c - voice->field_170;
        } else if (voice->field_160 == 3) {
            voice->field_180[0] =
                voice->field_180[0] +
                voice->field_180[1] * field_203c +
                voice->field_180[2] * field_2044;

            if (fabsf(voice->field_180[0]) < field_2018) {
                voice->field_180[1] = voice->field_170 * field_203c -
                    (voice->field_174 * field_2028 + 
                    voice->field_16c * field_2038);
                voice->field_180[2] = (voice->field_174 * field_2028 + 
                    voice->field_16c * field_2030) -
                    voice->field_170;
            } else {
                voice->field_180[1] = 
                    (voice->field_170 * field_203c -
                    (voice->field_174 * field_2028 + 
                    voice->field_16c * field_2038)) -
                    voice->field_180[0] * field_2034;
                voice->field_180[2] =
                    voice->field_180[0] * field_202c +
                    ((voice->field_174 * field_2028 + 
                    voice->field_16c * field_2030) -
                    voice->field_170);
            }
        } else if (voice->field_160 == 4) {
            voice->field_180[0] =
                voice->field_180[0] +
                voice->field_180[1] * field_203c +
                voice->field_180[2] * field_2044 +
                voice->field_180[3] * field_2048;

            if (fabsf(voice->field_180[0]) < field_2018) {
                voice->field_180[1] =
                    voice->field_178 * field_2050 +
                    (voice->field_170 * field_2044 -
                    (voice->field_174 * field_2030 +
                    voice->field_16c * field_2064));
                voice->field_180[2] =
                    ((voice->field_174 +
                    voice->field_16c * field_2058) -
                    voice->field_170 * field_2040) -
                    voice->field_178 * field_204c;
                voice->field_180[3] =
                    voice->field_178 * field_2054 +
                    (voice->field_170 * field_202c -
                    (voice->field_174 * field_2028 +
                    voice->field_16c * field_205c));
            } else {
                voice->field_180[1] =
                    (voice->field_178 * field_2050 +
                    (voice->field_170 * field_2044 -
                    (voice->field_174 * field_2030 +
                    voice->field_16c * field_2064))) -
                    voice->field_180[0] * field_2060;
                voice->field_180[2] =
                    voice->field_180[0] +
                    (((voice->field_174 +
                    voice->field_16c * field_2058) -
                    voice->field_170 * field_2040) -
                    voice->field_178 * field_204c);
                voice->field_180[3] =
                    (voice->field_178 * field_2054 +
                    (voice->field_170 * field_202c -
                    (voice->field_174 * field_2028 +
                    voice->field_16c * field_205c))) -
                    (voice->field_180[0] * field_204c);
            }
        }
    }

    float var_2 = voice->field_164 * field_2014;
    float var_3 = var_2;

    float output = voice->field_16c;

    for (int i = 0; i < voice->field_160; i++) {
        output += voice->field_180[i] * var_3;
        var_3 *= var_2;
    }

    voice->field_168 = voice->field_164;
    voice->field_164 = (voice->field_164 + field_2068) & 0x3fffffff;

    // dc offset blocking filter
    if (voice->dcBlock) {
        float filteredOutput = ((mDcBlockCoefficients[0] * output +
                                 mDcBlockCoefficients[1] * voice->dcBlockBuf[0] +
                                 mDcBlockCoefficients[2] * voice->dcBlockBuf[1]) -
                                (mDcBlockCoefficients[3] * voice->dcBlockBuf[2])) -
                               (mDcBlockCoefficients[4] * voice->dcBlockBuf[3]);

        voice->dcBlockBuf[1] = voice->dcBlockBuf[0];
        voice->dcBlockBuf[3] = voice->dcBlockBuf[2];
        voice->dcBlockBuf[0] = output;
        voice->dcBlockBuf[2] = filteredOutput;

        mOutput = output;

        return filteredOutput;
    }

    mOutput = output;

    return output;
}
