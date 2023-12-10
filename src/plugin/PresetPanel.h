#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "ProgramManager.h"

class PresetPanel : public juce::Component {


class MyTreeViewItem : public juce::TreeViewItem {
    bool mightContainSubItems() override {
        return true;
    }
};


public:
    explicit PresetPanel (ProgramManager& pm) : presetManager(pm), rootItem(new MyTreeViewItem()) {
        addAndMakeVisible(fileBrowser);
        fileBrowser.setBounds(10, 10, 200, 300);
        fileBrowser.setRootItem(rootItem.get());
    }

    void paint (juce::Graphics& g) override {
        g.setColour(juce::Colours::grey);
        g.fillRect(0, 0, getWidth(), getHeight());
    }

private:
    ProgramManager& presetManager;
    juce::TreeView fileBrowser;
    std::unique_ptr<MyTreeViewItem> rootItem;
};
