#pragma once

#include "Synth.h"
#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_processors/juce_audio_processors.h>

class ProgramManager {
private:
    juce::ValueTree static loadSysexProgramData(std::vector<uint8_t> sysexData);

    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::ValueTree programBank;
    int currentProgram = 0;

public:
    explicit ProgramManager(juce::AudioProcessorValueTreeState&);

    int getNumPrograms();
    void setCurrentProgram(int);
    int getCurrentProgram() const;
    juce::String getProgramName(int);

    juce::ValueTree static loadSysexProgramBank(std::vector<uint8_t> sysexData);
};
