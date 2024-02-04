#pragma once

#include "Preset.h"
#include "Synth.h"
#include "../data/Data.h"

#include <juce_audio_processors/juce_audio_processors.h>

// structure
// init -> this should load a fake directory with a single file called INIT
// factory -> should return presets from zip
// user -> should return user presets

class ProgramManager  {
public:
    class Listener {
    public:
        virtual void onPresetLoad(ProgramManager* pm) {
            juce::ignoreUnused(pm);
        }
    };

    explicit ProgramManager(juce::AudioProcessorValueTreeState&);

    void init();

    std::vector<juce::String> getPresets();
    std::vector<juce::String> getPresets(juce::String directory);

    juce::String getPresetName() { return currentPreset; }

    void loadPreset(const juce::String &fileName);
    void loadPreviousPreset();
    void loadNextPreset();

    void addListener(Listener* listener) {
        listeners.add(listener);
    }


private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    juce::ListenerList<Listener> listeners;

    juce::String currentPreset;
};
