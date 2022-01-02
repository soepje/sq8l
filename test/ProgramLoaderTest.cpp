#include <iostream>
#include <fstream>

#include "catch.h"
#include "program.h"

template <class T>
std::vector<T> slice(std::vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
    std::vector<T> vec(first, last);
    return vec;
}

Synth_settings ProgramLoader::loadDefaultProgram() {
    Synth_settings settings = { 0 };

    std::fill(settings.presetName, settings.presetName + 31, 0);

    for (auto & oscSetting : settings.oscSettings) {
        oscSetting.octave =  0;
        oscSetting.semi =  0;
        oscSetting.fine =  0;
        oscSetting.wave =  0;
        oscSetting.modulationSource1 =  -1;
        oscSetting.modulationAmount1 =  0;
        oscSetting.modulationSource2 =  -1;
        oscSetting.modulationAmount2 =  0;
        oscSetting.level =  50;
        oscSetting.enabled =  0;
        oscSetting.modulationSource3 =  -1;
        oscSetting.modulationAmount3 =  0;
        oscSetting.modulationSource4 =  -1;
        oscSetting.modulationAmount4 =  0;
    }

    settings.oscSettings[0].enabled = 1;

    for (auto & envSettings : settings.envSettings) {
        envSettings.levels[0] =  0;
        envSettings.levels[1] =  63;
        envSettings.levels[2] =  63;
        envSettings.levels[3] =  63;
        envSettings.levels[4] =  0;
        envSettings.levelModulation =  0;
        envSettings.attackTimeModulation =  0;
        envSettings.times[0] =  0;
        envSettings.times[1] =  0;
        envSettings.times[2] =  0;
        envSettings.times[3] =  0;
        envSettings.timeScaling =  0;
        envSettings.options =  0;
        envSettings.outputSmoothness =  0;
    }

    for (auto & lfoSettings : settings.lfoSettings) {
        lfoSettings.frequency =  24;
        lfoSettings.reset =  -1;
        lfoSettings.humanize =  0;
        lfoSettings.wave =  0;
        lfoSettings.startAmplitude =  0;
        lfoSettings.fading =  63;
        lfoSettings.finalAmplitude =  63;
        lfoSettings.amplitudeModulationSource =  -1;
        lfoSettings.amplitudeModulationAmount =  0;
        lfoSettings.frequencyModulationSource =  -1;
        lfoSettings.frequencyModulationAmount =  0;
        lfoSettings.playMode =  0;
        lfoSettings.phase =  0;
        lfoSettings.smoothness =  0;
    }

    for (auto & modSettings : settings.modulationMatrixSettings) {
        modSettings.modulationSource1 = -1;
        modSettings.modulationAmount1 = 0;
        modSettings.modulationSource2 = -1;
        modSettings.modulationAmount2 = 0;
        modSettings.modulationSource3 = -1;
        modSettings.modulationAmount3 = 0;
        modSettings.amplitudeModulationSource = -1;
        modSettings.amplitudeModulationAmount = 0;
    }

    settings.filterSettings.cutoff =  127;
    settings.filterSettings.resonance =  0;
    settings.filterSettings.noteModulationAmount =  0;
    settings.filterSettings.modulationSource1 =  -1;
    settings.filterSettings.modulationAmount1 =  0;
    settings.filterSettings.modulationSource2 =  -1;
    settings.filterSettings.modulationAmount2 =  0;

    settings.ampSettings.envelopeAmplitudeModulationAmount =  63;
    settings.ampSettings.stereoPanning =  0;
    settings.ampSettings.amplitudeModulationSource =  -1;
    settings.ampSettings.amplitudeModulationAmount =  0;
    settings.ampSettings.panningModulationSource =  -1;
    settings.ampSettings.panningModulationAmount =  0;
    settings.ampSettings.saturation =  0;

    settings.muffle =  0;
    settings.field_13c =  0;
    settings.sync =  0;
    settings.am =  0;
    settings.mono =  0;
    settings.glide =  0;
    settings.restartVoice =  0;
    settings.restartEnvelopes =  0;
    settings.restartOscillators =  0;
    settings.cycle =  0;
    settings.pitchBendRange =  2;
    settings.pitchBendMode =  0;
    settings.emuOptions =  1;
    settings.voiceStealingMode =  0;
    settings.field_196 =  0;

    return settings;
}

const short modulationSourceMapping[16] = {
        0, // LFO1
        1, // LFO2
        2, // LFO3
        4, // ENV1
        5, // ENV2
        6, // ENV3
        7, // ENV4
        8, // VEL
        9, // VEL-X
        10, // KEYB
        11, // KEYB2
        17, // WHEEL
        52, // PEDAL
        18, // BREATH
        12, // PRESS
        -1, // OFF
};

int8_t fromTwosComplement(uint8_t i) {
    bool negative = (i & 64) > 0;
    if (negative) {
        return (int8_t) (-(~i & 63) + 1);
    }
    return (int8_t) (i & 63);
}

Synth_settings ProgramLoader::loadSysexProgramData(std::vector<uint8_t> data) {
    Synth_settings settings = loadDefaultProgram();

    if (data.size() != 102) {
        return settings;
    }

    settings.presetNameLength = 6;

    for (int i = 0; i < 6; i++) {
        settings.presetName[i] = (char) data[i];
    }

    for (int i = 0; i < 4; i++) {
        Env_settings & envParameters = settings.envSettings[i];
        auto envData = slice(data, i * 10 + 6, (i + 1) * 10 + 6);

        envParameters.levels[1] = fromTwosComplement(envData[0] >> 1);
        envParameters.levels[2] = fromTwosComplement(envData[1] >> 1);
        envParameters.levels[3] = fromTwosComplement(envData[2] >> 1);

        envParameters.times[0] = envData[3] & 63;
        envParameters.times[1] = envData[4] & 63;
        envParameters.times[2] = envData[5] & 63;
        envParameters.times[3] = envData[6] & 63;

        envParameters.options = envData[7] & 1;
        envParameters.levelModulation = envData[7] >> 2;
        envParameters.attackTimeModulation = envData[8] & 63;
        envParameters.timeScaling = envData[9] & 63;
    }

    for (int i = 0; i < 3; i++) {
        SynthOscSettings & oscSettings = settings.oscSettings[i];
        auto oscData = slice(data, i * 10 + 58, (i + 1) * 10 + 58);

        oscSettings.octave = (oscData[0] / 12) - 3;
        oscSettings.semi = oscData[0] % 12;
        oscSettings.fine = (oscData[1] >> 3) & 31;

        oscSettings.modulationSource1 = modulationSourceMapping[oscData[2] & 15];
        oscSettings.modulationSource2 = modulationSourceMapping[(oscData[2] >> 4) & 15];
        oscSettings.modulationAmount1 = fromTwosComplement(oscData[3] >> 1);
        oscSettings.modulationAmount2 = fromTwosComplement(oscData[4] >> 1);

        oscSettings.enabled = (oscData[6] >> 7) & 1;
        oscSettings.level = (oscData[6] >> 1) & 63;

        oscSettings.modulationSource3 = modulationSourceMapping[oscData[7] & 15];
        oscSettings.modulationSource4 = modulationSourceMapping[(oscData[7] >> 4) & 15];
        oscSettings.modulationAmount3 = fromTwosComplement(oscData[8] >> 1);
        oscSettings.modulationAmount4 = fromTwosComplement(oscData[9] >> 1);
    }

    for (int i = 0; i < 3; i++) {
        SynthLfoSettings & lfoSettings = settings.lfoSettings[i];
        auto lfoData = slice(data, i * 4 + 46, (i + 1) * 4 + 46);

        lfoSettings.frequency = lfoData[0] & 63;
        lfoSettings.startAmplitude = lfoData[1] & 63;
        lfoSettings.finalAmplitude = lfoData[2] & 63;
        lfoSettings.phase = lfoData[3] & 63;

        lfoSettings.wave = (lfoData[1] & 192) >> 6;
        lfoSettings.frequencyModulationSource = modulationSourceMapping[((lfoData[1] & 192) >> 6) | ((lfoData[2] & 192) >> 4)];

        lfoSettings.humanize = (lfoData[3] & 64) >> 6;
        lfoSettings.reset = (lfoData[3] & 128) > 0 ? 0 : -1;
    }

    settings.ampSettings.envelopeAmplitudeModulationAmount = fromTwosComplement(data[88] >> 1);
    settings.ampSettings.amplitudeModulationSource = -1;

    settings.am = (data[88] & 128) >> 7;
    settings.sync = (data[89] & 128) >> 7;
    settings.restartVoice = (data[92] & 128) >> 7;
    settings.mono = (data[93] & 128) >> 7;
    settings.restartEnvelopes = (data[94] & 128) >> 7;
    settings.restartOscillators = (data[95] & 128) >> 7;
    settings.cycle = (data[101] & 128) >> 7;

    settings.glide = (char) (data[95] & 63);

    settings.filterSettings.cutoff = data[89] & 127;
    settings.filterSettings.resonance = data[90] & 31;
    settings.filterSettings.modulationSource1 = modulationSourceMapping[data[91] & 15];
    settings.filterSettings.modulationSource2 = modulationSourceMapping[(data[91] >> 4) & 15];
    settings.filterSettings.modulationAmount1 = fromTwosComplement(data[92]);
    settings.filterSettings.modulationAmount2 = fromTwosComplement(data[93]);
    settings.filterSettings.noteModulationAmount = (data[94] & 127) >> 1;

    const char panningMapping[16] = {
            -63, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 63
    };

    settings.ampSettings.stereoPanning = panningMapping[(data[100] >> 4) & 15];
    settings.ampSettings.panningModulationSource = modulationSourceMapping[data[100] & 15];
    settings.ampSettings.panningModulationAmount = fromTwosComplement(data[101] & 127);

    return settings;
}

Synth_settings ProgramLoader::loadSysexProgram(std::vector<uint8_t> sysexData) {
    // TODO verify sysex header
    // 00001111 ENSONIQ I.D. code
    // 00000010 ESQ/SQ-80 Product I.D. code
    // 0000nnnn MIDI channel number
    // 00000001 single program dump code

    std::vector<uint8_t> programData(102, 0);

    for (int i = 4, j = 0; j < 102; i += 2, j++) {
        programData[j] = (sysexData[i] & 15) | ((sysexData[i + 1] & 15) << 4);
    }

    return loadSysexProgramData(programData);
}

std::vector<Synth_settings> ProgramLoader::loadSysexProgramBank(std::vector<uint8_t> sysexData) {
    // TODO verify sysex header
    // 00001111 ENSONIQ I.D. code
    // 00000010 ESQ/SQ-80 Product I.D. code
    // 0000nnnn MIDI channel number
    // 00000010 all program dump code

    std::vector<Synth_settings> programBank;

    for (int k = 0; k < 40; k++) {
        std::vector<uint8_t> programData(102, 0);

        for (int i = 5 + k * 204, j = 0; j < 102; i += 2, j++) {
            programData[j] = (sysexData[i] & 15) | ((sysexData[i + 1] & 15) << 4);
        }

        programBank.push_back(ProgramLoader::loadSysexProgramData(programData));
    }

    return programBank;
}

std::vector<uint8_t> readSysexFile(std::string filePath) {
    std::ifstream instream(filePath, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    return std::vector<uint8_t>(data.begin() + 1, data.end() - 1);
}

TEST_CASE( "Preset loading", "[preset_anabas]" ) {
    Synth_settings settings = loadSysexProgram(readSysexFile("../testdata/ANABAS.syx"));

    REQUIRE(settings.presetNameLength == 6);
    REQUIRE(settings.presetName[0] == 65);
    REQUIRE(settings.presetName[1] == 78);
    REQUIRE(settings.presetName[2] == 65);
    REQUIRE(settings.presetName[3] == 66);
    REQUIRE(settings.presetName[4] == 65);
    REQUIRE(settings.presetName[5] == 83);
    REQUIRE(settings.presetName[6] == 0);
    REQUIRE(settings.presetName[7] == 0);
    REQUIRE(settings.presetName[8] == 0);
    REQUIRE(settings.presetName[9] == 0);
    REQUIRE(settings.presetName[10] == 0);
    REQUIRE(settings.presetName[11] == 0);
    REQUIRE(settings.presetName[12] == 0);
    REQUIRE(settings.presetName[13] == 0);
    REQUIRE(settings.presetName[14] == 0);
    REQUIRE(settings.presetName[15] == 0);
    REQUIRE(settings.presetName[16] == 0);
    REQUIRE(settings.presetName[17] == 0);
    REQUIRE(settings.presetName[18] == 0);
    REQUIRE(settings.presetName[19] == 0);
    REQUIRE(settings.presetName[20] == 0);
    REQUIRE(settings.presetName[21] == 0);
    REQUIRE(settings.presetName[22] == 0);
    REQUIRE(settings.presetName[23] == 0);
    REQUIRE(settings.presetName[24] == 0);
    REQUIRE(settings.presetName[25] == 0);
    REQUIRE(settings.presetName[26] == 0);
    REQUIRE(settings.presetName[27] == 0);
    REQUIRE(settings.presetName[28] == 0);
    REQUIRE(settings.presetName[29] == 0);
    REQUIRE(settings.presetName[30] == 0);

    REQUIRE(settings.envSettings[0].levels[0] == 0);
    REQUIRE(settings.envSettings[0].levels[1] == 25);
    REQUIRE(settings.envSettings[0].levels[2] == 8);
    REQUIRE(settings.envSettings[0].levels[3] == 20);
    REQUIRE(settings.envSettings[0].levels[4] == 0);
    REQUIRE(settings.envSettings[0].levelModulation == 0);
    REQUIRE(settings.envSettings[0].attackTimeModulation == 0);
    REQUIRE(settings.envSettings[0].times[0] == 0);
    REQUIRE(settings.envSettings[0].times[1] == 22);
    REQUIRE(settings.envSettings[0].times[2] == 63);
    REQUIRE(settings.envSettings[0].times[3] == 22);
    REQUIRE(settings.envSettings[0].timeScaling == 9);
    REQUIRE(settings.envSettings[0].options == 0);
    REQUIRE(settings.envSettings[0].outputSmoothness == 0);

    REQUIRE(settings.envSettings[1].levels[0] == 0);
    REQUIRE(settings.envSettings[1].levels[1] == 63);
    REQUIRE(settings.envSettings[1].levels[2] == 33);
    REQUIRE(settings.envSettings[1].levels[3] == 28);
    REQUIRE(settings.envSettings[1].levels[4] == 0);
    REQUIRE(settings.envSettings[1].levelModulation == 38);
    REQUIRE(settings.envSettings[1].attackTimeModulation == 4);
    REQUIRE(settings.envSettings[1].times[0] == 2);
    REQUIRE(settings.envSettings[1].times[1] == 18);
    REQUIRE(settings.envSettings[1].times[2] == 22);
    REQUIRE(settings.envSettings[1].times[3] == 12);
    REQUIRE(settings.envSettings[1].timeScaling == 11);
    REQUIRE(settings.envSettings[1].options == 0);
    REQUIRE(settings.envSettings[1].outputSmoothness == 0);

    REQUIRE(settings.envSettings[2].levels[0] == 0);
    REQUIRE(settings.envSettings[2].levels[1] == 0);
    REQUIRE(settings.envSettings[2].levels[2] == 3);
    REQUIRE(settings.envSettings[2].levels[3] == 0);
    REQUIRE(settings.envSettings[2].levels[4] == 0);
    REQUIRE(settings.envSettings[2].levelModulation == 0);
    REQUIRE(settings.envSettings[2].attackTimeModulation == 1);
    REQUIRE(settings.envSettings[2].times[0] == 0);
    REQUIRE(settings.envSettings[2].times[1] == 14);
    REQUIRE(settings.envSettings[2].times[2] == 0);
    REQUIRE(settings.envSettings[2].times[3] == 25);
    REQUIRE(settings.envSettings[2].timeScaling == 0);
    REQUIRE(settings.envSettings[2].options == 0);
    REQUIRE(settings.envSettings[2].outputSmoothness == 0);

    REQUIRE(settings.envSettings[3].levels[0] == 0);
    REQUIRE(settings.envSettings[3].levels[1] == 63);
    REQUIRE(settings.envSettings[3].levels[2] == 58);
    REQUIRE(settings.envSettings[3].levels[3] == 58);
    REQUIRE(settings.envSettings[3].levels[4] == 0);
    REQUIRE(settings.envSettings[3].levelModulation == 0);
    REQUIRE(settings.envSettings[3].attackTimeModulation == 63);
    REQUIRE(settings.envSettings[3].times[0] == 0);
    REQUIRE(settings.envSettings[3].times[1] == 33);
    REQUIRE(settings.envSettings[3].times[2] == 33);
    REQUIRE(settings.envSettings[3].times[3] == 4);
    REQUIRE(settings.envSettings[3].timeScaling == 0);
    REQUIRE(settings.envSettings[3].options == 0);
    REQUIRE(settings.envSettings[3].outputSmoothness == 0);

    REQUIRE(settings.oscSettings[0].octave == -1);
    REQUIRE(settings.oscSettings[0].semi == 0);
    REQUIRE(settings.oscSettings[0].fine == 4);
    REQUIRE(settings.oscSettings[0].wave == 0);
    REQUIRE(settings.oscSettings[0].modulationSource1 == 0);
    REQUIRE(settings.oscSettings[0].modulationAmount1 == 5);
    REQUIRE(settings.oscSettings[0].modulationSource2 == 6);
    REQUIRE(settings.oscSettings[0].modulationAmount2 == 18);
    REQUIRE(settings.oscSettings[0].level == 63);
    REQUIRE(settings.oscSettings[0].enabled == 1);
    REQUIRE(settings.oscSettings[0].modulationSource3 == -1);
    REQUIRE(settings.oscSettings[0].modulationAmount3 == 63);
    REQUIRE(settings.oscSettings[0].modulationSource4 == 2);
    REQUIRE(settings.oscSettings[0].modulationAmount4 == 0);

    REQUIRE(settings.oscSettings[1].octave == -1);
    REQUIRE(settings.oscSettings[1].semi == 0);
    REQUIRE(settings.oscSettings[1].fine == 5);
    REQUIRE(settings.oscSettings[1].wave == 0);
    REQUIRE(settings.oscSettings[1].modulationSource1 == 0);
    REQUIRE(settings.oscSettings[1].modulationAmount1 == 5);
    REQUIRE(settings.oscSettings[1].modulationSource2 == 6);
    REQUIRE(settings.oscSettings[1].modulationAmount2 == 0);
    REQUIRE(settings.oscSettings[1].level == 63);
    REQUIRE(settings.oscSettings[1].enabled == 1);
    REQUIRE(settings.oscSettings[1].modulationSource3 == -1);
    REQUIRE(settings.oscSettings[1].modulationAmount3 == 63);
    REQUIRE(settings.oscSettings[1].modulationSource4 == 0);
    REQUIRE(settings.oscSettings[1].modulationAmount4 == 0);

    REQUIRE(settings.oscSettings[2].octave == -1);
    REQUIRE(settings.oscSettings[2].semi == 0);
    REQUIRE(settings.oscSettings[2].fine == 3);
    REQUIRE(settings.oscSettings[2].wave == 0);
    REQUIRE(settings.oscSettings[2].modulationSource1 == 0);
    REQUIRE(settings.oscSettings[2].modulationAmount1 == 5);
    REQUIRE(settings.oscSettings[2].modulationSource2 == 6);
    REQUIRE(settings.oscSettings[2].modulationAmount2 == 0);
    REQUIRE(settings.oscSettings[2].level == 63);
    REQUIRE(settings.oscSettings[2].enabled == 1);
    REQUIRE(settings.oscSettings[2].modulationSource3 == -1);
    REQUIRE(settings.oscSettings[2].modulationAmount3 == 63);
    REQUIRE(settings.oscSettings[2].modulationSource4 == 0);
    REQUIRE(settings.oscSettings[2].modulationAmount4 == 0);

    REQUIRE(settings.modulationMatrixSettings[0].modulationSource1 == -1);
    REQUIRE(settings.modulationMatrixSettings[0].modulationAmount1 == 0);
    REQUIRE(settings.modulationMatrixSettings[0].modulationSource2 == -1);
    REQUIRE(settings.modulationMatrixSettings[0].modulationAmount2 == 0);
    REQUIRE(settings.modulationMatrixSettings[0].modulationSource3 == -1);
    REQUIRE(settings.modulationMatrixSettings[0].modulationAmount3 == 0);
    REQUIRE(settings.modulationMatrixSettings[0].amplitudeModulationSource == -1);
    REQUIRE(settings.modulationMatrixSettings[0].amplitudeModulationAmount == 0);

    REQUIRE(settings.modulationMatrixSettings[1].modulationSource1 == -1);
    REQUIRE(settings.modulationMatrixSettings[1].modulationAmount1 == 0);
    REQUIRE(settings.modulationMatrixSettings[1].modulationSource2 == -1);
    REQUIRE(settings.modulationMatrixSettings[1].modulationAmount2 == 0);
    REQUIRE(settings.modulationMatrixSettings[1].modulationSource3 == -1);
    REQUIRE(settings.modulationMatrixSettings[1].modulationAmount3 == 0);
    REQUIRE(settings.modulationMatrixSettings[1].amplitudeModulationSource == -1);
    REQUIRE(settings.modulationMatrixSettings[1].amplitudeModulationAmount == 0);

    REQUIRE(settings.modulationMatrixSettings[2].modulationSource1 == -1);
    REQUIRE(settings.modulationMatrixSettings[2].modulationAmount1 == 0);
    REQUIRE(settings.modulationMatrixSettings[2].modulationSource2 == -1);
    REQUIRE(settings.modulationMatrixSettings[2].modulationAmount2 == 0);
    REQUIRE(settings.modulationMatrixSettings[2].modulationSource3 == -1);
    REQUIRE(settings.modulationMatrixSettings[2].modulationAmount3 == 0);
    REQUIRE(settings.modulationMatrixSettings[2].amplitudeModulationSource == -1);
    REQUIRE(settings.modulationMatrixSettings[2].amplitudeModulationAmount == 0);

    REQUIRE(settings.filterSettings.cutoff == 2);
    REQUIRE(settings.filterSettings.resonance == 29);
    REQUIRE(settings.filterSettings.noteModulationAmount == 17);
    REQUIRE(settings.filterSettings.modulationSource1 == 5);
    REQUIRE(settings.filterSettings.modulationAmount1 == 56);
    REQUIRE(settings.filterSettings.modulationSource2 == 17);
    REQUIRE(settings.filterSettings.modulationAmount2 == 24);

    REQUIRE(settings.ampSettings.envelopeAmplitudeModulationAmount == 47);
    REQUIRE(settings.ampSettings.stereoPanning == 0);
    REQUIRE(settings.ampSettings.amplitudeModulationSource == -1);
    REQUIRE(settings.ampSettings.amplitudeModulationAmount == 0);
    REQUIRE(settings.ampSettings.panningModulationSource == 0);
    REQUIRE(settings.ampSettings.panningModulationAmount == 0);
    REQUIRE(settings.ampSettings.saturation == 0);

    REQUIRE(settings.muffle == 0);
    REQUIRE(settings.field_13c == 0);
    REQUIRE(settings.sync == 0);
    REQUIRE(settings.am == 0);
    REQUIRE(settings.mono == 1);
    REQUIRE(settings.glide == 12);
    REQUIRE(settings.restartVoice == 0);
    REQUIRE(settings.restartEnvelopes == 1);
    REQUIRE(settings.restartOscillators == 0);
    REQUIRE(settings.cycle == 0);
    REQUIRE(settings.pitchBendRange == 2);
    REQUIRE(settings.pitchBendMode == 0);
    REQUIRE(settings.emuOptions == 1);
    REQUIRE(settings.voiceStealingMode == 0);
    REQUIRE(settings.field_196 == 0);
}
