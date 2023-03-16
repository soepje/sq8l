#include <vector>

#include "ProgramManager.h"
#include "../data/Data.h"

ProgramManager::ProgramManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts) {
    auto data = std::vector<uint8_t>(std::begin(sq8l::data::programBank), std::end(sq8l::data::programBank));

    programBank = ProgramManager::loadSysexProgramBank(data);
    valueTreeState.replaceState(programBank.getChild(0));
}

int ProgramManager::getNumPrograms() {
    return programBank.getNumChildren();
}

void ProgramManager::setCurrentProgram(int i) {
    if (i >= 0 && i < programBank.getNumChildren()) {
        valueTreeState.replaceState(programBank.getChild(i));
        currentProgram = i;
    }
}

int ProgramManager::getCurrentProgram() const {
    return currentProgram;
}

juce::String ProgramManager::getProgramName(int i) {
    return programBank.getChild(i).getProperty("name");
}

template <class T>
std::vector<T> slice(std::vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
    std::vector<T> vec(first, last);
    return vec;
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
        return (int8_t) -(((i & 63) ^ 63) + 1);
    }
    return (int8_t) (i & 63);
}

template <class T>
juce::ValueTree createParam(const juce::String& id, T value) {
    auto param = juce::ValueTree("PARAM");
    param.setProperty("id", id, nullptr);
    param.setProperty("value", value, nullptr);
    return param;
}

juce::ValueTree ProgramManager::loadSysexProgramData(std::vector<uint8_t> data) {
    juce::ValueTree vt = juce::ValueTree("PROGRAM");

    juce::String name;
    for (size_t i = 0; i < 6; i++) {
        name += static_cast<char>(data[i]);
    }

    vt.setProperty("name", name, nullptr);

    for (int i = 0; i < 4; i++) {
        juce::String env = "ENV" + std::to_string(i + 1);
        auto envData = slice(data, i * 10 + 6, (i + 1) * 10 + 6);

        vt.appendChild(createParam(env + ".L1", fromTwosComplement(envData[0] >> 1)), nullptr);
        vt.appendChild(createParam(env + ".L2", fromTwosComplement(envData[1] >> 1)), nullptr);
        vt.appendChild(createParam(env + ".L3", fromTwosComplement(envData[2] >> 1)), nullptr);

        vt.appendChild(createParam(env + ".T1", envData[3] & 63), nullptr);
        vt.appendChild(createParam(env + ".T2", envData[4] & 63), nullptr);
        vt.appendChild(createParam(env + ".T3", envData[5] & 63), nullptr);
        vt.appendChild(createParam(env + ".T4", envData[6] & 63), nullptr);

        vt.appendChild(createParam(env + ".CYC", envData[7] & 1), nullptr);
        vt.appendChild(createParam(env + ".LV", envData[7] >> 2), nullptr);
        vt.appendChild(createParam(env + ".T1V", envData[8] & 63), nullptr);
        vt.appendChild(createParam(env + ".TK", envData[9] & 63), nullptr);
    }

    for (int i = 0; i < 3; i++) {
        juce::String osc = "OSC" + std::to_string(i + 1);
        juce::String dca = "DCA" + std::to_string(i + 1);
        auto oscData = slice(data, i * 10 + 58, (i + 1) * 10 + 58);

        vt.appendChild(createParam(osc + ".OCT",  (oscData[0] / 12) - 3), nullptr);
        vt.appendChild(createParam(osc + ".SEMI",oscData[0] % 12), nullptr);
        vt.appendChild(createParam(osc + ".FINE",(oscData[1] >> 3) & 31), nullptr);

        vt.appendChild(createParam(osc + ".MOD1.SEL",modulationSourceMapping[oscData[2] & 15] + 1), nullptr);
        vt.appendChild(createParam(osc + ".MOD2.SEL",modulationSourceMapping[oscData[2] >> 4] + 1), nullptr);
        vt.appendChild(createParam(osc + ".MOD1.AMT", fromTwosComplement(oscData[3] >> 1)), nullptr);
        vt.appendChild(createParam(osc + ".MOD2.AMT", fromTwosComplement(oscData[4] >> 1)), nullptr);

        vt.appendChild(createParam(osc + ".WAVE", oscData[5]), nullptr);

        vt.appendChild(createParam(dca + ".OUTPUT", fromTwosComplement((oscData[6] >> 7) & 1)), nullptr);
        vt.appendChild(createParam(dca + ".LEVEL", fromTwosComplement((oscData[6] >> 1) & 63)), nullptr);

        vt.appendChild(createParam(dca + ".MOD1.SEL",modulationSourceMapping[oscData[7] & 15] + 1), nullptr);
        vt.appendChild(createParam(dca + ".MOD2.SEL",modulationSourceMapping[oscData[7] >> 4] + 1), nullptr);
        vt.appendChild(createParam(dca + ".MOD1.AMT", fromTwosComplement(oscData[8] >> 1)), nullptr);
        vt.appendChild(createParam(dca + ".MOD2.AMT", fromTwosComplement(oscData[9] >> 1)), nullptr);
    }

    for (int i = 0; i < 3; i++) {
        juce::String lfo = "LFO" + std::to_string(i + 1);
        auto lfoData = slice(data, i * 4 + 46, (i + 1) * 4 + 46);

        vt.appendChild(createParam(lfo + ".FREQ", lfoData[0] & 63), nullptr);
        vt.appendChild(createParam(lfo + ".L1", lfoData[1] & 63), nullptr);
        vt.appendChild(createParam(lfo + ".L2", lfoData[2] & 63), nullptr);
        vt.appendChild(createParam(lfo + ".DELAY", lfoData[3] & 63), nullptr);
        vt.appendChild(createParam(lfo + ".WAVE", (lfoData[0] & 192) >> 6), nullptr);
        vt.appendChild(createParam(lfo + ".FMOD.SEL", modulationSourceMapping[((lfoData[1] & 192) >> 6) | ((lfoData[2] & 192) >> 6)] + 1), nullptr);
        vt.appendChild(createParam(lfo + ".HUMAN", (lfoData[3] & 64) >> 6), nullptr);
        vt.appendChild(createParam(lfo + ".RESET", ((lfoData[3] & 128) >> 7) - 1), nullptr);
    }

    const char panningMapping[16] = {
            -63, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 63
    };

    vt.appendChild(createParam("DCA4.AMT", fromTwosComplement(data[88] >> 1)), nullptr);
    vt.appendChild(createParam("DCA4.PAN", panningMapping[(data[100] >> 4) & 15]), nullptr);
    vt.appendChild(createParam("DCA4.PMOD.SEL", modulationSourceMapping[data[100] & 15] + 1), nullptr);
    vt.appendChild(createParam("DCA4.PMOD.AMT", fromTwosComplement(data[101] & 127)), nullptr);


    vt.appendChild(createParam("MODE.AM", (data[88] & 128) >> 7), nullptr);
    vt.appendChild(createParam("MODE.SYNC", (data[89] & 128) >> 7), nullptr);
    vt.appendChild(createParam("MODE.MONO", (data[93] & 128) >> 7), nullptr);
    vt.appendChild(createParam("MODE.CYC", (data[101] & 128) >> 7), nullptr);
    vt.appendChild(createParam("MODE.GLIDE", data[95] & 63), nullptr);
    vt.appendChild(createParam("MODE.REST.VC", (data[92] & 128) >> 7), nullptr);
    vt.appendChild(createParam("MODE.REST.ENV", (data[94] & 128) >> 7), nullptr);
    vt.appendChild(createParam("MODE.REST.OSC", (data[95] & 128) >> 7), nullptr);

    vt.appendChild(createParam("FILT.FREQ", data[89] & 127), nullptr);
    vt.appendChild(createParam("FILT.RES", data[90] & 31), nullptr);
    vt.appendChild(createParam("FILT.KEYBD", (data[94] & 127) >> 1), nullptr);
    vt.appendChild(createParam("FILT.MOD1.SEL", modulationSourceMapping[data[91] & 15] + 1), nullptr);
    vt.appendChild(createParam("FILT.MOD2.SEL", modulationSourceMapping[(data[91] >> 4) & 15] + 1), nullptr);
    vt.appendChild(createParam("FILT.MOD1.AMT", fromTwosComplement(data[92])), nullptr);
    vt.appendChild(createParam("FILT.MOD2.AMT", fromTwosComplement(data[93])), nullptr);

    return vt;
}

juce::ValueTree ProgramManager::loadSysexProgramBank(std::vector<uint8_t> sysexData) {
    // TODO verify sysex header
    // 00001111 ENSONIQ I.D. code
    // 00000010 ESQ/SQ-80 Product I.D. code
    // 0000nnnn MIDI channel number
    // 00000010 all program dump code

    juce::ValueTree programBank = juce::ValueTree("PROGRAMBANK");

    for (int k = 0; k < 40; k++) {
        std::vector<uint8_t> programData(102, 0);

        for (int i = 5 + k * 204, j = 0; j < 102; i += 2, j++) {
            programData[j] = (sysexData[i] & 15) | ((sysexData[i + 1] & 15) << 4);
        }

        programBank.appendChild(ProgramManager::loadSysexProgramData(programData), nullptr);
    }

    return programBank;
}
