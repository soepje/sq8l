#include <vector>

#include "ProgramManager.h"
#include "../data/Data.h"

ProgramManager::ProgramManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts) {
    
}

void ProgramManager::init() {

    if (valueTreeState.state.hasProperty("filename")) {
        currentPreset = valueTreeState.state.getProperty("filename").toString();

        // TODO if filename not exists on file system, rename filename to init preset file
    } else {
        loadPreset("factory/002 BRASSY");
    }

}

void ProgramManager::loadPreset(const juce::String &fileName) {
    juce::MemoryInputStream is(sq8l::data::presets, sq8l::data::presetsSize, false);
    juce::ZipFile zip(is);

    auto index = zip.getIndexOfFileName(fileName.substring(fileName.indexOf("/") + 1));
    if (index < 0) {
        // TODO error handling
        return;
    }

    std::unique_ptr<juce::InputStream> entryIs(zip.createStreamForEntry(index));
    
    std::vector<uint8_t> data;
    while (entryIs->getNumBytesRemaining() > 0) {
        data.push_back(entryIs->readByte());
    }
    




    auto preset = Preset::loadSysexProgramData(data);
    preset.setProperty("filename", fileName, nullptr);

    currentPreset = fileName;

    valueTreeState.replaceState(preset);
    

    listeners.call([this](Listener& listener) { listener.onPresetLoad(this); });
}

void ProgramManager::loadPreviousPreset() {
    auto presetDirectory = currentPreset.substring(0, currentPreset.lastIndexOf("/"));
    auto presetsInDirectory = getPresets(presetDirectory);
    for (size_t i = 0; i < presetsInDirectory.size(); i++) {
        if (presetsInDirectory[i] == currentPreset) {
            auto nextPreset = presetsInDirectory[i == 0 ? presetsInDirectory.size()-1 : i-1];
            loadPreset(nextPreset);
            break;
        }
    }
}

void ProgramManager::loadNextPreset() {
    auto presetDirectory = currentPreset.substring(0, currentPreset.lastIndexOf("/"));
    auto presetsInDirectory = getPresets(presetDirectory);
    for (size_t i = 0; i < presetsInDirectory.size(); i++) {
        if (presetsInDirectory[i] == currentPreset) {
            auto nextPreset = presetsInDirectory[(i+1) % presetsInDirectory.size()];
            loadPreset(nextPreset);
            break;
        }
    }
}

std::vector<juce::String> ProgramManager::getPresets() {
    juce::MemoryInputStream is(sq8l::data::presets, sq8l::data::presetsSize, false);
    juce::ZipFile zip(is);

    std::vector<juce::String> presets;
    for (int i = 0; i < zip.getNumEntries(); i++) {
        presets.push_back("factory/" + zip.getEntry(i)->filename);
    }

    return presets;
}

std::vector<juce::String> ProgramManager::getPresets(juce::String directory) {
    std::vector<juce::String> presetsInDirectory;
    auto presets = getPresets();
    for (auto preset : presets) {
        auto presetDirectory = preset.substring(0, preset.lastIndexOf("/"));
        if (presetDirectory == directory) {
            presetsInDirectory.push_back(preset);
        }
    }
    return presetsInDirectory;
}