#pragma once

#include <juce_data_structures/juce_data_structures.h>

class Preset {
public:

    juce::ValueTree static loadSysexProgramData(std::vector<uint8_t> sysexData);
    juce::ValueTree static loadSysexProgramBank(std::vector<uint8_t> sysexData);
};
