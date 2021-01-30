#include <cstdlib>
#include <cstring>
#include <cmath>

#include "Doc.h"
#include "Data.h"
#include "SoundLib.h"
#include "PackDlt.h"

unsigned int /* FUN_0045b8b4 */ getVolume2(int pitchIndex) {
    while (pitchIndex >= 0x8000) {
        pitchIndex -= 0xc00;
    }

    if (pitchIndex < 0) {
        pitchIndex = 0;
    } else {
        pitchIndex /= 8;
    }

    unsigned int volume;
    if ((pitchIndex & 1) == 0) {
        pitchIndex >>= 1;

        if (pitchIndex >= 0x800) {
            volume = docVolumeCurve2[0x800];
        } else {
            volume = docVolumeCurve2[pitchIndex];
        }
    } else {
        pitchIndex >>= 1;

        if (pitchIndex >= 0x800) {
            volume = docVolumeCurve2[0x800];
        } else {
            volume = (docVolumeCurve2[pitchIndex] + docVolumeCurve2[pitchIndex + 1]) / 2;
        }
    }

    return volume;
}

/* FUN_0045b9e0 */ Doc::Doc(unsigned int numVoices) {
    setNumVoices(numVoices);

    int outputBufferLength;

    mWaves = nullptr;

    PackDlt36 pack = PackDlt36();
    pack.unpack(docPackedWaves, 0x2b429, &outputBufferLength, &mWaves);

    init();
    setClockRate(38455.85546875);
}

/* FUN_0045ba78 */ Doc::~Doc() {
    if (mWaves != nullptr) {
        free(mWaves);
    }
}

void /* FUN_0045baac */ Doc::init() {
    field_200c[0] = 0.0;
    field_200c[1] = 0.0;
    field_2014 = 0.0;
    field_2018 = 0.001;
    field_201c = 0.00472441;
    field_2020 = 0.4;
    field_2024 = 0.00787402; // 1/127
    field_2028 = 0.25;
    field_202c = 0.5;
    field_2030 = 0.75;
    field_2034 = 1.5;
    field_2038 = 1.75;
    field_203c = 2.0;
    field_2040 = 2.5;
    field_2044 = 3.0;
    field_2048 = 4.0;
    field_204c = 0.16666667; // 1/6
    field_2050 = 0.11111111; // 1/9
    field_2054 = 0.05555556; // 1/18
    field_2058 = 1.66666663; // 5/3
    field_205c = 0.30555555; // 11/36
    field_2060 = 1.83333337; // 11/6
    field_2064 = 2.36111116; // 85/36
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
            osc->field_30 = osc->field_34;

            fun_45c5ec(voice, i, osc->field_34);

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

    updateClockRate();
}

void /* FUN_0045bc4c */ Doc::setSampleRate(float sampleRate) {
    mSampleRate = sampleRate;
    mSampleDuration = 1.0f / mSampleRate;
    field_2068 = floor((mClockRate / mSampleRate) * (2 << 29)); // TODO check floor
    mField_209c = round(mSampleRate * 0.0975); // TODO check round

    updateSampleRate();
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
            voiceSettings->oscSettings[i].field_14 = 0;
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
            if (voiceSettings->field_7c) {
                resetVoiceOutput(voice);

                voice->field_190 = voiceSettings->field_7c;
            } else {
                voice->field_190 = 0;
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

                int local_50 = 0; // step size
                unsigned int local_43 = 0; // wsr
                unsigned int local_44 = 0; // page

                if (update_osc_params) {
                    osc->wave = osc_settings->wave;
                    osc->semi = osc_settings->semi;
                    osc->fine = osc_settings->fine;

                    fun_45c49c(voice->note, voice->glideNote, &local_50, &local_43, &local_44,
                               osc_settings->wave, osc_settings->fine, osc_settings->semi);
                } else {
                    local_50 = osc->field_14;
                }

                bool local_24 = voiceSettings->restartOsc;
                unsigned int local_28;

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
                    local_30 = local_43;
                    local_2c = false;
                } else if (i == 1) {
                    local_2c = voiceSettings->am_enabled;
                } else {
                    local_2c = false;
                }

                osc->field_0 = osc_settings->enabled;
                osc->field_4 = true;
                osc->field_14 = local_50;

                local_50 += osc_settings->field_14;

                if (local_50 < 0) {
                    local_50 = 0;
                }

                osc->field_18 = local_50;
                osc->field_1c = osc_settings->field_14;
                osc->field_20 = osc_settings->level;
                osc->field_60 = fun_45c55c(local_50);

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
                    osc->rom = local_44;
                    osc->sync_state = local_28;

                    if (local_2c && voiceSettings->am_bug_enabled) {
                        osc->am_enabled = true;
                        osc->field_3c = ((local_43 & 0x40) >> 5) | (local_30 >> 7);
                    } else {
                        osc->am_enabled = false;
                        osc->field_3c = ((local_43 & 0x40) >> 5) | (local_43 >> 7);
                    }

                    osc->field_48 = ((local_43 & 0x38) >> 3) + 8;
                    osc->field_4c = 1 << osc->field_48;
                    osc->field_50 = local_43 & 7;
                    osc->field_44 = osc->field_3c * 0x10000 + osc->rom * 0x100;
                    osc->field_54 = (1 << ((osc->field_50 + 0x11) & 0x1f)) - 1;
                    osc->field_58 = (osc->field_50 + 0x11) - osc->field_48;
                }

                float volume = getVolume(osc_settings->level);
                
                if (!voiceSettings->field_48) {
                    if (voice->smoothingMode == 1) {
                        fun_45c5ec(voice, i, osc->field_34);

                        osc->field_38 = osc->field_34;
                        osc->field_34 = volume;
                    } else if (voice->smoothingMode == 2) {
                        fun_45c5ec(voice, i, volume);

                        osc->field_38 = osc->field_34;
                        osc->field_34 = volume;
                    } else {
                        fun_45c5ec(voice, i, volume);
                    }
                } else {
                    fun_45c5ec(voice, i, volume);

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
                        fun_45c5ec(voice, i, (osc->field_34 + osc->field_38) * 0.5);
                    } else if (voice->smoothingMode == 2) {
                        float local_10 = (osc->field_34 * 3.0 - osc->field_38) * 0.0;

                        if (local_10 < 0.0) {
                            local_10 = 0.0;
                        } else if (local_10 > 1.0) {
                            local_10 = 1.0;
                        }

                        fun_45c5ec(voice, i, local_10);
                    }
                }
            }
        }
    }
}

void /* FUN_0045c460 */ Doc::setGlideStart(unsigned int voiceIndex, unsigned int note) {
    setGlideStart(voiceIndex, note, false);
}

void /* FUN_0045c470 */ fun_45c470(int* semi, unsigned int* wsr) {
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

void /* FUN_0045c49c */ fun_45c49c(unsigned int note, unsigned int glide_note, int* param_4, unsigned int* wsr, unsigned int* page, unsigned int wave, int fine, int semi) {
    unsigned int /* local_10 */ fine_out;
    int /* local_c */ semi_out;

    // TODO fix call
    getWaveParameters(wave, note + semi, reinterpret_cast<unsigned char *>(page), &fine_out, &semi_out,
                      reinterpret_cast<unsigned char *>(wsr));

    semi += semi_out;

    fun_45c470(&semi, wsr);

    *param_4 = (glide_note + semi) * 256 + fine * 8 + fine_out;
}

// TODO test this function
void /* FUN_0045c4f8 */ Doc::FUN_0045c4f8(int note, unsigned int wave, unsigned int *param_4, char **param_5) {
    unsigned char page;
    unsigned char wsr;

    getWaveParameters(wave, note, &page, &wsr);

    unsigned char x = ((wsr & 0x40) >> 5) | (wsr >> 7);

    *param_4 = ((wsr & 0x38) >> 3) + 8;
    *param_5 = (char*) &mWaves[((x * 65536) + page * 256) & 0x3ffff]; // TODO fix char to unsigned char
}

unsigned int /* FUN_0045c55c */ fun_45c55c(int param_2) {
    return getVolume2(param_2 - 0x10);
}

float /* FUN_0045c568 */ Doc::getVolume(unsigned int volumeIndex) {
    float volume;
    
    if (volumeIndex == 0) {
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

void /* FUN_0045c5ec */ Doc::fun_45c5ec(Voice* voice, unsigned int oscIndex, float param_4) {
    Osc* osc = &voice->oscillators[oscIndex];

    if (oscIndex == 1 && voice->am_enabled) {
        osc->field_2c = param_4 * field_208c;
    } else {
        osc->field_2c = param_4 * field_2088;
    }
}

void /* FUN_0045c628 */ Doc::updateClockRate() {
    field_2080 = pow(0.01, 1.0 / (mClockRate * 0.002953125 + 1.0));
    field_2088 = 1 - field_2080;
    field_2084 = pow(0.01, 1.0 / (mClockRate * 0.0002220703125 + 1.0));
    field_208c = 1 - field_2084;
}

void /* FUN_0045c6dc */ Doc::updateSampleRate() {
    float arg = mSampleDuration * 111.123889803846899;
    float fVar1 = cos(arg);
    float fVar2 = sin(arg);
    float fVar3 = (fVar2 / 1.70711) + 1.0; // 1/sqrt(2) + 1 = 1.70711

    field_206c = ((fVar1 + 1.0) * 0.5) / fVar3;
    field_2070 = -(fVar1 + 1.0) / fVar3;
    field_2074 = ((fVar1 + 1.0) * 0.5) / fVar3;
    field_2078 = -(fVar1 * 2.0) / fVar3;
    field_207c = (1.0 - (fVar2 / 1.0)) / fVar3;
}

void /* FUN_0045c7bc */ Doc::resetVoiceOutput(Voice* voice) {
    voice->field_194 = 0;
    voice->field_198 = 0;
    voice->field_19c = 0;
    voice->field_1a0 = 0;
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
            // TODO check conversion
            unsigned char sample = mWaves[(osc1->sample_index >> (osc1->field_58 & 0x1f)) + (osc1->field_44 & 0x3ffff)];
        
            if (sample == 0) {
                osc1->stopped = true;
            } else {
                sample_osc1 = sample - 0x80;

                if (osc1->field_64) {
                    osc1->field_64 = false;
                    
                    if (osc2->sync_state == 2) {
                        osc2->sample_index = 0; // hmm
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
            var_12 = sample_osc2 * field_2024 * osc2->field_30;
            var_12 += sample_osc3 * field_2024 * osc3->field_30;

            osc2->field_30 = field_2084 * osc2->field_30 + 
                (sample_osc2 * field_201c + field_2020) * field_208c;
            osc3->field_30 = field_2080 * osc3->field_30 + osc3->field_2c;
        } else {
            var_12 = (osc1->field_0) ? sample_osc1 * field_2024 * osc1->field_30 : 0.0;
            var_12 += sample_osc2 * field_2024 * osc2->field_30;
            var_12 += sample_osc3 * field_2024 * osc3->field_30;

            osc1->field_30 = field_2080 * osc1->field_30 + osc1->field_2c;
            osc2->field_30 = field_2080 * osc2->field_30 + osc2->field_2c;
            osc3->field_30 = field_2080 * osc3->field_30 + osc3->field_2c;
        }

        // is this the dca smoothing part ???
        voice->field_16c = voice->field_170;
        voice->field_170 = voice->field_174;
        voice->field_174 = voice->field_178;
        voice->field_178 = var_12 + field_200c[voice->field_15c]; // hmm

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

    float var_12 = voice->field_16c;

    for (int i = 0; i < voice->field_160; i++) {
        var_12 += voice->field_180[i] * var_3;
        var_3 *= var_2;
    }

    voice->field_168 = voice->field_164;
    voice->field_164 = (voice->field_164 + field_2068) & 0x3fffffff;

    if (voice->field_190) {
        float temp = (field_206c * var_12 + 
            field_2070 * voice->field_194 +
            field_2074 * voice->field_198) -
            field_2078 * voice->field_19c - 
            field_207c * voice->field_1a0;

        voice->field_198 = voice->field_194;
        voice->field_194 = var_12;
        voice->field_1a0 = voice->field_19c;
        voice->field_19c = temp;
    }

    return var_12;
}
