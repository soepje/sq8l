#ifndef PLUGIN_EDITOR_H
#define PLUGIN_EDITOR_H

#include <juce_audio_utils/juce_audio_utils.h>

#include "PluginProcessor.h"
#include "LcdButton.h"
#include "LcdScreen.h"
#include "LookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    juce::MidiKeyboardComponent midiKeyboard;
    juce::ComboBox programMenu;

    LcdButton lcdButtonUp;
    LcdButton lcdButtonDown;
    LcdScreen lcdScreen;

    juce::ImageButton mixButton;
    juce::ImageButton osc1Button;
    juce::ImageButton osc2Button;
    juce::ImageButton osc3Button;
    juce::ImageButton dca1Button;
    juce::ImageButton dca2Button;
    juce::ImageButton dca3Button;
    juce::ImageButton filterButton;
    juce::ImageButton dca4Button;
    juce::ImageButton modesEmuButton;
    juce::OwnedArray<juce::ImageButton> lfoButtons;
    juce::OwnedArray<juce::ImageButton> envButtons;
    juce::OwnedArray<juce::ImageButton> matButtons;

    juce::ToggleButton syncButton;
    juce::ToggleButton amButton;
    juce::ToggleButton monoButton;

    juce::OwnedArray<LcdScreen::LcdParameterAttachment> lcdParameterAttachments;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonParameterAttachments;

    LookAndFeel lookAndFeel;

    void showPageGroup(int i);
    void updateLcdButtons();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};


#endif
